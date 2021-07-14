.. _Client_Management:


Client Management
=================

This chapter provides guidance to install and configure SIMP clients with the
resources supplied by the SIMP installation.

This guide also assumes that your SIMP server is a :term:`yum` package
repository and that you are configuring the ``production``
:term:`SIMP Omni-Environment`.


System Requirements
-------------------

Client systems should meet the following minimum requirements:

-  Hardware/:term:`Virtual Machine` (VM): Capable of running RHEL 6 or 7 x86_64
-  RAM: 2048 MB
-  HDD: 22 GB


Configuring the Puppet Server
-----------------------------

Perform the following actions as ``root`` on the :term:`Puppet Server` **prior**
to attempting to install a client.


Configure DNS
^^^^^^^^^^^^^

In SIMP, numerous and/or large configuration files are distributed via
:program:`rsync` by Puppet to minimize management cost. These managed files presently
include DNS configuration files and can be found at
:file:`/var/simp/environments/production/rsync/{OSTYPE}/{MAJORRELEASE}/bind_dns/default`.

This section is not a complete manual for named. For more complete documentation
on how to set up :program:`named`, see ``named(8)`` and ``named.conf(5)``.

The following configuration steps are for a SIMP-managed setup. However, you
can use an existing DNS infrastructure.

#. Navigate to :file:`/var/simp/environments/production/rsync/{OSTYPE}/{MAJORRELEASE}/bind_dns/default`

#. Modify the :program:`named` files to correctly reflect the environment.

   * The relevant files under :file:`bind_dns/default/` are as follows:

     * :file:`named/etc/named.conf`
     * :file:`named/etc/zones/your.domain`
     * :file:`named/var/named/forward/your.domain.db`
     * :file:`named/var/named/reverse/0.0.10.db`

   * Review :file:`named/etc/named.conf` and update the following:

     * Update the :term:`IP` for allow-query and allow-recursion
     * Delete any unnecessary zone stanzas (i.e. forwarding) if not
       necessary
     * Substitute in the :term:`FQDN` of your domain for all occurrences of
       :code:`your.domain`

   * Add clients to :file:`named/var/named/forward/your.domain.db` and
     :file:`named/var/named/reverse/0.0.10.db` and then rename these files
     to appropriately match your environment.

#. Run :command:`puppet agent -t --tags named` on the Puppet Server to apply
   the changes.
#. Validate DNS and ensure the :file:`/etc/resolv.conf` is updated appropriately.
#. If an :file:`rndc.key` error appears when starting :program:`named`, see the
   `BIND Documentation`_.  Once you have resolved the issue, re-run
   :command:`puppet agent -t` on the Puppet Server to apply.

.. NOTE::

   You can adjust the list of clients in your
   :file:`named/var/named/forward/<your.domain>.db` and
   :file:`named/var/named/reverse/<your reverse domain>.db` files at any time;
   just remember to run :command:`puppet agent -t --tags named` on the Puppet
   master to propagate the updates.


Configure DHCP
^^^^^^^^^^^^^^

.. NOTE::

   The :file:`dhcpd.conf` file was updated in SIMP 6.2 to include logic in the
   :code:`pxeclients` class that determines the appropriate boot loader file on
   the TFTP server, based on whether the client is booting in :term:`UEFI` or
   :term:`BIOS` mode.  If you have configured DHCP using an earlier version of
   SIMP and need to add UEFI support, make sure you update your
   :file:`dhcpd.conf` in the rsync directory, appropriately.

   MAC addresses in the following section need to be lower case letters.

Perform the following actions as ``root`` on the Puppet Server system
prior to attempting to install a client.

Open :file:`/var/simp/environments/production/rsync/<OSTYPE>/Global/dhcpd/dhcpd.conf`
and edit it to suit the necessary environment. Make sure the following is done:

  * The :code:`next-server` setting in the :code:`pxeclients` class block
    points to the IP Address of the :term:`TFTP` server.
  * Create a :code:`subnet` block and edit the following:

    - Make sure the :code:`routers`, :code:`subnet-mask`, and :code:`netmask`
      are correct for your environment.
    - Enter the :code:`hardware ethernet` and :code:`fixed-address` for each
      client that will be kickstarted.  For increased security, it is suggested
      that SIMP environments not allow clients to pick random IP Address in
      a subnet. The MAC address must be associated with and IP Address here.
      (You can add additional ones as needed.)
    - Enter the domain name for option :code:`domain-name`
    - Enter the IP Address of the DNS server for option :code:`domain-name-servers`
    - If this DHCP server is used for PXE booting, make sure each
      :code:`filename` parameter corresponds to the correct boot loader file on
      the TFTP server. If you are using SIMP's :code:`simp::server::kickstart`
      class to manage the TFTP server, the default :code:`filename` values listed
      in the :code:`pxeclients` class of the sample :file:`dhpcd.conf` will be
      correct.

Save and close the file.

Run :command:`puppet agent -t` on the Puppet Server to apply the changes.

.. _PXE_Boot:

.. include:: PXE_Boot.inc

.. _Certificates:

.. include:: Certificates/Certificates.inc
.. include:: Certificates/Official_Certificates.inc


.. _cm-setting-up-the-client:

Setting up the Client
=====================

Existing Clients
----------------

Cloud environments, such as AWS, Azure, OpenStack, and GCE do not need to follow
the PXE model shown below. Likewise, pre-existing physical clients can be
integrated into the SIMP environment using the method outlined in this section.

The SIMP system contains a bootstrap script that is able to be downloaded from
the server. You should examine the actual client application to determine if it
meets your needs as written but, in general, it should be well suited most
applications.

The following invocation waits for the server to provide a signed PKI
certificate prior to proceeding. This is the safest method but will hang if the
client certificate is not signed.

.. code-block:: bash

   curl -k -O https://<puppet.server.fqdn>/ks/bootstrap_simp_client

   # Use the puppet provided ruby for a guaranteed compatible version
   /opt/puppetlabs/puppet/bin/ruby ./bootstrap_simp_client \
     --puppet-server <puppet.server.fqdn> \
     --puppet-ca <puppet.server.fqdn> \
     --puppet-wait-for-cert 0 \
     --debug
     --print-stats

PXE Booting
-----------

The following lists the steps to :term:`PXE` boot the system and set up the
client.

#. Set up your client's boot settings to boot off of the network.
#. Make sure the :term:`MAC` address of the client is set up in :term:`DHCP`
   (see `Configure DHCP`_ for more info.)
#. Restart the system.
#. Once the client installs, reboots, and begins to bootstrap, it will check in
   for the first time.
#. By default, Puppet will not autosign Puppet certificates, so the agent
   initially runs with ``--waitforcert`` enabled.  This means the client will
   check in every 30 seconds for a signed certificate.  Log on to the Puppet
   master and run ``puppetserver ca sign --certname <puppet.agent.fqdn>``.

Upon successful deployment of a new client, it is highly recommended that
:ref:`LDAP administrative accounts <Managing LDAP Users>` be created.

.. _cm-troubleshoot-puppet-issues:

Troubleshooting Puppet Issues
-----------------------------

If the client has been kickstarted, but is not communicating with the Puppet
master, try the following options:

* Check the forward and reverse :term:`DNS` entries on the client and server;
  both must be correct. The :command:`nslookup` command will help here.
* Check the time on the systems. More than an hour's difference will cause
  serious issues with certificates.
* Remove :file:`/etc/puppetlabs/puppet/ssl` on the client system; run
  :command:`puppetserver ca clean --certname <CLIENT.FQDN>` on the
  Puppet Server and try again.

If you are getting permission errors, make sure the SELinux context is correct
on all files, as well as the owner and group permissions.

.. _cm-troubleshoot-cert-issues:


Troubleshooting Certificate Issues
----------------------------------

If host certificates do not appear to be working, ensure that all certificates
verify against the installed :term:`CA` certificates.

The table below lists the steps to determine which certificates are working and
which are not.

#. Navigate to :file:`/var/simp/environments/production/site_files/pki_files/files/keydist/`
#. Run :command:`find . -name "*<YOUR.DOMAIN>.pub” -exec openssl verify -CApath cacerts {} \;`

   The screen displays ``./<Host Name>.<Your.Domain>/<Hostname>.<Your.Domain>.pub: OK``
   If anything other than OK appears for each host, analyze the error and ensure
   that the CA certificates are correct.

   If the ``TXT_DB`` error number **2** appears, revoke the certificate that is
   being regenerated. The table below lists the steps to revoke the certificate.

#. Navigate to the directory containing the CA certificates.  For the FakeCA,
   it is :file:`/var/simp/environments/production/FakeCA/`.  The directory
   should contain the file :file:`default.cnf`.

#. Run

   .. code-block:: bash

     OPENSSL_CONF=default.cnf openssl ca -revoke /var/simp/environments/production\
     /site_files/pki_files/files/keydist/*<Host to Revoke>*/*<Host to Revoke>*.pub

.. _BIND Documentation: https://www.isc.org/bind/

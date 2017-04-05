.. _Client_Management:


Client Management
=================

This chapter provides guidance to install and configure SIMP clients, via
kickstart, with the resources supplied by the SIMP ISO.

This guide also assumes that your SIMP server is a :term:`yum` package repository.


System Requirements
-------------------

Client systems should meet the following minimum requirements:

-  Hardware/:term:`Virtual Machine` (VM) : Capable of running RHEL 6 or 7 x86_64
-  RAM: 512 MB
-  HDD: 20 GB


Configuring the Puppet Master
-----------------------------

Perform the following actions as ``root`` on the Puppet Master system **prior**
to attempting to install a client.


Add the Kickstart server profile
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the Puppet server-specific hiera file (by default located at
``/etc/puppetlabs/code/environments/simp/hieradata/hosts/puppet.<your.domain>.yaml``),
add the ``simp::server::kickstart`` class.

.. code-block:: yaml

  ---
  classes:
    - simp::server::kickstart

This profile class adds management of bind_dns and named, as well as sets up the
example provisioning script.

After adding the above class, run puppet: ``puppet agent -t``.


Configure DNS
^^^^^^^^^^^^^

In SIMP, numerous and/or large configuration files are distributed via
``rsync`` by Puppet to minimize management cost. These managed files presently
include DNS configuration files and can be found at
``/var/simp/environments/simp/rsync/<OSTYPE>/<MAJORRELEASE>/bind_dns/default``.

This section is not a complete manual for named. For more complete documentation
on how to set up named, see named(8) and named.conf(5).

The following configuration steps are for a SIMP-managed setup. However, you
can use an existing DNS infrastructure.

#. Navigate to ``/var/simp/environments/simp/rsync/<OSTYPE>/<MAJORRELEASE>/bind_dns/default``

#. Modify the ``named`` files to correctly reflect the environment.

   * The relevant files under ``bind_dns/default`` are as follows:

     * ``named/etc/named.conf``
     * ``named/etc/zones/your.domain``
     * ``named/var/named/forward/your.domain.db``
     * ``named/var/named/reverse/0.0.10.db``

   * Review ``named/etc/named.conf`` and update the following:

     * Update the :term:`IP` for allow-query and allow-recursion
     * Delete any unnecessary zone stanzas (i.e. forwarding) if not
       necessary
     * Substitute in the :term:`FQDN` of your domain for all occurrences of
       ``your.domain``

   * Add clients to ``named/var/named/forward/your.domain.db`` and
     ``named/var/named/reverse/0.0.10.db`` and then rename these files
     to appropriately match your environment.

#. Type ``puppet agent -t --tags named`` on the Puppet Master to apply
   the changes.
#. Validate DNS and ensure the ``/etc/resolv.conf`` is updated appropriately.
#. If an error about the ``rndc.key`` appears when starting ``named``, see the
   `Bind Documentation <https://www.isc.org/downloads/bind/>`_.  Once you have
   resolved the issue, re-run the puppet command ``puppet agent -t`` on the
   Puppet Master to apply.

.. NOTE::

   You can adjust the list of clients in your
   ``named/var/named/forward/<your.domain>.db`` and
   ``named/var/named/reverse/<your reverse domain>.db`` files at any time.
   Just remember to run ``puppet agent -t --tags named`` on the Puppet Server
   to propagate these updates.


Configure DHCP
^^^^^^^^^^^^^^

Perform the following actions as ``root`` on the Puppet Master system
prior to attempting to install a client.

Open the ``/var/simp/environments/simp/rsync/<OSTYPE>/Global/dhcpd/dhcpd.conf`` file
and edit it to suit the necessary environment.

Make sure the following is done in the ``dhcpd.conf`` :

  * The ``next-server`` setting in the ``pxeclients`` class block points to the
    IP Address of the :term:`TFTP` server.
  * Create a Subnet block and edit the following:

    - Make sure the **router** and **netmask** are correct for your
      environment.
    - Enter the hardware ethernet and fixed-address for each client that will
      be kickstarted.  For increased security, it is suggested that SIMP
      environments not allow clients to pick random IP Address in a subnet.
      The MAC address must be associated with and IP Address here. (You can add
      additional ones as needed.)
    - Enter the domain name for option **domain-name**
    - Enter the IP Address of the DNS server for option **domain-name-servers**

Save and close the file.

Run ``puppet agent -t`` on the Puppet Master to apply the changes.

.. _PXE_Boot:

.. include:: PXE_Boot.rst

.. _Certificates:

.. include:: Certificates.rst


Setting Up the Client
---------------------

The following lists the steps to :term:`PXE` boot the system and set up the
client.

#. Set up your client's :term:`BIOS` or virtual settings to boot off the
   network.
#. Make sure the :term:`MAC` address of the client is set up in :term:`DHCP`
   (see `Configure DHCP`_ for more info.)
#. Restart the system.
#. Once the client installs, reboots, and begins to bootstrap, it will check in
   for the first time.
#. Puppet will not autosign puppet certificates by default and waitforcert is
   enabled. The client will check in every 30 seconds for a signed cert. Log on
   to the puppet server and run ``puppet cert sign <puppet.client.fqdn>``.

Upon successful deployment of a new client, it is highly recommended that
:ref:`LDAP administrative accounts <Managing LDAP Users>` be created.


Troubleshooting Puppet Issues
-----------------------------

If the client has been kickstarted, but is not communicating with the Puppet
server, try the following options:

* Check the forward and reverse :term:`DNS` entries on the client and server;
  both must be correct. The ``nslookup`` command will help here.
* Check the time on the systems. More than an hour's difference will cause
  serious issues with certificates.
* Remove ``/etc/puppetlabs/puppet/ssl`` on the client system; run ``puppet cert
  --clean ***<Client Host Name>***`` on the Puppet server and try again.

.. _cm-troubleshoot-cert-issues:


Troubleshooting Certificate Issues
----------------------------------

If host certificates do not appear to be working, ensure that all certificates
verify against the installed :term:`CA` certificates.

The table below lists the steps to determine which certificates are working and
which are not.

#. Navigate to ``/var/simp/environments/simp/site_files/pki_files/files/keydist``
#. Run ``find . -name “****<your.domain>*.pub” -exec openssl verify -CApath cacerts {} \;``

   The screen displays ``./<Host Name>.<Your.Domain>/<Hostname>.<Your.Domain>.pub: OK``
   If anything other than OK appears for each host, analyze the error and ensure
   that the CA certificates are correct.

   If the ``TXT_DB`` error number **2** appears, revoke the certificate that is
   being regenerated. The table below lists the steps to revoke the certificate.

#. Navigate to ``/var/simp/environments/simp/site_files/pki_files/files/keydist``
#. Run

   .. code-block:: bash

     OPENSSL_CONF=default.cnf openssl ca -revoke \
     keydist/*<Host to Revoke>*/*<Host to Revoke>*.pub

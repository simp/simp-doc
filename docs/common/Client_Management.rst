.. _Client_Management:

Client Management
=================

This chapter provides guidance to install and configure SIMP clients
based on the standard SIMP system installed using the SIMP DVD.

System Requirements
-------------------

Before installing clients, the system should consist of the following
minimum requirements:

-  Hardware/:term:`Virtual Machine` (VM) : Capable of running RHEL 6 or 7 ; 64-bit compatible

-  RAM: 512 MB

-  HDD: 5 GB

Configuring the Puppet Master
-----------------------------

Perform the following actions as ``root`` on the Puppet Master system
prior to attempting to install a client.

Configure DNS
-------------

Most static files are pulled over ``rsync`` by Puppet in this
implementation for network efficiency. Specific directories of interest
are noted in this section.

It is possible to use an existing DNS setup; however, the following
table lists the steps for a local setup.

.. only:: simp_4

  1. Navigate to ``/srv/rsync/bind_dns``

.. only:: not simp_4

  1. Navigate to ``/var/simp/rsync/OSTYPE/MAJORRELEASE/bind_dns``
2. Modify the named files to correctly reflect the environment. At a
   minimum, the following files under ``/srv/rsync/bind_dns/default``
   should be edited:

  * ``named/etc/named.conf``
  * ``named/etc/zones/your.domain``
  * ``named/var/named/forward/your.domain.db``
  * ``named/var/named/reverse/0.0.10.db``

.. important::

  For the ``named/var/named/forward/your.domain.db`` and
  ``named/var/named/reverse/0.0.10.db`` files, add clients as needed.
  Make sure to rename both of these files to more appropriately match
  your system configuration.

* At a minimum, review ``named/etc/named.conf`` and check/update the
   following:

  * Update the :term:`IP` for allow-query and allow-recursion
  * Delete any unnecessary zone stanzas (i.e. forwarding) if not
    necessary
  * Substitute in the :term:`FQDN` of your domain for all occurrences of
    your.domain

#. Type ``puppet agent -t --tags named`` on the Puppet Master to apply
   the changes. Validate DNS and ensure the ``/etc/resolv.conf`` is
   updated appropriately
#. If an error about the rndc.key appears when starting bind, copy the
   ``rndc.key`` to ``/etc`` then re-run the puppet command: ``cp -p
   /var/named/chroot/etc/rndc.key /etc/rndc.key``

Configure DHCP
--------------

Perform the following actions as ``root`` on the Puppet Master system
prior to attempting to install a client.

.. only:: simp_4

 Open the ``/srv/rsync/dhcpd/dhcpd.conf`` file and edit it to suit the
 necessary environment.

.. only:: not simp_4

 Open the ``/var/simp/rsync/OSTYPE/MAJORRELEASE/dhcpd/dhcpd.conf`` file
 and edit it to suit the necessary environment.

Make sure the following is done in the ``dhcpd.conf`` :

  * The ``next-server`` setting in the pxeclients class block points to
    the IP Address of the :term:`TFTP` server.
  * Create a Subnet block and edit the following:

    - Make sure the **router** and **netmask** are correct for your
      environment.
    - Enter the hardware ethernet and fixed-address for each client that will
      be kickstarted.  SIMP environments should not allow clients to pick
      random IP Address in a subnet.  The MAC address must be associated with
      and IP Address here. (You can add additional ones as needed.)
    - Enter the domain name for option **domain-name**
    - Enter the IP Address of the DNS server for option **domain-name-servers**

Save and close the file.

Run ``puppet agent -t`` on the Puppet Master to apply the changes.

.. include:: ../common/PXE_Boot.rst

Setting Up the Client
---------------------

The following lists the steps to :term:`PXE` boot the system and set up
the client.

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
:ref:`LDAP administrative accounts <ldap_user_management>` be created.

Troubleshooting Issues
----------------------

If the client has been kickstarted, but is not communicating with the Puppet
server, try the following options:

* Check the forward and reverse :term:`DNS` entries on the client and server;
  both must be correct.
* Check the time on the systems. More than an hour's difference will cause
  serious issues with certificates.
* Remove ``/var/lib/puppet/ssl`` on the client system; run ``puppet cert
  --clean ***<Client Host Name>***`` on the Puppet server; and try again.

Troubleshoot Certificate Issues
-------------------------------

If host certificates do not appear to be working and the banner is not getting
rsync'd to the clients, ensure that all certificates verify against the
installed :term:`CA` certificates.

The table below lists the steps to determine which certificates are working and
which are not.

#. Navigate to ``/etc/puppet/environments/simp/keydist``
#. Run ``find . -name “****<Your.Domain>*.pub” -exec openssl verify -CApath cacerts {} \;``

.. important::

  The screen displays ``./<Host Name>.<Your.Domain>/<Host
  Name>.<Your.Domain>.pub: OK`` If anything other than OK appears for each
  host, analyze the error and ensure that the CA certificates are correct.

If the TXT_DB error number 2 appears, revoke the certificate that is being
regenerated. The table below lists the steps to revoke the certificate.

#. Navigate to ``/etc/puppet/environments/simp/keydist;``
#. Run ``OPENSSL_CONF=default.cnf openssl ca -revoke ../../keydist/***<Host to Revoke>*/*<Host to Revoke>*.pub**``

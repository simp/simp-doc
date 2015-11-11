.. _Client_Management:

Client Management
=================

This chapter provides guidance to install and configure SIMP clients based on the standard SIMP system installed using the SIMP DVD.

System Requirements
-------------------

Before installing clients, the system should consist of the following
minimum requirements:

-  Hardware/:term:`Virtual Machine` (VM) : Capable of running RHEL 6 or 7 ; 64-bit compatible

-  RAM: 512 MB

-  HDD: 5 GB

Configuring the Puppet Master
-----------------------------

Perform the following actions as ``root`` on the Puppet Master system prior to attempting to install a client.

Configure DNS
-------------

Most static files are pulled over ``rsync`` by Puppet in this
implementation for network efficiency. Specific directories of interest
are noted in this section.

It is possible to use an existing DNS setup; however, the following
table lists the steps for a local setup.

.. only:: simp_4
   
 1. Type ``cd /srv/rsync/bind_dns``

.. only:: not simp_4

 1. Type ``cd /var/simp/rsync/OSTYPE/MAJORRELEASE/bind_dns``

2. Modify the named files to correctly reflect the environment. At a minimum, the following files under ``/srv/rsync/bind_dns/default`` should be edited:

  A. ``named/etc/named.conf``
  B. ``named/etc/zones/your.domain``
  C. ``named/var/named/forward/your.domain.db``
  D. ``named/var/named/reverse/0.0.10.db``

.. important:: For the ``named/var/named/forward/your.domain.db`` and ``named/var/named/reverse/0.0.10.db`` files, add clients as needed. Make sure to rename both of these files to more appropriately match your system configuration.

3. At a minimum, review ``named/etc/named.conf`` and check/update the following:

  A. Update the IP for allow-query and allow-recursion
  B. Delete any unnecessary zone stanzas (i.e. forwarding) if not necessary
  C. Substitute in the FQDN of your domain for all occurrences of your.domain

4. Type ``puppet agent -t --tags named`` on the Puppet Master to apply the changes. Validate DNS and ensure the ``/etc/resolv.conf`` is updated appropriately
5. If an error about the rndc.key appears when starting bind, copy the ``rndc.key`` to ``/etc`` then re-run the puppet command: ``cp -p /var/named/chroot/etc/rndc.key /etc/rndc.key``

Table: DNS Configuration Procedure

Configure DHCP
--------------

Log on as *root*.

.. only:: simp_4

 Open the ``/srv/rsync/dhcpd/dhcpd.conf`` file and edit it to suit the necessary environment.

.. only:: not simp_4

 Open the ``/var/simp/rsync/OSTYPE/MAJORRELEASE/dhcpd/dhcpd.conf`` file and edit it to suit the necessary environment.

An example ``dhcpd.conf`` is listed in the appendix.  This can be used as a baseline. Make sure the following is done :

  A. The ``next-server`` setting in the pxeclients class block points to the IP Address of the TFTP server.
  B. Create a Subnet block (example of ``192.168.122.0`` in Appendix)  and edit the following:

    -    Make sure the **router** and **netmask** are correct for your environment.
    -    Enter the hardware ethernet and fixed-address for each client that will be kickstarted. (example ``192.168.122.16`` in Appendix.) SIMP does not allow clients to pick random IP Address in a subnet.  The MAC address must be associated with and IP Address here. (You can add additional ones as needed.)
    -    Enter the domain name for option **domain-name** 
    -    Enter the IP Address of the DNS server for option **domain-name-servers**

Save and close the file.
Type ``puppet agent -t --tags dhcpd`` on the Puppet Master to apply the changes.

.. include:: ../common/PXE_Boot.rst

Setting Up the Client
---------------------

The following lists the steps to PXE boot the system and set up the client.

1. Set up your client's bios or virtual settings to boot off the network.
2. Make sure the MAC address of the client is set up in DHCP (see Setting Up DHCP for more info.) 
3. Restart the system.
4. Once the client installs, reboots, and begins to bootstrap, it will check in for the first time.
5. Puppet will not autosign puppet certificates by default and waitforcert is enabled. The client will check in every 30 seconds for a signed cert. Log on to the puppet server and run ``puppet cert sign puppet.client.fqdn``.

Upon successful deployment of a new CentOS or RHEL client, it is highly recommended that LDAP administrative accounts be created. See Chapter 2 of the SIMP Users Guide for user management.

Troubleshooting Issues
----------------------

If the client has been kickstarted, but is not communicating with the Puppet server, try the following options:

-  Check the forward and reverse DNS entries on the client and server; both must be correct.
-  Check the time on the systems. More than an hour's difference will cause serious issues with certificates.
-  Remove ``/var/lib/puppet/ssl`` on the client system; run ``puppet cert --clean ***<Client Host Name>***`` on the Puppet server; and try again.

Troubleshoot Certificate Issues
-------------------------------

If host certificates do not appear to be working and the banner is not getting rsync'd to the clients, ensure that all certificates verify against the installed CA certificates.

The table below lists the steps to determine which certificates are working and which are not.

1. Type ``cd /etc/puppet/environments/simp/keydist``
2. Type ``find . -name “****<Your.Domain>*.pub” -exec openssl verify -CApath cacerts {} \;``

.. important::

    The screen displays ``./<Host Name>.<Your.Domain>/<Host Name>.<Your.Domain>.pub: OK``
    If anything other than OK appears for each host, analyze the error and ensure that the CA certificates are correct.

If the TXT\_DB error number 2 appears, revoke the certificate that is being regenerated. The table below lists the steps to revoke the certificate.

1. Type ``cd /etc/puppet/environments/simp/keydist;``
2. Type ``OPENSSL_CONF=default.cnf openssl ca -revoke ../../keydist/***<Host to Revoke>*/*<Host to Revoke>*.pub**``


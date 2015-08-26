Client Management
=================

This chapter provides guidance to install and configure SIMP clients based on the standard SIMP system installed using the SIMP DVD.

System Requirements
-------------------

Before installing clients, the system should consist of the following
minimum requirements:

-  Hardware/:term:`Virtual Machine (VM)` : Capable of running RHEL 6 or 7 ; 64-bit compatible

-  RAM: 512 MB

-  HDD: 5 GB

Configuring the Puppet Master
-----------------------------

Perform the following actions as ``root`` on the Puppet Master system prior to attempting to install a client.

Configure DHCP
--------------

1. Log on as *root*.
2. Open the ``/srv/rsync/dhcpd/dhcpd.conf`` file and edit it to suit the necessary environment.

    .. note:: Enter the hardware ethernet and fixed-address for each client that will be kickstarted. An example ``dhcpd.conf`` is listed in the Appendix, where ``192.168.1.100`` is the client targeted for PXE/provisioning. Be sure to substitute in the actual values appropriate to your environment, including the ethernet address of the target client.

3. Save and close the file.
4. Type ``puppet agent -t --tags dhcpd`` on the Puppet Master to apply the changes.


Configure DNS
-------------

Most static files are pulled over ``rsync`` by Puppet in this
implementation for network efficiency. Specific directories of interest
are noted in this section.

It is possible to use an existing DNS setup; however, the following
table lists the steps for a local setup.

1. Type ``cd /srv/rsync/bind_dns``
2. Modify the named files to correctly reflect the environment. At a minimum, the following files under ``/srv/rsync/bind_dns/default`` should be edited:

  a. ``named/etc/named.conf``
  b. ``named/etc/zones/your.domain``
  c. ``named/var/named/forward/your.domain.db``
  d. ``named/var/named/reverse/0.0.10.db``

  .. note:: For the ``named/var/named/forward/your.domain.db`` and ``named/var/named/reverse/0.0.10.db`` files, add clients as needed. Make sure to rename both of these files to more appropriately match your system configuration.

3. At a minimum, review ``named/etc/named.conf`` and check/update the following:

  a. Update the IP for allow-query and allow-recursion
  b. Delete any unnecessary zone stanzas (i.e. forwarding) if not necessary
  c. Substitute in the FQDN of your domain for all occurrences of your.domain

4. Type ``puppet agent -t --tags named`` on the Puppet Master to apply the changes. Validate DNS and ensure the ``/etc/resolv.conf`` is updated appropriately
5. If an error about the rndc.key appears when starting bind, copy the ``rndc.key`` to ``/etc`` then re-run the puppet command: ``cp -p /var/named/chroot/etc/rndc.key /etc/rndc.key``

Table: DNS Configuration Procedure

Configure the Kickstart
-----------------------

1. Locate the following files in the ``/var/www/ks`` directory:

  a. ``pupclient_x86_64.cfg``
  b. ``puppet_x86_64.cfg``
  c. ``diskdetect.sh``

2. Open the applicable files and follow the instructions provided within them to replace the variables.

  .. note:: The files that need to be edited vary based on the information entered in the Manifest section. Type ``sed -i ‘s/#KSSERVER#/***<Server IP Address>***/g’ *.cfg`` to set the IP address of the kickstart server.

  .. note:: Use the following command to obtain a hashed value of the passwords that will be changed ``ruby -r 'digest/sha2' -e 'puts "password".crypt("$6$" + rand(36**8).to_s(36))'``

3. Type ``chown root.apache /srv/www/ks/*`` to ensure that all files are owned by ``root`` and in the ``apache`` group.
4. Type ``chmod 640 /srv/www/ks/*`` to change the permissions so the owner can read and write the file and the ``apache`` group can only read. Once this is complete, the system is ready to kickstart the clients.

Table: Kickstart Configuration Procedure

The ``diskdetect.sh`` script is responsible for detecting the first active disk and applying a disk configuration. Edit this file to meet any necessary requirements or use this file as a starting point for further work.

Setting Up the Client
---------------------

The table below lists the steps to PXE boot the system and set up the client.

1. Power up the system and navigate to the **Other Options** menu.
2. Select the **BIOS Setup option**.
3. Select **Enable Onboard NIC**.
4. Select **Enabled with PXE**.

  .. note:: If a virtualization option is available, select that as well.

5. Save the new settings and close.

  .. note:: The system restarts.

6. As the system powers up again, navigate to the **Other Options** menu.
7. Select **Onboard NIC**.

  .. note:: The PXE boot of the system occurs and CentOS or RHEL is installed.

8. Puppet will not autosign domains by default and waitforcert is enabled. The client will check in every 30 seconds for a signed cert.
9. Once the client installs, reboots, and begins to bootstrap, it will check in for the first time. You will be required to run ``puppet cert sign puppet.client.fqdn``.

Table: PXE Boot Procedure

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

  .. note::

    The screen displays ``./<Host Name>.<Your.Domain>/<Host Name>.<Your.Domain>.pub: OK``
    If anything other than OK appears for each host, analyze the error and ensure that the CA certificates are correct.

If the TXT\_DB error number 2 appears, revoke the certificate that is being regenerated. The table below lists the steps to revoke the certificate.

1. Type ``cd /etc/puppet/environments/simp/keydist;``
2. Type ``OPENSSL_CONF=default.cnf openssl ca -revoke ../../keydist/***<Host to Revoke>*/*<Host to Revoke>*.pub**``


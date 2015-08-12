.. _Client_Management:

Client Management
=================

This chapter provides guidance to install and configure SIMP clients
based on the standard SIMP system installed using the SIMP DVD.

System Requirements
-------------------

Before installing clients, the system should consist of the following
minimum requirements:

-  Hardware/:term:`Virtual Machine (VM)` : Capable of running RHEL 6 or 7 ; 64-bit compatible

-  RAM: 512 MB

-  HDD: 5 GB

Configuring the Puppet Master
-----------------------------

Perform the following actions as *root* on the Puppet Master system
prior to attempting to install a client.

Configure DHCP
--------------

The table below lists the steps to configure DHCP.

.. list-table::
   :widths: 8 344
   :header-rows: 1

   * - Step
     - Process/Action
   * - 1.
     - Log on as *root*.
   * - 2.
     - Open the */var/simp/rsync/CentOS/RHEL\_MAJOR\_VERSION/dhcpd/dhcpd.conf* file and edit it to suit the necessary environment.
   * - 
     - 
   * - 
     - **NOTE**: Enter the hardware ethernet and fixed-address for each client that will be kickstarted. An example dhcpd.conf is listed in the Appendix, where 192.168.1.100 is the client targeted for PXE/provisioning. Be sure to substitute in the actual values appropriate to your environment, including the ethernet address of the target client.
   * - 3.
     - Save and close the file.
   * - 4.
     - Type ``puppet agent -t --tags dhcpd`` on the Puppet Master to apply the changes.

Table: DHCP Configuration Procedure

Configure DNS
-------------

Most static files are pulled over *rsync* by Puppet in this
implementation for network efficiency. Specific directories of interest
are noted in this section.

It is possible to use an existing DNS setup; however, the following
table lists the steps for a local setup.


Table: DNS Configuration Procedure

Configure the Kickstart
-----------------------

The table below lists the steps to configure the kickstart.


Table: Kickstart Configuration Procedure

The *diskdetect.sh\** script is responsible for detecting the first
active disk and applying a disk configuration. Edit this file to meet
any necessary requirements or use this file as a starting point for
further work.

Setting Up the Client
---------------------

The table below lists the steps to PXE boot the system and set up the
client.


Table: PXE Boot Procedure

Upon successful deployment of a new CentOS or RHEL client, it is highly
recommended that LDAP administrative accounts be created. See Chapter 2
of the SIMP Users Guide for user management.

Troubleshooting Issues
----------------------

If the client has been kickstarted, but is not communicating with the
Puppet server, try the following options:

-  Check the forward and reverse DNS entries on the client and server;
   both must be correct.

-  Check the time on the systems. More than an hour's difference will
   cause serious issues with certificates.

-  Remove */var/lib/puppet/ssl* on the client system; run **puppet cert
   --clean ***<Client Host Name>***** on the Puppet server; and try
   again.

Troubleshoot Certificate Issues
-------------------------------

If host certificates do not appear to be working and the banner is not
getting rsync'd to the clients, ensure that all certificates verify
against the installed CA certificates.

The table below lists the steps to determine which certificates are
working and which are not.


Table: Certificate Verification Procedure

If the TXT\_DB error number 2 appears, revoke the certificate that is
being regenerated. The table below lists the steps to revoke the
certificate.


Table: Revoke Certificate Procedure

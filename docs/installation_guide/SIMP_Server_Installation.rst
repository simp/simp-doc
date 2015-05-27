SIMP Server Installation
========================

This chapter provides guidance on installing and configuring SIMP using
the *simp config* utility.

System Requirements
-------------------

SIMP scales well, but how much depends on a number of factors, including
the number of nodes, the processor speed, the total memory, and the
complexity of the manifests. The following minimal system requirements are
recommended:

-  :term:`Central Processing Unit (CPU)` : 2 Cores

-  :term:`Random Access Memory (RAM)` :  2.2 GB

-  :term:`Hard Disk Drive (HDD)` : 50 GB

Using the SIMP Utility
----------------------

The SIMP Utility does not assist users through the entire configuration
process; however, it does make the initial configuration easier and more
repeatable.

    **Important**

    Correct time across all systems is important to the proper
    functioning of SIMP and Puppet in general.

    If a user has trouble connecting to the Puppet server and errors
    regarding certificate validation appear, check the Puppet server and
    client times to ensure they are synchronized.

Using the configuration script, the following items are configured:

**NOTE** this needs updated for the new puppetserver settings 
which includes Puppet Environments

-  Grub password in */boot/grub/grub.conf*

-  Basic network setup

-  Autosigning in */etc/puppet/autosign.conf*

-  Fileserving in */etc/puppet/fileserver.conf*

-  Puppet server and Certificate Authority (CA) information in
   */etc/puppet/puppet.conf*

-  */etc/puppet/hieradata/simp\_def.yaml*

-  Server certificates for the puppet server itself (Fake CA)

-  Base YUM repositories

    **Warning**

    Keep in mind as the installation process begins that Puppet does not
    work well with capital letters in host names. Therefore, they should
    not be used.

Below is a table containing the default passwords found on a basic SIMP
server.

    **Important**

    All default passwords should be changed during the initial
    configuration proceess.

+-------------+-----------------------------------+
| Utility     | Password                          |
+=============+===================================+
| Grub        | Initi@lGruubCredential$           |
+-------------+-----------------------------------+
| Root User   | Plea$e Ch@nge Th1s Immediately!   |
+-------------+-----------------------------------+
| Simp User   | CorrectHorseBatteryStaple         |
+-------------+-----------------------------------+

Table: SIMP Default Passwords

Below is a table containing sample variables and their corresponding
values that apply to this SIMP deployment. These variables and values
will be helpful in illustrating how configuration files are set up. Your
values will obviously differ, depending on your installation
environment.

+------------------------+-------------------------------------------+
| Variable name          | Value                                     |
+========================+===========================================+
| Domain name            | simp.net                                  |
+------------------------+-------------------------------------------+
| Fully qualified name   | puppet.simp.net                           |
+------------------------+-------------------------------------------+
| IP address             | 192.168.1.10                              |
+------------------------+-------------------------------------------+
| Gateway                | 192.168.1.1                               |
+------------------------+-------------------------------------------+
| DNS server             | 192.168.1.10                              |
+------------------------+-------------------------------------------+
| DNS search entry       | simp.net                                  |
+------------------------+-------------------------------------------+
| Kickstart server       | 192.168.1.10                              |
+------------------------+-------------------------------------------+
| Yum server             | 192.168.1.10                              |
+------------------------+-------------------------------------------+
| LDAP URI               | ldap://puppet.simp.net                    |
+------------------------+-------------------------------------------+
| LDAP Base DN           | [dc=simp,dc=net]                          |
+------------------------+-------------------------------------------+
| LDAP Root DN           | [cn=LDAPAdmin,ou=People,dc=simp,dc=net]   |
+------------------------+-------------------------------------------+
| LDAP Bind DN           | [cn=hostAuth,ou=Hosts,dc=simp,dc=net]     |
+------------------------+-------------------------------------------+
| LDAP Sync DN           | [cn=LDAPSync,ou=People,dc=simp,dc=net]    |
+------------------------+-------------------------------------------+

Table: Sample values for SIMP install

Preparing the SIMP Server Environment
-------------------------------------

The following table outlines the steps to prepare a system for SIMP
installation.

+--------+-------------------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                                  |
+========+=================================================================================================+
| 1.     | Boot the system and ensure the SIMP ISO is selected.                                            |
+--------+-------------------------------------------------------------------------------------------------+
| 2.     | Press **Enter** to run the standard SIMP install, or choose from the customized options list.   |
+--------+-------------------------------------------------------------------------------------------------+
| 3.     | When the installation is complete, the system will restart automatically.                       |
+--------+-------------------------------------------------------------------------------------------------+
| 4.     | Log on as *root* and type the default password shown in **Table 2.1.**                          |
+--------+-------------------------------------------------------------------------------------------------+
| 5.     | Type the default password again when prompted for the (current) UNIX password.                  |
|        | Type a new password when prompted for the New Password. Retype the password when prompted.      |
+--------+-------------------------------------------------------------------------------------------------+

Table: SIMP Pre-Install Procedures

Installing the SIMP Server
--------------------------

The following table outlines the steps to install a SIMP server.

+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                                                                                                                                                                                              |
+========+=============================================================================================================================================================================================================================================================+
| 1.     | Log on as *simp*\ and **su -** to gain root access.                                                                                                                                                                                                         |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2.     | Type **simp config**                                                                                                                                                                                                                                        |
|        |                                                                                                                                                                                                                                                             |
|        | Type **simp config -a ***<Config File>***** to load a previously generated configuration instead of generating the configuration from the script. This is the option to run for systems that will be rebuilt often.                                         |
|        |                                                                                                                                                                                                                                                             |
|        | For a list of additional commands, type **simp help**. Type **simp help ***<Command>***** for more information on a specific command.                                                                                                                       |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 3.     | Configure the system as prompted.                                                                                                                                                                                                                           |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 4.     | Type **simp bootstrap**                                                                                                                                                                                                                                     |
|        |                                                                                                                                                                                                                                                             |
|        | **NOTE**: If progress bars are of equal length and the bootstrap finishes quickly, a problem has occured. This is most likely due to an error in SIMP configuration. Refer to the previous step and make sure that all configuration options are correct.   |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 5.     | Type **reboot**                                                                                                                                                                                                                                             |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Table: SIMP Install Procedure

Performing Post-installation Setup on the SIMP Server
-----------------------------------------------------

The following table outlines the SIMP post-installation procedures.

+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                                                                                                       |
+========+======================================================================================================================================================================+
| 1.     | Log on as *root*                                                                                                                                                     |
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2.     | Run puppet for the first time. Errors will appear for DHCP. These can be safely ingored at this stage. Type:                                                         |
|        | **puppet agent -t**                                                                                                                                                  |
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 3.     | Copy CentOS RHEL\_MAJOR\_MINOR\_VERSION ISO(s) to the server and unpack using the unpack\_dvd utility. This creates a new tree under /var/www/yum/CentOS. Execute:   |
|        | **unpack\_dvd CentOS-RHEL\_MAJOR\_MINOR\_VERSION-\ *####*-x86\_64-Everything.iso**                                                                                   |
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 4.     | Update your system using yum. The updates applied will be dependent on what ISO you initially used. Execute:                                                         |
|        | **yum clean all; yum makecache**                                                                                                                                     |
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 5.     | Run puppet. Ignore the same DHCP errors.                                                                                                                             |
|        | **puppet agent -t**                                                                                                                                                  |
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 6.     | Type **reboot**                                                                                                                                                      |
+--------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Table: SIMP Post-Installation Procedure

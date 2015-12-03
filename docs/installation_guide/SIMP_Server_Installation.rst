SIMP Server Installation
========================

This chapter provides guidance on installing and configuring SIMP using
the ``simp config`` utility.

System Requirements
-------------------

SIMP scales well, but how much depends on a number of factors, including
the number of nodes, the processor speed, the total memory, and the
complexity of the manifests. The following minimal system requirements are
recommended:

-  :term:`Central Processing Unit` (CPU) : 2 Cores

-  :term:`Random Access Memory` (RAM) :  2.2 GB

-  :term:`Hard Disk Drive` (HDD) : 50 GB

Using the SIMP Utility
----------------------

The SIMP Utility does not assist users through the entire configuration
process; however, it does make the initial configuration easier and more
repeatable.

.. important::

    Correct time across all systems is important to the proper
    functioning of SIMP and Puppet in general.

    If a user has trouble connecting to the Puppet server and errors
    regarding certificate validation appear, check the Puppet server and
    client times to ensure they are synchronized.

..  warning::
    Keep in mind as the installation process begins that Puppet does not
    work well with capital letters in host names. Therefore, they should
    not be used.


SIMP Default Passwords and Settings
-----------------------------------

Below is a table containing the default passwords found on a basic SIMP
server.

.. important::

    All default passwords should be changed during the initial
    configuration process.

Table: SIMP Default Passwords

========= ========
Utility   Password
========= ========
Grub      GrubPassword
Root User RootPassword
Simp User UserPassword
========= ========

A table of settings that can be changed/defined during installation is located in Appendix B, :ref:`List of Installation Variables`.
Review this if you are unfamiliar with SIMP.  

Preparing the SIMP Server Environment
-------------------------------------

1. Boot the system and ensure the SIMP ISO is selected.
2. Press *Enter** to run the standard SIMP install, or choose from the customized options list.
3. When the installation is complete, the system will restart automatically.
4. Log on as ``root`` and type the default password shown in **Table 2.1.**
5. Type the default password again when prompted for the (current) UNIX password.
6. Type a new password when prompted for the New Password. Retype the password when prompted.

Installing the SIMP Server
--------------------------

..  warning::
    Keep in mind as the installation process begins that Puppet does not
    work well with capital letters in host names. Therefore, they should
    not be used.

1. Log on as ``simp`` and run ``su -`` to gain root access.
2. Type ``simp config``

  a. Type ``simp config -a <Config File>`` to load a previously generated configuration instead of generating the configuration from the script. This is the option to run for systems that will be rebuilt often.
  b. For a list of additional commands, type ``simp help``. Type ``simp help ***<Command>***`` for more information on a specific command.
  c.  A list of the variables that are set and more details are contained in :ref:`List of Installation Variables`.

.. note:: Once simp config has been run, a simp config file with all your settings is saved in /root/.simp/simp_conf.yaml

3. Configure the system as prompted.
4. Type ``simp bootstrap``

.. note:: If progress bars are of equal length and the bootstrap finishes quickly, a problem has occurred. This is most likely due to an error in SIMP configuration. Refer to the previous step and make sure that all configuration options are correct.

5. Type ``reboot``

Performing Post-installation Setup on the SIMP Server
-----------------------------------------------------

1. Log on as ``root``
2. Run puppet for the first time. Errors will appear for DHCP. These can be safely ignored at this stage. Type: ``puppet agent -t``
3. Copy CentOS RHEL\_MAJOR\_MINOR\_VERSION ISO(s) to the server and unpack using the ``unpack_dvd`` utility. This creates a new tree under ``/var/www/yum/CentOS``. Execute: ``unpack_dvd CentOS-RHEL_MAJOR_MINOR_VERSION- *####*-x86_64-Everything.iso``
4. Update your system using yum. The updates applied will be dependent on what ISO you initially used. Execute: ``yum clean all; yum makecache``
5. Run puppet. Ignore the same DHCP errors: ``puppet agent -t``
6. Type ``reboot``

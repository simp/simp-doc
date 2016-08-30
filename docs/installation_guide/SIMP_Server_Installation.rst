SIMP Server Installation
========================

This chapter provides guidance on installing, configuring, and bootstrapping
the SIMP server using the SIMP Utility, ``simp``.

System Requirements
-------------------

SIMP scales well, but how much depends on a number of factors, including the
number of nodes, the processor speed, the total memory, and the complexity of
the manifests. The following are the minimal system requirements for the
SIMP server:

-  :term:`Central Processing Unit` (CPU) : 2 Cores
-  :term:`Random Access Memory` (RAM) :  2.2 GB
-  :term:`Hard Disk Drive` (HDD) : 50 GB

Using the SIMP Utility
----------------------

In these instructions we will be using the ``config`` and ``bootstrap``
commands of the SIMP Utility, ``simp``.   The SIMP Utility does not
assist users through the entire configuration process; however, it does
make the initial configuration easier and more repeatable.  

.. note::
   For a list of the commands ``simp`` provides, type ``simp help``. Type
   ``simp help <Command>`` for more information on a specific command.

SIMP Default Passwords and Settings
-----------------------------------

Below is a table containing the default passwords found on a basic SIMP server
upon install.

.. important::
    All default passwords must be changed during the initial configuration
    process.

Table: SIMP Default Passwords

========= ============
Utility   Password
========= ============
Grub      GrubPassword
Root User RootPassword
Simp User UserPassword
========= ============

A table of variables that can be changed/defined during installation is located
in :ref:`List of Installation Variables`.  Review this if you are unfamiliar 
with SIMP, as you will be prompted for the values for these variables during
the SIMP server installation.

Preparing the SIMP Server Environment
-------------------------------------

1. Boot the system and ensure the SIMP ISO is selected.

  - If you do not have a SIMP ISO, see :ref:`SIMP ISO`.

2. Press *Enter* to run the standard SIMP install, or choose from the
   customized options list.

   - For a detailed description of the the disk encryption enabled via the
     ``simp_disk_crypt`` boot option, see :ref:`ig-disk-encryption`.

3. When the installation is complete, the system will restart automatically.
4. Change the ``root`` user password

   a. At the console, log on as ``root`` and type the default password shown
      in **Table 2.1.**
   b. Type the default password again when prompted for the (current) UNIX
      password.
   c. Type a new password when prompted for the New Password. Retype the
      password when prompted.
5. Change the ``simp`` user password

   a. At the console, log on as ``simp`` and type the default password shown
      in **Table 2.1.**
   b. Type the default password again when prompted for the (current) UNIX
      password.
   c. Type a new password when prompted for the New Password. Retype the
      password when prompted.


Installing the SIMP Server
--------------------------

.. important::
    Correct time across all systems is important to the proper functioning of
    SIMP and Puppet in general.

    If a user has trouble connecting to the Puppet server and errors regarding
    certificate validation appear, check the Puppet server and client times to
    ensure they are synchronized.

..  warning::
    Keep in mind as the installation process begins that Puppet does not
    work well with capital letters in host names. Therefore, they should
    not be used.

1. Log on as ``simp`` and run ``su -`` to gain root access.
2. Type ``simp config``

  - Type ``simp config -a <Config File>`` to load a previously generated
    configuration, instead of being prompted for settings.  This is the
    option to run for systems that will be rebuilt often.
  - For a list of additional options, type ``simp help config``. 

3. Configure the system as prompted.

  - ``simp config`` will present you with a recommendation for each variable
    that may be derived from existing OS settings.  To keep a recommended
    value, press *Enter*. Otherwise, enter your desired value.
  - A list of the variables that are set by ``simp config`` is contained in
    :ref:`List of Installation Variables`.
  - A description of the installation preparation actions taken by
    ``simp config``, in addition to the generation of a SIMP configuration
    file, is contained in :ref:`simp config Actions`.

.. note::
  Once ``simp config`` has been run, a SIMP configuration file with all your
  settings is written to ``/etc/puppet/environments/simp/hieradata/simp_def.yaml``
  and also archived in ``/root/.simp/simp_conf.yaml``.

4. Type ``simp bootstrap``

.. note::
  If progress bars are of equal length and the bootstrap finishes quickly, a
  problem has occurred. This is most likely due to an error in SIMP
  configuration. Refer to the previous step and make sure that all
  configuration options are correct.

5. Type ``reboot``

Performing Post-installation Setup on the SIMP Server
-----------------------------------------------------

1. Log on as ``root``
2. Run puppet for the first time. Errors will appear for DHCP. These can be
   safely ignored at this stage. 

   Type: ``puppet agent -t``

3. Copy CentOS RHEL\_MAJOR\_MINOR\_VERSION ISO(s) to the server and unpack
   using the ``unpack_dvd`` utility. This creates a new tree under
   ``/var/www/yum/CentOS``.

   Type: ``unpack_dvd CentOS-RHEL_MAJOR_MINOR_VERSION- *####*-x86_64-Everything.iso``

4. Update your system using yum. The updates applied will be dependent on what
   ISO you initially used. 

   Type: ``yum clean all; yum makecache``

5. Run puppet. Ignore the same DHCP errors: 

   Type: ``puppet agent -t``

6. Reboot your system:

   Type ``reboot``

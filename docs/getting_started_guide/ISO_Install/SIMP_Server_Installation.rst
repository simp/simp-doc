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
-  :term:`Random Access Memory` (RAM) : 2.2 GB
-  :term:`Hard Disk Drive` (HDD) : 50 GB

Using the SIMP Utility
----------------------

In these instructions we will be using the ``config`` and ``bootstrap``
commands of the SIMP Utility, ``simp``.   The SIMP Utility does not
assist users through the entire configuration process; however, it does
make the initial configuration easier and more repeatable.

.. NOTE::
   For a list of the commands ``simp`` provides, type ``simp help``. Type
   ``simp help <Command>`` for more information on a specific command.

.. _ig-default-passwords:

SIMP Default Passwords
----------------------

Below is a table containing the default passwords found on a basic SIMP server
upon install.

.. IMPORTANT::
    All default passwords must be changed during the initial configuration
    process.

========= ============
Utility   Password
========= ============
Grub      GrubPassword
Root User RootPassword
Simp User UserPassword
========= ============

Table: SIMP Default Passwords

Preparing the SIMP Server Environment
-------------------------------------

#. Boot the system and ensure the SIMP ISO is selected.

   - If you do not have a SIMP ISO, see :ref:`gsg-building_simp_from_tarball`.

#. Press *Enter* to run the standard SIMP install, or choose from the
   customized options list.

   - For a detailed description of the the disk encryption enabled via the
     ``simp_disk_crypt`` boot option, see :ref:`ig-disk-encryption`.

#. When the installation is complete, the system will restart automatically.
#. Change the ``root`` user password

   a. At the console, log on as ``root`` and type the default password shown
      in **Table 2.1.**
   b. Type the default password again when prompted for the (current) UNIX
      password.
   c. Type a new password when prompted for the New Password. Retype the
      password when prompted.

#. Change the ``simp`` user password

   a. At the console, log on as ``simp`` and type the default password shown
      in **Table 2.1.**
   b. Type the default password again when prompted for the (current) UNIX
      password.
   c. Type a new password when prompted for the New Password. Retype the
      password when prompted.


Installing the SIMP Server
--------------------------

.. IMPORTANT::
    Correct time across all systems is important to the proper functioning of
    SIMP and Puppet in general.

    If a user has trouble connecting to the Puppet server and errors regarding
    certificate validation appear, check the Puppet server and client times to
    ensure they are synchronized.

.. WARNING::
    Keep in mind as the installation process begins that Puppet does not
    work well with capital letters in host names. Therefore, they should
    not be used.

1. Log on as ``simp`` and run ``su -`` to gain root access.
2. Type ``simp config`` and configure the system as prompted.

  - ``simp config`` will prompt you for system settings and then apply the 
    smallest settings subset that is required to bootstrap the system.
  - When applicable, ``simp config`` will present you with a
    recommendation for each setting (variable).  To keep a recommended
    value, press *Enter*. Otherwise, enter your desired value.
  - ``simp config``  generates a log file containing details of the
    configuration selected and actions taken.
  - For more details about the installation variables set by ``simp config``
    and the corresponding actions, see :ref:`Initial_Configuration`.
  - For a list of additional options, type ``simp help config``.

    - ``simp config --dry-run`` will run through all of the ``simp config``
      prompts without applying any changes to the system. This is the
      option to run to become familiar with the variables set by
      ``simp config`` or generate a configuration file to be used as
      a template for subsequent ``simp config`` runs.
    - ``simp config -a <Config File>`` will load a previously generated
      configuration in lieu of prompting for settings, and then apply the
      settings.  This is the option to run for systems that will be rebuilt
      often.

.. NOTE::
  Once ``simp config`` has been run, three SIMP configuration files
  will be generated:

  - ``/root/.simp/simp_conf.yaml``: File containing  all your
    ``simp config`` settings; can include additional settings related
    to ones you entered and other settings required for SIMP.
  - ``/etc/puppetlabs/code/environments/simp/hieradata/simp_config_settings.yaml``:
    File containing global hieradata relevant to SIMP clients and
    the SIMP server.
  - ``/etc/puppetlabs/code/environments/simp/hieradata/hosts/<host>.yaml``:
    SIMP server host YAML file.

3. Type ``simp bootstrap``

.. NOTE::
  If progress bars are of equal length and the bootstrap finishes quickly, a
  problem has occurred. This is most likely due to an error in SIMP
  configuration. Refer to the previous step and make sure that all
  configuration options are correct.

4. Type ``reboot``

Performing Post-installation Setup on the SIMP Server
-----------------------------------------------------

#. Log on as ``root``
#. Run puppet for the first time.

   Type: ``puppet agent -t``

#. Copy CentOS RHEL\_MAJOR\_MINOR\_VERSION ISO(s) to the server and unpack
   using the ``unpack_dvd`` utility. This creates a new tree under
   ``/var/www/yum/CentOS``.

   Type: ``unpack_dvd CentOS-RHEL_MAJOR_VERSION-x86_64-DVD-####.iso``

#. Update your system using yum. The updates applied will be dependent on what
   ISO you initially used.

   Type: ``yum clean all; yum makecache``

#. Run puppet.

   Type: ``puppet agent -t``

#. Reboot your system:

   Type ``reboot``


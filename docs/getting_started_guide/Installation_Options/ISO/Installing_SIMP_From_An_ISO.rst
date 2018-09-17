.. _gsg-installing_simp_from_an_iso:

Installing SIMP From An ISO
===========================

The benefits of using a SIMP ISO are:

* Suitable for enclave or offline environments
* It is the easiest way to get started and ensure that all files are present
* Your SIMP load will have a disk partitioning scheme compatible with most
  security guides
* Your system will start in :term:`FIPS` mode
* Your disks can be encrypted

  * Please pay attention to the caveats in the :ref:`ig-disk-encryption`
    section

Obtaining the ISO
-----------------

The SIMP ISO can be downloaded from the `official SIMP ISO Share`_.

Alternatively, you can compile your own ISO by following the documentation in
:ref:`gsg-building_a_simp_iso`.

Installation
------------

The ISO will install on any system that supports the underlying operating system.

When you first boot the ISO, there will be a menu of options. You can either
modify the installation according to those instructions or simply hit
``<Enter>`` to proceed with the automated installation.

.. WARNING::

   There are default passwords present on the system that should be changed
   prior to deploying the system.

   **Please make sure that you change these passwords!**

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

.. include:: ../jump_to_config.inc

.. _official SIMP ISO Share: https://download.simp-project.com/simp/ISO

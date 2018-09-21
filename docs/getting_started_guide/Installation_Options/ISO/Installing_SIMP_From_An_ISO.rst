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

.. WARNING::

   There are default passwords present on the system that should be changed
   prior to deploying the system.

   **Please make sure that you change these passwords!**

Preparing the SIMP Server Environment
-------------------------------------

#. Boot the system using the SIMP ISO

#. Press *Enter* to run the standard SIMP install, or choose from the
   customized options list.

   - For a detailed description of the the disk encryption enabled via the
     ``simp_disk_crypt`` boot option, see :ref:`ig-disk-encryption`.

   .. NOTE::

      Once installation starts, you may see the graphical interface spawn. You
      should not interact with the GUI unless you have selected the option to
      manage your own disk partitions.

      Also, if you have chosen to encrypt your disks, you may want to generate
      some system entropy by smashing on the keyboard for a bit.

#. When the installation is complete, the system will restart automatically.
#. Change the ``root`` user password

   a. At the console, log on as ``root`` and type the default password shown
      in :ref:`ig-default-passwords`
   b. Follow the prompts to complete the password change

      * See the :ref:`faq-password-complexity` FAQ for tips on setting a
        functional password.

#. Change the ``simp`` user password

   a. At the console, log on as ``simp`` and type the default password shown
      in :ref:`ig-default-passwords`
   b. Follow the prompts to complete the password change

      * See the :ref:`faq-password-complexity` FAQ for tips on setting a
        functional password.

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

.. include:: ../jump_to_config.inc

.. _official SIMP ISO Share: https://download.simp-project.com/simp/ISO

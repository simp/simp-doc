.. _gsg-installing_simp_from_an_iso:

Installing SIMP from an ISO
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

Install as follows:

#. Boot the system using the SIMP ISO

#. Press *Enter* to run the standard SIMP install, or choose from the
   customized options list.

   .. NOTE::

      * For a detailed description of the disk encryption enabled via boot options,
        see :ref:`ig-disk-encryption`.

      * Once installation starts, you may see the graphical interface spawn. You
        should not interact with the GUI **unless** you have selected the option
        to manage your own disk partitions.

      * When you have opted to manage your own disk partitions, follow the GUI
        instructions to enter your partition scheme. For example, for SIMP for
        CentOS 7, select the ``INSTALLATION DESTINATION`` menu, enter the
        selected partitioning, select the ``DONE`` button to finalize your disk
        selections, and then select the ``Begin Installation`` button on the
        main GUI page to continue. No further GUI interaction will be required.

      * If you have chosen to encrypt your disks, your installation seems to
        be paused, and a messages about increasing entropy appears on the
        screen, you may want to generate some system entropy by pressing random
        keys on the keyboard for a bit.  This will speed up the installation.

#. When the installation is complete, the system will restart automatically.

   .. NOTE::

      * When the system boots it will show: "error on start module sha1 not found could not insert sha_256 [...]". This is expected and is a known issue with FIPS and RedHat, it is safe to ignore.

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

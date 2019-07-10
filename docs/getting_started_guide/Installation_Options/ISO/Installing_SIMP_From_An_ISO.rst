.. _gsg-installing_simp_from_an_iso:

Installing SIMP from an ISO
===========================

SIMP can be installed from a bootable ISO, which provides many advantages:

* Provides a ready-to-go OS, Puppet server, and SIMP installation
* Suitable for use in network-isolated enclaves/offline environments
* Ensures OS is configured with compliance-relevant install-time options:

  * The :ref:`ig-disk-partitioning` scheme is compatible with most security
    guides
  * The OS will boot in :term:`FIPS` mode
  * Disks will be encrypted

    * (Please note the important caveats in the :ref:`ig-disk-encryption`
      section)


Obtaining a SIMP ISO file
-------------------------

You can obtain a SIMP installation ISO using one of the following methods:

#. Downloading an ISO image file

   * Official releases are available at https://download.simp-project.com/simp/ISO/

#. *[Advanced]* :ref:`gsg-building_a_simp_iso` for yourself

   * The contents of the ISO can be customized to your preferences.
   * This is the only way to obtain a SIMP ISO that installs a licensed
     commercial OS, such as Red Hat Enterprise Linux (:term:`RHEL`).


Installing the OS
-----------------

A SIMP ISO will install its OS + SIMP on any host that supports the underlying
operating system.

Install as follows:

#. Boot the system using the SIMP ISO.

   The ISO will load into a screen of boot options.  The presentation will
   differ, depending on the boot firmware and ISO OS:

   .. |bios_boot_options|           image:: ../../../images/screenshots/simp_boot_options.png
      :alt: SIMP boot options screen (BIOS)
   .. |efi_grub097_boot_options|    image:: ../../../images/screenshots/simp_boot_options_efi__grub097.png
      :alt: SIMP boot options screen (UEFI, el6)
   .. |efi_grub2_boot_options|      image:: ../../../images/screenshots/simp_boot_options_efi__grub2.png
      :alt: SIMP boot options screen (UEFI, el7)
   .. |efi_grub2_boot_options_submenu| image:: ../../../images/screenshots/simp_boot_options_efi__grub2_submenu.png
      :alt: SIMP boot options screen (UEFI, el7)


   +------------+----------------------------------+
   | BIOS       | |bios_boot_options|              |
   +------------+----------------------------------+
   | UEFI (el6) | |efi_grub097_boot_options|       |
   +------------+----------------------------------+
   | UEFI (el7) | |efi_grub2_boot_options|         |
   +------------+----------------------------------+
   |            | |efi_grub2_boot_options_submenu| |
   +------------+----------------------------------+


#. Press *Enter* to boot the standard SIMP installer, or customize the
   installation using the boot options.

   .. NOTE::

      For details about how SIMP implements disk encryption (enabled by
      default), see: :ref:`ig-disk-encryption`.

#. Once installation starts, you may see the graphical interface spawn.

   .. WARNING::

      You should NOT interact with the GUI **unless** you have elected to manage
      your own disk partitions (e.g., ``simp-prompt``).

   .. NOTE::

      If you have opted to manage your own disk partitions with
      (e.g., ``simp-prompt``), follow the GUI instructions to enter your
      partition scheme.

      For example, using SIMP for CentOS 7:

        #. Click the ``INSTALLATION DESTINATION`` button
        #. Configure the desired partitioning
        #. Click the ``DONE`` button to finalize your disk selections
        #. Click the ``Begin Installation`` button on the main GUI page to
           continue.

      No further GUI interaction will be required.

   .. TIP::

      When applying disk encryption (enabled by default), the system may seem
      to pause and display messages about increasing entropy. You can speed up
      the installation by pressing random keys on the keyboard for a bit (this
      will generate additional entropy).

#. When the installation is complete, the system will restart automatically.

   .. NOTE::

      When the system boots, it may display: ``error on start module sha1 not
      found could not insert sha_256 [...]``. This is expected and is a known
      issue.

#. Change the default passwords.

   .. WARNING::

      There are default passwords present on the system that should be changed
      prior to deploying the system.

      **Please make sure that you change these passwords!**

   .. NOTE::

      See the :ref:`faq-password-complexity` FAQ for tips on setting a
      functional password.

  a. Change the ``root`` user password.

    i.  At the console, log on as ``root`` and type the default password shown
        in :ref:`ig-default-passwords`
    ii. Follow the prompts to complete the password change

  b. Change the ``simp`` user password.

    i.  At the console, log on as ``simp`` and type the default password shown
        in :ref:`ig-default-passwords`
    ii. Follow the prompts to complete the password change

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

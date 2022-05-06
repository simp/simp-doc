.. _Configure_PXE_Boot:

Configure PXE Boot
==================

Sample kickstart templates have been provided in the :file:`/var/www/ks`
directory on the SIMP server  and on the SIMP DVD under :file:`/ks`. Pre-boot
images are located in the DVD under :file:`/images/pxeboot`. If you have an
existing :term:`Preboot Execution Environment` (PXE) setup you can use these to
PXE a SIMP client. Follow your own sites procedures for this.

In this section we describe how to configure the Kickstart and :term:`TFTP`
servers to PXE boot a SIMP client.  (The DNS and DHCP server setup, also
required for PXE booting, are discussed in an earlier chapter.)

.. NOTE::

   This example sets up a PXE boot for a system that is the same OS as the SIMP
   Server. If you are setting up a PXE boot for a different OS then you must
   make sure that the OS packages are available for all systems you are trying
   to PXE boot through YUM. There are notes throughout the instructions to help
   in setting multiple OS but they are not comprehensive. You should
   understand DHCP, KS, YUM and TFTP relationships for PXE booting before
   attempting this.

.. _Setting_Up_Kickstart:

Setting up Kickstart
--------------------

This section describes how to configure the kickstart server.

#. Add the Kickstart Server Profile

   - In the Puppet server-specific :term:`Hiera` file (by default located at
     :file:`/etc/puppetlabs/code/environments/production/data/hosts/puppet.{your.domain}.yaml`),
     add the :code:`simp::server::kickstart` class.

     .. code-block:: yaml

        ---
        simp::classes:
          - simp::server::kickstart

   - This profile class adds management of DHCP, DNS, the Kickstart service,
     as well as the example provisioning script.

   - After adding the above class, run puppet: :command:`puppet agent -t`.

#. Locate the following files in the :file:`/var/www/ks` directory

   - :file:`pupclient_x86_64.cfg`: Example client kickstart configuration
     script.
   - :file:`diskdetect.sh`:  Example script to determine disks available
     on a system and then apply disk configuration. This script is used
     by :file:`pupclient_x86_64.cfg`.

#. Open the :file:`pupclient_x86_64.cfg` file and follow the instructions
   provided within it to replace the variables listed and to customize for
   :term:`BIOS`/:term:`UEFI` boot and/or FIPS/non-FIPS mode. If you have
   servers that require different boot mode or FIPS options, you will need to
   make customized copies of this file to provide those distinct
   configurations. You will also have to configure TFTP to point to the
   appropriate files.

   - Instructions are provided both at the top of the file and throughout the
     body of the file.
   - You need to know the IP Addresses of the YUM, Kickstart, and TFTP servers.
     (They default to the SIMP server in :program:`simp config`).
   - Use the commands described in the comments at the top of the file to
     generate the root and GRUB passwords hashes. Be sure to replace
     :code:`password` with your root password.

     - The GRUB password can also be managed later with the
       :pupmod:`simp/simp_grub` module. See :ref:`Managing GRUB Users
       <grub_user_management>` for more information.

   - Follow the instructions throughout the file to customize for BIOS/UEFI boot.
   - Follow the instructions throughout the file to customize for FIPS/non-FIPS
     mode.

#. Open the :file:`diskdetect.sh` script and customize the disk device names and/or
   partitions as appropriate for your site. The sample :file:`diskdetect.sh` script
   will work, as is, for most systems, as long as your disk device names are in the
   list. In addition, the sample script provides STIG-compliant partitioning.

#. Type :command:`chown root.apache /var/www/ks/*` to ensure that all files are
   owned by ``root`` and in the ``apache`` group.

#. Type :command:`chmod 640 /var/www/ks/*` to change the permissions so the owner can
   read and write the file and the ``apache`` group can only read.

.. NOTE::

   Two major changes were made to :file:`pupclient_x86_64.cfg` in SIMP 6.2:

   - UEFI PXE support was added.
   - To address timeout issues that caused Puppet bootstrap failures, the use of
     the :program:`runpuppet` script to bootstrap Puppet on the client was
     replaced with the use of two scripts, both provided by the
     :code:`simp::server::kickstart` class:

     - A :program:`systemd` unit file
     - A common bootstrap script (:file:`bootstrap_simp_client`) used by both.

.. NOTE::

   The URLs and locations in the file are set up for a default SIMP install.
   That means the same OS and version as the SIMP server, all servers in one
   location (on the SIMP server) and in specific directories. If you have
   installed these servers in a different location than the defaults, you may
   need to edit URLs or directories.

.. NOTE::

   If you want to PXE boot more than this operating system, make a copy of
   these files, name them appropriately and update URLS and links inside and
   anything else you may need. (You must know what you are doing before
   attempting this.) If you are booting more than one OS you must also make
   sure your YUM server has the OS packages for the other OSs. By default, the
   YUM server on SIMP has the packages only for the version of OS installed on
   the SIMP server.

Setting up TFTP
---------------

This section describes the process of setting up static files and manifests for
:term:`TFTP`.  The example in this section assumes we are configuring the
``production`` :term:`SIMP Omni-Environment`.

.. NOTE::

   The tftp root directory was changed in SIMP 6.2 to conform to DISA STIG
   standards. In previous versions it was :file:`/tftpboot`, and in 6.2 and
   later it is :file:`/var/lib/tftpboot`. If you are upgrading to 6.2 from a
   prior release and wish the files to remain in the :file:`/tftpboot`
   directory, set :code:`tftpboot::tftpboot_root_dir` to :file:`/tftpboot` in
   :term:`Hiera`.

Static Files
^^^^^^^^^^^^

Verify the static files are in the correct location:

1. cd into :file:`/var/simp/environments/production/rsync/{OSTYPE}/Global/tftpboot/`

   (``{OSTYPE}`` under rsync is the OS type of the SIMP **server**.)

2. Verify there is a :file:`linux-install` directory and cd to this directory.

3. Under the linux-install directory you should find a directory named
   :file:`{OSTYPE}-{MAJORRELEASE}.{MINORRELEASE}-{ARCH}/` and a
   **relative**-link to this directory named
   :file:`{OSTYPE}-{MAJORRELEASE}-{ARCH}/`.

4. Under :file:`{OSTYPE}-{MAJORRELEASE}.{MINORRELEASE}-{ARCH}/`, you should find
   the files:

   * :file:`initrd.img`
   * :file:`vmlinuz`

   If these files are not where they should be, then create the directories as
   needed and copy the files from
   :file:`/var/www/yum/<OSTYPE>/<MAJORRELEASE>/<ARCH>/images/pxeboot/`
   or from the images directory on the SIMP DVD. The link name is what is used
   in the resources in the :file:`tftpboot.pp` manifest examples.

   .. NOTE::

      The images in :file:`tftpboot/` need to match the distribution. For example,
      if you upgrade your repo from CentOS 7.3 to 7.4 and will be using this repo
      to kickstart machines, you must also upgrade the images in :file:`tftpboot/`.
      If they do not match you may encounter errors, such as ``unknown file
      system type 'xfs'``.

5. Next, you need to set up the boot files for either BIOS boot mode, UEFI
   mode, or both.

  For more information, see the `RedHat 7 Installation Source`_  or `RedHat 8
  Installation Source`_ Installation Guides.

   .. NOTE::

      UEFI support has been automated since SIMP 6.2. If you are using an older
      version of SIMP please refer to that documentation for setting up UEFI
      manually.

Dynamic Linux Model Files
^^^^^^^^^^^^^^^^^^^^^^^^^

Create a :term:`site profile` module for the TFTP server on the :term:`Puppet Server`
to set up the various files to model different systems.

#. Create the file :file:`tftpboot.pp` in your :term:`site profile`.
   This file will contain Linux models for different types of systems and
   a mapping of MAC addresses to each model.

   Use the source code example below. Linux model examples are given for
   CentOS 7 and 8 using both UEFI and BIOS boot mode.

   * Replace :code:`KSSERVER` with the IP address of kickstart server (or the code
     to look up the IP Address using :term:`Hiera`).

   * Replace :code:`OSTYPE`, :code:`MAJORRELEASE` and :code:`ARCH` with the
     correct values for the systems you will be PXE booting.

   * :code:`MODEL NAME` is usually of the form :code:`OSTYPE-MAJORRELEASE-ARCH`
     for consistency.

   * You will need to know what kickstart file you are using. UEFI and BIOS
     mode require separate kickstart files. Other things that might require a
     different kickstart file to be configured are disk drive configurations and
     FIPS configuration. Create a different Linux model file for each different
     kickstart file needed.

     .. NOTE::

        If using the default cfg files, know that they do not have the
        :file:`_el{[8,7]}` tags at the end of their names.

     .. NOTE::

        The :code:`simp_disk_crypt` option shown below switches on transparent
        disk encryption as described in the :ref:`ig-disk-encryption`
        documentation and is recommended if you have a requirement for disk
        encryption and cannot enter a password at system boot time.

        Simply omit the option if you do not wish to use the capability.

     .. code-block:: ruby

        class site::tftpboot {
          include '::tftpboot'

          #--------
          # BIOS MODE MODEL EXAMPLES

          # for CentOS/RedHat 7 Legacy/BIOS boot
          tftpboot::linux_model { 'el7_x86_64':
            kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64_el7.cfg",
            extra  => "inst.noverifyssl ksdevice=bootif simp_disk_crypt\nipappend 2"
          }

          # For CentOS/RedHat 8 Legacy/BIOS boot
          tftpboot::linux_model { 'el8_x86_64':
            kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64_el8.cfg",
            extra  => "inst.noverifyssl ksdevice=bootif simp_disk_crypt\nipappend 2"
          }

          #------
          # UEFI MODE MODEL EXAMPLES

          # NOTE UEFI boot uses the linux_model_efi module and has different
          # `extra` arguments. You also would use a different kickstart file
          # because the bootloader command within the kickstart file is
          # different. Read the instructions in the default pupclient_x86_64.cfg
          # file and make sure you have the correct bootloader line.
          #
          # For CentOS/RedHat 7 UEFI boot
          tftpboot::linux_model_efi { 'el7_x86_64_efi':
            kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64_efi_el7.cfg",
            extra  => "inst.noverifyssl simp_disk_crypt"
          }

          # For CentOS/RedHat 8 UEFI boot
          tftpboot::linux_model_efi { 'el8_x86_64_efi':
            kernel      => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
            initrd      => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
            ks          => "https://KSSERVER/ks/pupclient_x86_64_el8.cfg",
            extra       => "inst.noverifyssl simp_disk_crypt"
          }

          #------
          # DEFAULT HOST BOOT CONFIGURATION EXAMPLES

          # If desired, create defaults boot configuration for BIOS and UEFI.
          # Note that the name of the default UEFI configuration file needs
          # to be 'grub.cfg'.
          tftpboot::assign_host { 'default': model => 'el7_x86_64' }
          tftpboot::assign_host_efi { 'grub.cfg': model => 'el7_x86_64_efi' }


          #------
          # HOST BOOT CONFIGURATION ASSIGNMENT EXAMPLES

          # For each system define what module you want to use by pointing
          # its MAC address to the appropriate model. Note that the MAC
          # address is preceded by ``01-``.
          tftpboot::assign_host { '01-aa-ab-ac-1d-05-11': model => 'el7_x86_64' }
          tftpboot::assign_host_efi { '01-aa-bb-cc-dd-00-11': model => 'el7_x86_64_efi' }
        }


#. Add the ``site::tftpboot`` class on your puppet server node via
   :term:`Hiera`. Create the file (or edit if it exists):
   :file:`/etc/puppetlabs/code/environments/production/data/hosts/<tftp.server.fqdn>.yaml`.
   (By default, the TFTP server is the same as your puppet server so it should
   exist.) Add the following example code to that yaml file.

   .. code-block:: yaml

      ---
      simp::classes:
        - 'site::tftpboot'


#. After updating the above file, type :command:`puppet agent -t --tags tftpboot` on
   the Puppet Server.

   .. NOTE::

      To provide PXE boot configuration for more OSs, create, in the
      :file:`tftpboot.pp` file, a :code:`tftpboot::linux_model` or
      :code:`tftpboot::linux_model_efi` block for each OS type. Then, assign
      individual hosts to each model by adding :code:`tftpboot::assign_host` or
      :code:`tftpboot::assign_host_efi` resources.

#. Finally, make sure DHCP is set up correctly. In SIMP 6.2 the example
   :file:`dhcpd.conf` was updated to determine the appropriate boot loader file to
   use, depending upon the boot mode of the PXE client. These changes are
   needed if you are booting UEFI systems.

For more information see the `RedHat 8 PXE`_ or `RedHat 7 PXE`_ Installation Guides.

.. _RedHat 7 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/chap-installation-server-setup#sect-network-boot-setup
.. _RedHat 7 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/sect-making-media-additional-sources#sect-making-media-sources-network
.. _RedHat 8 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/system_design_guide/getting-started_system-design-guide#booting-the-installation-using-pxe_booting-the-installer
.. _RedHat 8 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/system_design_guide/preparing-for-your-installation_system-design-guide#prepare-installation-source_preparing-for-your-installation

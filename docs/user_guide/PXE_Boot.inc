.. _Configure_PXE_Boot:


Configure PXE Boot
==================

Sample kickstart templates have been provided in the ``/var/www/ks`` directory
on the SIMP server  and on the SIMP DVD under ``/ks``.  Pre-boot images are
located in the DVD under ``/images/pxeboot``.  If you have an existing
:term:`Preboot Execution Environment` (PXE) setup you can use these to PXE a
SIMP client. Follow your own sites procedures for this.

In this section we describe how to configure the Kickstart and TFTP servers to
PXE boot a SIMP client.  (The DNS and DHCP server setup, also required for PXE
booting, are discussed in an earlier chapter.)

.. NOTE::

   This example sets up a PXE boot for a system that is the same OS as the SIMP
   Server. If you are setting up a PXE boot for a different OS then you must
   make sure that the OS packages are available for all systems you are trying
   to PXE boot through YUM. There are notes throughout the instructions to help
   in setting multiple OS but they are not comprehensive.  You should
   understand DHCP, KS, YUM and TFTP relationships for PXE booting before
   attempting this.

.. _Setting_Up_Kickstart:

Setting up Kickstart
--------------------

This section describes how to configure the kickstart server.

#. Locate the following files in the ``/var/www/ks`` directory

   -  ``pupclient_x86_64.cfg``: Example client kickstart configuration script.
   -  ``diskdetect.sh``:  Example script to determine disks available
      on a system and then apply disk configuration.  This script is used
      by ``pupclient_x86_64.cfg``.

#. Open the ``pupclient_x86_64.cfg`` file and follow the instructions provided
   within it to replace the variables listed and to customize for
   :term:`BIOS`/:term:`UEFI` boot and/or FIPS/non-FIPS mode.  If you have servers
   that require different boot mode or FIPS options, you will need to make
   customized copies of this file to provide those distinct configurations. You
   will also have to configure TFTP to point to the appropriate files.

   - Instructions are provided both at the top of the file and throughout the
     body of the file.
   - You need to know the IP Addresses of the YUM, Kickstart, and TFTP servers.
     (They default to the SIMP server in ``simp config``).
   - Use the commands described in the comments at the top of the file to
     generate the root and grub passwords hashes.  Be sure to replace ``password``
     with your root password.
   - Follow the instructions throughout the file to customize for BIOS/UEFI boot.
   - Follow the instructions throughout the file to customize for FIPS/non-FIPS
     mode.

#. Open the ``diskdetect.sh`` script and customize the disk device names and/or
   partitions as appropriate for your site.  The sample ``diskdetect.sh`` script
   will work, as is, for most systems, as long as your disk device names are in the
   list.  In addition, the sample script provides STIG-compliant partitioning.

#. Type ``chown root.apache /var/www/ks/*`` to ensure that all files are owned
   by ``root`` and in the ``apache`` group.

#. Type ``chmod 640 /var/www/ks/*`` to change the permissions so the owner can
   read and write the file and the ``apache`` group can only read.

.. NOTE::

   Two major changes were made to ``pupclient_x86_64.cfg`` in SIMP 6.2:

   - UEFI PXE support was added.
   - To address timeout issues that caused Puppet bootstrap failures, the use of
     the ``runpuppet`` script to bootstrap Puppet on the client was replaced
     with the use of two scripts, both provided by the ``simp::server::kickstart``
     class:

     - A ``systemd`` unit file for CentOS 7 (``simp_client_bootstrap.service``)
       or a ``systemv`` init script for CentOS 6 (``simp_client_bootstrap``).
     - A common bootstrap script (``bootstrap_simp_client``) used by both.

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
   sure your YUM server has the OS packages for the other OSs. By default the
   YUM server on SIMP has the packages only for the version of OS installed on
   the SIMP server.

Setting up TFTP
---------------

This section describes the process of setting up static files and manifests for
:term:`TFTP`.

.. NOTE::

  The tftp root directory was changed in SIMP 6.2 to conform to DISA STIG
  standards.  In previous versions it was ``/tftpboot``, and in 6.2 and later
  it is ``/var/lib/tftpboot``.  If you are upgrading to 6.2 from a prior
  release and wish the files to remain in the ``/tftpboot`` directory, set
  ``tftpboot::tftpboot_root_dir`` to ``/tftpboot`` in :term:`Hiera`.

Static Files
^^^^^^^^^^^^

Verify the static files are in the correct location:

Type ``cd /var/simp/environments/simp/rsync/<OSTYPE>/Global/tftpboot``

(<OSTYPE> and <MAJORRELEASE> under rsync are the type and version of the SIMP **server**.)

Verify there is a ``linux-install`` directory and cd to this directory.

Under the linux-install directory you should find a directory named
``OSTYPE-MAJORRELEASE.MINORRELEASE-ARCH`` and a link to this directory named
``OSTYPE-MAJORRELEASE-ARCH``.

Under OSTYPE-MAJORRELEASE.MINORRELEASE-ARCH you should find the files:

* ``initrd.img``
* ``vmlinuz``

If these are not there then you must create the directories as needed and copy
the files from ``/var/www/yum/<OSTYPE>/<MAJORRELEASE>/<ARCH>/images/pxeboot``
or from the images directory on the SIMP DVD.  The link name is what is used in
the resources in the tftpboot.pp manifest examples.

.. NOTE::

   The images in the tftp directory need to match the distribution.  For example,
   if you upgrade your repo from CentOS 7.3 to 7.4 and will be using this repo
   to kickstart machines, you must also upgrade the images in the tftp directory.
   If they do not match you can get an error such as "unknown file system type 'xfs'"

Next you need to set up the boot files for either BIOS boot mode, UEFI mode, or both.

.. NOTE::

  UEFI support was automated in SIMP 6.2.  If you are using an older version of
  SIMP please refer to that documentation for setting up UEFI manually.

For more information see the `RedHat 7 Installation Source`_  or `RedHat 6 Installation Source`_ Installation Guides.

Dynamic Linux Model Files
^^^^^^^^^^^^^^^^^^^^^^^^^
Create a site manifest for the TFTP server on the Puppet master to set up the
various files to model different systems.

1. Create the file
   ``/etc/puppetlabs/code/environments/simp/modules/site/manifests/tftpboot.pp``.
   This file will contain Linux models for different types of systems and
   a mapping of MAC addresses to each model.

   Use the source code example below.  Linux model examples are given for
   CentOS 6 and 7 using both UEFI and BIOS boot mode.

   * Replace ``KSSERVER`` with the IP address of kickstart server (or the code
     to look up the IP Address using :term:`Hiera`).

   * Replace ``OSTYPE``, ``MAJORRELEASE`` and ``ARCH`` with the correct values
     for the systems you will be PXE booting.

   * ``MODEL NAME`` is usually of the form ``OSTYPE-MAJORRELEASE-ARCH`` for
     consistency.

   * You will need to know what kickstart file you are using.  UEFI and BIOS
     mode require separate kickstart files.  Other things that might require a
     different kickstart file to be configured are disk drive configurations and
     FIPS configuration.  Create a different Linux model file for each different
     kickstart file needed.

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
       extra  => "inst.noverifyssl ksdevice=bootif\nipappend 2"
     }

     # For CentOS/RedHat 6 Legacy/BIOS boot
     # Note the difference in the `extra` arguments here.
     tftpboot::linux_model { 'el6_x86_64':
       kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
       initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
       ks     => "https://KSSERVER/ks/pupclient_x86_64_el6.cfg",
       extra  => "noverifyssl ksdevice=bootif\nipappend 2"
     }

     #------
     # UEFI MODE MODEL EXAMPLES

     # NOTE UEFI boot uses the linux_model_efi module and has different
     # `extra` arguments.  You also would use a different kickstart file
     # because the bootloader command within the kickstart file is
     # different.  Read the instructions in the default pupclient_x86_64.cfg
     # file and make sure you have the correct bootloader line.
     #
     # For CentOS/RedHat 7 UEFI boot
     tftpboot::linux_model_efi { 'el7_x86_64_efi':
       kernel => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
       initrd => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
       ks     => "https://KSSERVER/ks/pupclient_x86_64_efi_el7.cfg",
       extra  => "inst.noverifyssl"
     }

     # For CentOS/RedHat 6 UEFI boot
     # Note the extra attribute legacy_grub.
     tftpboot::linux_model_efi { 'el6_x86_64_efi':
       kernel      => 'OSTYPE-MAJORRELEASE-ARCH/vmlinuz',
       initrd      => 'OSTYPE-MAJORRELEASE-ARCH/initrd.img',
       ks          => "https://KSSERVER/ks/pupclient_x86_64_el6.cfg",
       extra       => "noverifyssl",
       legacy_grub => true
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
     # its MAC address to the appropriate model.  Note that the MAC
     # address is preceded by ``01-``.
     tftpboot::assign_host { '01-aa-ab-ac-1d-05-11': model => 'el6_x86_64' }
     tftpboot::assign_host_efi { '01-aa-bb-cc-dd-00-11': model => 'el7_x86_64_efi' }
   }


2. Add the tftpboot site manifest on your puppet server node via :term:`Hiera`.  Create
   the file (or edit if it exists):
   ``/etc/puppetlabs/code/environments/simp/hieradata/hosts/<tftp.server.fqdn>.yaml``.
   (By default the TFTP server is the same as your puppet server so it should
   exist.) Add the following example code to that yaml file.

.. code-block:: yaml

  ---
  classes:
    - 'site::tftpboot'


3. After updating the above file, type ``puppet agent -t --tags tftpboot`` on
   the Puppet master.

.. NOTE::

   To provide PXE boot configuration for more OSs, create, in the ``tftpboot.pp``
   file, a ``tftpboot::linux_model`` or ``tftpboot::linux_model_efi`` block for
   each OS type. Then, assign individual hosts to each model by adding
   ``tftpboot::assign_host`` or ``tftpboot::assign_host_efi`` resources.

Lastly, make sure DHCP is set up correctly.  In SIMP 6.2 the example ``dhcpd.conf``
was updated to determine the appropriate boot loader file to use, depending upon
the boot mode of the PXE client.  These changes are needed if you booting UEFI
systems.

For more information see the `RedHat 6 PXE`_ or `RedHat 7 PXE`_ Installation Guides.

.. _RedHat 7 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/chap-installation-server-setup#sect-network-boot-setup

.. _RedHat 7 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/sect-making-media-additional-sources#sect-making-media-sources-network

.. _RedHat 6 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/installation_guide/s1-netboot-pxe-config

.. _RedHat 6 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/installation_guide/ch-Preparing-x86#s1-steps-network-installs-x86

Configure PXE Boot
==================

Sample kickstart templates have been provided in the ``/var/www/ks`` directory
on the SIMP server  and on the SIMP DVD under ``/ks``.  Pre-boot images are
located in the DVD under ``/images/pxeboot``.  If you have an existing
:term:`Preboot Execution Environment` (PXE) setup you can use these to PXE a
SIMP client. Follow your own sites procedures for this.

In this section we describe how to configure the Kickstart and TFTP servers to
PXE boot a SIMP client.  (The DHCP server setup, also required for PXE booting,
is discussed in and earlier chapter.)

.. NOTE::

   This example sets up a PXE boot for a system that is the same OS as the SIMP
   Server. If you are setting up a PXE boot for a different OS then you must
   make sure that the OS packages are available for all systems you are trying
   to PXE boot through YUM. There are notes throughout the instructions to help
   in setting multiple OS but they are not comprehensive.  You should
   understand DHCP, KS, YUM and TFTP relationships for PXE booting before
   attempting this.


Setting Up Kickstart
--------------------

This section describes how to configure the kickstart server.

#. Locate the following files in the ``/var/www/ks`` directory

   -  ``pupclient_x86_64.cfg``
   -  ``diskdetect.sh``

#. Open each of the files and follow the instructions provided within them to
   replace the variables.  You need to know the IP Addresses of the YUM,
   Kickstart, and TFTPserver. (They default to the simp server in
   ``simp config``).  You may need to make several copies of this file depending
   on the different configurations of your clients.

   - ``pupclient_x86_64.cfg``: Replace the variables noted at the top and
     generate and enter the passwords.
     You  need to set up seperate files for clients that will be booting in UEFI
     mode as opposed to Leagacy (BIOS) mode.  There are also different files for
     sysvinit systems and systemd systems.

   - ``diskdetect.sh``:  The ``diskdetect.sh`` script is responsible for
     detecting the first active disk and applying a disk configuration. Edit
     this file to meet any necessary requirements or use this file as a
     starting point for further work. It will work as is for most systems as
     long as your disk device names are in the list.

.. NOTE:

   In SIMP 6.2 EFI PXE boot was automted.  UEFI and Legacy boot modes require
   different ``bootloader`` lines in the kickstart file.  You will need to create
   seperate kickstart files if you wish to boot systems in both modes and point to
   the correct one in the linux model you create for it in the .

#. Type ``chown root.apache /var/www/ks/*`` to ensure that all files are owned
   by ``root`` and in the ``apache`` group.

#. Type ``chmod 640 /var/www/ks/*`` to change the permissions so the owner can
   read and write the file and the ``apache`` group can only read.

.. NOTE::

   The URLs and locations in the file are setup for a default SIMP install.
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
  The tftp root directory was changed in SIMP 6.2.  In previous versions it was
  ``/tftproot``, and in 6.2 and later it is ``/var/lib/tftpboot``.  If you are upgrading
  to 6.2 from a prior release and wish the files to remain in the ``/tftpboot`` directory
  set ``tftpboot::tftpboot_root_dir`` to ``/tftpboot`` in hiera.

Static Files
^^^^^^^^^^^^

Verify the static files are in the correct location:

Type ``cd /var/simp/environments/simp/rsync/<OSTYPE>/Global/tftpboot``

(<OSTYPE> and <MAJORRELEASE> under rsync are the type and version of the SIMP **server**)

Verify there is a ``linux-install`` directory and cd to this directory.

Under the linux-install directory you should find a directory named
``OSTYPE-MAJORRELEASE.MINORRELEASE-ARCH`` and a link to this directory named
``OSTYPE-MAJORRELEASE-ARCH``.

Under OSTYPE-MAJORRELEASE.MINORRELEASE-ARCH you should find the files:

* initrd.img
* vmlinuz

If these are not there then you must create the directories as needed and copy
the files from ``/var/www/yum/<OSTYPE>/<MAJORRELEASE>/<ARCH>/images/pxeboot``
or from the images directory on the SIMP DVD.  The link name is what is used in
the resources in the tftpboot.pp manifest examples.

.. NOTE::
   The images in the tftp directory need to match the distribution.  For example,
   if you upgrade your repo from CentOS 7.3 to 7.4 and will be using this repo
   to kickstart machines, you must also upgrade the images in the tftp directory.
   If they do not match you can get an error such as "unknown file system type 'xfs'"

Next you need to set up the boot files for either legacy boot mode, UEFI mode, or both.

.. NOTE::
  UEFI support was automated in SIMP 6.2.  If you are using an older version of
  SIMP please refer to that documentation for setting up UEFI manually.

For more information see the `RedHat 7 Installation Source`_  or `RedHat 6 Installation Source`_ Istallation Guides

Dynamic Linux Model Files
^^^^^^^^^^^^^^^^^^^^^^^^^
Create a site manifest for the TFTP server on the Puppet server to set up the various
files to model different systems.

1. Create the file
   ``/etc/puppetlabs/code/environments/simp/modules/site/manifests/tftpboot.pp``.
   Use the source code example below.  Examples are given for Centos 6 and 7 for both
   UEFI and legacy boot mode.

   * Replace ``KSSERVER`` with the IP address of Kickstart server (or the code
     to look up the IP Address using :term:`Hiera`).

   * Replace ``OSTYPE``, ``MAJORRELEASE`` and ``ARCH`` with the correct values
     for the systems you will be PXE booting.

   * ``MODEL NAME`` is usually of the form ``OSTYPE-MAJORRELEASE-ARCH`` for
     consistency.

   * You will need to know what kickstart file you are using.  UEFI and Legacy mode
     require seperate kickstart files.  Other things that might require a different
     kickstart file to be configure are disk drive configurations, if FIPS is being
     used and other things.  Create a different linux model file for each different
     kickstart file you have to use.  (See the ``pupclient_x86_64.cfg`` file for
     comments on how to change that file to handle different system types.


.. code-block:: ruby

   class site::tftpboot {
     include '::tftpboot'

     #--------
     # LEGACY MODE EXAMPLES

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
     # UEFI MODE EXAMPLES

     # NOTE for UEFI boot you need a different kickstart file from legacy
     # mode because the bootloader command is different.  Read the instructions
     # in the pupclient_x86_64 file and make sure you have the correct bootloader
     # line.
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
     # All systems need the following

     # For each system define what module you want to use by pointing
     # its macaddress to the appropriate model.  Note that the macaddress
     # is preceded by ``01-``.
     tftpboot::assign_host { 'default': model => 'el7_x86_64' }
     tftpboot::assign_host { 01-aa-bb-cc-dd-00-11: model => 'el7_x86_64_efi' }
     tftpboot::assign_host { 01-aa-ab-ac-1d-05-11: model => 'el6_x86_64' }
   }


2. Add the tftpboot site manifest on your puppet server node via Hiera.  Create
   the file (or edit if it exists):
   ``/etc/puppetlabs/code/environments/simp/hieradata/hosts/<tftp.server.fqdn>.yaml``.
   (By default the TFTP server is the same as your puppet server so it should
   exist.) Add the following example code to that yaml file.

.. code-block:: yaml

  ---
  classes:
    - 'site::tftpboot'


3. After updating the above file, type ``puppet agent -t --tags tftpboot`` on
   the Puppet server.

.. NOTE::

   To PXE boot more OSs, create, in the tftpboot.pp file, a
   ``tftpboot::linux_model`` block for each OS type using the extra directories
   and kickstart files created using the notes in previous sections. Point
   individual systems to them by adding assign_host lines with their MAC
   pointing to the appropriate model name.

Lastly, make sure DHCP is set up correctly.  In SIMP 6.2 the DHCP template was updated to
include a test for architecture type.  These changes are needed if you booting
UEFI systems.

For more information see the `RedHat 6 PXE`_ or `RedHat 7 PXE`_ Installation Guides.

.. _RedHat 7 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/chap-installation-server-setup#sect-network-boot-setup

.. _RedHat 7 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/installation_guide/sect-making-media-additional-sources#sect-making-media-sources-network

.. _RedHat 6 PXE: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/installation_guide/s1-netboot-pxe-config

.. _RedHat 6 Installation Source: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/installation_guide/ch-Preparing-x86#s1-steps-network-installs-x86

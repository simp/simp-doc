.. _Configure_PXE_Boot:

Configure PXE Boot
==================

SIMP provides sample kickstart files in the `simp-core kickstart examples`_.
The SIMP ISO ships the files for its own EL release under :file:`/ks`, and an
ISO install copies them to :file:`/var/www/ks` on the SIMP server.  Pre-boot
images are located in the ISO under :file:`/images/pxeboot`.  If you have an
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

#. Put the kickstart files in the :file:`/var/www/ks` directory

   The kickstart files are:

   - :file:`pupclient_x86_64.cfg`: Example client kickstart configuration
     script.  It installs :package:`openvox-agent` and enables
     :file:`simp_client_bootstrap.service`, which runs the SIMP client
     bootstrap on the first boot.
   - :file:`simp_server_x86_64.cfg`: Example SIMP server kickstart
     configuration script.  See :ref:`ug-pxe-simp-server` below.
   - :file:`diskdetect.sh`: Example script to determine disks available
     on a system and then apply disk configuration.
   - :file:`repodetect.sh`: Script that writes the repositories for the
     installed system.  The comments at the top of the file describe the
     YUM server layout it expects.

   There is one :file:`pupclient_x86_64.cfg` and one
   :file:`simp_server_x86_64.cfg` for each of EL8, EL9 and EL10.  The copies
   for each release have the same file names, so this guide adds the release
   to the name, for example :file:`pupclient_x86_64_el9.cfg`.
   :file:`diskdetect.sh` and :file:`repodetect.sh` support EL8, EL9 and EL10.
   The kickstarts download them from :file:`https://{KSSERVER}/ks/`, so do not
   rename them.

   - If you installed the SIMP server from the ISO, the files for its EL
     release are already in :file:`/var/www/ks`.  Rename the two
     :file:`.cfg` files to add the release.
   - Otherwise, or to add other EL releases, download the files.  Remove the
     releases you do not serve from the first loop:

     .. code-block:: bash

        ks=https://raw.githubusercontent.com/simp/simp-core/master/build/distributions/RedHat
        for el in 8 9 10; do
          for f in pupclient_x86_64 simp_server_x86_64; do
            sudo curl -fsSLo /var/www/ks/${f}_el${el}.cfg ${ks}/${el}/x86_64/DVD/ks/${f}.cfg
          done
        done
        for f in diskdetect.sh repodetect.sh; do
          sudo curl -fsSLo /var/www/ks/${f} ${ks}/10/x86_64/DVD/ks/${f}
        done

     The scripts come from the EL10 directory because the EL8 and EL9
     directories hold symbolic links to them, which do not download as
     files.

   The :code:`simp::server::kickstart` class publishes
   :file:`simp_client_bootstrap.service` and :file:`bootstrap_simp_client` in
   the same directory.

#. Open each client kickstart, such as :file:`pupclient_x86_64_el9.cfg`, and
   replace the placeholders that the comments at the top of the file list:

   - :code:`#BOOTPASS#` and :code:`#ROOTPASS#`: the hashed GRUB and root
     passwords.  Use the commands in the comments to generate them.

     - The GRUB password can also be managed later with the
       :pupmod:`simp/simp_grub` module. See :ref:`Managing GRUB Users
       <grub_user_management>` for more information.

   - :code:`#KSSERVER#` and :code:`#YUMSERVER#`: the host names or IP
     addresses of the kickstart and YUM servers.  They default to the SIMP
     server in :program:`simp config`.
   - :code:`#LINUXDIST#`: the OS directory name on the YUM server, which is
     the ``os.name`` fact of the installed system (``RedHat``, ``AlmaLinux``,
     ``Rocky`` or ``OracleLinux``).  It is case sensitive.

   The same file works for both :term:`BIOS` and :term:`UEFI` boot.  You do
   not edit it to choose FIPS mode or disk encryption.  The installer takes
   these from kernel command line options, which you set in the TFTP
   configuration below:

   - :code:`fips=1` or :code:`fips=0`: install in FIPS mode or not.  The
     installation stops when neither is given.  The :code:`tftpboot` module
     always passes one, from the ``fips`` parameter of each Linux model,
     which defaults to :code:`simp_options::fips`.
   - :code:`simp_disk_crypt`: encrypt the disk.
   - :code:`simp_opt=prompt`: partition the disk by hand.

#. Open the :file:`diskdetect.sh` script and customize the disk device names and/or
   partitions as appropriate for your site. The sample :file:`diskdetect.sh` script
   will work, as is, for most systems, as long as your disk device names are in the
   list. In addition, the sample script provides STIG-compliant partitioning.

#. Type :command:`chown root:apache /var/www/ks/*` to ensure that all files are
   owned by ``root`` and in the ``apache`` group.

#. Type :command:`chmod 640 /var/www/ks/*` to change the permissions so the owner can
   read and write the file and the ``apache`` group can only read.

.. NOTE::

   The URLs and locations in the file are set up for a default SIMP install.
   That means the same OS and version as the SIMP server, all servers in one
   location (on the SIMP server) and in specific directories. If you have
   installed these servers in a different location than the defaults, you may
   need to edit URLs or directories.

.. NOTE::

   If you want to PXE boot more than one OS release, use the kickstart files
   for each release, named as described above.  Your YUM server must also
   have the OS packages for each release, in the layout that
   :file:`repodetect.sh` describes. By default, the YUM server on SIMP has the
   packages only for the version of OS installed on the SIMP server.

.. _ug-pxe-simp-server:

Kickstarting a SIMP server
^^^^^^^^^^^^^^^^^^^^^^^^^^

:file:`simp_server_x86_64.cfg` installs a SIMP server over the network.  It
installs :package:`openvox-server` and, when the YUM server has a SIMP
repository, the :package:`simp` and :package:`simp-adapter` packages.  It uses
the same placeholders and kernel command line options as
:file:`pupclient_x86_64.cfg`.  After the first boot, continue with
:ref:`ug-initial_server_configuration`.

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

      The images in :file:`tftpboot/` need to match the distribution. For
      example, if you upgrade your repo from EL9.5 to EL9.6 and will be using
      this repo to kickstart machines, you must also upgrade the images in
      :file:`tftpboot/`. If they do not match you may encounter errors, such
      as ``unknown file system type 'xfs'``.

5. Next, you need to set up the boot files for either BIOS boot mode, UEFI
   mode, or both.

   For more information, see `Preparing a PXE installation source`_ in the
   RHEL 9 documentation.

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
   EL9 and EL10 using both UEFI and BIOS boot mode.  For EL8, copy the EL9
   models and change ``9`` to ``8``.

   * Replace :code:`KSSERVER` with the IP address of kickstart server (or the code
     to look up the IP Address using :term:`Hiera`).

   * Replace :code:`OSTYPE` with the correct value for the systems you will be
     PXE booting.

   * You will need to know what kickstart file you are using.  BIOS and UEFI
     boot use the same kickstart file, but each EL release has its own.  The
     example uses the renamed files described in :ref:`Setting_Up_Kickstart`.
     Create a different Linux model for each kickstart file you need, for
     example when some systems need a different disk configuration.

   * Each Linux model passes :code:`fips=1` or :code:`fips=0` to the installer,
     from :code:`simp_options::fips`.  To install a model's systems in a
     different FIPS mode, set the ``fips`` parameter of that model.

   * Use the ``inst.`` prefix for installer options in ``extra``, such as
     ``inst.noverifyssl``.  EL9 and EL10 ignore the un-prefixed options
     (``ks=``, ``repo=``, ``ksdevice=``, ``stage2=`` and ``method=``), and the
     :code:`tftpboot` module logs a warning when ``extra`` contains one.

     .. NOTE::

        The :code:`simp_disk_crypt` option shown below switches on transparent
        disk encryption as described in the :ref:`ig-disk-encryption`
        documentation and is recommended if you have a requirement for disk
        encryption and cannot enter a password at system boot time.

        Simply omit the option if you do not wish to use the capability.

     .. code-block:: puppet

        class site::tftpboot {
          include 'tftpboot'

          #--------
          # BIOS MODE MODEL EXAMPLES

          # "ipappend 2" makes pxelinux pass the MAC address of the interface
          # the system booted from, so the installer uses that interface.

          # For EL9 Legacy/BIOS boot
          tftpboot::linux_model { 'el9_x86_64':
            kernel => 'OSTYPE-9-x86_64/vmlinuz',
            initrd => 'OSTYPE-9-x86_64/initrd.img',
            ks     => 'https://KSSERVER/ks/pupclient_x86_64_el9.cfg',
            extra  => "inst.noverifyssl simp_disk_crypt\nipappend 2",
          }

          # For EL10 Legacy/BIOS boot
          tftpboot::linux_model { 'el10_x86_64':
            kernel => 'OSTYPE-10-x86_64/vmlinuz',
            initrd => 'OSTYPE-10-x86_64/initrd.img',
            ks     => 'https://KSSERVER/ks/pupclient_x86_64_el10.cfg',
            extra  => "inst.noverifyssl simp_disk_crypt\nipappend 2",
          }

          #------
          # UEFI MODE MODEL EXAMPLES

          # UEFI boot uses tftpboot::linux_model_efi, with the same kickstart
          # file as BIOS boot.  GRUB has no "ipappend", so on a system with
          # several network interfaces, name the one to use in `extra`, for
          # example "ip=eth2:dhcp".

          # For EL9 UEFI boot
          tftpboot::linux_model_efi { 'el9_x86_64_efi':
            kernel => 'OSTYPE-9-x86_64/vmlinuz',
            initrd => 'OSTYPE-9-x86_64/initrd.img',
            ks     => 'https://KSSERVER/ks/pupclient_x86_64_el9.cfg',
            extra  => 'inst.noverifyssl simp_disk_crypt',
          }

          # For EL10 UEFI boot
          tftpboot::linux_model_efi { 'el10_x86_64_efi':
            kernel => 'OSTYPE-10-x86_64/vmlinuz',
            initrd => 'OSTYPE-10-x86_64/initrd.img',
            ks     => 'https://KSSERVER/ks/pupclient_x86_64_el10.cfg',
            extra  => 'inst.noverifyssl simp_disk_crypt',
          }

          #------
          # DEFAULT HOST BOOT CONFIGURATION EXAMPLES

          # If desired, create defaults boot configuration for BIOS and UEFI.
          # Note that the name of the default UEFI configuration file needs
          # to be 'grub.cfg'.
          tftpboot::assign_host { 'default': model => 'el9_x86_64' }
          tftpboot::assign_host_efi { 'grub.cfg': model => 'el9_x86_64_efi' }


          #------
          # HOST BOOT CONFIGURATION ASSIGNMENT EXAMPLES

          # For each system define what module you want to use by pointing
          # its MAC address to the appropriate model. Note that the MAC
          # address is preceded by "01-".
          tftpboot::assign_host { '01-aa-ab-ac-1d-05-11': model => 'el10_x86_64' }
          tftpboot::assign_host_efi { '01-aa-bb-cc-dd-00-11': model => 'el10_x86_64_efi' }
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

For more information see `Preparing a PXE installation source`_ in the RHEL 9
documentation.

.. _Preparing a PXE installation source: https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/9/html/automatically_installing_rhel/preparing-for-a-network-install_rhel-installer
.. _simp-core kickstart examples: https://github.com/simp/simp-core/tree/master/build/distributions/RedHat

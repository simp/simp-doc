.. _howto-unpack-dvd:

HOWTO Use unpack_dvd to Extract the Full OS RPM Package Set
===========================================================

The SIMP ISO provides a minimal set of packages.

If you require additional OS packages, you can extract them from vendor ISOs using
:program:`unpack_dvd`.  Additionally you can extract the PXE
files from the ISO to the rsync directories.

Use option :code:`unpack_dvd --help` to see all options available for :program:`unpack_dvd`.

The defaults used in :program:`unpack_dvd` correspond to defaults used in SIMP puppet modules
to configure the tftp server and local os yum repository installed when SIMP is installed from ISO.

Extract the OS Packages
-----------------------

:program:`Unpack_dvd` extracts the OS RPMs to the :file:`/var/www/yum/<os-family>/<os-version>` directory.
It creates links under the :file:`Updates` directory locate under that directory to all RPMs extracted and then runs :program:`createrepo` in the :file:`Updates` directory.

By default the os-family and os-version are determined from files on the ISO. The base directory
can be changed using -d option and the os-version can be changed using the ``-v`` option.  The base directory
must already exist.

If run as root, :program:`unpack_dvd` will recursively change the group on any directory containing RPMs to ``apache`` and give group access to the files.  The group can be changed with the ``-g`` option.

:program:`Unpack_dvd` will create a link from the version unpacked to the major version.  (For example :file:`7.6` will be linked to :file:`7`).  Therefore if :program:`unpack_dvd` can only determine the major OS version from the files on the ISO, it will fail and ask you to supply a more descriptive version number using ``-v`` option.

The following example will:

* extract the RPMs to :file:`/var/www/yum/CentOS/7.8.2003`
* create a repository in :file:`/var/www/yum/CentOS/7.8.2003/Updates` directory
* link :file:`/var/www/yum/CentOS/7` to  :file:`/var/www/yum/CentOS/7.8.2003`.

#. Log on and run :command:`sudo su - root`.
#. Copy the appropriate vendor OS ISO(s) to the server.
#. If the server where you are unpacking the vendor ISO was **NOT** built using the SIMP ISO ,
   you must create :file:`/var/www/yum` (or the directory you indicated in ``-d``
   option.)
#. unpack using :program:`unpack_dvd`

   .. code:: bash

      # unpack_dvd -v <os version> <full path to iso>
      unpack_dvd -v 7.8.2003 /myisodir/CentOS-7-x86_64-DVD-2003.iso

#. Ensure that subsequent :term:`yum` operations are aware of the new RPM
   packages by refreshing the system's yum cache:

   .. code:: bash

      yum clean all && yum makecache

.. WARNING::

   At this time :program:`unpack_dvd` does not work entirely with EL8 ISOs.
   EL8 introduced modules to repositories and :program:`unpack_dvd` can not handle these.
   It will extract some but not all of RPMs on an EL8 ISO. The files extracted
   are enough to kickstart a basic EL8 system but some of the application stream
   files are not extracted and added to the repository correctly.

Extract PXE files
-----------------

Extracting the PXE files was added to :program:`unpack_dvd` in :package:`simp-utils-6.4.0`.  Use the ``-X`` option to tell :program:`unpack_dvd` to extract the PXE files and the ``--no-unpack-yum`` option if you do not want to extract the yum files.

By default :program:`unpack_dvd` will pull information off the ISO and, using this information, create a directory named <os-family>-<version>-<arch> under the tftpboot rsync directory and extract the PXE files there.

The default rsync directory is :file:`/var/simp/environments/production/rsync/<os family>/Global/tftpboot/linux-install/`.  To change the environment in rsync directory use ``-e`` option.  To use an alternate directory specify the path after the ``-X`` option.

The rsync directory or the directory you specified must exist before running :program:`unpack_dvd`.

If run as root, :program:`unpack_dvd` will the set permissions on the PXE files from the
directory it copies them to.

The following example will just extract the PXE files

* extract the PXE files to :file:`/var/simp/environments/production/rsync/CentOS/Global/tftpboot/linux-install/centos-8.0.1905-x86_64`
* link `/var/simp/environments/production/rsync/CentOS/Global/tftpboot/linux-install/centos-8-x86_64` to the above directory.

.. code:: bash

   # Place the -X options after the ISO name
   unpack_dvd -v 8.0.1905 --no-unpack-yum /myisodir/CentOS--x86_64-1905-dvd1.iso -X

The following example will

* extract the RPMs to :file:`/my/repodir/yum/CentOS/8.0.1905`
* create the repo under :file:`/my/repodir/yum/CentOS/8.0.1905/Updates`
* extract the PXE files to :file:`/my/tftpboot/`

.. code:: bash

   # The PXE directory must follow the -X option.
   # Use the -n to prevent the creation of the links.
   unpack_dvd -v 8.0.1905 -d /my/repodir /myisodir/CentOS--x86_64-1905-dvd1.iso -X /my/tftpboot


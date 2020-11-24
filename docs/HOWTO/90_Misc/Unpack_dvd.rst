.. _howto-unpack-dvd:

HOWTO Use unpack_dvd to Extract the Full OS RPM Package Set
===========================================================

The SIMP ISO provides a minimal set of packages.

If you require additional OS packages, you can extract them from vendor ISOs using
:program:`unpack_dvd`.  Additionally you can extract the PXE
files from the ISO to the rsync directories.

Use option :code:`unpack_dvd --help` to see all options available for :program:`unpack_dvd`.

Extract the OS Packages
-----------------------

:program:`Unpack_dvd` extracts the OS RPMs to :file:`/var/www/yum/<OperatingSystem>`
in a directory named after the OS version it determines from files on the ISO.
It then creates a link to the major version.  If version 7.6 is extracted,
7 will be linked to 7.6.

After extracting all the RPMs it will create or update a repository in the
versioned directory.

.. NOTE::

   If unpack_dvd can only determine the major OS version from the files
   on the ISO, :program:`unpack_dvd` will ask you to supply a more descriptive
   version number using ``-v`` option.

The following example will:

* extract the RPMs to :file:`/var/www/yum/CentOS/7.8.2003`
* create a repository in the above directory
* link :file:`/var/www/yum/CentOS/7` to the above directory.

#. Log on as ``simp`` and run :command:`sudo su - root`.  If run as root it will
   change the permissions on the repo directory to be owned by group ``apache``
   and make them group accessable.  The group can be changed with the -g option.
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

Extracting the PXE files along with the OS files, was added to :program:`unpack_dvd` in :package:`simp-utils-6.4.0`.  Use the -X option to tell it to extract the PXE files and add the --no-unpack-yum option if you do not also want to extract the yum files.

By default :program:`unpack_dvd` will pull information off the ISO and, using this information, create a directory named <os-family>-<version>-<arch> under the tftpboot rsync directory and extract the PXE files there.

The default rsync directory is :file:`/var/simp/environments/production/rsync/<os family>/Global/tftpboot/linux-install/`.  Options exist to change the environment in the rsync directory or to specify an alternate directory.

The rsync directory or the directory you specified must exist before running :program:`unpack_dvd`.

If run as root, :program:`unpack_dvd` will the set permissions on the PXE files from the
directory it it copies them to.


The following example will just extract the PXE files

* extract the PXE files to :file:`/var/simp/environments/production/rsync/CentOS/Global/tftpboot/linux-install/centos-8.0.1905-x86_64`
* link `/var/simp/environments/production/rsync/CentOS/Global/tftpboot/linux-install/centos-8-x86_64` to the above directory.

.. code:: bash

   # Place the -X options after the ISO name
   unpack_dvd -v 8.0.1905 --no-unpack-yum /myisodir/CentOS--x86_64-1905-dvd1.iso -X

The following example will

* extract the RPMs  to :file:`/my/repodir/yum/CentOS/8.0.1905`
* create a repository in the above directory
* extract the PXE files to :file:`/my/tftpboot/`

.. code:: bash

   # The PXE directory must follow the -X option.
   # Use the -n to prevent the creation of the links.
   unpack_dvd -v 8.0.1905 -d /my/repodir /myisodir/CentOS--x86_64-1905-dvd1.iso -X /my/tftpboot


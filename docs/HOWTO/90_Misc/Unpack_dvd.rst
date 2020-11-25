.. _howto-unpack-dvd:

HOWTO Use unpack_dvd to Extract the Full OS RPM Package Set
===========================================================

The SIMP ISO provides a minimal set of packages.

If you require additional OS packages, you can extract them from vendor ISOs using
:program:`unpack_dvd`.  Additionally you can extract the PXE
files from the ISO to the rsync directories.



Extract the OS Packages
-----------------------

:program:`Unpack_dvd` will, by default

  * extract OS metadata from the ISO
  * create a directory named :file:`<os-family>/<os-version>` under :file:`/var/www/yum`, the directory used for the yum server in :pupmod:`simp/simp`.
  * extract the files on the ISO that directory.
  * create a link  :file:`/var/www/yum/<os-family>/<os-major-version>` to above directory.
  * create :file:`Updates` directory if it does not exist
  * create a link for all RPMs extracted from the ISO under the :file:`Updates` directory.
  * run :program:`createrepo` in the :file:`Updates` directory.
  * If run as root, change permissions recursively on :file:`Updates` and any directory with RPMs to be owned and readable by group ``apache``

Use :code:`unpack_dvd --help` to see all options available.

.. NOTE::

  If unpack_dvd can only determine the OS major version from the ISO metadata
  it will fail and ask you to supply a more detail version number using
  the ``-v`` option.

The following example will:

* extract the RPMs to :file:`/var/www/yum/CentOS/7.8.2003`
* create a repository in :file:`/var/www/yum/CentOS/7.8.2003/Updates` directory
* link :file:`/var/www/yum/CentOS/7` to  :file:`/var/www/yum/CentOS/7.8.2003`.

#. Log on and run :code:`sudo su - root`.
#. Copy the appropriate vendor OS ISO(s) to the server.
#. If the server where you are unpacking the vendor ISO was **NOT** built using the SIMP ISO , you must create :file:`/var/www/yum`.
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

When PXE file extraction is enabled with the :command:-X option,
:program:`unpack_dvd` will, by default,

  * extract OS version information from the ISO metadata
  * create a directory for the PXE files in the rsync directory used by :pupmod:`simp/tftpboot` in the production environment.
    * named <os-family>-<version>-<arch>
    * The default rsync directory is :file:`/var/simp/environments/production/rsync/<os family>/Global/tftpboot/linux-install/`.
  * extract the files to the created OS directory.
  * create a link <os-family>-<major-version>-<arch> to the above directory.
  * if run as root, change the permisions recursivley on created directoy and its contents to those of the rsync directory.

Use :code:`unpack_dvd --help` for options to change some of the defaults.

The following example will extract only the PXE files to the rsync directory:

.. code:: bash

   # Place the -X options after the ISO name.
   sudo su - root
   # copy the iso to the system
   unpack_dvd -v 8.0.1905 --no-unpack-yum /myisodir/CentOS--x86_64-1905-dvd1.iso -X

The following example will extract both the RPMs and PXE files to alternate directories:

.. code:: bash

   sudo su - root
   # Make the directories.  Set the permissions as needed.
   mkdir -p /my/repodir
   mkdir -p /my/tftpboot
   # The PXE directory must follow the -X option.
   # The -d options changes the directory to extract OS files
   unpack_dvd -v 8.0.1905 -d /my/repodir /myisodir/CentOS--x86_64-1905-dvd1.iso -X /my/tftpboot
   yum clean all && yum makecache


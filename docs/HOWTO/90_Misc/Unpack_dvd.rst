.. _howto-unpack-dvd:

Extract the Full OS RPM Package Set
-----------------------------------

The SIMP ISO provides a minimal set of packages.

If you require additional OS packages, you can extract them from vendor ISOs using
the :program:`unpack_dvd`.

The :program:`unpack_dvd` extracts the OS rpms to :file:`/var/www/yum/<OperatingSystem>`
in a directory named after the OS version in the :file:`.treeinfo` file on the DVD.
It then creates a link to the major version.  If version 7.6 is extracted, 
7 will be linked to 7.6.

.. NOTE::

   If the OS version in the :file:`.treeinfo`  is just the major version, as it is on
   most CentOS ISOs, :program:`unpack_dvd` will ask you to supply a more descriptive
   version number using ``-v`` option.

#. Log on as ``simp`` and run :command:`sudo su - root`.
#. Copy the appropriate vendor OS ISO(s) to the server.
#. If the server where you are unpacking the vendor ISO was **NOT** built using the SIMP ISO ,
   you must create a SIMP directory in :file:`/var/www/yum/` (or the directory you indicated in ``-d``
   option.)
#. unpack using  :program:`unpack_dvd`

   .. code:: bash

      # unpack_dvd -v <os version> <full path to iso>
      unpack_dvd -v 7.8.2003 /myisodir/CentOS-7-x86_64-DVD-2003.iso


#. Ensure that subsequent :term:`yum` operations are aware of the new RPM
   packages by refreshing the system's yum cache:

   .. code:: bash

      yum clean all && yum makecache

Use the help option :code:`unpack_dvd --help` to see all options available for unpack_dvd.

Run :code:`unpack_dvd --help` to see more options and features of :program:`unpack_dvd`.

.. WARNING::

   At this time :program:`unpack_dvd` does not work with EL8 DVDs.  EL8 introduced
   modules to repositories and  it can not handle these.



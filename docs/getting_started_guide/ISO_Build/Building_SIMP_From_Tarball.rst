.. _gsg-building_simp_from_tarball:

Building SIMP From Tarball
==========================

.. NOTE::
  Building SIMP from a pre-built tarball is the fastest method for getting a
  known stable build of a SIMP ISO and should be preferred over other methods.

Getting Started
---------------

.. WARNING::
  Please have your environment prepared as specified by
  :ref:`gsg-environment_preparation` before continuing.

Download the SIMP release tarball, found on our `SIMP artifacts repository`_.

Download the latest tarball according to your needs. If you are not sure what
version you need, check the :ref:`faq-simp_version_guide`.

  * The `latest 6.1.0-0 release (for CentOS 6)`_
  * The `latest 6.1.0-0 release (for CentOS 7)`_
  * The `latest checksums`_

Generating The ISO
------------------

Clone simp-core:

.. code-block:: bash

  $ git clone https://github.com/simp/simp-core

Change into the ``simp-core`` directory and make sure you are on the correct
branch for your target SIMP version:

.. code::

   $ cd simp-core
   $ git checkout tags/6.1.0-0 # for SIMP 6.1

Run ``bundle install`` to make sure that all of the build tools and dependencies are
installed and up to date:

.. code::

   $ bundle install

Copy the pre-built tarball to the ``DVD_Overlay`` directory that corresponds
with the version of base OS you want to build. For instance, if you wanted to
build with CentOS-7,

.. code::

   $ cp </path/to/.tar> build/distributions/CentOS/7/x86_64/DVD_Overlay

Run the ``build:auto`` rake task to create a bootable ISO:

.. NOTE::
  Do **not** add any whitespace before or after the commas. This is an artifact
  of using ``rake``.

.. code::

   $ RSYNC_NO_SELINUX_DEPS=yes bundle exec rake build:auto[<directory containing source ISOs>,6.X]

Build ENV vars:

  * ``SIMP_BUILD_docs`` - (yes|no) - Toggle doc builds.

    * The docs take a long time to build!

  * ``RSYNC_NO_SELINUX_DEPS`` - (yes|no) - Force the earliest version of
    ``policycoreutils<-python>`` and ``selinux-policy<-devel>`` for the major
    EL release. For more information on why this is useful, see
    ``build/simp-rsync.spec`` in the ``simp-rsync-skeleton`` project.

    * This will most likely need to be set to ``yes``, as your system
      repositories are (probably) not going to contain the base versions of
      ``policycoreutils<-python>`` and ``selinux-policy<-devel>``

Once the process completes, you should have a bootable SIMP ISO, in:
``build/distributions/<OS>/<rel>/<arch>/SIMP_ISO/``


.. _SIMP artifacts repository: http://simp-project.com/ISO/SIMP/
.. _latest 6.1.0-0 release (for CentOS 6): http://simp-project.com/ISO/SIMP/SIMP-6.1.0-0-Powered-By-CentOS-6.8-x86_64.iso
.. _latest 6.1.0-0 release (for CentOS 7): http://simp-project.com/ISO/SIMP/SIMP-6.1.0-0-Powered-By-CentOS-7.0-x86_64.iso
.. _latest checksums: http://simp-project.com/ISO/SIMP/SHA512SUM

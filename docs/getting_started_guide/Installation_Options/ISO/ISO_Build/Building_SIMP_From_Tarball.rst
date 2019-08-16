.. _gsg-building_simp_from_tarball:

Building SIMP From Tarball
==========================

.. NOTE::

   Building SIMP from a pre-built tar file is the fastest method for getting a
   known stable build of a SIMP ISO and should be preferred over other methods.

.. WARNING::

   You must be on the SAME operating system that you wish to build. For
   instance, to build a CentOS 6 ISO, you need to be on a CentOS 6 system.
   Likewise, to build a RHEL 7 ISO, you must be on a RHEL 7 system.

   This is so that the build scripts can find the proper upstream repositories.

Getting Started
---------------

.. WARNING::

   Please have your environment prepared as specified by
   :ref:`gsg-environment_preparation` before continuing.

Download the SIMP release tar file, found on our `SIMP artifacts repository`_.

Download the latest tar file according to your needs. If you are not sure what
version you need, check the :ref:`faq-simp_version_guide`.

  * The `latest 6.2.0-0 release (for EL 6)`_
  * The `latest 6.2.0-0 release (for EL 7)`_
  * The `latest checksums`_

.. NOTE::

   Even though the tar files are labelled as ``CentOS``, they will work
   properly for ``RHEL`` systems as well.

Generating The ISO
------------------

Clone the Repo
^^^^^^^^^^^^^^

Clone simp-core:

.. code-block:: bash

  git clone https://github.com/simp/simp-core

Change into the ``simp-core`` directory and make sure you are on the correct
branch for your target SIMP version:

.. code::

   cd simp-core
   git checkout tags/6.2.0-0 # for SIMP 6.2

Update your Dependencies
^^^^^^^^^^^^^^^^^^^^^^^^

Run ``bundle update`` to make sure that all of the build tools and dependencies are
installed and up to date:

.. code::

   bundle update


Inject the Tar File
^^^^^^^^^^^^^^^^^^^

Copy the pre-built tar file to the ``DVD_Overlay`` directory that corresponds
with the version of base OS you want to build. For instance, if you wanted to
build with CentOS 7,

.. NOTE::

   For building on a RHEL system, you will need to replace the word ``CentOS``
   with the word ``RedHat`` in the tar file.

.. code::

   mkdir build/distributions/CentOS/7/x86_64/DVD_Overlay
   cp </path/to/.tar> build/distributions/CentOS/7/x86_64/DVD_Overlay


Optional - Update your Source Repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, the SIMP ISO builds from various Internet repositories. However, if
you are on a disconnected system, or building RHEL, you will need to tell the
build system how to get to your repositories.

To do this, make sure that you have a copy of the files listed in
``build/distributions/<OS>/<Release>/<Arch>/yum_data/packages.yaml`` in a
:term:`YUM` repo available to the build operating system.

Then, add properly formatted YUM repository configuration files that point to
the repositories that you wish to use at
``build/distributions/<OS>/<Release>/<Arch>/yum_data/repos`` in the same way
that you would update files in ``/etc/yum.repos.d``.

.. NOTE::

   The YUM repository configuration files will be used by the ``yumdownloader``
   command on the build host.

Build the ISO
^^^^^^^^^^^^^

Run the ``build:auto`` rake task to create a bootable ISO:

.. NOTE::

   Do **not** add any whitespace before or after the commas. This is an
   artifact of using ``rake``.

.. code::

   SIMP_BUILD_rm_staging_dir=no SIMP_BUILD_prompt=yes SIMP_ENV_NO_SELINUX_DEPS=yes bundle exec rake build:auto[<directory containing source ISOs>]

**Answer ``N`` when asked if you want to overwrite the tar file.**

Once the process completes, you should have a bootable SIMP ISO, in:
``build/distributions/<OS>/<Release>/<Arch>/SIMP_ISO/``

.. _SIMP artifacts repository: https://download.simp-project.com/simp/ISO
.. _latest 6.2.0-0 release (for EL 6): https://download.simp-project.com/simp/ISO/tar_bundles/SIMP-6.2.0-0.el6-CentOS-6-x86_64.tar.gz
.. _latest 6.2.0-0 release (for EL 7): https://download.simp-project.com/simp/ISO/tar_bundles/SIMP-6.2.0-0.el7-CentOS-6-x86_64.tar.gz
.. _latest checksums: https://download.simp-project.com/simp/ISO/tar_bundles/SHA512SUM

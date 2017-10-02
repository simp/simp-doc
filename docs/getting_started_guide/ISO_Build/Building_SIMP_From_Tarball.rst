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

Run the ``build:auto`` rake task to create a bootable ISO:

.. NOTE::
  Do **not** add any whitespace before or after the commas. This is an artifact
  of using ``rake``.

.. code::

   $ bundle exec rake build:auto[<directory containing source ISOs>,<SIMP version>,<path to tarball>]

For example:

.. code::

   $ # for SIMP 6 and CentOS 7
   $ bundle exec rake build:auto[.,6.X,SIMP-6.1.0-0-Overlay-EL-7-x86_64.tar.gz]

   $ # for SIMP 6 and CentOS 6
   $ bundle exec rake build:auto[.,6.X,SIMP-6.1.0-0-Overlay-EL-6-x86_64.tar.gz]

Once the process completes, you should have a bootable SIMP ISO ready for
installation!


.. _SIMP artifacts repository: http://simp-project.com/ISO/SIMP/
.. _latest 6.1.0-0 release (for CentOS 6): http://simp-project.com/ISO/SIMP/SIMP-6.1.0-0-Powered-By-CentOS-6.8-x86_64.iso
.. _latest 6.1.0-0 release (for CentOS 7): http://simp-project.com/ISO/SIMP/SIMP-6.1.0-0-Powered-By-CentOS-7.0-x86_64.iso
.. _latest checksums: http://simp-project.com/ISO/SIMP/SHA512SUM

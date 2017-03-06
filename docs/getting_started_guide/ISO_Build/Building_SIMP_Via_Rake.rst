.. _gsg-building_simp_via_rake:

Building SIMP via Rake
======================

The `simp-core`_ acceptance tests provide a method for building the SIMP ISOs
directly from online resources and is the method that the SIMP team uses to
ensure clean ISO builds.

.. NOTE::
   This method is **slow** but requires the least amount of modification to
   your build system.

.. WARNING::
   This method will only work on ``simp-core`` repositories that respond to
   ``rake build:auto``.

Getting Started
---------------

.. WARNING::
   Please have your environment prepared as specified by
   :ref:`gsg-environment_preparation` before continuing.

.. NOTE::
   You do **not** need ``mock`` on your system if you are using this method.

.. IMPORTANT::
   You **must** have Internet accessibility for this method to work!

Install Either Docker or VirtualBox
-----------------------------------

This build method relies on :term:`Beaker` from Puppet, Inc. and can use either
:term:`Docker` or :term:`VirtualBox` as the build back-end. The Docker method
is faster but the VirtualBox method is far easier to debug if issues arise.

You must have a working :term:`Vagrant` installation to use this method.

.. NOTE::
   Make sure that you use the Vagrant installation directly from the
   `Vagrant Homepage`_.

Download the Appropriate EL ISO
-------------------------------

You will need to download the correct :term:`EL` ISO for the version of SIMP
that you are trying to build. If you have doubts, check the
``build/release_mappings.yaml`` file.

The ISOs should be downloaded into the ``spec/fixtures/ISO`` directory (that
you create).

Required Environment Variables
------------------------------

If you simply run the :term:`rake` commands, the system will attempt to build
**all** versions of SIMP that are coded into the tests.

If you want to build a specific version of SIMP, you must use the following
environment variables.

SIMP_BEAKER_build_version
  The git branch or tag that you want to build

SIMP_BEAKER_build_map
  The ``release_mapper.yaml`` key that you want to use.
  Defaults to ``SIMP_BEAKER_build_version`` if not specified.

Build Using Docker
------------------

The :term:`Docker` method is the faster of the two methods but can make
debugging quite difficult if something goes wrong.

To build the suite via Docker run the following command:

.. code::

  $ rake beaker:suites[default]

Build Using VirtualBox
----------------------

If you wish to build using :term:`VirtualBox` simply run the following command:

.. code::

  $ rake beaker:suites[iso_vbox]

Results
-------

The ISOs from the :term:`rake` commands will be placed under a
``SIMP_ISO/rake_generated/<branch>/<timestamp>`` directory structure.

.. _simp-core: https://github.com/simp/simp-core
.. _Vagrant Homepage: https://www.vagrantup.com/downloads.html

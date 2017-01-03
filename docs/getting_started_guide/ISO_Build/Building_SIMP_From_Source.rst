.. _gsg-building_simp_from_source:

Building SIMP From Source
=========================

Getting Started
---------------

Please have your environment prepared as specified by :ref:`gsg-environment_preparation` before continuing.

Download the CentOS/RedHat installation media:

  * SIMP_5.X: `CentOS-7-x86_64-DVD-1511.iso`_
  * SIMP_4.X: `DVD1 and DVD2`_ of the CentOS 6.8 release. For example, CentOS-6.8-x86_64-bin-DVD1.iso

Generating The ISO!
-------------------

Change into the ``simp-core`` directory.

.. code::

   $ cd simp-core

Check out your desired branch of SIMP:

* To check out a stable SIMP release, check out a tag (*Recommended*):

.. code::

   $ git checkout tags/5.2.0-0

* To check out an unstable SIMP release, check out the latest 5.X or 4.X HEAD:

.. code::

   $ git checkout 5.1.X
   $ git checkout 4.2.X

.. NOTE::

   SIMP >= 5.2.X, >= 4.3.X are still developed on the 5.1.X and
   4.2.X branches, respectively.  We have not migrated our development
   to new branches.

Run ``bundle`` to make sure that all of the build tools and dependencies are installed and up to date:

.. code::

   $ bundle install

Make sure all of the source materials that were downloaded above are in your current working directory.

Run the ``build:auto`` rake task to create a bootable ISO using the following template:

.. code::

   $ bundle exec rake build:auto[<Directory containing install media>,<SIMP version>]

For example:

.. code::

   $ # for SIMP 5 and CentOS 7
   $ bundle exec rake build:auto[`pwd`,5.1.X]

   $ # for SIMP 4 and CentOS 6
   $ bundle exec rake build:auto[`pwd`,4.2.X]

Once the process completes, you should have a bootable SIMP ISO ready for installation!


.. _CentOS-7-x86_64-DVD-1511.iso: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1511.iso
.. _DVD1 and DVD2: http://isoredirect.centos.org/centos/6/isos/x86_64/

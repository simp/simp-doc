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

.. NOTE::
  You do **not** need ``mock`` on your system if you are generating an ISO
  based on the SIMP Tarball!

Download the CentOS/RedHat installation media:

  * SIMP_5.X: `CentOS-7-x86_64-DVD-1511.iso`_
  * SIMP_4.X: `DVD1 and DVD2`_ of the CentOS 6.8 release. For example,
    ``CentOS-6.8-x86_64-bin-DVD1.iso``

Download the SIMP release tarball, found on our `BinTray artifacts repository`_.

Download the latest tarball according to your needs. If you are not sure what
version you need, check the :ref:`faq-simp_version_guide`.

  * The `latest 5.2.0-0 release (for CentOS 7)`_
  * The `latest 4.3.0-0 release (for CentOS 6)`_

Generating The ISO!
-------------------

Change into the ``simp-core`` directory and make sure you are on the correct
branch for your target SIMP version:

.. code::

   $ cd simp-core
   $ git checkout tags/5.2.0-0 # for SIMP 5 and CentOS 7
   $ git checkout tags/4.3.1-0 # for SIMP 4 and CentOS 6

Run ``bundle install`` to make sure that all of the build tools and dependencies are
installed and up to date:

.. code::

   $ bundle install

Make sure all of the source materials that were downloaded above are in your
current working directory.

Run the ``build:auto`` rake task to create a bootable ISO:

.. NOTE::
  Do **not** add any whitespace before or after the commas. This is an artiface
  of using ``rake``.

.. code::

   $ bundle exec rake build:auto[<directory containing source ISOs>,<SIMP version>,<path to tarball>]

For example:

.. code::

   $ # for SIMP 5 and CentOS 7
   $ bundle exec rake build:auto[.,5.1.X,SIMP-DVD-CentOS-5.2.0-0.tar.gz]

   $ # for SIMP 4 and CentOS 6
   $ bundle exec rake build:auto[.,4.2.X,SIMP-DVD-CentOS-4.3.0-0.tar.gz]

Once the process completes, you should have a bootable SIMP ISO ready for
installation!


.. _BinTray artifacts repository: https://bintray.com/simp/Releases/Artifacts#files
.. _latest 5.2.0-0 release (for CentOS 7): https://bintray.com/simp/Releases/download_file?file_path=SIMP-DVD-CentOS-5.2.0-0.tar.gz
.. _latest 4.3.0-0 release (for CentOS 6): https://bintray.com/simp/Releases/download_file?file_path=SIMP-DVD-CentOS-4.3.0-0.tar.gz
.. _CentOS-7-x86_64-DVD-1511.iso: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1511.iso
.. _DVD1 and DVD2: http://isoredirect.centos.org/centos/6/isos/x86_64/

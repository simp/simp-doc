Building SIMP From Tarball
==========================

Getting Started
---------------

Download everything you need
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to build SIMP, you need to download the following source materials:

* The SIMP release tarball, found on our `BinTray artifacts repository`_.

  * The `latest 5.2.0-0 release (for CentOS 7)`_
  * The `latest 4.3.0-0 release (for CentOS 6)`_

* The CentOS/RedHat installation media

  * The `CentOS-7-x86_64-DVD-1511.iso`_ for SIMP 5
  * Both `DVD1 and DVD2`_ of the CentOS 6.8 release, named like CentOS-6.8-x86_64-bin-DVD1.iso, for SIMP 4


Preparing your Environment
^^^^^^^^^^^^^^^^^^^^^^^^^^

Please have your environment prepared as specified by :ref:`environment-prep` before continuing.


Generating the ISO!
-------------------

Now that everything is downloaded, we are ready to begin compilation or our ISO.

Change into the ``simp-core`` directory and make sure you are on the correct branch for your target SIMP version:

.. code::

   $ cd simp-core
   $ git checkout 5.1.X # for SIMP 5 and CentOS 7
   $ git checkout 4.2.X # for SIMP 4 and CentOS 6

Run ``bundle`` to make sure that all of our build tools and dependencies are installed and up to date:

.. code::

   $ bundle

Make sure all of the source materials that were downloaded above are in your current working directory.

Run our ``build:auto`` rake task to create a bootable ISO using the following template:

.. code::

   $ bundle exec rake build:auto[<SIMP version>,<directory containing source ISOs>,<path to tarball>]

If all of my source material is in my ``simp-core`` directory, my command might look like this:

.. code::

   $ # for SIMP 5 and CentOS 7
   $ bundle exec rake build:auto[5.1.X,.,SIMP-DVD-CentOS-5.2.0-0.tar.gz]

   $ # for SIMP 4 and CentOS 6
   $ bundle exec rake build:auto[4.2.X,.,SIMP-DVD-CentOS-4.3.0-0.tar.gz]

Once the process completes, you should have a bootable SIMP ISO ready for installation!


.. _BinTray artifacts repository: https://bintray.com/simp/Releases/Artifacts#files
.. _latest 5.2.0-0 release (for CentOS 7): https://bintray.com/simp/Releases/download_file?file_path=SIMP-DVD-RHEL-5.2.0-0.tar.gz
.. _latest 4.3.0-0 release (for CentOS 6):https://bintray.com/simp/Releases/download_file?file_path=SIMP-DVD-CentOS-4.3.0-0.tar.gz
.. _CentOS-7-x86_64-DVD-1511.iso: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1511.iso
.. _DVD1 and DVD2: http://isoredirect.centos.org/centos/6/isos/x86_64/

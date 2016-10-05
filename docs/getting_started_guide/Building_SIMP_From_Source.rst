Building SIMP From Source
=========================

Getting Started
---------------

Please have your environment prepared as specified by :ref:`Environment_Preparation` before continuing.

Download the CentOS/RedHat installation media:

  * SIMP_5.X: `CentOS-7-x86_64-DVD-1511.iso`_
  * SIMP_4.X: `DVD1 and DVD2`_ of the CentOS 6.8 release. For example, CentOS-6.8-x86_64-bin-DVD1.iso

Generating The ISO!
-------------------

Change into the ``simp-core`` directory and make sure you are on the correct branch for your target SIMP version:

.. code::

   $ cd simp-core
   $ git checkout 5.1.X # for SIMP 5 and CentOS 7
   $ git checkout 4.2.X # for SIMP 4 and CentOS 6

Run ``bundle`` to make sure that all of the build tools and dependencies are installed and up to date:

.. code::

   $ bundle

Make sure all of the source materials that were downloaded above are in your current working directory.

Run the ``build:auto`` rake task to create a bootable ISO using the following template:

.. code::

   $ bundle exec rake build:auto[<SIMP version>,<Directory containing install media>]

For example:

.. code::

   $ # for SIMP 5 and CentOS 7
   $ bundle exec rake build:auto[5.1.X,.]

   $ # for SIMP 4 and CentOS 6
   $ bundle exec rake build:auto[4.2.X,.]

Once the process completes, you should have a bootable SIMP ISO ready for installation!


.. _CentOS-7-x86_64-DVD-1511.iso: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1511.iso
.. _DVD1 and DVD2: http://isoredirect.centos.org/centos/6/isos/x86_64/

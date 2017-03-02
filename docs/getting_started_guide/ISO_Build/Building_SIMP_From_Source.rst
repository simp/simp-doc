.. _gsg-building_simp_from_source:

Building SIMP From Source
=========================

Getting Started
---------------

Please have your environment prepared as specified by :ref:`gsg-environment_preparation` before continuing.

Download the CentOS/RedHat installation media:

  * SIMP_6.X:
    * :term:`EL` 7: `CentOS-7-x86_64-DVD-1611.iso`_
    * :term:`EL` 6: `DVD1 and DVD2`_ of the :term:`CentOS` 6.8 release. For example, CentOS-6.8-x86_64-bin-DVD1.iso
  * SIMP_5.X: `CentOS-7-x86_64-DVD-1611.iso`_
  * SIMP_4.X: `DVD1 and DVD2`_ of the CentOS 6.8 release. For example, CentOS-6.8-x86_64-bin-DVD1.iso

Generating The ISO!
-------------------

Change into the ``simp-core`` directory.

.. code::

   $ cd simp-core

Check out your desired branch of SIMP:

* To check out a stable SIMP release, check out a tag (*Recommended*):

.. code::

   $ git checkout tags/6.0.0-0

* To check out an unstable SIMP release, check out the latest ``master``:

.. code::

   $ git checkout master

Run ``bundle`` to make sure that all of the build tools and dependencies are installed and up to date:

.. code::

   $ bundle install

Make sure all of the source materials that were downloaded above are in your current working directory.

Run the ``build:auto`` rake task to create a bootable ISO using the following template:

.. code::

   $ bundle exec rake build:auto[<Directory containing install media>,<SIMP version>]

For example:

.. code::

   $ # for SIMP 6
   $ bundle exec rake build:auto[/path/to/ISOs,6.X]

   $ # for SIMP 5 and CentOS 7
   $ bundle exec rake build:auto[/path/to/ISOs,5.1.X]

   $ # for SIMP 4 and CentOS 6
   $ bundle exec rake build:auto[/path/to/ISOs,4.2.X]

Once the process completes, you should have a bootable SIMP ISO ready for installation!

After You Build
---------------

If you've built from source, you will probably have noticed that a development
GPG key has been generated for this build.

This key is only valid for one week from generation and has been specifically
generated for your ISO build.

Doing this allows you to have a validly signed set of RPMs while reducing the
risk that you will have invalid RPMs distributed around your infrastructure.

.. NOTE::
   If you need to build and sign your RPMs with your own key, you can certainly
   do so using the ``rpm --resign`` command.

The new development key will be placed at the root of your ISO and will be
called ``RPM-GPG-KEY-SIMP_dev``. This key can be added to your clients, or
served via a web server, if you need to install from a centralized :term:`yum`
repository.

Please see the `Red Hat Guide to Configuring YUM and YUM Repositories`_ for
additional information.

.. _CentOS-7-x86_64-DVD-1611.iso: http://isoredirect.centos.org/centos/7/isos/x86_64/CentOS-7-x86_64-DVD-1611.iso
.. _DVD1 and DVD2: http://isoredirect.centos.org/centos/6/isos/x86_64/
.. _Red Hat Guide to Configuring YUM and YUM Repositories: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/7/html/System_Administrators_Guide/sec-Configuring_Yum_and_Yum_Repositories.html

.. _gsg-building_simp_from_source:

Building SIMP From Source
=========================

Getting Started
---------------

Please have your environment prepared as specified by
:ref:`gsg-environment_preparation` before continuing.

Download the CentOS/RedHat installation media:

  * SIMP_6.X:

    * Refer to ``release_mappings.yaml`` to determine the distribution ISO
      compatible with the version of SIMP you want to build.
      ``release_mappings.yaml`` is maintained the `simp-core`_ module in the
      ``build/distributions/<distribution>/<release>/<arch>`` directory.

  * SIMP_5.X: `CentOS-7-x86_64-DVD-1611.iso`_
  * SIMP_4.X: `DVD1 and DVD2`_ of the CentOS 6.8 release. For example, CentOS-6.8-x86_64-bin-DVD1.iso

Generating The ISO!
-------------------

Clone simp-core:

.. code-block:: bash

   $ git clone https://github.com/simp/simp-core
   $ cd simp-core

Check out your desired branch of SIMP:

* To check out a stable SIMP release, check out a tag (*Recommended*):

.. code::

   $ git checkout tags/6.1.0-0

* To check out an unstable SIMP release, check out the latest ``master``:

.. code::

   $ git checkout master

Run ``bundle`` to make sure that all of the build tools and dependencies are
installed and up to date:

.. code::

   $ bundle install

Make an ``ISO`` directory, and copy in the CentOS/RHEL installation media:

.. code-block:: bash

   $ mkdir ISO
   $ cp </path/to/dvd*.iso> ISO

Run the ``rpm_docker`` beaker suite, toggling build options with environment
variables:

.. code-block:: bash

   $ <build ENV vars> bundle exec rake beaker:suites[rpm_docker]

Build ENV vars:

  * ``SIMP_BUILD_docs`` - (yes|no) - Toggle doc builds.

    * The docs take a long time to build!

  * ``RSYNC_NO_SELINUX_DEPS`` - (yes|no) - Force the earliest version of
    ``policycoreutils<-python>`` and ``selinux-policy<-devel>`` for the major
    EL release.

    * In order to maintain the backward compatability of simp-rsync with each
      major EL release, we must bring in the selinux policies supplied by the
      original major EL release being built.  SELinux policies are forward
      compatible during a major release, but not necessarily backwards
      compatible.  If you opt to use repositories that bring in updated selinux
      policies, you will need to set this to ``YES``.

  * ``BEAKER_destroy`` - (yes|no) - Setting ``BEAKER_destroy=no`` will preserve
    the docker container used to build SIMP.

Once the process completes, you should have a bootable SIMP ISO, in:
``build/distributions/<OS>/<rel>/<arch>/SIMP_ISO/``

After You Build
---------------

You may have noticed that a development GPG key has been generated for the
build.

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
.. _Red Hat Guide to Configuring YUM and YUM Repositories: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system_administrators_guide/sec-configuring_yum_and_yum_repositories
.. _simp-core: https://github.com/simp/simp-core

.. _gsg-installing_simp_from_a_repository:

Installing SIMP From A Repository
=================================

Using the `official SIMP YUM repositories`_ is the simplest method for getting
up and running with SIMP on an existing infrastructure. If you are using a
virtual infrastructure, such as `AWS`_, `Microsoft Azure`_, `Google Cloud`_, or
your own internal VM stack, this is the method that you will want to use.

.. NOTE::

   This method does **not** modify your system's partitioning scheme or
   encryption scheme to meet any regulatory policies. If you want an example of
   what that should look like see the :term:`Kickstart` files in the
   `simp-core Git repository`_.


Enable EPEL
-----------

.. NOTE::

   On RHEL, enable the :term:`EPEL` repositories as described in
   `Getting started with EPEL`_. On Oracle Linux, install
   ``oracle-epel-release-el<N>`` (for example ``oracle-epel-release-el9``)
   instead of ``epel-release``.

.. code-block:: bash

   sudo dnf install -y epel-release

Install The OpenVox Repository
------------------------------

SIMP runs on :term:`OpenVox`, the community fork of Puppet. Install the OpenVox
release package for your OS major version, which adds the OpenVox repository
at https://yum.voxpupuli.org:

.. code-block:: bash

   sudo dnf install -y https://yum.voxpupuli.org/openvox9-release-el-$(rpm -E %rhel).noarch.rpm

.. NOTE::

   SIMP modules support OpenVox 8 and 9. To use OpenVox 8, install
   ``openvox8-release-el-<N>.noarch.rpm`` instead.

Install The SIMP-Project Repositories
-------------------------------------

Please reference :ref:`howto-use-the-simp-release-rpm`.

Rebuild The DNF Cache
---------------------

.. code-block:: bash

   sudo dnf makecache

Install the SIMP Server
-----------------------

Install the ``openvox-server`` package as follows:

.. code-block:: bash

   sudo dnf install -y openvox-server

.. NOTE::

   ``openvox-server`` replaces the ``puppetserver`` package but does not
   provide it, so ``dnf install puppetserver`` does not install it.

Install the core SIMP packages as follows:

.. code-block:: bash

   sudo dnf install -y simp

The ``simp`` RPM installs the SIMP core Puppet modules and other critical
SIMP assets such as its environment skeleton, custom SELinux policy, CLI,
and utilities.

* The Puppet modules are installed into ``/usr/share/simp`` and do not affect
  any existing Puppet environment.  Other steps in the SIMP server setup will
  deploy the modules into a Puppet environment.

SIMP also provides a large number of 'extra' Puppet module packages that you
can install as needed (``pupmod-simp-gnome``, ``pupmod-simp-nfs``, etc.).  You
can discover what extra modules are available by searching for ``pupmod`` via
``dnf``.  Alternatively, you can install all of the extra Puppet modules into
``/usr/share/simp`` by simply running ``sudo dnf install -y simp-extras``.

.. include:: ../jump_to_config.inc

.. _AWS: https://aws.amazon.com/
.. _Getting started with EPEL: https://docs.fedoraproject.org/en-US/epel/getting-started/
.. _Google Cloud: https://cloud.google.com
.. _Microsoft Azure: https://azure.microsoft.com/en-us/
.. _official SIMP YUM repositories: https://download.simp-project.com/simp/yum
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/CentOS/8/x86_64/DVD/ks

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
   `simp-core Git repository`_. Those examples were written for EL8. There are
   no EL9 or EL10 examples yet.


Enable EPEL
-----------

Some :term:`EPEL` packages depend on the CodeReady Builder (CRB) repository,
which is named PowerTools on EL8 and is disabled by default. Enable it, then
install the EPEL release package. Run the commands for your distribution.

AlmaLinux, Rocky Linux and CentOS Stream, EL9 and EL10:

.. code-block:: bash

   sudo dnf config-manager --set-enabled crb
   sudo dnf install -y epel-release

AlmaLinux and Rocky Linux, EL8:

.. code-block:: bash

   sudo dnf config-manager --set-enabled powertools
   sudo dnf install -y epel-release

RHEL:

.. code-block:: bash

   sudo subscription-manager repos --enable codeready-builder-for-rhel-$(rpm -E %rhel)-$(arch)-rpms
   sudo dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-$(rpm -E %rhel).noarch.rpm

Oracle Linux:

.. code-block:: bash

   sudo dnf config-manager --set-enabled ol$(rpm -E %rhel)_codeready_builder
   sudo dnf install -y oracle-epel-release-el$(rpm -E %rhel)

See `Getting started with EPEL`_ for details.

Install The OpenVox Repository
------------------------------

SIMP runs on :term:`OpenVox`, the community fork of Puppet. Install the OpenVox
9 release package, which adds the OpenVox repository at
https://yum.voxpupuli.org:

.. code-block:: bash

   sudo dnf install -y https://yum.voxpupuli.org/openvox9-release-el-$(rpm -E %rhel).noarch.rpm

.. NOTE::

   SIMP modules support OpenVox 8 and 9. To use OpenVox 8, run this instead:

   .. code-block:: bash

      sudo dnf install -y https://yum.voxpupuli.org/openvox8-release-el-$(rpm -E %rhel).noarch.rpm

.. NOTE::

   :pupmod:`simp/pupmod` defaults to OpenVox 8. If you installed OpenVox 9 and
   later set ``pupmod::master::install::install_release_package: true`` in
   :term:`Hiera`, also set ``pupmod::master::install::version: 9``. Otherwise
   Puppet adds the OpenVox 8 repository next to the OpenVox 9 one.

Install The SIMP-Project Repositories
-------------------------------------

Please reference :ref:`howto-use-the-simp-release-rpm`.

.. WARNING::

   That HOWTO has not been updated for EL8+ and OpenVox yet:

   * SIMP release RPMs exist for EL7 and EL8 only. There is no release RPM
     for EL9 or EL10, so the remaining steps on this page do not work there
     yet.
   * On EL8, the release RPM also enables repositories that carry the Puppet
     packages, which conflict with the OpenVox packages. Disable them so
     that OpenVox has a single package source:

     .. code-block:: bash

        sudo dnf config-manager --set-disabled puppet--simp puppet6--simp

Rebuild The DNF Cache
---------------------

.. code-block:: bash

   sudo dnf makecache

Install the SIMP Server
-----------------------

Install the :package:`openvox-server` package as follows:

.. code-block:: bash

   sudo dnf install -y openvox-server

.. NOTE::

   There is no :package:`puppetserver` package in the OpenVox repository.
   Install :package:`openvox-server`, which obsoletes and conflicts with
   :package:`puppetserver`. The service is still named ``puppetserver``.

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
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/RedHat/8/x86_64/DVD/ks

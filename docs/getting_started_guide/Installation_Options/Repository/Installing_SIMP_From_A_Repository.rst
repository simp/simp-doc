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

   RHEL systems will need to enable the :term:`EPEL` Repositories manually.

.. code-block:: bash

   sudo yum install epel-release -y
   sudo yum install pygpgme yum-utils -y

Install The SIMP-Project Repositories
-------------------------------------

Please reference :ref:`howto-use-the-simp-release-rpm`.

Rebuild The Yum Cache
---------------------

.. code-block:: bash

   sudo yum makecache

Install the SIMP Server
-----------------------

Install the ``puppetserver`` package as follows:

.. code-block:: bash

   sudo yum install -y puppetserver

Install the core SIMP packages as follows:

.. code-block:: bash

   sudo yum install -y simp

The ``simp`` RPM installs the SIMP core Puppet modules and other critical
SIMP assets such as its environment skeleton, custom SELinux policy, CLI,
and utilities.

* The Puppet modules are installed into ``/usr/share/simp`` and do not affect
  any existing Puppet environment.  Other steps in the SIMP server setup will
  deploy the modules into a Puppet environment.

SIMP also provides a large number of 'extra' Puppet module packages that you
can install as needed (``pupmod-simp-gnome``, ``pupmod-simp-nfs``, etc.).  You
can discover what extra modules are available by searching for ``pupmod`` via
``yum``.  Alternatively, you can install all of the extra Puppet modules into
``/usr/share/simp`` by simply running ``sudo yum install -y simp-extras``.

.. include:: ../jump_to_config.inc

.. _AWS: https://aws.amazon.com/
.. _Google Cloud: https://cloud.google.com
.. _Microsoft Azure: https://azure.microsoft.com/en-us/
.. _official SIMP YUM repositories: https://download.simp-project.com/simp/yum
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/CentOS/7/x86_64/DVD/ks

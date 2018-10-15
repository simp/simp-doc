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

   RHEL systems will need to enable the `EPEL Repositories`_ manually.

.. code-block:: bash

   $ sudo yum install epel-release -y
   $ sudo yum install pygpgme yum-utils -y

Install The SIMP-Project Repositories
-------------------------------------

Add the following to ``/etc/yum.repos.d/simp-project.repo``, replacing
``6`` with the appropriate version of SIMP. If the repo file does not exist,
create it. The repo file contents for ``SIMP 6.X`` is shown below.

If you don't know what versions map together, please see the
:ref:`faq-simp_version_guide`.

.. IMPORTANT::

   :term:`RHEL` Users should replace ``$releasever`` below with the actual release
   version.

   This would be ``7`` for RHEL 7 and ``6`` for RHEL 6

.. NOTE::

   The 'dependencies' repository may contain items from external vendors, most
   notably Puppet, Inc. and EPEL but may also contain non-SIMP project files
   that have been compiled for distribution.

.. WARNING::

   The **whitespace** and **alignment** shown before the additional ``gpgkey``
   values **must be preserved**

.. code-block:: bash

   [simp-project_6_X]
   name=simp-project_6_X
   baseurl=https://packagecloud.io/simp-project/6_X/el/$releasever/$basearch
   gpgcheck=1
   enabled=1
   gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
          https://download.simp-project.com/simp/GPGKEYS/RPM-GPG-KEY-SIMP-6       
   sslverify=1
   sslcacert=/etc/pki/tls/certs/ca-bundle.crt
   metadata_expire=300

   [simp-project_6_X_dependencies]
   name=simp-project_6_X_dependencies
   baseurl=https://packagecloud.io/simp-project/6_X_Dependencies/el/$releasever/$basearch
   gpgcheck=1
   enabled=1
   gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
          https://download.simp-project.com/simp/GPGKEYS/RPM-GPG-KEY-SIMP-6       
          https://yum.puppet.com/RPM-GPG-KEY-puppetlabs
          https://yum.puppet.com/RPM-GPG-KEY-puppet
          https://apt.postgresql.org/pub/repos/yum/RPM-GPG-KEY-PGDG-96
          https://artifacts.elastic.co/GPG-KEY-elasticsearch
          https://grafanarel.s3.amazonaws.com/RPM-GPG-KEY-grafana
          https://dl.fedoraproject.org/pub/epel/RPM-GPG-KEY-EPEL-$releasever
   sslverify=1
   sslcacert=/etc/pki/tls/certs/ca-bundle.crt
   metadata_expire=300

Rebuild The Yum Cache
---------------------

.. code-block:: bash

   $ sudo yum makecache

Install the SIMP Server
-----------------------

#. Select the simp-adapter package appropriate for the version of Puppet
   you will be using

   * **simp-adapter-foss**:  Version appropriate for FOSS Puppet
   * **simp-adapter-pe**:   Version appropriate for Puppet Enterprise

#. Install the simp-adapter package

   .. code-block:: bash

      $ sudo yum install -y simp-adapter-foss

#. Install the remaining SIMP packages

   .. code-block:: bash

      $ sudo yum install -y simp

.. NOTE::

   The ``simp`` RPM installs the SIMP core Puppet modules. Breaking changes in
   these modules trigger a breaking change update in SIMP itself.

   There are a large number of additional 'extra' modules that may be
   individually installed. Search for ``pupmod`` via ``yum`` to discover what
   is available.

   If you wish to install all of the extra modules, you can simply run ``sudo
   yum install -y simp-extras``

.. include:: ../jump_to_config.inc

.. _AWS: https://aws.amazon.com/
.. _EPEL Repositories: https://fedoraproject.org/wiki/EPEL
.. _Google Cloud: https://cloud.google.com
.. _Microsoft Azure: https://azure.microsoft.com
.. _official SIMP YUM repositories: https://packagecloud.io/simp-project
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/CentOS/7/x86_64/DVD/ks

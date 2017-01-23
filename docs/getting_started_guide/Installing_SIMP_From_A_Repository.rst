.. _gsg-installing_simp_from_a_repository:

Installing SIMP From A Repository
=================================

Using the `official SIMP YUM repositories`_ is the simplest method for getting
up and running with a SIMP system. If you are using a virtual infrastructure,
such as `AWS`_, `Microsoft Azure`_, `Google Cloud`_, or your own internal VM
stack, this is the method that you will almost definitely want to use.

.. NOTE::
  This method does *not* modify your system's partitioning scheme or encryption
  scheme to meet any regulatory policies. If you want an example of what that
  should look like either see the :ref:`simp-installation-guide` or check out the
  `Kickstart`_ files in the `simp-core Git repository`_.


Enable EPEL
-----------

.. code-block:: bash

   $ sudo yum install epel-release -y
   $ sudo yum install pygpgme yum-utils

Install The SIMP-Project Repository
-----------------------------------

Add the following to ``/etc/yum.repos.d/simp-project.repo``, replacing ``7`` with
the appropriate version of EL and ``5`` with the appropriate version of SIMP. If
the repo file does not exist, create it.
``EL 7`` with ``SIMP 5.X`` is shown below.

If you don't know what versions map together, please see the
:ref:`faq-simp_version_guide`.


.. code-block:: bash

  [simp-project_5_X]
  name=simp-project_5_X
  baseurl=https://packagecloud.io/simp-project/5_X/el/7/$basearch
  gpgcheck=1
  enabled=1
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

  [simp-project_5_X-source]
  name=simp-project_5_X-source
  baseurl=https://packagecloud.io/simp-project/5_X/el/7/SRPMS
  gpgcheck=1
  enabled=0
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

Install The SIMP-project_dependencies Repository
------------------------------------------------

.. NOTE::
  The repository may contain items from external vendors, most notably Puppet,
  Inc. and EPEL but may also contain non-SIMP project files that have been
  compiled for distribution.

Add the following to ``/etc/yum.repos.d/simp-project_dependencies.repo``,
replacing ``7`` with the appropriate version of EL and ``5`` with the appropriate
version of SIMP.  ``EL 7`` with ``SIMP 5.X`` is shown below. If
the repo file does not exist, create it.

If you don't know what versions map together, please see the
:ref:`faq-simp_version_guide`.

.. NOTE::
  The **whitespace** and **alignment** shown before the additional ``gpgkey`` values
  **must be preserved**

.. code-block:: bash

  [simp-project_5_X_dependencies]
  name=simp-project_5_1_X_dependencies
  baseurl=https://packagecloud.io/simp-project/5_X_Dependencies/el/7/$basearch
  gpgcheck=1
  enabled=1
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
         https://yum.puppetlabs.com/RPM-GPG-KEY-puppetlabs
         https://getfedora.org/static/352C64E5.txt
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

  [simp-project_5_X_dependencies-source]
  name=simp-project_5_X_dependencies-source
  baseurl=https://packagecloud.io/simp-project/5_X_Dependencies/el/7/SRPMS
  gpgcheck=1
  enabled=0
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
         https://yum.puppetlabs.com/RPM-GPG-KEY-puppetlabs
         https://getfedora.org/static/352C64E5.txt
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

Rebuild The Yum Cache
---------------------

.. code-block:: bash

   $ sudo yum makecache

Install SIMP
-------------
.. code-block:: bash

   $ sudo yum install -y simp

Modify Yum URLs
---------------

Set the following variables to repositories of your choosing in
``/etc/puppetlabs/code/environments/production/hieradata/default.yaml``

.. code-block:: yaml

   # Full URL to a YUM repo for Operating System packages
   simp::yum::os_update_url: 'http://mirror.centos.org/centos/$releasever/os/$basearch/'
   # Full URL to a YUM repo for SIMP packages
   simp::yum::simp_update_url: 'https://packagecloud.io/simp-project/5_X/el/7/$basearch'

SIMP Config
-----------

Run simp config:

.. code-block:: bash

   $ simp config

.. NOTE::
  If you intend to use FIPS, set ``use_fips=true`` during simp config and follow
  the `Enable FIPS`_ instructions after config is complete.  Otherwise, set it to
  ``false`` and skip Enable FIPS.

Enable FIPS
-----------

.. code-block:: bash

   $ rm -rf /etc/puppetlabs/puppet/ssl
   $ yum-config-manager --enable base
   $ yum install dracut-fips
   $ dracut -f
   $ reboot now

SIMP Bootstrap
--------------

.. code-block:: bash

   $ simp bootstrap

Clients
-------

Use the ``runpuppet`` script from the newly created SIMP server to bootstrap
your clients.

.. NOTE::
  This would be the general technique that you would use to auto-bootstrap your
  clients via ``user-data`` scripts in cloud environments.

  Be ready to sign your client credentials as systems check in with the server!

.. code-block:: bash

   $ curl http://<puppet.server.fqdn>/ks/runpuppet | bash

.. _official SIMP YUM repositories: https://packagecloud.io/simp-project
.. _AWS: https://aws.amazon.com/
.. _Microsoft Azure: https://azure.microsoft.com
.. _Google Cloud: https://cloud.google.com
.. _Kickstart: http://pykickstart.readthedocs.io/en/latest
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/5.1.X/src/DVD/ks

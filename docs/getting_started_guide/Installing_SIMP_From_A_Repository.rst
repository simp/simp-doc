.. _installing_simp_from_a_repository:

Installing SIMP From A Repository
=================================

Enable EPEL
-----------

.. code-block:: bash

  sudo yum install epel-release -y
  sudo yum install pygpgme yum-utils

Install The SIMP-Project Repository
-----------------------------------

.. code-block:: bash

  touch /etc/yum.repos.d/simp-project.repo

Add the following to simp-project.repo, replacing 7 with the appropriate version
of EL and 5 with the appropriate version of SIMP (EL 7,SIMP 5.X shown below)

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

.. note::

  The repository may contain items from external vendors, most notably
  Puppet, Inc. and EPEL but may also contain non-SIMP project files that have
  been compiled for distribution.

.. code-block:: bash

  touch /etc/yum.repos.d/simp-project_dependencies.repo

Add the following to simp-project_dependencies.repo, replacing 7 with the
appropriate version of EL and 5 with the appropriate version of SIMP (EL 7,
SIMP 5.X shown below)

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

  yum makecache

Install SIMP!
-------------
.. code-block:: bash

  sudo yum install simp

Modify Yum URLs
---------------

Set the following variables to repositories of your choosing in
/etc/puppet/environments/production/hieradata/default.yaml

.. code-block:: yaml

  # Full URL to a YUM repo for Operating System packages
  simp::yum::os_update_url: "http://mirror.centos.org/centos/$releasever/os/$basearch/"
  # Full URL to a YUM repo for SIMP packages
  simp::yum::simp_update_url: "https://packagecloud.io/simp-project/5_X/el/7/$basearch"
 
SIMP Config
-----------
Run simp config, setting use_fips: true!

.. code-block:: bash

  simp config

Enable FIPS
-----------

.. code-block:: bash

  rm -rf /var/lib/puppet/ssl
  yum-config-manager --enable base
  yum install dracut-fips
  dracut -f
  reboot now

Bootstrap Bootstrap Bootstrap
-----------------------------

.. code-block:: bash

  simp bootstrap

Clients
-------

Add clients as you would a normal Puppet client.

Alternatively, you can download the runpuppet script from the SIMP server

.. code-block:: bash

  curl http://puppet.server.fqdn/ks/runpuppet > runpuppet
  chmod +x runpuppet
  ./runpuppet

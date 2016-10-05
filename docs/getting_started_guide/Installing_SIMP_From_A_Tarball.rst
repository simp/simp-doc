Installing SIMP From A Tarball
==============================

If you've already got an existing installation of a supported operating system
that you'd like to install a SIMP server on, this is the guide for you.

Prerequisites
-------------

Ensure createrepo is installed:

.. code-block:: bash

  $ yum install createrepo

Obtain a tarball.  Create your own by :ref:`Building_SIMP_From_Source` or
download a release from `Bintray`_.

Create The SIMP Repo
--------------------

Unpack the tarball.  For example:

.. code-block:: bash

  $ tar xfv SIMP-DVD-CentOS-5.2.0-0.tar.gz

Add the SIMP GPG keys:

.. code-block:: bash

  $ curl https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP | gpg --import -
  $ gpg --import SIMP/GPGKEYS/*

Create a local repo in SIMP. Make a note of the location of this repo, as you
will need it in the next step.

.. code-block:: bash

  $ createrepo -p SIMP

Add Repo Files For SIMP Dependencies
------------------------------------

.. note::

  These repo files are available in build/yum_data/SIMP*/repos/ in your build
  folder.

elasticsearch.repo:

.. code-block:: bash

  [elasticsearch-1.3]
  name=Elasticsearch repository for 1.3.x packages
  baseurl=http://packages.elastic.co/elasticsearch/1.3/centos
  gpgcheck=1
  gpgkey=http://packages.elastic.co/GPG-KEY-elasticsearch
  enabled=1
  [elasticsearch-1.6]
  name=Elasticsearch repository for 1.6.x packages
  baseurl=http://packages.elastic.co/elasticsearch/1.6/centos
  gpgcheck=1
  gpgkey=http://packages.elastic.co/GPG-KEY-elasticsearch
  enabled=1

epel.repo:

.. code-block:: bash

  [epel]
  name=epel
  mirrorlist=http://mirrors.fedoraproject.org/mirrorlist?repo=epel-6&arch=x86_64&country=US
  failovermethod=priority

simp.repo:

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

simp-local.repo:

The baseurl of the local repo should be the absolute path to the local SIMP
repo created earlier, starting with file://

.. code-block:: bash

  [simp-local]
  name=SIMP-5.2.X for CentOS
  baseurl=file:///path/to/simp/repo
  enabled=1
  gpgcheck=1

Install SIMP
------------

.. code-block:: bash

  $ yum install simp


.. _Bintray: https://bintray.com/simp/Releases

Installing SIMP From A Tarball
==============================

If you've already got an existing installation of a supported operating system
that you'd like to install a SIMP server on, this is the guide for you.

Prerequisites
-------------

* Ensure createrepo is installed:

.. code-block:: bash

  $ yum install createrepo

* Obtain a tarball.  Create your own by :ref:`Building_SIMP_From_Source` or
  download a release from `Bintray`_.

* For the purpose of this doc, we will assume you want to install the SIMP repo
  in /var/www/yum.

Create The SIMP Repo
--------------------

As root, unpack the tarball and createrepo:

.. code-block:: bash

  $ cd /var/www/yum
  $ tar xfv SIMP-DVD-CentOS-5.2.0-0.tar.gz
  $ createrepo -p SIMP
  $ chown -R root.apache SIMP
  $ chmod -R g+rX SIMP

Add the SIMP GPG keys:

.. code-block:: bash

  $ curl https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP | gpg --import -
  $ gpg --import SIMP/GPGKEYS/*

Create the simp repofile:

.. code-block:: bash

  [simp-local]
  name=SIMP-5.2.X for CentOS
  baseurl=file:///var/www/yum/SIMP
  enabled=1
  gpgcheck=1

Rebuild the YUM cache:

.. code-block:: bash

  $ yum makecache

Add Repo Files For SIMP Dependencies
------------------------------------

Choose the yum_data directory for your flavor of SIMP:

* `SIMP-5.X yum_data`_
* `SIMP-4.X yum_data`_

Navigate to the repos directory corresponding to your flavor of EL, for
example: SIMP5.1.0_CentOS7.0_x86_64/repos

Copy each .repo file to your system, to /etc/yum.repos.d/

Rebuild the YUM cache:

.. code-block:: bash

  $ yum makecache

Install SIMP
------------

.. code-block:: bash

  $ yum install simp

.. _Bintray: https://bintray.com/simp/Releases
.. _SIMP-5.X yum_data: https://github.com/simp/simp-core/tree/5.1.X/build/yum_data
.. _SIMP-4.X yum_data: https://github.com/simp/simp-core/tree/4.2.X/build/yum_data

Enable EPEL
~~~~~~~~~~~

.. code-block:: bash

   $ sudo yum install epel-release -y
   $ sudo yum install pygpgme yum-utils

Install The SIMP-project_dependencies Repository
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. NOTE::
   The repository may contain items from external vendors, most notably Puppet,
   Inc. and EPEL but may also contain non-SIMP project files that have been
   compiled for distribution.

Add the following to ``/etc/yum.repos.d/simp-project_dependencies.repo``,
replacing ``6`` with the appropriate version of SIMP. If the repo file
does not exist, create it. The repo file for ``SIMP 6.X`` is shown below.

If you don't know what versions map together, please see the
:ref:`faq-simp_version_guide`.

.. NOTE::
   The **whitespace** and **alignment** shown before the additional ``gpgkey``
   values **must be preserved**

.. code-block:: bash

  [simp-project_6_X_dependencies]
  name=simp-project_6_X_dependencies
  baseurl=https://packagecloud.io/simp-project/6_X_Dependencies/el/$releasever/$basearch
  gpgcheck=1
  enabled=1
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
         https://yum.puppetlabs.com/RPM-GPG-KEY-puppetlabs
         https://yum.puppetlabs.com/RPM-GPG-KEY-puppet
         https://apt.postgresql.org/pub/repos/yum/RPM-GPG-KEY-PGDG-94
         https://getfedora.org/static/352C64E5.txt
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

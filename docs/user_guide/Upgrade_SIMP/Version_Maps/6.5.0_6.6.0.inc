.. _upgrade-6.5.0-to-6.6.0:

Upgrading from SIMP-6.5.0 to SIMP-6.6.0
---------------------------------------

.. contents:: :local:
   :depth: 3

.. _before-upgrading-to-6.6.0:

Before Upgrading to 6.6.0
^^^^^^^^^^^^^^^^^^^^^^^^^

SSSD hiera changes
""""""""""""""""""
There are two changes for systems that use :term:`SSSD`:


* If LDAP and SSSD are both used, set the type of LDAP server in hiera. For EL7 servers with :term:`Openldap` set the following:

  .. code-block:: yaml

      simp::sssd::client:ldap_server_type: 'plain'

* In previous versions of Red Hat, SSSD would fail without a LOCAL domain.  SSSD no longer requires a LOCAL domain and the :code:`simp::sssd::client` manifest no longer configures a LOCAL domain.  If you have not configured a LOCAL domain for some other reason you should remove LOCAL from the list of domains in hiera under the :code:`sssd::domains:` entry and disable the creation of the local domain by :code:`simp::sssd::client`.

  .. code-block:: yaml

     sssd::domains:
       - LOCAL

* If there are errors in systems before 6.6.0 is fully installed the creation of the local domain can be prevented by setting the following in hiera:

  .. code-block:: yaml

     simp::sssd::client::local_domain: false

NTP Changes
"""""""""""

The simp_options parameter used to set the list of NTP servers has been changed to simp_options::ntp::servers.  Both ntpd and chronyd use simp_options::ntp::servers as the default.

In hiera rename simp_options::ntpd::servers to simp_options::ntp::servers and create an alias for backwards compatibility:

Old hiera:

.. code-block:: yaml

   simp_options::ntpd::servers:
   - server1.time.server.org
   - server2.time.server.org
   - server3.time.server.org
   - 127.127.1.0

New Hiera:

.. code-block:: yaml

   simp_options::ntp::servers:
   - server1.time.server.org
   - server2.time.server.org
   - server3.time.server.org
   - 127.127.1.0
   # The following is work around needed during upgrade
   simp_options::ntpd::servers: "%{alias('simp_options::ntp::servers')}"


Update Yum Repositories
"""""""""""""""""""""""

The following replaces step 1, Update the YUM Repositories,  of  :ref:`Incremental Upgrades <ug-incremental-upgrades>` instructions.

The SIMP repo has been moved from :file:`/var/www/yum/SIMP` to :file:`/var/www/yum/SIMP/<os name>/<os version>`.

Updates were made to the  :code:`unpack_dvd` script in :package:`simp-utils` to extract the rpms to the new location.  The following commands step through updating simp_utils from the iso, extracting the iso, removing the old repository and linking the new repository to the old location for backwards compatibility.

.. code-block::

  # Mount the ISO.
  mkdir /mnt/iso
  mount -o loop -t iso9660 <location of iso file>
  # Install the simp_utils RPM.
  cd /mnt/iso/SIMP/noarch
  yum install ./simp-utils*.rpm
  # Unmount the ISO
  cd /
  umount /mnt/iso
  # Extract the ISO.
  unpack_dvd  -v 7.9.2009  <location of iso file>
  # Remove old files and link to the new files
  cd /var/www/yum/SIMP
  rm -rf noarch x86_64
  ln -s ./CentOS/7/noarch ./noarch
  ln -s ./CentOS/7/x86_64 ./x86_64

If you use kickstart you will need to extract the PXE files before kickstarting new CentOS 7 machines.  See :ref:`howto-unpack-dvd-pxe` for more information.

Puppetlabs has a new :term:`GPG` key that must be used to install the new puppet agent.
Install this key on the yum server by updating :package:`simp-gpgkeys`.

.. code-block:: sh

   yum install simp-gpgkeys

To ensure that all the clients pick up the new key add the following to the top level of Hiera:

.. code-block:: yaml

   # This can be removed once the upgrade is complete
   simp::yum::repo::local_simp::extra_gpgkey_urls:
     - https://<your yum server ip address>/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406

Complete the General Steps
""""""""""""""""""""""""""

After resolving all applicable items in :ref:`Before Upgrading to 6.6.0 <before-upgrading-to-6.6.0>`,
complete from step 2 on in the  :ref:`Incremental Upgrades <ug-incremental-upgrades>` instructions.


After Upgrading to 6.6.0
^^^^^^^^^^^^^^^^^^^^^^^^

Once the upgrade is complete do the following to clean up work arounds:

On the puppetserver the following items can be removed if you used them:

.. code-block:: yaml

   simp::yum::repo::local_simp::extra_gpgkey_urls:
     - https://<your yum server ip address>/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406
   simp::sssd::client::local_domain: false
   simp_options::ntpd::servers: "%{alias('simp_options::ntp::servers')}"

On the yum server remove the links in /var/www/yum/SIMP.

.. code-block:: sh

   cd /var/www/yum/SIMP
   unlink noarch
   unlink x86_64

Upgrading to EL8
^^^^^^^^^^^^^^^^

To upgrade your puppet server to EL8, you would follow the installation instructions and then the :ref:`howto-migrate-to-new-puppet-server`

Scripts to transfer user and group entries from Openldap to 389-DS were included in :package:`simp-utils`.  They are installed under /usr/share/simp/transition_scripts/openldap_to_389ds with instructions in the README file in that directory.


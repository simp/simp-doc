.. _upgrade-6.5.0-to-6.6.0:

Upgrading from SIMP-6.5.0 to SIMP-6.6.0
---------------------------------------

.. contents:: :local:
   :depth: 3

.. _before-upgrading-to-6.6.0:

Before Upgrading to 6.6.0
^^^^^^^^^^^^^^^^^^^^^^^^^

SSSD Hiera Changes
""""""""""""""""""

There are two changes for systems that use :term:`SSSD`:

Set simp::sssd::client::ldap_server_type
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If :term:`LDAP:` and :term:`SSSD` are both used, you must set the type of LDAP
server in :term:`hiera` using the :code:`simp::sssd::client::ldap_server_type`
parameter.

If the system is using an OpenLDAP server, you would set this option to
:code:`plain`. For 389-DS-derived systems (389-DS, FreeIPA, RHDS, etc...) you
would set this to :code:`389ds`. An example is provided below

.. code-block:: yaml

   # The upstream server is OpenLDAP
   simp::sssd::client:ldap_server_type: 'plain'

   # The upstream server is 389-DS
   simp::sssd::client:ldap_server_type: '389ds'

Requirement for SSSD >= 1.16.0
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In systems using :program:`sssd` prior to 1.16.0, the daemon would fail to start
without a :code:`LOCAL` domain. For security and supportability reasons, SIMP no
longer supports :program:`sssd` prior to 1.16.0. Likewise, the
:code:`simp::sssd::client` class no longer configures a :code:`LOCAL` domain.

If you have configured a :code:`LOCAL` domain for some other reason you must:

* Remove :code:`LOCAL` from the list of domains in :term:`hiera` from
  :code:`sssd::domains:`
* Disable the creation of the local domain by :code:`simp::sssd::client`

.. code-block:: yaml

   # These are examples of the sssd::domains entry you might see in hiera.

   # LOCAL is the only entry, remove everything
   sssd::domains:
     - LOCAL

   # LOCAL is not the only entry, only remove LOCAL
   sssd::domains:
     - LOCAL #remove this
     - MY_DOMAIN
     - SOMETHING_ELSE

If there are errors in systems before SIMP 6.6.0 has been fully installed, the
creation of the :code:`LOCAL` domain can be prevented by setting the following
in :term:`hiera`:

.. code-block:: yaml

   simp::sssd::client::local_domain: false

NTP Changes
"""""""""""

The :code:`simp_options::ntpd::servers` parameter has been changed to
:code:`simp_options::ntp::servers`.

Both :program:`ntpd` and :program:`chronyd` use
:code:`simp_options::ntp::servers` as the default.

In :term:`hiera`, users must:

* Rename :code:`simp_options::ntpd::servers` to :code:`simp_options::ntp::servers`
* Create an :code:`alias` for backwards compatibility

Old :term:`hiera`:

.. code-block:: yaml

   simp_options::ntpd::servers:
   - server1.time.server.org
   - server2.time.server.org
   - server3.time.server.org
   - 127.127.1.0

New :term:`hiera`:

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

Migrate to the New Format
^^^^^^^^^^^^^^^^^^^^^^^^^

**The following replaces Step 1**, `Update the YUM Repositories`, of the
:ref:`Incremental Upgrades <ug-incremental-upgrades>` instructions.

The SIMP :term:`YUM` repository has been moved from :file:`/var/www/yum/SIMP` to
:file:`/var/www/yum/SIMP/<os name>/<os version>` to make supporting multiple
client operating systems easier.

Updates were made to the  :code:`unpack_dvd` script in :package:`simp-utils` to
extract the RPMs to the new location.

Use the following as `root` to properly migrate to the new format:

.. code-block:: bash

  # Mount the ISO.
  mkdir /mnt/iso
  mount -o loop -t iso9660 <location of iso file>

  # Install the simp_utils RPM.
  cd /mnt/iso/SIMP/noarch
  yum install -y ./simp-utils*.rpm || yum update -y ./simp-utils*.rpm

  # Unmount the ISO
  cd $HOME
  umount /mnt/iso

  # Extract the ISO.
  unpack_dvd  -v 7.9.2009  <location of iso file>

  # Remove old files and link to the new files
  cd /var/www/yum/SIMP
  rm -rf noarch x86_64
  ln -s ./CentOS/7/noarch ./noarch
  ln -s ./CentOS/7/x86_64 ./x86_64

Install the new Puppet GPG Key
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Puppet, Inc. has a new :term:`GPG` key that must be used to install the new
puppet agent.  Install this key on the YUM server by updating
:package:`simp-gpgkeys`.

.. code-block:: sh

   yum install simp-gpgkeys

To ensure that all the clients pick up the new key add the following to the top
level of :term:`Hiera`:

.. code-block:: yaml

   # This can be removed once the upgrade is complete
   simp::yum::repo::local_simp::extra_gpgkey_urls:
     - https://<your yum server ip address>/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406

Kickstart Prep
^^^^^^^^^^^^^^

If you use kickstart, you will need to extract the PXEboot files before
kickstarting new EL7 machines.  See :ref:`howto-unpack-dvd-pxe` for more
information.

Complete the General Steps
""""""""""""""""""""""""""

After resolving all applicable items in :ref:`Before Upgrading to 6.6.0
<before-upgrading-to-6.6.0>`, resume :ref:`Incremental Upgrades
<ug-incremental-upgrades>` instructions **from Step 2**.


After Upgrading to 6.6.0
^^^^^^^^^^^^^^^^^^^^^^^^

Cleanup Tasks
^^^^^^^^^^^^^

The following items can be removed from the :term:`puppetserver`:

.. code-block:: yaml

   simp::yum::repo::local_simp::extra_gpgkey_urls:
     - https://<your yum server ip address>/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406

   simp::sssd::client::local_domain: false

   simp_options::ntpd::servers: "%{alias('simp_options::ntp::servers')}"

The following items can be removed fro the :term:`YUM` server:

.. code-block:: bash

   unlink /var/www/yum/SIMP/noarch
   unlink /var/www/yum/SIMP/x86_64

Upgrading to EL8
----------------

Main Server Migration
^^^^^^^^^^^^^^^^^^^^^

To upgrade your puppet server to EL8, you would follow the installation
instructions and then the :ref:`howto-migrate-to-new-puppet-server`

Migration to 389-DS from OpenLDAP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Scripts to transfer user and group entries from OpenLDAP to 389-DS are included
in :package:`simp-utils`.

The scripts are installed at :file:`/usr/share/simp/transition_scripts/openldap_to_389ds`.

Instructions can be found in the :file:`README` in the same directory.

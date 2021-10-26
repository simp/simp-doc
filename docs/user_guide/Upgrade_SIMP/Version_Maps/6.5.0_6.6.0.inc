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

This section describes two changes for systems that use :term:`SSSD`.

Set simp::sssd::client::ldap_server_type
''''''''''''''''''''''''''''''''''''''''

If :term:`LDAP` and :term:`SSSD` are both used, you must now set the type of LDAP
server in :term:`hiera` using the :code:`simp::sssd::client::ldap_server_type`
parameter.

If the system is using an OpenLDAP server, you would set this option to
:code:`plain`. For 389-DS-derived systems (389-DS, FreeIPA, RHDS, etc...) you
would set this to :code:`389ds`. Examples are provided below:

* When the upstream server is OpenLDAP

  .. code-block:: yaml

     simp::sssd::client::ldap_server_type: 'plain'

* When the upstream server is 389-DS

  .. code-block:: yaml

     simp::sssd::client::ldap_server_type: '389ds'

Remove SIMP-managed LOCAL domain configuration
''''''''''''''''''''''''''''''''''''''''''''''

In systems using :program:`sssd` prior to 1.16.0, the daemon would fail to start
if no domains were configured. Earlier versions of SIMP worked around this by
creating a :code:`LOCAL` domain that used the SSSD :code:`files` provider.

For security and supportability reasons, SIMP no longer supports :program:`sssd`
prior to 1.16.0. Likewise, the :code:`simp::sssd::client` class no longer
configures a :code:`LOCAL` domain. If you have configured a :code:`LOCAL`
domain you must do the following:

* Remove :code:`LOCAL` from the list of domains in :term:`hiera` from
  :code:`sssd::domains`
* Disable the creation of the :code:`LOCAL` domain by :code:`simp::sssd::client`

  * This is only required until SIMP is upgraded to 6.6.0.

Below are examples:

* When LOCAL is the only :code:`sssd::domains` entry

  .. code-block:: yaml

     # BEFORE hieradata
     sssd::domains:
     - LOCAL

  .. code-block:: yaml

     # AFTER hieradata
     # sssd::domains has been removed!

     # This can be removed once the upgrade is complete
     simp::sssd::client::local_domain: false

* When LOCAL is not the only :code:`sssd::domains` entry

  .. code-block:: yaml

     # BEFORE hieradata
     sssd::domains:
     - LOCAL
     - MY_DOMAIN
     - SOMETHING_ELSE

  .. code-block:: yaml

     # AFTER hieradata
     # LOCAL has been removed from sssd::domains
     sssd::domains:
     - MY_DOMAIN
     - SOMETHING_ELSE

     # This can be removed once the upgrade is complete
     simp::sssd::client::local_domain: false

NTP Hiera Changes
"""""""""""""""""

The :code:`simp_options::ntpd::servers` parameter has been changed to
:code:`simp_options::ntp::servers` in order to support both :program:`ntpd`
and :program:`chronyd` configuration, via :pupmod:`simp/ntpd` and
:pupmod:`puppet/chrony`, respectively.

In :term:`hiera`, users must:

* Rename :code:`simp_options::ntpd::servers` to :code:`simp_options::ntp::servers`
* Add an :code:`alias` for :code:`chrony::servers`
* Add a temporary :code:`alias` for :code:`simp_options::ntpd::servers` for
  backwards compatibility

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

   chrony::servers:  "%{alias('simp_options::ntp::servers')}"

   # This can be removed once the upgrade is complete
   simp_options::ntpd::servers: "%{alias('simp_options::ntp::servers')}"


SIMP YUM Repository Changes
"""""""""""""""""""""""""""

If you are using local SIMP :term:`YUM` repositories, such as those installed on the
SIMP server during a SIMP ISO install, you will need to migrate to the new
SIMP repo format and ensure the latest Puppet GPG key is installed.

Migrate to new format and update SIMP repos
'''''''''''''''''''''''''''''''''''''''''''

**THIS REPLACES STEP 1**, `Update the YUM Repositories`, **of the**
:ref:`Incremental Upgrades <ug-incremental-upgrades>` **instructions,
'Upgrading systems using the local deployment scenario' subsection.**

The SIMP :term:`YUM` repository has been moved from :file:`/var/www/yum/SIMP` to
:file:`/var/www/yum/SIMP/<os name>/<os version>` to make supporting multiple
client operating systems easier. In addition, the :code:`unpack_dvd` script in
:package:`simp-utils` was updated to extract the RPMs to the new location.

Do the following as `root` to properly migrate to the new format:

1. Manually migrate your existing /var/www/yum/SIMP repo to the new format

   .. code-block:: bash

      cd /var/www/yum/SIMP
      mkdir -p CentOS/7.8.2003
      chown -R root:apache CentOS
      chmod -R g+rX CentOS
      mv noarch CentOS/7.8.2003
      mv x86_64/ CentOS/7.8.2003

2. Install the latest :package:`simp-utils`.

   .. code-block:: bash

      # Mount the ISO
      mkdir /mnt/iso
      mount -o loop -t iso9660 <location of iso file> /mnt/iso

      # Install the latest simp_utils RPM
      cd /mnt/iso/SIMP/noarch
      yum install -y ./simp-utils*.rpm || yum update -y ./simp-utils*.rpm

      # Unmount the ISO
      cd $HOME
      umount /mnt/iso

3. Extract latest SIMP packages from SIMP ISO to the new format

   .. code-block:: bash

      # Extract the ISO.
      unpack_dvd  -v 7.9.2009  <SIMP 6.6.0 ISO file path>

4. Create temporary links to the new repo directories that can be removed
   once the upgrade is complete

   .. code-block:: bash

      cd /var/www/yum/SIMP
      ln -s ./CentOS/7/noarch ./noarch
      ln -s ./CentOS/7/x86_64 ./x86_64

5. Update the :code:`flocal` repo :code:`baseurl` in
   :file:`/etc/yum.repos.d/simp_filesystem.repo`, if your SIMP server was
   installed from ISO and that repository is enabled.

   * Original :code:`baseurl`

     .. code-block:: bash

        baseurl=file:///var/www/yum/SIMP/x86_64

   * Updated :code:`baseurl`

     .. code-block:: bash

        baseurl=file:///var/www/yum/SIMP/CentOS/7/x86_64

Install the new Puppet GPG key
''''''''''''''''''''''''''''''

Puppet, Inc. has a new :term:`GPG` key that must be used to install the new
:package:`puppet-agent`. Install this key as follows:

1. Install this key on the YUM server by updating :package:`simp-gpgkeys`.

  .. code-block:: sh

     yum install -y simp-gpgkeys

2.  To ensure that all the clients pick up the new key, add the following to the
    top level of :term:`Hiera`:

    .. code-block:: yaml

     # This can be removed once the upgrade is complete
     simp::yum::repo::local_simp::extra_gpgkey_urls:
     - https://<your yum server ip address>/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406

3. To ensure the SIMP server picks up the new key, add the new key to the
   :code:`flocal` repo :code:`gpgkey` in
   :file:`/etc/yum.repos.d/simp_filesystem.repo`, if your SIMP server was
   installed from ISO and that repository is enabled.

   * Original :code:`gpgkey`

    .. code-block:: yaml

       gpgkey=file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppetlabs
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-SIMP-6
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-SIMP
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-PGDG-96

   * Updated :code:`gpgkey`

    .. code-block:: yaml

       gpgkey=file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppetlabs
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-SIMP-6
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-SIMP
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-PGDG-96
           file:///var/www/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406

Kickstart Prep
""""""""""""""

If you use kickstart, you will need to extract the PXEboot files before
kickstarting new EL7 machines.  See :ref:`howto-unpack-dvd-pxe` for more
information.

Complete the General Upgrade Steps
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

After resolving all applicable items in :ref:`Before Upgrading to 6.6.0
<before-upgrading-to-6.6.0>`, resume :ref:`Incremental Upgrades
<ug-incremental-upgrades>` instructions **from Step 2**.


After Upgrading to 6.6.0
^^^^^^^^^^^^^^^^^^^^^^^^

Cleanup Tasks
"""""""""""""

The following items can be removed from :term:`hiera`:

.. code-block:: yaml

   simp::yum::repo::local_simp::extra_gpgkey_urls:
     - https://<your yum server ip address>/yum/SIMP/GPGKEYS/RPM-GPG-KEY-puppet-20250406

   simp::sssd::client::local_domain: false

   simp_options::ntpd::servers: "%{alias('simp_options::ntp::servers')}"

The following links can be removed on the :term:`YUM` server:

.. code-block:: bash

   unlink /var/www/yum/SIMP/noarch
   unlink /var/www/yum/SIMP/x86_64

Upgrading to EL8
----------------

Main Server Migration
^^^^^^^^^^^^^^^^^^^^^

To upgrade your puppet server to EL8, you would follow the installation
instructions and then the :ref:`howto-migrate-to-new-puppet-server`

Migration to from OpenLDAP to 389-DS
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

See: :ref:`howto-migrate_openldap_to_389ds`.

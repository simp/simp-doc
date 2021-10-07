.. _changelog-latest:
.. _changelog-6.6.0:

SIMP Community Edition (CE) 6.6.0
=================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak


OS compatibility
----------------

.. contents::
  :depth: 2
  :local:

This release is known to work with:

  * CentOS 7.0 2009 x86_64
  * CentOS 8.4 2015 x86_64
  * OEL 7.9 x86_64
  * OEL 8.4 x86_64
  * RHEL 7.9 x86_64
  * RHEL 8.4 x86_64


Important OS compatibility limitations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

EL6 support has been removed
""""""""""""""""""""""""""""

EL6 is no longer supported by SIMP CE.

If you need support for EL6 systems, please consider purchasing commercial
support.

.. _changelog-6.6.0-breaking-changes:

Breaking Changes
----------------

.. contents::
  :depth: 2
  :local:

Unpacked ISOs Use a Full Base Path
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prior to this release, the :file:`SIMP` RPMs were all placed into :file:`/var/www/yum/SIMP`. This
made it difficult to support multiple operating system releases. Starting from this release, items
are now placed into :file:`/var/www/yum/SIMP/<os name>/<os version>/<arch>` which mirrors the layout
of the base operating system repositories.

The :program:`unpack_dvd` script has been updated to ensure that only compatible items are unpacked
into the underlying repository and to fail with guidance if incompatibilities are discovered.

Rsyslog < 8.24.0 is no Longer Supported
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The vendor recommends using :program:`rsyslog` 8 or later, therefore support for
:program:`rsyslog` versions under 8.24.0 are no longer supported by the default
module.

If you need to support older :program:`rsyslog` versions, please use
:module:`simp/rsyslog` 7.6.4 in an alternate puppet environment.

SSSD < 1.16.0 is no Longer Supported
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Multiple issues exist in versions of :program:`sssd` prior to 1.16.0 and users
should upgrade to the latest release.

.. _changelog-6.6.0-significant-updates:

Significant Updates
-------------------

.. contents::
  :depth: 2
  :local:

EL8 SIMP Client Node Support
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This release provides full support for both EL8 server and client systems.

One of the biggest changes was the deprecation of OpenLDAP in EL8.

SIMP has replaced the native LDAP capabilities with 389-DS.

Existing infrastructures will not be affected on upgrade but new environments
will need to correctly configure their environment for the target LDAP server.

.. todo::

   Add links to the appropriate documentation sections

Puppet 7 Support
^^^^^^^^^^^^^^^^

All SIMP Puppet modules now work with both Puppet 6 and Puppet 7.

Puppet 5 support has been dropped due to end-of-life.

PuppetDB no Longer Configured by Default
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A review of the newer :program:`puppetserver` defaults as well as the concept of "only run what you
require" led to the removal of :program:`puppetdb` as a default installed/configured application.

This change should make it easier to run in resource-limited environments.

Existing systems will not be affected but new systems will need to enable :program:`puppetdb` per
:ref:`ht-enable-puppetdb`.

Switch from OpenLDAP to 389-DS in EL8
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The upstream vendors dropped support for the OpenLDAP server in EL8+. The SIMP project now uses
389-DS as the de-facto LDAP server on EL8 server systems.

Clients are able to connect to either OpenLDAP or 389-DS as necessary. Please read the upgrade
guide if you are switching from OpenLDAP to 389-DS. New systems will work out of the box.

Switch from Cron to Systemd
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Where possible, all SIMP puppet modules have been updated to remove old :program:`cron` jobs and
move to using :program:`systemd` timers instead. Eventually, this will allow the removal of
:program:`cron` by default and has the added benefit of being easier to manage.

Switch from Iptables to Firewalld
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All SIMP modules now use :program:`firewalld` by default instead of directly managing
:program:`iptables`. In general, this should be seamless for users unless advanced
:program:`iptables` rulesets were being managed (NAT, etc...).

Users still have the ability to directly manage :program:`iptables` rules but should be aware that no
further development will occur on :module:`simp-iptables` outside of maintaining the shims that hook
it into :program:`firewalld`.

.. _changelog-6.6.0-security-anouncements:

Security Announcements
----------------------

.. contents::
  :depth: 2
  :local:

.. _changelog-6.6.0-rpm-updates:

RPM Updates
-----------

Puppet RPMs
^^^^^^^^^^^

.. todo::

   Update the RPM list

The following Puppet RPMs are packaged with the SIMP 6.6.0 ISOs:

+-----------------------------+---------+
| Package                     | Version |
+=============================+=========+
| :package:`puppet-agent`     | FIXME   |
+-----------------------------+---------+
| :package:`puppet-bolt`      | FIXME   |
+-----------------------------+---------+
| :package:`puppetdb`         | FIXME   |
+-----------------------------+---------+
| :package:`puppetdb-termini` | FIXME   |
+-----------------------------+---------+
| :package:`puppetserver`     | FIXME   |
+-----------------------------+---------+

Removed Puppet Modules
----------------------

The following modules were removed from the release

* :package:`simp_pki_service`
* :package:`simp_bolt`

.. _changelog-6.6.0-fixed-bugs:

Fixed Bugs
----------

.. contents::
  :depth: 2
  :local:

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^

* Aligned the EL8 STIG settings
* Always add the :code:`head` rules since they are required for proper functionality of the system
* Use :code:`-F key=` instead of :code:`-k` to match the STIG recommendations
* Switched the audit rules to :code:`always,exit` instead of :code:`exit,always` to match the man pages

pupmod-simp-aide
^^^^^^^^^^^^^^^^

* Changed to using :code:`--check` instead of :code:`-C` by default to match the expectation of most security
  scanners
* Randomized the scheduling :code:`minute` field so that I/O load is reduced on hosting platforms

pupmod-simp-cron
^^^^^^^^^^^^^^^^

* Manage the :program:`cron` packages by default

pupmod-simp-fips
^^^^^^^^^^^^^^^^

* Use the :program:`simplib__crypto_policy_state` fact instead of :program:`crypto_policy__state`
* Ensure that :program:`dracut_rebuild` is called when the :code:`fips` kernel parameter is changed

pupmod-simp-gdm
^^^^^^^^^^^^^^^

* Fixed minor errors in the :file:`compliance_markup` data
* Properly handle integration of :program:`systemd-logind` with the :code:`hidepid` flag on :file:`/proc`
* Added a :code:`pam_access` entry for the :program:`gdm` user so that the greeter session can start

pupmod-simp-haveged
^^^^^^^^^^^^^^^^^^^

* Mask the :program:`haveged` service when disabling it so that it is not restarted on reboot
* Ensure that :program:`haveged` does not start if :program:`rngd` is running

pupmod-simp-incron
^^^^^^^^^^^^^^^^^^

* No longer pin the version of :program:`incron` since the upstream versions have been fixed

pupmod-simp-libreswan
^^^^^^^^^^^^^^^^^^^^^

* Removed obsolete configuration items that prevented functionality on EL8
  * :code:`libreswan::ikeport`
  * :code:`libreswan::nat_ikeport`
  * :code:`libreswan::klipsdebug`
  * :code:`libreswan::perpeerlog`
  * :code:`libreswan::perpeerlogdir`

pupmod-simp-libvirt
^^^^^^^^^^^^^^^^^^^

* Removed :package:`ipxe-roms` from the OEL package lists since they are now optional

pupmod-simp-network
^^^^^^^^^^^^^^^^^^^

* Ensure that the :code:`network::eth` defined type honors the :code:`network::auto_restart` parameter

pupmod-simp-nfs
^^^^^^^^^^^^^^^

* Added :code:`_netdev` to the default mount options
* Ensure that :code:`remote-fs.target` is enabled

pupmod-simp-ntpd
^^^^^^^^^^^^^^^^

* Fixed a bug where :code:`ntp::allow::rules` was not being honored
* Added :code:`simp_options::ntp::servers` to the default lookup list for :code:`ntpd::servers`

pupmod-simp-openscap
^^^^^^^^^^^^^^^^^^^^

* Fixed the default data stream name in EL7

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Silenced unnecessary TTY messages
* Added default Hiera deep merges for :code:`pam::access::users` and :code:`pam::limits::rules`
* Fixed a bug in :file:`system-auth` where :program:`pam_tty_audit` was not skipped if
  the login did not have a TTY. This prevented the GDM service login from
  succeeding.
* Set :program:`quiet` on :program:`pam_listfile` so that warnings do not get logged that look
  like authentication failures

pupmod-simp-pupmod
^^^^^^^^^^^^^^^^^^

* Changed all instances of setting items in the :code:`master` section to use :code:`server` instead
* Added :code:`pupmod::master::sysconfig::use_code_cache_flushing` to reduce excessive memory usage
* Disconnected the puppetserver from the system FIPS libraries since it causes
  conflicts with the vendor provided settings
* Allow :code:`pupmod::puppet_server` to accept Arrays
* Properly configure the server list when multiple puppet servers are specified
* Converted all :program:`cron` settings to :program:`systemd` timers
* Converted the 'cleanup' jobs to :program:`systemd.tmpfile` jobs
* Fixed a bug where the :code:`pupmod::master::sysconfig` class was not being applied
* Get :program:`certname` from trusted facts only for authenticated remote requests
* Fix bolt compatibility

pupmod-simp-resolv
^^^^^^^^^^^^^^^^^^

* Fixed bugs in the Augeas template
* Use configuration files to manage the global :program:`NetworkManager` configuration

pupmod-simp-rkhunter
^^^^^^^^^^^^^^^^^^^^

* Changed the :code:`minute` parameter on scheduled tasks to a random number to reduce
  I/O load on hosting platforms
* Updated to use :program:`systemd` timers instead of :program:`cron` by default
* Added default :code:`user_fileprop_files_dirs` to covert he puppet applications
* Ensure that the initial :program:`propupd` command runs after the puppet run is complete
* Added a :code:`rkhunter::propupd` class to ensure that the first cut of properties
  is updated after all packages have competed in the puppet run

pupmod-simp-rsync
^^^^^^^^^^^^^^^^^

* Fixed the documentation
* Noted that :program:`sebool_use_nfs` and :program:`sebool_cifs` will be
  deprecated in the future

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

* Fixed a bug where the :program:`rsyslog` service would start without errors
  but fail to log when :code:`rsyslog::config::default_template` was set to
  :code:`traditional`

pupmod-simp-selinux
^^^^^^^^^^^^^^^^^^^

* Fixed a dependency cycle when using :code:`vox_selinux::boolean`
* Fixed a bug where the module would attempt to create :code:`selinux_login` resources
  when :code:`selinux::login_resources` was set but :program:`selinux` was disabled

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* Corrected the :code:`HeapDumpOnOutOfMemoryError` setting for :program:`puppetdb`
* Ensure that :program:`nsswitch` :program:`SSSD` options for :file:`sudoers` do
  not stop on files
* Do not include the :code:`auditors` :program:`sudo` user specification if the
  aliases have not been included
* Added the following to the :file:`sudoers` defaults:

  * :code:`!visiblepw`
  * :code:`always_set_home`
  * :code:`match_group_by_gid`
  * :code:`always_query_group_plugin`

* Now use relative paths for the location for the SIMP GPG keys on YUM servers by default
* Support all valid values for :code:`simp::pam_limits::max_logins::value`
* Added additional parameters to :code:`simp::admin` to allow for more
  fine-grained control of global :code:`admin` and :code:`auditor` :program:`sudo` rules

pupmod-simp-simp_apache
^^^^^^^^^^^^^^^^^^^^^^^

* Ensure that all :code:`file` resources that manage more than permissions have
  an :code:`ensure` attribute
* Moved the :file:`magic` file into an EPP template to work better with :program:`bolt`
* Use :program:`systemd` to reload/restart the :program:`httpd` service

pupmod-simp-simp_gitlab
^^^^^^^^^^^^^^^^^^^^^^^

* Fixed a bug where the :program:`change_gitlab_root_password` script did not
  work with GitLab after 13.6.0

pupmod-simp-simp_nfs
^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in :program:`create_home_directories.rb` where EL8 systems could
  not talk to EL7 LDAP servers when the servers were in FIPS mode

pupmod-simp-simp_openldap
^^^^^^^^^^^^^^^^^^^^^^^^^

* Fixed :code:`pki::copy` since the :program:`ldap` group is no longer created by the
  OpenLDAP client packages
* Fixed :code:`Float` to :code:`String` comparison error in
  :code:`simp_openldap::server::conf::tls_protocol_min`
* Deprecated parameters only applicable to EL6:

  * :code:`simp_openldap::client::strip_128_bit_ciphers`
  * :code:`simp_openldap::client::nss_pam_ldapd_ensure`

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

* Increased randomization in :code:`simplib::gen_random_password`
* :code:`simplib::cron::hour_entry` now supports comma separated lists
* :code:`simplib::cron::minute_entry` now supports comma separated lists
* Fixed the :program:`simplib__networkmanager` fact
* Fixed a bug where the :program:`ipa` fact did not detect when an EL8 client
  was joined to an IPA domain
* Ensure that the :program:`puppet_settings` fact supports both the
  :code:`server` and :code:`master` sections for backwards compatibility
* Added a tertiary check to the :program:`grub_version` fact

pupmod-simp-ssh
^^^^^^^^^^^^^^^

* Fixed a bug where some changes to the :program:`sshd` configuration did not
  cause a service restart
* Fixed a bug that caused a compilation error when
  :code:`ssh::conf::ensure_sshd_packages` was set to :code:`true`
* Ensure that :code:`vox_selinux` is included prior to calling :code:`selinux_port`
* Ensure that parameters that do not apply to EL8+ systems are not set on the
  target system
* No longer set :code:`HostKeyAlgorithms` on the client configuration by default

pupmod-simp-sssd
^^^^^^^^^^^^^^^^

* Fixed multiple compatibility issues with non-OpenLDAP LDAP servers
* No longer use :code:`concat` but instead drop configuration items into the
  :file:`/etc/sssd/conf.d` directory
* Ensure that systems bound to FreeIPA, but not connected, do not cause
  compilation issues

pupmod-simp-svckill
^^^^^^^^^^^^^^^^^^^

* Added :program:`rngd` to the default list of services to never be killed
* Removed obsolete documentation

pupmod-simp-swap
^^^^^^^^^^^^^^^^

* Disable :code:`dynamic_swappiness` by default
* Set static system swappiness to 60 by default

pupmod-simp-tlog
^^^^^^^^^^^^^^^^

* Corrected the login in :file:`tlog.sh.epp` in the case where a user does not
  have a login shell

pupmod-simp-tpm2
^^^^^^^^^^^^^^^^

* Overrode the :program:`systemd` unit file for :program:`tpm2-abrmd` for TCTI compatibility

pupmod-simp-vsftpd
^^^^^^^^^^^^^^^^^^

* Fixed :program:`sysctl` updates on service restart

simp-doc
^^^^^^^^

* Added HOWTO for managing PuppetDB
* Added HOWTO for enabling client reports
* Corrected SSL recovery documentation
* Corrected documentation relating to using :program:`sudo` in STIG mode
* Added documentation for using EYAML in SIMP environments

simp-environment
^^^^^^^^^^^^^^^^

* Add the EYAML hierarchy to the default :file:`hiera.yaml`

simp-gpgkeys
^^^^^^^^^^^^

* Fixed the target location for copying the GPG keys into the YUM repository

simp-rsync
^^^^^^^^^^

* Removed dynamic BIND files from the list of files to :program:`rsync`

simp-utils
^^^^^^^^^^

* Fixed the :program:`puppetlast` script and enabled it to read from filesystem reports

  * You will need to follow the instructions in :ref:`ht-enable-client-reporting`

rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Changed set/get from :program:`master` to :program:`server` when updating the puppet configuration
* Use the status endpoint instead of a CRL query to validate the puppetserver status
* Use puppet to set the GRUB password
* Ensure that updating entries in :file:`/etc/hosts` is idempotent
* Removed the :program:`LOCAL` domain from the default :program:`sssd` configuration
* No longer use the deprecated :code:`simp_options::ntpd::servers` setting
* Simplified the instructions for the 'local user lockout' warning

.. _changelog-6.6.0-new-features:

New Features
------------

.. contents::
  :depth: 2
  :local:

The following items are common to most module updates and do not warrant
specific inclusion below. For full details, see the :file:`CHANGELOG` of all delivered
packages.

  * Removal of old Puppet version support
  * Removal of EL6 support
  * Addition of EL8 support
  * Puppet module dependency updates

pupmod-simp-ds389
^^^^^^^^^^^^^^^^^

* New module for managing 389-DS

pupmod-simp-gnome
^^^^^^^^^^^^^^^^^

* Removed support for GNOME2 since EL6 is no longer supported
  * Also removed all gconf parameters and settings since they no longer have any use

pupmod-simp-logrotate
^^^^^^^^^^^^^^^^^^^^^

* Allow all log size configuration parameters to be specified in bytes, kilobytes, megabytes, or
  gigabytes

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Added a :program:`pre` section for setting auth file content to work with third party plugins
* Added the ability to set extra content in the :program:`su` configuration

pupmod-simp-resolv
^^^^^^^^^^^^^^^^^^

* Added the ability to precisely update the :file:`resolv.conf` contents
* Added the ability to specify the entire contents of :file:`resolv.conf`
* Added the ability to remove :file:`resolv.conf` completely

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

Please read the module documentation and :file:`CHANGELOG` since there were numerous changes!

* Dropped support for :program:`rsyslog` < 8.24.0
* Added the ability to set the default template used for forwarding via
  :code:`rsyslog::config::default_forward_template`
* Added parameters to allow additional configuration of the modules and main
  queue
* Added :code:`Direct` and :code:`Disk` to the allowed main message queue types
* Removed parameters only relevant to :program:`rsyslog` < 8.6.0

  * :code:`rsyslog::config::host_list`
  * :code:`rsyslog::config::domain_list`

* Replaced obsolete parameters with modern replacements:

  * :code:`rsyslog::config::action_send_stream_driver_mode` => :code:`rsyslog::config::imtcp_stream_driver_mode`
  * :code:`rsyslog::config::action_send_stream_driver_auth_mode` => :code:`rsyslog::config::imtcp_stream_driver_auth_mode`
  * :code:`rsyslog::config::disable_remote_dns` => :code:`rsyslog::config::net_enable_dns`
  * :code:`rsyslog::config::suppress_noauth_warn` => :code:`rsyslog::config::net_permit_acl_warning`

* Deprecated :code:`rsyslog::config::default_template` for :code:`rsyslog::config::default_file_template`
* Updated various parts of the configuration from legacy to RainerScript format

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* Added :code:`simp::puppetdb::disable_update_checking` to disable default analytics
  in accordance with NIST guidance
* :program:`puppetdb` now sets :code:`UseCodeCacheFlushing` by default
* The :program:`sssd` client configuration now sets the LDAP schema based on the
  :code:`simp::sssd:;client::ldap_server_type`
* The :code:`simp::sssd::client` no longer creates a :code:`LOCAL` provider

pupmod-simp-simp_ds389
^^^^^^^^^^^^^^^^^^^^^^

* New module providing SIMP-specific settings for 389-DS for providing a
  suitable replacement for OpenLDAP

pupmod-simp-simp_gitlab
^^^^^^^^^^^^^^^^^^^^^^^

* Now default :code:`simp_gitlab::allow_fips` to :code:`true` which works with GitLab 14.0.0+

pupmod-simp-simp_nfs
^^^^^^^^^^^^^^^^^^^^

* Provide host PKI information to upstream LDAP servers

pupmod-simp-simp_options
^^^^^^^^^^^^^^^^^^^^^^^^

* Added :code:`simp_options::ntp` for more generalized configuration of both
  :program:`ntpd` and :program:`chronyd`

pupmod-simp-simpkv
^^^^^^^^^^^^^^^^^^

* Added an LDAP backend plugin

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

* Added :code:`simplib::cron::to_systemd()` to convert :program:`cron` resource
  parameters to :program:`systemd` timespec format
* Added :code:`simplib::cron::expand_range()` to expand ranges into comma
  separated strings
* Added :code:`simplib::params2hash()` to return all of the calling scope's
  parameters as a Hash
* Added :program:`net.ipv6.conf.all.disable_ipv6` to the :program:`simplib_sysctl` fact
* Added a :program:`simplib__cryhpto_policy_state` fact

pupmod-simp-sssd
^^^^^^^^^^^^^^^^

* Made installing the :program:`sssd` client optional (enabled by default)
* No longer support :program:`sssd` < 1.16.0
* Users can now set :code:`sssd::custom_config` to a string that will be placed
  into :file:`/etc/sssd/conf.d/zz_puppet_custom.conf`
* Users can optionally purge the :file:`/etc/sssd/conf.d` directory if they want
  puppet to be authoritative

pupmod-simp-tpm2
^^^^^^^^^^^^^^^^

* Updated :code:`tpm2::ownership` and the :program:`tpm2` fact to support
  :package:`tpm2_tools` version 4
* Added a provider for the :program:`tpm2_changeauth` functionality to provide
  ownership update capabilities

simp-environment
^^^^^^^^^^^^^^^^

* No longer configure :program:`puppetdb` by default

simp-gpgkeys
^^^^^^^^^^^^

* Added the EL8 GPG keys
* Added the new Puppet signing key

simp-utils
^^^^^^^^^^

* Updated the :program:`unpack_dvd` scripts to work with EL8 ISOs
* Added transition scripts for upgrading from 6.5.0 to 6.6.0

rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Removed management of :program:`puppetdb` components since it is no longer enabled by default
* Removed support for EL6
* Use OpenLDAP by default on EL7 and 389-DS otherwise
* Set the defaults for both :program:`ntpd` and :program:`chronyd`

Known Bugs and Limitations
--------------------------

Below are bugs and limitations known to affect this release. If you discover
additional problems, please `submit an issue`_ to let use know.

* None at this time!

.. _submit an issue: https://simp-project.atlassian.net
.. _simp-project.com: https://simp-project.com

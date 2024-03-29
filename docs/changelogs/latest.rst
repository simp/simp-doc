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
  * CentOS 8.5 2111 x86_64
  * CentOS 8 Stream 20220423 x86_64
  * OEL 7.9 x86_64
  * OEL 8.5 x86_64
  * RHEL 7.9 x86_64
  * RHEL 8.5 x86_64


Full support for EL8
^^^^^^^^^^^^^^^^^^^^

This release introduces **full** EL8 support for the SIMP Puppet server and
agents across the entire SIMP framework.

EL8 support :ref:`was previously limited <changelog-6-5-0-el8-client-only>` to
managing Puppet agents with the core SIMP Puppet modules.

EL6 support has been removed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

EL6 is EOL and is no longer supported by SIMP CE.

All logic and testing in support of EL6 has been **completely removed** from
the entire SIMP framework.

If you require further support for EL6 systems, consider purchasing commercial support.


.. _changelog-6.6.0-breaking-changes:

Breaking Changes
----------------

.. contents::
  :depth: 2
  :local:

ISOs Unpack into Unique Repository Paths
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The directory structure of yum repositories unpacked from SIMP ISOs has changed.

Previously, all SIMP RPMs were placed into a single yum repository on the SIMP
server, under :file:`/var/www/yum/SIMP/`.  This directory structure wasn't
flexible enough to serve multiple operating systems/releases simultaneously
without significant customization.

Starting from this release, repositories will be placed under the directory
structure :file:`/var/www/yum/SIMP/<os name>/<os version>/<arch>/`, which
mirrors the layout of the base operating system repositories.

The :program:`unpack_dvd` script has been updated to ensure that only
compatible items are unpacked into the underlying repository.  If
the script detects incompatibilities, it will fail and provide guidance.

Rsyslog < 8.24.0 is no Longer Supported
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Due to vendor recommendations, :pupmod:`simp/rsyslog` no longer supports
:program:`rsyslog` versions under 8.24.0

If you need to support older versions of :program:`rsyslog`, please use
:pupmod:`simp/rsyslog` 7.6.4 in an alternate :term:`Puppet environment`.

SSSD < 1.16.0 is no Longer Supported
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are multiple issues in versions of :program:`sssd` prior to 1.16.0.
Users should upgrade to the latest release.

.. _changelog-6.6.0-significant-updates:

Significant Updates
-------------------

.. contents::
  :depth: 3
  :local:


.. _changelog-6.6.0-el8-server-support:

SIMP Server Support on EL8
^^^^^^^^^^^^^^^^^^^^^^^^^^

This release provides full support for managing SIMP Puppet servers on EL8.

Puppet 7 Support
^^^^^^^^^^^^^^^^

All SIMP Puppet modules now work with both Puppet 6 and Puppet 7.


Puppet 5 Support Removed
^^^^^^^^^^^^^^^^^^^^^^^^

Puppet 5 is EOL and support for it has been removed from all modules.


PuppetDB no Longer Configured by Default
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A review of the newer :program:`puppetserver` defaults as well as the concept
of "only run what you require" led to the removal of :program:`puppetdb` as
a default installed/configured application.

This change should make it easier to run in resource-limited environments.

Existing systems will not be affected, but new systems will need to enable
:program:`puppetdb` per :ref:`ht-enable-puppetdb`.


389 DS replaces OpenLDAP on EL8
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

On EL8, :term:`389 Directory Server` replaces the (deprecated) :term:`OpenLDAP`
server as the default LDAP service.

Existing infrastructures will not be affected on upgrade, but new environments
will need to configure correctly for their environment's LDAP server.

LDAP Clients are still able to connect to either OpenLDAP server or 389 DS as
necessary. :ref:`Please read the upgrade guide <before-upgrading-to-6.6.0>` if
you are switching from OpenLDAP to 389 DS. New systems will require no
additional configuration.

.. TODO::

   * Confirm that the upgrade guide link above is enough
   * Otherwise, add links to the appropriate documentation sections
   * FIXME: When done, remove this notice


Switch from Cron to Systemd
^^^^^^^^^^^^^^^^^^^^^^^^^^^

With the deprecation of EL6, all supported OSes use systemd.  The framework
is now in a position to take advantage of systemd-specific features that
improve system maintenance and administration.

Where possible, all SIMP puppet modules have been updated to replace old
:program:`cron` jobs with :program:`systemd` timers. This enhances execution
control and reporting for the scheduled jobs.

This practice may eventually enable systems to opt out of installing
:program:`cron` altogether, to the benefit of certain compliance profiles.  It
also has the benefit of being easier to manage.

Switch from Iptables to Firewalld
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All SIMP modules now use :program:`firewalld` by default instead of directly
managing :program:`iptables`. In general, the transition should be seamless for
users unless advanced :program:`iptables` rulesets were being managed (NAT,
etc...).

Users still have the ability to directly manage :program:`iptables` rules, but
should be aware that there will be no further development on
:pupmod:`simp/iptables` outside of maintaining the shims that hook it into
:program:`firewalld`.

.. _changelog-6.6.0-security-anouncements:

Security Announcements
----------------------

.. TODO::

   Were there really no security announcements?

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

+-----------------------------+----------------------+
| Package                     | Version              |
+=============================+======================+
| :package:`puppet-agent`     | 6.27.1-1 or 7.16.0-1 |
+-----------------------------+----------------------+
| :package:`puppet-bolt`      | 3.22.1-1             |
+-----------------------------+----------------------+
| :package:`puppetdb`         | 6.21.0-1 or 7.10.1-1 |
+-----------------------------+----------------------+
| :package:`puppetdb-termini` | 6.21.0-1 or 7.10.1-1 |
+-----------------------------+----------------------+
| :package:`puppetserver`     | 6.19.0-1 or 7.7.0-1  |
+-----------------------------+----------------------+

Removed Puppet Modules
----------------------

The following modules were removed from the release:

* :package:`simp_pki_service`
* :package:`simp_bolt`

Replaced Puppet Modules
-----------------------

+---------------------------+-------------------------+
| Original                  | Replacement             |
+===========================+=========================+
| :pupmod:`aboe/chrony`     | :pupmod:`puppet/chrony` |
+---------------------------+-------------------------+
| :pupmod:`camptocamp/kmod` | :pupmod:`puppet/kmod`   |
+---------------------------+-------------------------+

.. _changelog-6.6.0-fixed-bugs:

Fixed Bugs
----------

.. contents::
  :depth: 2
  :local:

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^

* Aligned the EL8 STIG settings
* Always add the :code:`head` rules since they are required for proper
  functionality of the system
* Use :code:`-F key=` instead of :code:`-k` to match the STIG recommendations
* Switched the audit rules to :code:`always,exit` instead of
  :code:`exit,always` to match the man pages

pupmod-simp-aide
^^^^^^^^^^^^^^^^

* Changed to using :code:`--check` instead of :code:`-C` by default to match
  the expectation of most security scanners
* Randomized the scheduling :code:`minute` field so that I/O load is reduced on
  hosting platforms

pupmod-simp-cron
^^^^^^^^^^^^^^^^

* Manage the :program:`cron` packages by default

pupmod-simp-fips
^^^^^^^^^^^^^^^^

* Use the :program:`simplib__crypto_policy_state` fact instead of
  :program:`crypto_policy__state`
* Ensure that :program:`dracut_rebuild` is called when the :code:`fips` kernel
  parameter is changed

pupmod-simp-gdm
^^^^^^^^^^^^^^^

* Fixed minor errors in the :file:`compliance_markup` data
* Properly handle integration of :program:`systemd-logind` with the
  :code:`hidepid` flag on :file:`/proc`
* Added a :code:`pam_access` entry for the :program:`gdm` user so that the
  greeter session can start

pupmod-simp-haveged
^^^^^^^^^^^^^^^^^^^

* Mask the :program:`haveged` service when disabling it so that it is not
  restarted on reboot
* Ensure that :program:`haveged` does not start if :program:`rngd` is running

pupmod-simp-incron
^^^^^^^^^^^^^^^^^^

* No longer pin the version of :program:`incron` since the upstream versions
  have been fixed

pupmod-simp-libreswan
^^^^^^^^^^^^^^^^^^^^^

* Removed obsolete configuration items that prevented functionality on EL8:

  * :code:`libreswan::ikeport`
  * :code:`libreswan::nat_ikeport`
  * :code:`libreswan::klipsdebug`
  * :code:`libreswan::perpeerlog`
  * :code:`libreswan::perpeerlogdir`

pupmod-simp-libvirt
^^^^^^^^^^^^^^^^^^^

* Removed :package:`ipxe-roms` from the OEL package lists since they are now
  optional

pupmod-simp-network
^^^^^^^^^^^^^^^^^^^

* Ensure that the :code:`network::eth` defined type honors the
  :code:`network::auto_restart` parameter

pupmod-simp-nfs
^^^^^^^^^^^^^^^

* Added :code:`_netdev` to the default mount options
* Ensure that :code:`remote-fs.target` is enabled

pupmod-simp-ntpd
^^^^^^^^^^^^^^^^

* Fixed a bug where :code:`ntp::allow::rules` was not being honored
* Added :code:`simp_options::ntp::servers` to the default lookup list for
  :code:`ntpd::servers`

pupmod-simp-openscap
^^^^^^^^^^^^^^^^^^^^

* Fixed the default data stream name in EL7

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Silenced unnecessary TTY messages
* Added default Hiera deep merges for :code:`pam::access::users` and
  :code:`pam::limits::rules`
* Fixed a bug in :file:`system-auth` where :program:`pam_tty_audit` was not
  skipped if the login did not have a TTY. This prevented the GDM service login
  from succeeding.
* Set :program:`quiet` on :program:`pam_listfile` so that warnings do not get
  logged that look like authentication failures

pupmod-simp-pupmod
^^^^^^^^^^^^^^^^^^

* Changed all instances of setting items in the :code:`master` section to use
  :code:`server` instead
* Updated :code:`pupmod::conf` to automcatically switch :code:`master` to :code:`server`
* Automatically remove items from the puppet config in the :code:`master` section that are set in
  the :code:`server` section
* Added :code:`pupmod::master::sysconfig::use_code_cache_flushing` to reduce
  excessive memory usage
* Removed SHA1 ciphers from the server cipher list
* Disconnected the puppetserver from the system FIPS libraries since it causes
  conflicts with the vendor provided settings
* Allow :code:`pupmod::puppet_server` to accept Arrays
* Properly configure the server list when multiple puppet servers are specified
* Converted all :program:`cron` settings to :program:`systemd` timers
* Converted the 'cleanup' jobs to :program:`systemd.tmpfile` jobs
* Fixed a bug where the :code:`pupmod::master::sysconfig` class was not being
  applied
* Get :program:`certname` from trusted facts only for authenticated remote
  requests
* Fix bolt compatibility

pupmod-simp-resolv
^^^^^^^^^^^^^^^^^^

* Fixed bugs in the Augeas template
* Use configuration files to manage the global :program:`NetworkManager`
  configuration

pupmod-simp-rkhunter
^^^^^^^^^^^^^^^^^^^^

* Changed the :code:`minute` parameter on scheduled tasks to a random number to
  reduce I/O load on hosting platforms
* Updated to use :program:`systemd` timers instead of :program:`cron` by default
* Added default :code:`user_fileprop_files_dirs` to covert he puppet
  applications
* Ensure that the initial :program:`propupd` command runs after the puppet run
  is complete
* Added a :code:`rkhunter::propupd` class to ensure that the first cut of
  properties is updated after all packages have competed in the puppet run

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
* Fixed a bug where the module would attempt to create :code:`selinux_login`
  resources when :code:`selinux::login_resources` was set but :program:`selinux`
  was disabled

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* Updated :code:`simp::yum::repo::local_os_updates` to use the gpg keys installed into :file:`<yum
  directory>/SIMP/GPGKEYS` to work around changes in EL8
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

* Now use relative paths for the location for the SIMP GPG keys on YUM servers
  by default
* Support all valid values for :code:`simp::pam_limits::max_logins::value`
* Added additional parameters to :code:`simp::admin` to allow for more
  fine-grained control of global :code:`admin` and :code:`auditor`
  :program:`sudo` rules

pupmod-simp-simp_apache
^^^^^^^^^^^^^^^^^^^^^^^

* Ensure that all :code:`file` resources that manage more than permissions have
  an :code:`ensure` attribute
* Moved the :file:`magic` file into an EPP template to work better with
  :program:`bolt`
* Use :program:`systemd` to reload/restart the :program:`httpd` service

pupmod-simp-simp_gitlab
^^^^^^^^^^^^^^^^^^^^^^^

* Fixed a bug where the :program:`change_gitlab_root_password` script did not
  work with GitLab after 13.6.0

pupmod-simp-simp_grub
^^^^^^^^^^^^^^^^^^^^^

* Updated the documentation to better reflect GRUB2

pupmod-simp-simp_nfs
^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in :program:`create_home_directories.rb` where EL8 systems could
  not talk to EL7 LDAP servers when the servers were in FIPS mode

pupmod-simp-simp_openldap
^^^^^^^^^^^^^^^^^^^^^^^^^

* Fixed :code:`pki::copy` since the :program:`ldap` group is no longer created
  by the OpenLDAP client packages
* Fixed :code:`Float` to :code:`String` comparison error in
  :code:`simp_openldap::server::conf::tls_protocol_min`
* Deprecated parameters only applicable to EL6:

  * :code:`simp_openldap::client::strip_128_bit_ciphers`
  * :code:`simp_openldap::client::nss_pam_ldapd_ensure`

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

* Fixed the call to `klist` to properly handle cache issues
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
* Ensure that :code:`vox_selinux` is included prior to calling
  :code:`selinux_port`
* Ensure that parameters that do not apply to EL8+ systems are not set on the
  target system
* No longer set :code:`HostKeyAlgorithms` on the client configuration by default

pupmod-simp-sssd
^^^^^^^^^^^^^^^^

* Added an option to :code:`sssd::install` to prevent installation of the :program:`sssd` client to
  increase compatibility with other operating systems
* Fixed multiple compatibility issues with non-OpenLDAP LDAP servers
* No longer use :code:`concat` but instead drop configuration items into the
  :file:`/etc/sssd/conf.d` directory
* Ensure that systems bound to FreeIPA, but not connected, do not cause
  compilation issues

pupmod-simp-stunnel
^^^^^^^^^^^^^^^^^^^

* Worked around a bug in EL7 where a connection denied by :program:`tcpwrappers` would cause
  :program:`stunnel` to hang and spike to 100% CPU usage indefinitely. All connections are still
  blocked by the firewall but now are always allowed in :program:`tcpwrappers`.

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

* Add a :code:`file` resource if the file writer is specified
* Corrected the login in :file:`tlog.sh.epp` in the case where a user does not
  have a login shell

pupmod-simp-tpm2
^^^^^^^^^^^^^^^^

* Overrode the :program:`systemd` unit file for :program:`tpm2-abrmd` for TCTI
  compatibility

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
* Removed EL6 keys
* Updated the Red Hat release key

simp-rsync
^^^^^^^^^^

* Removed dynamic BIND files from the list of files to :program:`rsync`

simp-utils
^^^^^^^^^^

* Fixed the :program:`puppetlast` script and enabled it to read from filesystem
  reports

  * You will need to follow the instructions in :ref:`ht-enable-client-reporting`

rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Changed set/get from :program:`master` to :program:`server` when updating the
  puppet configuration
* Use the status endpoint instead of a CRL query to validate the puppetserver
  status
* Use puppet to set the GRUB password
* Ensure that updating entries in :file:`/etc/hosts` is idempotent
* Removed the :program:`LOCAL` domain from the default :program:`sssd`
  configuration
* No longer use the deprecated :code:`simp_options::ntpd::servers` setting
* Simplified the instructions for the 'local user lockout' warning

.. _changelog-6.6.0-new-features:

New Features
------------

.. contents::
  :depth: 2
  :local:

The following items are common to most module updates and do not warrant
specific inclusion below. For full details, see the :file:`CHANGELOG` of all
delivered packages.

  * Removal of old Puppet version support
  * Removal of EL6 support
  * Addition of EL8 support
  * Puppet module dependency updates

pupmod-simp-ds389
^^^^^^^^^^^^^^^^^

* New module for managing 389 DS

pupmod-simp-simp_firewalld
^^^^^^^^^^^^^^^^^^^^^^^^^^

* Added the :pupmod:`simp/simp_firewalld` module and set it to the default on EL8+

pupmod-simp-gnome
^^^^^^^^^^^^^^^^^

* Removed support for GNOME2 since EL6 is no longer supported
* Also removed all gconf parameters and settings since they no longer have any
  use

pupmod-simp-logrotate
^^^^^^^^^^^^^^^^^^^^^

* Allow all log size configuration parameters to be specified in bytes,
  kilobytes, megabytes, or gigabytes

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Added `dictcheck` and `faillock_log_file` parameter support
* Added Amazon Linux 2 support
* Added a :program:`pre` section for setting auth file content to work with
  third party plugins
* Added the ability to set extra content in the :program:`su` configuration

pupmod-simp-resolv
^^^^^^^^^^^^^^^^^^

* Added the ability to precisely update the :file:`resolv.conf` contents
* Added the ability to specify the entire contents of :file:`resolv.conf`
* Added the ability to remove :file:`resolv.conf` completely

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

Please read the module documentation and :file:`CHANGELOG` since there were
numerous changes!

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

* Added EL8 support
* Added :code:`simp::puppetdb::disable_update_checking` to disable default
  analytics in accordance with NIST guidance
* :program:`puppetdb` now sets :code:`UseCodeCacheFlushing` by default
* The :program:`sssd` client configuration now sets the LDAP schema based on the
  :code:`simp::sssd:;client::ldap_server_type`
* The :code:`simp::sssd::client` no longer creates a :code:`LOCAL` provider

pupmod-simp-simp_ds389
^^^^^^^^^^^^^^^^^^^^^^

* New module providing SIMP-specific settings for 389 DS for providing a
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

pupmod-simp-ssh
^^^^^^^^^^^^^^^

* Added an option to turn off managing the :code:`AuthorizedKeysFile` parameter in
  :file:`/etc/ssh/sshd_config`

pupmod-simp-sssd
^^^^^^^^^^^^^^^^

* Made installing the :program:`sssd` client optional (enabled by default)
* No longer support :program:`sssd` < 1.16.0
* Users can now set :code:`sssd::custom_config` to a string that will be placed
  into :file:`/etc/sssd/conf.d/zz_puppet_custom.conf`
* Users can optionally purge the :file:`/etc/sssd/conf.d` directory if they want
  puppet to be authoritative

pupmod-simp-sudo
^^^^^^^^^^^^^^^^

* Added the ability for users to create :code:`include` clauses in :file:`/etc/sudoers`

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

* Removed management of :program:`puppetdb` components since it is no longer
  enabled by default
* Removed support for EL6
* Use OpenLDAP by default on EL7 and 389 DS otherwise
* Set the defaults for both :program:`ntpd` and :program:`chronyd`

Known Bugs and Limitations
--------------------------

Below are bugs and limitations known to affect this release. If you discover
additional problems, please `submit an issue`_ to let use know.

* :program:`sssd` does not always start the :program:`ds389` LDAP server immediately after kickstarting
  an EL8 system.  An additional puppet run clears the problem.  The error in the log is

  sssd.dataprovider.getDomains: Error [1432158215]: DP target is not configured


.. _submit an issue: https://simp-project.atlassian.net
.. _simp-project.com: https://simp-project.com

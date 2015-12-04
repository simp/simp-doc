=========================
SIMP 5.1.0-1
=========================

---------
Changelog
---------

.. raw:: pdf

  PageBreak

.. contents::

.. raw:: pdf

  PageBreak

SIMP 5.1.0-0

**Package**: 5.1.0-0

This release is known to work with:

  * RHEL 7.0 and 7.1 x86_64
  * CentOS 7.0 x86_64 (1406 and 1503)

.. warning::
  The default system passwords have changed! Please see the User's Guide for details.

.. note::
  This is a point release update that was missing some corrected documentation as well as the migration from common -> simplib in simp-bootstrap.

Manual Changes Requred
----------------------

* Bugs in the `simplib::secure_mountpoints` class (formerly
  `common::secure_mountpoints`)

.. note::
    This only affects you if you did not have a separate partition for /tmp!

+ There were issues in the secure_mountpoints class that caused /tmp and
  /var/tmp to be mounted against the root filesystem. While the new code
  addresses this, it cannot determine if your system has been modified
  incorrectly in the past.

+ To fix the issue, you need to do the following:
  - Unmount /var/tmp (may take multiple unmounts)
  - Unmount /tmp (may take multiple unmounts)
  - Remove the 'bind' entries for /tmp and /var/tmp from /etc/fstab
  - Run **puppet** with the new code in place

Deprecations
------------

* simp-hiera

  The `simp-hiera` RPM has been replaced by the upstream `hiera` package from
  Puppet Labs. The original simp-hiera fork had been maintained due to a need
  that the 'alias()' function now serves. Please run the `hiera_upgrade` script
  to convert your existing SIMP environment. You may also set the environment
  variable `HIERA_UPGRADE` to a path of your choice to update any other
  hieradata that you may have on your system.

* pupmod-simp-common

  The `::common` namespace has been deprecated in favor of the new `::simplib`
  namespace. This removes a commonly conflicting module name from the SIMP
  ecosystem.

  You will need to run the `migrate_to_simplib` script to update all of the
  relevant files. This script will only migrate items in the existing SIMP
  environment. You may also set the environment variable `UPGRADE_PATHS` to run
  the script on multiple external paths.

  All code was migrated.

* pupmod-simp-functions

  The `::functions` namespace has been deprecated in favor of the new
  `::simplib` namespace. This removes a commonly conflicting module name from
  the SIMP ecosystem.

  You will need to run the `migrate_to_simplib` script to update all of the
  relevant files. This script will only migrate items in the existing SIMP
  environment. You may also set the environment variable `UPGRADE_PATHS` to run
  the script on multiple external paths.

  The following items were not migrated:

    + append_if_no_such_line  => Use simp_file_line{}
    + delete_lines            => Use augeas{}
    + init_mod_nice           => Use init_ulimit{}
    + init_mod_open_files     => Use init_ulimit{}
    + line                    => Use augeas{}
    + prepend_if_no_such_line => Use simp_file_line{}
    + renice                  => No replacement, was not correct
    + replace_line            => Use augeas{}

Significant Updates
-------------------
* FIPS Mode is now enabled by default!

  + This is a SIGNIFICANT change and may impact many of your running
    applications that use encryption.
  + If you are upgrading, do **NOT** enable FIPS mode without extensive
    testing as it may cause various applications to not function properly any
    longer.

* The rsyslog module has been completely rewritten to support rsyslog 7.4.
  This is a breaking change from previous releases and will require active
  updates to existing systems.  All modules with rsyslog integration ave been
  updated to accommodate this change:

  + Critical Variable Changes

    - The global *rsyslog::log_server_list* variable is now set to send to
      **all** of the servers in the Array by default.

      * This variable defaults to the global *log_servers* Array in Hiera.

    - There is a new variable *rsyslog::failover_log_servers* which is an Array
      of failover log servers to be used for your system. These will be tried,
      in order, until successful messages can be sent.

  + Updated Modules:

    - aide
    - apache
    - auditd
    - dhcp
    - logstash
    - openldap
    - rsync
    - simp
    - sudosh

* There was a bug in previous versions of SIMP that require the following LDIF
  to be run manually on the systems to correct the password policy checking.

  dn: cn=default,ou=pwpolicies,dc=your,dc=domain
  changetype: modify
  replace: pwdCheckModule
  pwdCheckModule: simp_check_password.so
  -
  dn: cn=noExpire_noLockout,ou=pwpolicies,dc=your,dc=domain
  changetype: modify
  replace: pwdCheckModule
  pwdCheckModule: simp_check_password.so

* The Electrical and SIMP modules for elasticsearch have been combined.

Upgrade Guidance
----------------

Fully detailed upgrade guidance can be found in the **Upgrading SIMP** portion
of the *User's Guide*.

.. WARNING::
  You must have at least **2.2GB** of **free** RAM on your system to upgrade to
  this release.

.. NOTE::
  Upgrading from releases older than 5.0 is not supported.

Expectations
~~~~~~~~~~~~

Before you begin, please be aware that the following actions will take place as
a result of the migrate_to_environments script:

* The *puppet-server* RPM will be removed

* The *puppetserver* RPM will be installed (no, that's not a typo)

* **ALL** SIMP Puppet code will be migrated into a new *simp* environment

  * This will be located at */etc/puppet/environments/simp*

* A backup of your running environment will be made available at
  */etc/puppet/environments/pre_migration.simp*

  * You will find timestamped directories under the *pre_migration.simp*
    directory that correspond to runs of the migration script

  * Your old files will be in a *backup_data* directory and will be linked to a
    local bare Git repository in the same space

The upgrade steps will also have you install PuppetDB. PuppetDB is installed by
default if you kick from the DVD.

Security Announcements
----------------------

CVEs Addressed
~~~~~~~~~~~~~~

RPM Updates
-----------

Numerous RPMs were updated in the creation of this release. Several were
included due to our use of *repoclosure* to ensure that RPM dependencies are met
when releasing a DVD.

* This version include the latest RedHat 7.1 and CentOS 7.0 (1503) RPMs.
* Facter upgraded to 2.4.
* PuppetDB upgraded to 2.3.8-1

Fixed Bugs
----------

* pupmod-aide

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-apache

  - Removed all reliance on 'lsb*' facts since some environments do now wish to
    install the prerequisites for those facts to run.
  - Remove the apache_version fact and simply use the version controls built
    into the Apache configuration language.
  - Update all custom functions to properly scope definitions.
  - Ensure that mod_ldap is installed in SIMP >= 5.0.
  - Prevent apache from restarting after downloading a CRL.

* pupmod-clamav

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-common => Deprecated - Replaced by pupmod-simplib!
* pupmod-simplib

  - Fixed the secure_mountpoints code so that it no longer incorrectly bind
    mounts /tmp or /var/tmp.
  - We no longer supply crontab or anacrontab in global_etcd.
  - Remove dynamic_swappiness cron job if a static value is set.
  - Ensure that the *passgen()* function fails on invalid scenarios. This
    prevents the accidental cration of empty passwords.
  - Allow the value *2* to be used for ``rp_filter`` in ``simplib::sysctl``.
  - Added ability to return remote ip addrs.

* pupmod-dhcp

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-elasticsearch

  - Ensured that Elasticsearch works properly with the new version of Apache.
  - Removed our default ES tuning since the default works better for LogStash.
  - Ensure that Puppet manages the Elasticsearch logging file.

* pupmod-functions

  - Fixed sysv.rb to explicitly require puppet/util/selinux, which caused
    puppet describe to have errors.

* pupmod-iptables

  - Fixed a bug that would cause issues with Ruby 1.8.7.
  - Fixed DNS resolution in IPv6.
  - Prevent IPv6 ::1 spoofed addresses by default.

* pupmod-simp-logstash

  - Fix issues with both TCPWrappers and IPTables when used with LogStash.

* pupmod-nfs

  - Updated the *mountd* port to be *20048* by default for SELinux issues in
    RHEL7.

* pupmod-ntp

  - Updated against NTP Security Vulnerabilities (Red Hat Article #1305723).
  - Ensure that *restrict* entries use DDQ format.

* pupmod-openldap

  - The Password Policy overlay was getting loaded into the default.ldif
    even if you didn't want to use it. This has been fixed.
  - Made the password policy overlay align with the latest SIMP build of
    the plugin.

    + This means that you *must* have version
      simp-ppolicy-check-password-2.4.39-0 or later available to the system
      being configured.

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.
  - Fixed reported bugs in syncrepl.pp.
  - Removed all reliance on the 'lsb*' facts since some users do not
    wish to install the prerequisite RPMs for LSB compliance.

* pupmod-openscap

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.
  - Changed default ssg base path to /usr/share/xml/scap/ssg/content

* pupmod-pam

  - Removed all reliance on the 'lsb*' facts since some users do not
    wish to install the prerequisite RPMs for LSB compliance.

* pupmod-pki

  - Now allow directories in the cacerts directories. This previously
    caused failures that needed to be manually addressed on each node.

* pupmod-rsync

  - Fixed provider to run with --dry-run when puppet is run with a --noop.

* pupmod-simp

  - Ensure that SSSD is used by default on EL7+ systems since nscd and
    nslcd have functionality issues.
  - Removed all reliance on the 'lsb*' facts since some users do not
    wish to install the prerequisite RPMs for LSB compliance.

* pupmod-ssh

  - Modernized the Ciphers, MACs, and Kex.
  - Added explicit cases for FIPS and non-FIPS mode (as well as reasonable
    default cases for RHEL7 and below).
  - Updated to use the new augeasproviders module dependencies.
  - Added a function *ssh_format_host_entry_for_sorting()* that will properly
    sort SSH *Host* entries for inclusion with concat.

* pupmod-stunnel

  - Had a variable **options** in *stunnel.erb* that should have been scoped as
    **@options**.

* pupmod-sudo

   - Removed all reliance on the 'lsb*' facts since some users do not wish to
     install the prerequisite RPMs for LSB compliance.

* pupmod-sudosh

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-sysctl

  - Removed support for the old parsed-file provider and moved to using the new
    Augeas-based provider.

* pupmod-tftpboot

  - Purging of non-Puppet-managed items in *pxelinux.cfg* is now optional.

* pupmod-simp-tpm

  - IMA is disabled by default.

* simp-gpgkeys

  - Ensure that the keys are set in the correct locations for the target
    SIMP distribution.

* simp-rsync

  - Removed spurious install messages.

* simp-util

  - Fixed the targets of unpack_dvd.
  - Added a **use_fips** boolean to *simp config*

* pupmod-xinetd

  - Fixed: The default log_type should be 'SYSLOG authpriv' instead of 'SYSLOG
    daemon info'.

* pupmod-vnc

  - Removed banners that broke some vnc clients.

* simp-cli

  - `simp config -a ANSWERFILE` fails when an item has no answer
  - `simp config -A ANSWERFILE` prompts when an an item has no answer
  - The misleading `--help` documentation for `-ff` has been removed
  - The Config::Item `use_fips` now echoes its command unless `@silent`
  - The `simp doc` command path to the documentation has been corrected.
  - General usability improvements.

* DVD

  - NetworkManager-wait-online is now set by default in the ISO supplied
    kickstart images. Without ths, it is possible for the 'runpppet' script to
    attempt to run prior to the network being initialized.

  - A default IP is no longer provided when booting from the ISO; *simp config*
    will set the network properly.

  - The default kickstart no longer attempts to chkconfig any services
    in the %post section.

New Features
------------

* pupmod-auditd

  - Completely overhauled the module with a focus on better acceptance
    testing and format compliance.

* pupmod-augeasproviders

  - This was updated to 2.1.3.
  - The update to 2.1.3 caused the addition of all of the
    pupmod-augeasproviders modules below.

* augeasproviders_apache

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_base

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_core

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_grub

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_mounttab

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_nagios

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_pam

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_postgresql

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_puppet

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_shellvar

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_ssh

  - Imported 2.1.3 to support the Augeasproviders stack.

* augeasproviders_sysctl

  - Imported 2.1.3 to support the Augeasproviders stack.

* pupmod-augeasproviders

  - This was updated to 2.1.3.
  - The update to 2.1.3 caused the addition of all of the
    pupmod-augeasproviders modules below.

* pupmod-cgroups

  - Added acceptance tests.

* pupmod-common => Deprecated - Replaced by pupmod-simplib!
* pupmod-simplib

  - Created parse_hosts function.
  - Added full tests for evaluating the ability to toggle FIPS mode.

* pupmod-richardc-datacat

  - Incorporated the *richardc/datacat* module into the core for user convenience.

* pupmod-freeradius

  - Split the Freeradius module based on version so that it can be properly
    selected against the *installed* version of Freeradius. This may take two
    runs to coalesce.

* pupmod-puppetlabs-inifile

  - Updated to version 1.2.0.

* pupmod-puppetlabs-puppetdb

  - Updated to version 5.0.0-0.

* pupmod-simp-kibana

  - Add Kibana dashboards to the Kibana module.
  - Allows users to apply default SIMP kibana Dashboards.

* pupmod-simp-logstash

  - Integrated SIMP and Electrical Logstash modules.
  - Changes the existing Logstash module to allow users to apply default SIMP
    filters.

* pupmod-pki

  - Now generate a system RSA public key against the passed private key.

* pupmod-puppetlabs-postgresql

  - Initial import of the Puppet Labs PostgreSQL module.
  - Modifications were made to support the SIMP concat.

* pupmod-puppetlabs-puppetdb

  - New import of the Puppet Labs PuppetDB module.

* pupmod-simp-rsyslog

   - Module has been rewritten to support rsyslog 7.4.

* pupmod-simp-simp

    - Set the SELinux Boolean 'use_nfs_home_dirs' to 'on' if a remote NFS
      server is used for home directories.
    - The 'runpuppet' script was modified to run 'fixfiles' on systems prior to
      the final puppet runs since RHEL7, in some cases, does not appear to
      honor the '/.autorelabel' file.

* pupmod-puppetlabs-stdlib

  - Updated to version 4.5.1.

* pupmod-sysctl

  - Moved the configuration file updates from sysctl.conf to
    sysctl.d/20-simp.conf to use the latest update mechanisms.

* pupmod-tftpboot

  - Updated to use native packages and pull as muchs possible.

* simp-doc

  - Updated tables across the board to be more readable.
  - Updated documentation relating to user management and user key
    management using SSH.
  - Rebranded the documentation and updated the color scheme.
  - Updated the default system passwords.

* simp-rsync

  - Content has been restructured to eliminate licensing conflicts.
  - ClamAV has been refactored into a separate (GPL) package.

* simp-utils

  - simp config was rewritten to allow for new features and flexibility.
  - Now provided as a Ruby gem "simp-cli".

* Mcollective

  - Mcollective is now available to be installed and used with SIMP. It uses
    SSL/TLS along with user certificates for proper encryption and
    authentication.

* PuppetDB

  - PuppetDB is now supported by SIMP and installed by default.

* Puppetserver

  - The puppet master service has been replaced by the puppetserver service.
    This is a major rewrite by Puppetlabs. Puppetserver scales better for larger
    agent deployments with a single puppet master.
  - Uses Environments by default, this allows for tools such as r10K.
    Production environment is a link to simp by default.

* Facter 2.4

  - Facter now returns the following facts as their actual boolean or integer
    values, instead of converting them into strings:

    activeprocessorcount
    is_virtual
    mtu_<INTERFACE>
    physicalprocessorcount
    processorcount
    selinux_enforced
    selinux
    sp_number_processors
    sp_packages

Known Bugs
----------

  * There is a symlink that is created at /etc/puppet/environments/simp/simp
    which should not be in place. This is being tracked as SIMP-661
  * SSSD is currently broken and will allow logins via SSH even if your password
    has expired. This has been noted by Red Hat and is in the pipeline.
  * If you are running libvirtd, when svckill runs it will always attempt to
    kill dnsmasq unless you are deliberately trying to run the dnsmasq
    service.  This does *not* actually kill the service but is, instead, an
    error of the startup script and causes no damage to your system.

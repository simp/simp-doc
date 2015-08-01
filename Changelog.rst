================
SIMP 5.1.0-Beta
================

---------
Changelog
---------

.. raw:: pdf

  PageBreak

.. contents::

.. raw:: pdf

  PageBreak

SIMP 5.1.0-Beta
================

**Package**: 5.1.0-Beta

This release is known to work with:

  * RHEL 7.0 and 7.1 x86_64
  * CentOS 7.0 x86_64 (1406 and 1503)

Significant Updates
-------------------
* The rsyslog module has been completely rewritten to support rsyslog 7.4.
  This is a breaking change from previous releaases and will require active
  updates to existing systems.  All modules with rsyslog integration ave been
  updated to accommodate this change:

    - aide
    - apache
    - auditd
    - dhcp
    - logstash
    - openldap
    - rsync
    - simp
    - sudosh

* In RHEL6, we updated the OpenLDAP password policy overlay to not conflict
  with the upcoming 6.7 update. However, this will require you to update your
  LDAP server schema *manually* with the attached LDIF. Additionally, there was
  a bug in previous versions of SIMP that can be fixed by running this LDIF as
  is in RHEL7 and replacing simp_check_password.so with check_password.so in
  RHEL6.


* The Electrical and SIMP modules for elasticsearch have been combined.


Upgrade Guidance
----------------

Fully detailed upgrade guidance can be found in the **Upgrading SIMP** portion
of the *User's Guide*.

.. WARNING::
  You must have at least **2GB** of **free** RAM on your system to upgrade to
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
* This version upgrades Facter to 2.4.

Fixed Bugs
----------

* pupmod-aide

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-apache

  - Remove the apache_version fact and simply use the version controls built
    into the Apache configuration language.
  - Update all custom functions to properly scope definitions.
  - Ensure that mod_ldap is installed in SIMP >= 5.0.

* pupmod-simp-apache

  - Prevent apache from restarting after downloading a CRL.

* pupmod-clamav

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-common

  - We no longer supply crontab or anacrontab in global_etcd.
  - Remove dynamic_swappiness cron job if a static value is set.
  - Ensure that the *passgen()* function fails on invalid scenarios. This
    prevents the accidental cration of empty passwords.
  - Allow the value *2* to be used for *rp_filter* in *common::sysctl*.
  - Added ability to return remote ip addrs.

* pupmod-dhcp

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.

* pupmod-iptables

  - Fixed a bug that would cause issues with Ruby 1.8.7.
  - Fixed DNS resolution in IPv6.
  - Prevent IPv6 ::1 spoofed addresses by default.

* pupmod-simp-elasticsearch

  - Ensured that Elasticsearch works properly with the new version of Apache.
  - Removed our default ES tuning since the default works better for LogStash.
  - Ensure that Puppet manages the Elasticsearch logging file.

* pupmod-functions

  - Fixed sysv.rb to explicitly require puppet/util/selinux, which caused
    puppet describe to have errors.

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
    - This means that you *must* have version
      simp-ppolicy-check-password-2.4.39-0 or later available to the system
      being configured.
  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.
  - Fixed reported bugs in syncrepl.pp.

* pupmod-openscap

  - Change the call to the *rsyslog* init script to the *service* command to
    seamlessly support both RHEL6 and RHEL7.
  - Changed default ssg base path to /usr/share/xml/scap/ssg/content

* pupmod-rsync

  - Fixed provider to run with --dry-run when puppet is run with a --noop.

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

* simp-utils

  - Fixed the targets of unpack_dvd.

* pupmod-xinetd

  - Fixed: The default log_type should be 'SYSLOG authpriv' instead of 'SYSLOG
    daemon info'.

* pupmod-vnc

  - Removed banners that broke some vnc clients.

* DVD

  - A default IP is no longer provided when booting from the ISO; simp config
    will set the network properly.


New Features
------------

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

* pupmod-common

  - Created parse_hosts function.

* pupmod-richardc-datacat

  - Incorporated the *richardc/datacat* module into the core for user convenience.

* pupmod-freeradius

  - Split the Freeradius module based on version so that it can be properly
    selected against the *installed* version of Freeradius. This may take two
    runs to coalesce.

* pupmod-puppetlabs-inifile

  - Updated to version 1.2.0.

* pupmod-pki

  - Now generate a system RSA public key against the passed private key.

* pupmod-puppetlabs-postgresql

  - Initial import of the Puppet Labs PostgreSQL module.
  - Modifications were made to support the SIMP concat.

* pupmod-puppetlabs-puppetdb

  - New import of the Puppet Labs PuppetDB module.

* pupmod-puppetlabs-stdlib

  - Updated to version 4.5.1.

* pupmod-tftpboot

  - Updated to use native packages and pull as muchs possible.

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

* simp config

  - simp config was rewritten to allow for new features and flexibilty.
  - Now provided as a Ruby gem "simp-cli".

* pupmod-simp-logstash

  - Integrated SIMP and Electrical Logstash modules.
  - Changes the existing Logstash module to allow users to apply default SIMP
    filters.

* simp-rsync

  - Content has been restructured to eliminate licensing conflicts.
  - ClamAV has been refactored into a separate (GPL) package.

* pupmod-simp-rsyslog

   - Module has been rewritten to support rsyslog 7.4.

* pupmod-simp-kibana

  - Add Kibana dashboards to the Kibana module.
  - Allows users to apply default SIMP kibana Dashboards.

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

  * Setting pwdReset to 'true' in LDAP does not force a user to reset their
    password like it is supposed to. This works with FreeIPA and we are
    looking to move to support that system in the future.
  * SSSD is currently broken and will allow logins via SSH even if your password
    has expired. This has been noted by Red Hat and is in the pipeline. Their
    suggestion it to move to FreeIPA from OpenLDAP. We are looking to do this
    in the future.
  * If you are running libvirtd, when svckill runs it will always attempt to
    kill dnsmasq unless you are deliberately trying to run the dnsmasq
    service.  This does *not* actually kill the service but is, instead, an
    error of the startup script and causes no damage to your system.

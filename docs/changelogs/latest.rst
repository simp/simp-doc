SIMP Community Edition (CE) 6.3.0-Beta
======================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak

This release is known to work with:

  * CentOS 6.10 x86_64
  * CentOS 7.0 1804 x86_64
  * OEL 6.10 x86_64
  * OEL 7.5 x86_64
  * RHEL 6.10 x86_64
  * RHEL 7.5 x86_64


The overriding priority for SIMP 6.3.0 is interoperability with Puppet 5/Hiera 5.

Integration testing is done primarily with the versions of puppet that are delivered
with Puppet Enterprise 2018:

  * Puppet Server  5.3.X
  * Puppet Agent 5.5.X

A much wider range of versions is used for unit and acceptance testing.  See the .gitlab.yml
file in each module to see what versions it is tested under.

.. WARNING::

   Puppet 4 is no longer supported as of SIMP 6.3. Users can continue with the
   SIMP 6.2 release and can obtain commercial support if further Puppet 4
   support is required.

   From this point on, all components are tested againt Puppet 5.

   Puppet 4 might work but there are no guarantees over time.

Breaking Changes
----------------

Upgrading from Puppet 4 and earlier versions of :term:`Hiera` requires some
preparation.  Please be sure to read the Upgrade Guide in its entirety.

:term:`Hiera` 5 is fully compatible with  Hiera 3.  However there have been some changes
with the configuration to utilize new capabilities.

* The hiera.yaml file which defines the hierarchy used to search for parameter values
  has been moved to the environment level to utilize the ability to customize environments.
* The default data directory has been renamed from ``hieradata`` to  ``data`` to
  match hiera 5 conventions.

You should review the  puppet documentation for `upgrading to Hiera 5`_ to see how to
upgrade any custom modules or backends that you have created.

.. _upgrading to Hiera 5: https://puppet.com/docs/puppet/5.5/hiera_migrate.html


Significant Updates
-------------------

puppet-simp-tlog
^^^^^^^^^^^^^^^^
Sudosh has been replaced by TLOG as the default for logging privileged
user activities.  The default command for a user to switch to privileged access is now:

.. code-block:: bash

  sudo su - root

Package Installation Settings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Several of the SIMP modules have been updated to use
the simp_options::package_ensure setting as the default for package resource
ensure state.  The default for simp_options::package_ensure is `installed`.
This will change the default behavior of some modules that were
hard-coded to `latest`. This will not affect anything that was explicitly
set.

This change makes the SIMP modules consistent and allows the administrator
to set the default across the system with one variable.  Also, by setting the default
to `installed` packages will be updated only if the administrator
has explicitly set the variable to `latest` ensuring there are no surprise updates.

The following modules were updated:

* pupmod-simp-aide
* pupmod-simp-auditd
* pupmod-simp-clamav
* pupmod-simp-dhcp
* pupmod-simp-fips
* pupmod-simp-iptables
* pupmod-simp-krb5
* pupmod-simp-mozilla
* pupmod-simp-oddjob
* pupmod-simp-openscap
* pupmod-simp-rsync
* pupmod-simp-rsyslog
* pupmod-simp-simp_apache
* pupmod-simp-simp_nfs
* pupmod-simp-simp_openldap
* pupmod-simp-ssh
* pupmod-simp-sudo
* pupmod-simp-sudosh
* pupmod-simp-tcpwrappers
* pupmod-simp-tuned
* pupmod-simp-vnc
* pupmod-simp-vsftpd
* pupmod-simp-xinetd

Oracle-Linux
^^^^^^^^^^^^
The testing of the modules on Oracle Linux was expanded and automated.

RPM Updates
-----------

ELG Stack
^^^^^^^^^

The application RPMs for Elasticsearch, Logstash and Grafana (ELG) will no longer
be delivered with the SIMP ISO. Updates in the same major version of Elasticsearch
and Logstash have been shown to randomly corrupt data and are therefore too dangerous
to potentially drop into upstream updates repositories by default. Users must now download
their own ELG packages from their preferred repositories

Removed Modules
---------------

pupmod-simp-freeradius
^^^^^^^^^^^^^^^^^^^^^^

There was not enough time to get the ``freeradius`` components updated
sufficiently for Puppet 5 prior to release. This module may reappear in
future releases if there is significant demand.

Security Announcements
----------------------

None

Fixed Bugs
----------

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^
* Revert back to using the native service provider for the auditd service since
  puppet fixed the service handling.
* Allow users to opt-out of hooking the audit dispatchers into the SIMP rsyslog
  module using `auditd::config::audisp::syslog::rsyslog = false` or,
  alternatively, setting `simp_options::syslog = false`.
* Add a `write_logs` option to the `auditd_class` and multiplex between the
  `log_format = NOLOG` setting and `write_logs = false` since there were
  breaking changes in these settings after `auditd` version `2.6.0`.
* Add support for `log_format = ENHANCED` for `auditd` version >= `2.6.0`.
  Older versions will simply fall back to `RAW`.
* Removed unnecessary dependencies from metadata.json.  Now, when users install
  auditd stand-alone i.e. `puppet module install`, they will not have
  extraneous modules clutter their environment.

pupmod-simp-nfs
^^^^^^^^^^^^^^^
* Allow users to set the 'ensure' state of their client mount points in
  case they don't want them to be mounted by default.

pupmod-simp-rsyslogd
^^^^^^^^^^^^^^^^^^^^
* Updated templates to use RainerScript for rsyslogd V8 and later
* Fixed the MainMsgQueueDiscardMark and MainMsgQueueWorkerThreads
  parameters
* Updated rsyslog::rule::remote to select a more intelligent default
  for StreamDriverPermittedPeers when TLS is enabled.  This improvement
  fixes the bug in which forwarding of logs to servers in different domains
  was not possible within one call.
* Added logic to properly handle rsyslogd parameters for V8.6 and later
  as documented in CentOS 7.5 Release notes.  These include moving -x and -w
  options to global.conf and issuing deprecation warning for -l and -s
  options.

pupmod-simp-simp_grafana
^^^^^^^^^^^^^^^^^^^^^^^^

* Fix bug in resource ordering of pki::copy and grafana::service
* Use simplib::passgen() in lieu of deprecated passgen()

pupmod-simp-simp_logstash
^^^^^^^^^^^^^^^^^^^^^^^^^

* Workaround for upstream bug where OEL6 logstash::service_provider must
  be set.

pupmod-simp-simp_rsyslog
^^^^^^^^^^^^^^^^^^^^^^^^

* Make directory where logs are gathered configurable and make rules that organize
  them configurable.
* Updated simp_rsyslog::forward to allow configuration of the
  StreamDriverPermittedPeers directive in the forwarding rule actions
  for the remote rsyslog servers.  This allows the user to set the correct
  StreamDriverPermittedPeers value when the default value is incorrect
  (e.g., when IP addresses are used in simp_rsyslog::log_servers or
  simp_rsyslog::failover_servers and one or more of those servers
  is not in the same domain as the client).
* Remove redundant rules for sudosh since the puppet module will correctly take
  care of adding those rules.
* Add support for tlog since it will be commonly replacing sudosh across the
  SIMP infrastructure.

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^
* Fixed bug where uid_min would throw errors under operating systems
  without /etc/login.defs.
* Fixed bug where simplib_sysctl would throw an undefined method error
  on non-Linux OS's.  (both those with sysctl (MacOS X) and without (Windows))
* Fixed bug  with the `boot_dir_uuid` fact where it would throw an error if running
  on a system without a `/boot` partition (like a container).
* Ensure that reboot_notify updates resources based on a modified 'reason'

pupmod-simp-ssh
^^^^^^^^^^^^^^^
* Hardened all ssh_host_* keys for security and compliance

pupmod-simp-sudo
^^^^^^^^^^^^^^^^
* Enable support for Default of `cmnd` type in sudoers file.

pupmod-simp-svckill
^^^^^^^^^^^^^^^^^^^
* Added 7.5 rhel services to svckill::ignore_defaults list for EL7.

rubygem_simp_cli
^^^^^^^^^^^^^^^^
* Updated 'simp config' to support environment-specific :term:`Hiera` 5
  configuration provided by SIMP-6.3.0.

  - Assumes a legacy Hiera 3 configuration, when the 'simp'
    environment only contains a 'hieradata' directory.
  - Assumes a Hiera 5 configuration configuration, when the 'simp'
    environment contains both a 'hiera.yaml' file and a 'data/'

* Fixed `simp bootstrap` errors in puppetserver 5+:

  - No longer overwrites `web-routes.conf` (fix fatal config errors)
  - No longer adds `-XX:MaxPermSize` for Java >= 8 (fix warnings)

* The `trusted_server_facts` was removed in Puppet 5.0.0.
  The presence of this setting will cause each puppet run to emit the warning::
      Warning: Setting trusted_server_facts is deprecated.
  This patch causes `simp config` to quietly remove the setting if it is present
  and Puppet is version 5 or later.

NewFeatures
------------

pupmod-simp-x2go and pupmod-simp-mate
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
These modules are used to configure the x2go client and server to allow for
remote access to desktops and servers. This is an alternative to VNC. An example
configuration is documented in the User Guide.

pupmod-simp-tlog
^^^^^^^^^^^^^^^^
This module configures TLOG for logging privileged user activities.  Both sudosh
and TLOG are currently available but sudosh is no longer being maintained and is
expected to go away.

pupmod-simp-simp_pki_service
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Traditionally, SIMP has used an internal "FakeCA" `openssl`-based CA. Over
time, this has proven insufficient for our needs, particularly for capabilities
in terms of Key Enrollment (SCEP and CMC), OCSP, and overall management of
certificates.

Additionally, it was found that users wanted to adjust the certificate
parameters for the Puppet subsystem itself outside of the defaults and/or use a
"real", and more scalable CA system for all certificate management.

The pupmod-simp-simp_pki_service module  can be used to configure a
Certificate Authority (CA) using the Dogtag server.  This CA can be configured
either for the puppet server CA, the site CA in lieu of FakeCA, or both.

See the README in the module for details on how to configure it.

The Dogtag server was chosen because it is part of the FreeIPA suite.


Known Bugs
----------

Upgrading from previous SIMP 6.X versions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
There are known problems upgrading from Puppet 4 to Puppet 5.  Make sure you read the
upgrade instructions before attempting an upgrade.

TLOG
^^^^

TLOG can hang up in a specific circumstance.  If a user is logged into a system using a graphical
display and attempts to su to root from more than one terminal window in the same session, the second su will hang.
The above error does not affect ssh logins. If a user requires more than one root shell they should ssh into the
local system and su from that terminal.

This bug is tracked as SIMP-5426

.. _file bugs: https://simp-project.atlassian.net

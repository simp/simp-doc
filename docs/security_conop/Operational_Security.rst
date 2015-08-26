Operational Security
====================

This chapter contains SIMP security concepts that are related to the
operational security controls in `NIST
800-53 <http://csrc.nist.gov/publications/PubsSPs.html>`__.

Configuration Management
------------------------

This section describes the management of various configurations within
SIMP.

Baseline Configurations
~~~~~~~~~~~~~~~~~~~~~~~

SIMP baselines include configuration settings and Puppet modules.
Currently, baselines are maintained for both Red
Hat/CentOS 6.x, and Red Hat/CentOS 7.x. Each configuration item that is
managed by a Puppet module has an RPM installed on the Puppet Master in
the form of ``pupmod-name-x.x.x-x``. This process allows for one main SIMP
baseline to be maintained and modules to be upgraded easily. An overall
SIMP RPM is also installed on the Puppet Master, which denotes the
version number of SIMP that is installed. [CM-2, CM-2(2), CM-2(3), CM-6]

SIMP installs a minimal set of RPMs, which can be found in ?. RPMs,
services, and IPtables rules all use a deny-all, but allow-by-exception
module. Additional RPMs must be installed by each implementation.
Services must be declared explicitly or they will be disabled by Puppet;
IPtables rules must allow a service explicitly. [CM-2(5)]

Managing Configuration Changes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Configuration change approvals are managed by each implementation; SIMP
only provides the mechanisms to apply changes on clients. A combination
of Puppet, rsync, and YUM is used to apply those changes across all (or
selected) Puppet clients. All changes made are audited with auditd or
are logged to other files via syslog. [CM-3(a), CM-3(3)]

UNIX systems are made up of hundreds of configuration files that can
contain dozens of settings. SIMP does not make an attempt to manage all
of the settings in every file. Instead, critical operating system files
or files that need to be controlled centrally are managed.
Implementations can manage additional files if they are deemed
necessary. [CM-6]

Security Verification and Flaw Remediation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SIMP cannot detect flaws automatically; each implementation is
responsible for tracking flaws. However, SIMP provides a way for flaws
to be fixed across all clients. One or all of the following can help
automate flaw remediation [CM-6, SI-2, SI-2(1), SI-2(4)]:

-  **Puppet:** Apply a configuration change to files that are managed by
   Puppet.

-  **rsync:** Use this mechanism to deliver a file to a client. This can
   be used with or without Puppet to synchronize files.

-  **YUM:** Update packages nightly with YUM. Placing an updated package
   in YUM and running a YUM update manually, or allowing time for the
   cron job to run, will ensure packages on all clients are updated.
   Otherwise, a cron job will perform a daily update of packages with
   YUM.

-  **PSSH:** Allow commands to run across a set of nodes with the PSSH
   utility. Through the use of keys, this becomes a powerful way to run
   a one-time operation against a large number of nodes.

The extent of security verification that is performed currently is based
on changes to files that Puppet or the Advanced Intrusion Detection
Environment (AIDE) provides. There are also Security Content Automation
Protocol (SCAP) profiles available from the SCAP-Security-Guide project
that check security configuration settings. [SI-6]

Malicious Code Protection
~~~~~~~~~~~~~~~~~~~~~~~~~

For most environments, SIMP will use ClamAV to protect against malicious
code. Rsync is used to push out new definitions, which should be updated
by the local administrator regularly. SIMP also comes with a
``mcafee::uvscan`` module that manages an installation of uvscan, if it is
preferred. The module can configure ``.dat`` file updates to occur over
rsync.

Both the ClamAV and McAfee modules provide a method to run a scan via
cron on a customer scheduled basis. [SI-3] SIMP also comes with the
``chkrootkit`` tool to check for *rootkits*. The tool runs as a cron job
and places its output into syslog. [SI-3]

Software and Information Integrity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Unauthorized changes to a local client can be detected by Puppet or AIDE
(for any file managed by Puppet). In the event that a managed file is
changed locally, Puppet will revert the file back to its original state.
It is important to note that this is a function of Puppet and is
intended to be more of a configuration management feature rather than a
security feature. If a Puppet client has been compromised, the Puppet
Master may not have the ability to retake control over that client.
However, the Puppet Master can configure all other nodes to deny traffic
from the compromised node if they are configured by the administrator to
do so. There are additional configuration files that are checked by
AIDE, which is triggered by a cron job. AIDE logs any detected file
changes in syslog. Each implementation may add additional files that are
managed by Puppet or watched by AIDE. The AIDE baseline database is
updated periodically to handle the installation and updating of system
RPMs and reduce false positives. [SI-7, SI-7(1), SI-7(2), SI-7(3)]

Remote Maintenance
------------------

Remote maintenance can be performed on SIMP using SSH. Local maintenance
can be performed at the console or via serial port (if available). SSH
sessions are tracked and logged using the security features built into
SIMP. Console access requires someone to have access to the physical (or
virtual) console along with the ``root`` password. Auditing of those
actions also occurs in accordance with the configured audit policy. It
is up to the implementation to decide how to distribute authentication
information for remote maintenance. [MA-4, MA-4(1), MA-6]

Incident Response
-----------------

While Puppet is not intended to be a security product primarily, its
features help provide security functionality such as dynamic
reconfigurations and wide-scale consistent mitigation application. If an
implementation chooses, they can leverage Puppet's ability to
reconfigure systems as part of incident response. [IR]

Contingency Planning
--------------------

SIMP does not provide any direct support for contingency planning. Some
of the mechanisms provided by SIMP might be used to support an
implementation's contingency plan.

System Backup
-------------

SIMP comes with a module called ``backuppc``. This module provides a base
configuration of the `BackupPC <http://backuppc.sourceforge.net/>`__
software and allows Puppet servers and clients to perform backups.

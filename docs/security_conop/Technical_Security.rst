Technical Security
==================

This chapter contains SIMP security concepts that are related to the
technical security controls described in *NIST 800-53*.

Identification and Authentication
---------------------------------

This section addresses the identification and authentication of users
and devices.

User Identification and Authentication
--------------------------------------

Identification and authentication of system and service users can occur
at the system level or globally in the SIMP architecture. While local
accounts and groups can be created manually, the SIMP team suggests
adding users via the ``/etc/puppet/environments/simp/localusers`` file or by using the
native Puppet user and group types. System users can authenticate their
access using Secure Shell (SSH) keys or passwords. For more centralized
control, identify and authenticate users by using the Lightweight
Directory Access Protocol (LDAP). [IA-2]

The SIMP team recommends using LDAP as the primary source for user
management and provides a functional default OpenLDAP configuration for
this purpose. LDAP and Pluggable Authentication Modules (PAM) work
together closely and, with the default SIMP configuration, the PAM
settings are enforced on top of the LDAP settings for two layers of
control. Due to this partnership, items such as account lockouts may
need to be reset on both the local system and the LDAP server. If the
suggested settings in the SIMP-provided default Lightweight Directory
Interchange Formats (LDIF) are not used, implementations must ensure
that security is maintained through manual procedures. Use of group
accounts for users is strongly discouraged. System services may need to
have accounts, but all of these should be managed by Puppet using the
user and group native types. [IA-2(5)].

Device Identification and Authentication
----------------------------------------

Devices are identified by a Media Access Control (MAC) address prior to
receiving an IP address via the Dynamic Host Configuration Protocol
(DHCP). In the default SIMP architecture, IP addresses are fixed
mappings to their associated MAC address (i.e., not assigned
dynamically). There is no authentication for the binding of MAC
addresses to IP addresses due to the nature of the DHCP protocol.

Device authentication occurs through the mapping of the MAC to the IP
through the internally controlled DHCP and the mapping of the IP to the
host name through the internally controlled Domain Name System (DNS)
service for each individual Puppet client. After kickstart, each client
system generates an internal cryptographic identifier and communicates
that information with the Puppet server to be approved by an
administrator at a later time. All further communication between the
Puppet server and the clients over the Puppet protocol is encrypted
subsequently and authenticated with this identifier. Automatic approval
can be set up in tightly controlled environments; however, this option
is not suggested for open environments. [IA-3, IA-3(3)]

Identifier Management
---------------------

Managing user identifiers (also known as user names) involves
administrative procedures that are unique for each implementation.
Disabling unused local accounts is the only control that SIMP can
enforce technologically. In this case, if an account has an expired
password that has not been changed 35 days after expiration, the account
will be disabled. If a user does not have a password (e.g., he or she
only authenticates with SSH keys), then there is no inherent
technological mechanism for enforcement due to the nature of the
software. [IA-4(e)]

Authenticator Management
------------------------

Authenticators for users are passwords and/or SSH keys; the management
of each is implementation specific. SSH keys do not expire; therefore,
implementations must provide a procedure for removing invalid keys.
Removing public keys from LDAP is one practical solution.

When using passwords, local and LDAP passwords provided for users should
be set to change at first log on. This is the default in the
SIMP-provided LDIFs. Once a user attempts to change a password, the
settings in PAM and LDAP enforce complexity requirements. By default,
SIMP requires 14-character passwords with at least one character from
three of the four designated categories (i.e., upper case letters, lower
case letters, numbers, or special characters), and no more than three
consecutive characters from each category. [IA-5, IA-5(1), IA-5(4)]
Password ageing and history is enforced through a combination of PAM and
LDAP. By default, the previous 24 passwords cannot be reused.
[IA-5(1)(e)]

There are a number of default passwords in SIMP that are required for
installation. Each implementation requires the user to change the
default passwords and protect the new passwords. In addition, there are
embedded passwords within the SIMP system that are used due to a lack of
software-supported alternatives.

Access Control
--------------

This section describes the various levels of access control, including
account management, access enforcement, information flow enforcement,
separation of duties, least privilege, session controls, permitted
actions without identification and authentication, security attributes,
and remote access.

Account Management
------------------

Account management procedures should be created and maintained for each
implementation of SIMP. The procedures should include the information
listed in *NIST 800-53* control AC-2. SIMP has the mechanisms in place
to enforce most account management policies. The mechanisms for account
management have several default settings including:

-  Central account management using OpenLDAP. [AC-2(1)]

-  Password expiration. Local accounts expire 35 days after password
   expiration. [AC-2(3)]LDAP accounts do not expire automatically due to
   inactivity; implementations should audit LDAP accounts regularly.

-  Auditing of administrative actions to capture local account creation
   and modifications to LDAP accounts is done via the
   ``/var/log/slapd.audit`` file for ldap accounts and
   ``/var/log/audit/audit.log`` for local accounts. [AC-2(4)]

-  Shell session timeouts after 15 minutes of inactivity. [AC-2(5)] This
   can be circumvented by running a command that opens an endless pipe
   such as ``/bin/cat``. However, this command cannot be enforced more
   heavily due to the high likelihood of breaking system applications.
   If the optional gnome module is used, the GNOME screen saver will
   lock the screen after 15 minutes of inactivity.

-  Assignment of users into groups locally or centrally via LDAP.
   [AC-2(7)] By default, SIMP will have an administrators groups that
   has the ability to run ``sudosh``. Implementations should further
   define administrators or user groups and limit them with the Puppet
   ``sudo`` class.

Access Enforcement
------------------

SIMP uses the implementation of Discretionary Access Control (DAC) that
is native to Linux. Specific file permissions have been assigned based
on published security guidance for Red Hat, CentOS, and UNIX.

Default permissions on files created by users are enforced with user
file access mask settings (using the ``umask`` command) that allow only
the owner to read and write to the file. Implementations may further
extend the access control in UNIX by restricting access to application
files or using the file Access Control List (ACL) commands ``getfacl``
and ``setacl``. Users of SIMP should not change file permissions on
operating system files as it may decrease the overall security of the
system. If a group needs access to a particular file or directory, use
the ``setfacl`` command to allow the necessary access without lessening
the permissions on the system. [AC-3]

.. _Flow_Enforcement:

Information Flow Enforcement
----------------------------

IPtables on each SIMP system is controlled by the IPtables Puppet
module. When developing a new module, the IPtables rules needed for an
application should be included with the module by calling the
appropriate methods from the IPtables module. The end result should be a
running IPtables rule set that includes the default SIMP rules and any
rules needed for applications. The default communications allowed are
included in :ref:`default_server_ports` and :ref:`default_client_ports`. [AC-4]

.. _default_server_ports:

Default Server Ports
~~~~~~~~~~~~~~~~~~~~

.. list-table::
  :widths: 12 12 12 12 10 42
  :header-rows: 1

  * - Application
    - Direction
    - Protocol
    - Transport
    - Ports
    - Comment
  * - Puppet
    - Localhost
    - HTTP
    - TCP
    - 8140
    - The port upon which the Puppet master listens for client connections via Apache
  * - Puppet CA
    - In
    - HTTPS
    - TCP
    - 8141
    - This is used to ensure that Apache can verify all certificates from external systems properly prior to allowing access to Puppet.
  * - Apache/YUM
    - In
    - HTTP
    - TCP
    - 80
    - This is used for YUM and is unencrypted, since YUM will not work otherwise.
  * - DHCPD
    - In
    - DHCP/BOOTP
    - TCP/UDP
    - 546,
    - 547	 DHCP pooling is disabled by default and should only be used if the implementation requires the use of this protocol.
  * - TFTP
    - In
    - TFTP
    - TCP/UDP
    - 69
    - This is used for kickstart. It could also be used to update network devices. TFTP does not support encryption.
  * - rsyslog
    - Out
    - syslog
    - TCP/UDP
    - 6514
    - This is encrypted when communicating with a SIMP syslog server (not installed by default).
  * - named
    - In/Out
    - DNS
    - TCP/UDP
    - 53
    - Inbound connections happen to the locally managed hosts. Outbound connections happen to other domains per the normal operations of DNS.
  * - NTPD
    - Out
    - NTP
    - TCP/UDP
    - 123
    - Only connects to an external time source by default.
  * - SSHD
    - In
    - SSH
    - TCP
    - 22
    - SSH is always allowed from any source IP by default.
  * - stunnel
    - In
    - TLS
    - TCP
    - 8730
    - Stunnel is a protected connection for rsyncing configuration files to Puppet clients.
  * - rsync
    - Localhost
    - RSYNC
    - TCP
    - 873
    - This accepts connections to the localhost and forwards through Stunnel.
  * - LDAP
    - In
    - LDAP
    - TCP
    - 389
    - Connections are protected by bi-directional, authenticated encryption.
  * - LDAPS
    - In
    - LDAPS
    - TCP
    - 636
    - Used for LDAP over SSL.

.. _default_client_ports:

Default Client Ports
~~~~~~~~~~~~~~~~~~~~

.. list-table::
  :widths: 12 12 12 12 10 42
  :header-rows: 1

  * - Application
    - Direction
    - Protocol
    - Transport
    - Ports
    - Comment
  * - Puppet
    - Out
    - HTTPS
    - TCP
    - 8140
    - Communications to the Puppet server.
  * - rsyslog
    - Out
    - syslog
    - TCP/UDP
    - 6514
    - This is encrypted when communicating with a SIMP syslog server.
  * - DNS Client
    - Out
    - DNS
    - TCP/UDP
    - 53
    - Normal name resolution.
  * - NTPD
    - Out
    - NTP
    - TCP/UDP
    - 123
    - Only connects to an external time source by default.
  * - SSHD
    - In
    - SSH
    - TCP
    - 22
    - SSH is allowed from any source IP by default.
  * - LDAP
    - Out
    - LDAP
    - TCP
    - 389
    - Connections are protected by bi-directional authenticated encryption.

Separation of Duties
--------------------

SIMP enforces separation of duties using account groups. Groups are
created with each implementation to separate roles or duties properly.
The SIMP team recommends that this management be done using posixGroups
in LDAP for full operating System support. [AC-5]

Least Privilege
---------------

SIMP does not allow ``root`` to directly SSH into a system. The ``root``
user must be at a console (or at a virtual instance of the physical
console) to log on. Otherwise, users must log on as themselves and
perform privileged commands using ``sudo`` or ``sudosh``. [AC-6]

*NIST 800-53* least privilege security controls give people access to
objects only as needed. SIMP provides only the needed software,
services, and ports to allow the system to be functional and scalable.
The system then relies on a given implementation to perform proper
account management and user role assignments. [AC-6]

Session Controls
----------------

SIMP provides a number of security features for sessions. These features
include:

-  Accounts are locked after five invalid log on attempts over a
   15-minute period. The account is then locked for 15 minutes. No
   administrator action is required to unlock an account. [AC-7]

-  System banners are presented to a user both before and after logging
   on. The default banner should be customized for each implementation.
   [AC-8]

-  After a successful log on, the date, time, and source of the last log
   on is presented to the user. The number of failed log on attempts
   since the last log on is also provided. [AC-9 and AC-9(1)]

-  A limit of 10 concurrent SSH sessions are allowed per user. This can
   be further limited if an implementation decides it is set too high.
   Given the way SSH is used in operational settings, this default value
   is reasonable. [AC-10]

-  Session lock only applies if the ``windowmanager::gnome`` module is
   used. Sessions lock automatically after 15 minutes of inactivity.
   Users must authenticate their access with valid credentials to
   reestablish a session. [AC-11]

Permitted Actions without Identification and Authentication
-----------------------------------------------------------

SIMP has a number of applications that do not require both
identification and authentication. These services are listed below along
with an explanation of why these aspects are not required.
Implementations should include any additional services that do require
identification and/or authentication. [AC-14]

.. list-table::
  :widths: 16 84
  :header-rows: 1

  * - Service/Application
    - Rationale
  * - TFTP
    - TFTP is a simple file transfer application that, in the SIMP environment, does not allow for writing to the files being accessed. This application is primarily used to support the Preboot Execution Environment (PXE) booting of hosts and the updating of network devices. There is no option to authenticate systems at this level by protocol design. TFTP is limited to a user’s local subnet using IPtables and is enforced additionally with TCPWrappers.
  * - DHCP
    - By default, system IP addresses are not pooled, but are rather statically assigned to a client, which is identified by the MAC address. DHCP is limited to the local subnet.
  * - Apache/YUM
    - RPMs are stored in a directory for systems to use for both kickstart and package updating. Sensitive information should never be stored here. Apache/YUM is limited to the local subnet.
  * - DNS
    - The DNS protocol does not require identification nor authentication. DNS is limited to the local subnet.

Table: Actions Without Identification and Authentication

Security Attributes
-------------------

SELinux is now available in SIMP. SELinux is an implementation of
mandatory access control. It can be set to enforcing mode during the
SIMP configuration or turned on at a later time. All of the SIMP
packaged modules have been designed to work with SELinux set to
enforcing. [AC-16]

Remote Access
-------------

Remote access in SIMP is performed over SSH, specifically using the
OpenSSH software. OpenSSH provides both confidentiality and integrity of
remote access sessions. The SSH IPtables rules allow connections from
any host. SSH relies on other Linux mechanisms to provide identification
and authentication of a user. As discussed in the auditing section, user
actions are audited with the audit daemon and sudosh. [AC-17]

Systems and Communications Protection
-------------------------------------


The following sections provide information regarding application
partitioning, shared resources, and various levels of protection for
systems and communications.

User and Administration Application Separation (Application Partitioning)
-------------------------------------------------------------------------

SIMP can be used in a variety of ways. The most common is a platform for
hosting other services or applications. In that case, there are only
administrative users present. Users with accounts will be considered as
a type of privileged user.

SIMP can also be used as a platform for workstations or general users
performing non-administrative activities. In both cases, general users
with accounts on an individual host are allowed access to the host using
the ``pam::access`` module, so long as they have an account on the target
host. No user may perform or have access to administrative functions
unless given sudo or sudosh privileges via Puppet.

Shared Resources
----------------

There are several layers of access control that prevent the unauthorized
sharing of resources in SIMP. Account access, operating system DAC
settings, and the use of PKI collectively prevent resources from being
shared in ways that were not intended. [SC-4]

Denial of Service Protection
----------------------------

SIMP has limited ability to prevent or limit the effects of Denial of
Service (DoS) attacks. The primary measures in place are to drop
improperly formatted packets using IPtables and Kernel configurations
such as syncookies. [SC-5]

Boundary Protection
-------------------

SIMP does not provide boundary protection. [SC-7]

Transmission Security
---------------------

SIMP traffic is protected with protocols that provide confidentiality
and integrity of data while in transit. The tables in :ref:`Flow_Enforcement` describe the
protocols used to encrypt traffic and explain the protocols that cannot
be protected at the transmission layer. SSH, SSL, and TLS all provide
data transmission integrity and confidentiality. The software that
controls them on Red Hat and CentOS are OpenSSH and OpenSSL. The SIMP
team takes industry guidance into consideration when configuring these
services. For example, the list the cryptographic ciphers available is
limited to the highest ciphers that SIMP needs. All others are removed.
[SC-8, SC-9, SC-23, SC-7]

Single User Mode
----------------

SIMP systems have a password requirement for single user mode. In the
event maintenance needs to be performed at a system console, users must
be in possession of the ``root`` password before they can be
authenticated. Grub passwords are also set to prevent unauthorized
modifications to boot parameters. [SC-24]

PKI and Cryptography
--------------------

SIMP has two native certificate authorities. The first is known as Fake
CA. A local certificate authority is used to create properly formed
server certificates if an implementation does not have other means of
obtaining them. Many SIMP services require certificates; therefore, SIMP
provides this tool for testing or for situations where other
certificates are not available. The second certificate authority, Puppet
CA, is built into Puppet. Puppet creates, distributes, and manages
certificates that are specifically for Puppet. More information on the
Puppet CA can be found in the Puppet Labs `security
documentation <http://projects.puppetlabs.com/projects/1/wiki/certificates_and_security>`__.
[SC-17, SC-13]

.. warning::
    Fake CA certificates should not be used in an operational setting.

Mobile Code
-----------

SIMP does not use mobile code; however, there are not any particular
tools that will prevent its use. [SC-18]

Protection of Information at Rest
---------------------------------

There are no additional protections for information at rest beyond
operating system capabilities in SIMP. There are also no measures in
place to encrypt or sign data before transmission. Each implementation
should determine how to further protect information at rest. [SC-28]

Audit and Accountability
------------------------

This section discusses the content, storage, and protection of auditable
events.

Auditable Events
----------------

Auditd and rsyslog provide the foundation for SIMP auditing. Auditd
performs the majority of the security-related events; however, other
Linux logs also have security information in them, which are captured
using rsyslog.

The default auditable events for SIMP were developed based on several
industry best practices including those from the SCAP Security Guide and
several government configuration guides. The suggested rules by those
guides were fine-tuned so the audit daemon would not fill logs with
useless records or reduce performance. These guides should be referenced
for a detailed explanation of why rules are applied. Additional
justification can be found in the comments of the SIMP audit rules found
in the appendix of this guide. [AU-2]

The SIMP development team reviews every release of the major security
guides for updated auditable events suggestions. Each of those
suggestions is reviewed and applied if deemed applicable. [AU-2(3)]
Privileged commands are audited as part of the SIMP auditing
configuration. This is accomplished by monitoring ``sudo`` commands with
auditd. Keystrokes for administrators that use ``sudosh`` are also
logged. Each session can be replayed using ``sudosh-replay``. [AU-2(4)]

Content of Audit Records
------------------------

Audit records capture the following information [AU-3]:

-  Date and Time

-  UID and GID of the user performing the action

-  Command

-  Event ID

-  Key

-  Node Hostname/IP Address

-  Login Session ID

-  Executable

Audit Storage
-------------

Audit logs are stored locally on a separate partition in the ``/var/log``
directory. The size of this partition is configurable. Other default
audit storage configurations include:

-  A syslog log is written when the audit partition has 75MB free. (This
   can be changed to e-mail, if e-mail infrastructure is in place.)
   [AU-5(a), AU-5(1)]

-  The log file rotates once it reaches 30MB.

Audit Reduction and Response
----------------------------

SIMP provides a means to capture the proper information for audit
records and stores them centrally. Each implementation must decide and
document how it reduces, analyzes, and responds to audit events. [AU-5]

Auditd, like all services in SIMP, is controlled by Puppet. Stopping the
service without disabling Puppet means the service will always be
started automatically during a Puppet run. The files that control the
audit configuration will also revert to their original state if changed
manually on a client node. In the event auditd fails, the system will
continue to operate. Several security guides have suggested that the
system should shut down if auditd fails for any reason. However, SIMP
will not shut down, but will provide an alert via syslog when this
happens. [AU-5(1)]

SIMP also comes with an optional module for the
Elasticsearch/Logstash/Kibana (ELK) stack. These three open source tools
can be combined to parse, index, and visualize logs. There are also SIMP
provided dashboards for the Kibana web interface. Implementations can
build their own dashboards to meet local security or functional needs
for log reduction and management. [AU-6]

Protection of Audit Information
-------------------------------

The primary means of protecting the audit logs is through the use of
file permissions. Audit records are stored in the ``/var/log`` directory
and can only be accessed by ``root``. Audit logs are rotated off daily if
the implementation has not developed a way of offloading the logs to
another location where they can be backed up. Lastly, if the
``rsyslog::stock::log_server`` module is implemented, logs are
transmitted to the log server over a TLS protected link.

Time Synchronization
--------------------

Each SIMP client (including the Puppet Master) has NTPD enabled by
default. Part of the installation directs the clients to a time server.
If no servers are available, the SIMP clients can use the Puppet Master
as the central time source. Audit logs receive their time stamp from the
local server's system clock; therefore, the SIMP client must be
connected to a central time source for time stamps in audit logs to be
accurate.

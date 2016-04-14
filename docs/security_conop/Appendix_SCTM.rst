SIMP SCTM
=========

This SCTM was developed based on the National Institute of Standards and
Technology (NIST) Specical Publication 800-53 (Revision 3) controls that
SIMP currently meets. Empty contents means SIMP does not meet that
control. Implementations are free to take these tables and use them as a
starting point for any accreditation activities that follow NIST 800-53.

SIMP SCTM Technical Controls
----------------------------

.. list-table::
   :widths: 12 20 13 55
   :header-rows: 1

   * - Control ID
     - Control Name
     - Control Family
     - SIMP Implementation Method
   * - AC-1
     - Access Control Policy and Procedures
     - Access Control
     -
   * - AC-2(1)
     - Account Management (Control Enhancement)
     - Access Control
     - LDAP is used to centrally manage accounts. Local accounts can optionally be added and managed by puppet.
   * - AC-2(2)
     - Account Management (Control Enhancement)
     - Access Control
     -
   * - AC-2(3)
     - Account Management (Control Enhancement)
     - Access Control
     - Inactive local accounts expire 35 days after password expiration. LDAP accounts can be set to expire in LDAP and using PAM. There is no automated method (included with SIMP) to check inactive LDAP accounts. Implementations should address inactive LDAP accounts with automated or administrative measures.
   * - AC-2(4)
     - Account Management (Control Enhancement)
     - Access Control
     - Local account creation is audited with auditd. (as are all of root's actions). Sudosh logs all commands for someone running sudosh. This will not work if the SIMP implementation uses specific sudo rules. Instead, sudo actions are logged using auditd. Ldap modifications are logged in the ldap logs.
   * - AC-2(5)
     - Account Management (Control Enhancement)
     - Access Control
     - Shell accounts are logged out after 15 minutes of inactivity
   * - AC-2(6)
     - Account Management (Control Enhancement)
     - Access Control
     -
   * - AC-2(7)
     - Account Management (Control Enhancement)
     - Access Control
     - SIMP has a default administrators group (700) that users can be assigned to. Additional roles and groups are up to the implementations. Role changes are logged in the LDAP logs.
   * - AC-3
     - Access Enforcement
     - Access Control
     -
   * - AC-3(2)
     - Access Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-3(3)
     - Access Enforcement (Control Enhancement)
     - Access Control
     - DAC has been built into Unix for a long time and is expected to work. Implementations may want to check that user assignments to groups properly enforce DAC they way they expect. New as of SIMP 5.0 is the use of MAC. All stock SIMP modules work with MAC enabled. It's up to each implementation to ensure their applications and modules are made to work with MAC enabled.
   * - AC-3(4)
     - Access Enforcement (Control Enhancement)
     - Access Control
     - DAC has been built into Unix for a long time and is expected to work. Implements may want to check that user assignments to groups properly enforce DAC they way they expect.
   * - AC-3(5)
     - Access Enforcement (Control Enhancement)
     - Access Control
     - SIMP implements file permissions per the SCAP-Security-Guide (SSG) RHEL7 guidance. There are some exceptions of file permissions being more or less restrictive than the guide. Mitigations and responses to those variances will be published once final RHEL7 SCAP content is available.
   * - AC-3(6)
     - Access Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(1)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     - IPTables enforces flow control to the puppet master and clients. The default rules allow the services needed for kick start and puppet (and SSH of course). IPTables is managed by puppet so that any user modifications to /etc/sysconfig/iptables is rewritten with the rules from the manifest. The rules can and should be tailored per implementation.
   * - AC-4(2)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(3)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(4)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(5)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(6)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(7)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(8)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(9)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(10)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(11)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(12)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(13)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(14)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(15)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(16)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-4(17)
     - Information Flow Enforcement (Control Enhancement)
     - Access Control
     -
   * - AC-5
     - Separation of Duties
     - Access Control
     -
   * - AC-6
     - Least Privilege
     - Access Control
     - SIMP was built using a minimalist approach. Only the services, applications (RPMs and their dependencies), and network rules that are needed are implemented. Adding additional services, users, or software are done using built in RedHat/CentOS features or puppet. For example, services cannot be manually added without first registering them with puppet.
   * - AC-6(1)
     - Least Privilege (Control Enhancement)
     - Access Control
     - File permissions and administrative functions are denied to users who are not administrators using Unix DAC. Roles can be defined by a implementation. Typically it's done using ldap groups and sudosh. Suoders rules can be set for roles that need a limited set of commands/functions.
   * - AC-6(2)
     - Least Privilege (Control Enhancement)
     - Access Control
     - Direct remote root login is not allowed on SIMP. Users must assume their role first (defined in LDAP or locally). There is a local simp user on the puppet master that has a password assigned. That allows for emergency maintenance via SSH. Single user mode is password protected, but will allow direct access before escalation. Protection of the single user mode and simp user's password is up to the implementation. Privilege escalation is performed using sudosh or sudo. Most implementations will use sudosh for global admins and sudo for roles that need minimal admin ability. Lastly, serial port access is does allow direct root login (/etc/securetty). Implementations may further restrict this at the risk.
   * - AC-6(3)
     - Least Privilege (Control Enhancement)
     - Access Control
     -
   * - AC-6(4)
     - Least Privilege (Control Enhancement)
     - Access Control
     -
   * - AC-6(5)
     - Least Privilege (Control Enhancement)
     - Access Control
     -
   * - AC-6(6)
     - Least Privilege (Control Enhancement)
     - Access Control
     -
   * - AC-7
     - Unsuccessful Login Attempts
     - Access Control
     - SIMP locks accounts after 5 invalid attempts over 15 minutes span. It then keeps the account locked for 15 minutes. After that, the account is unlocked automatically.
   * - AC-7(1)
     - Unsuccessful Login Attempts (Control Enhancement)
     - Access Control
     - An account is never locked to a point an admin must unlock it. It will continue to be unlocked after 15 minutes. This should meet most modern policies. It can be further restricted if required by local policies.
   * - AC-7(2)
     - Unsuccessful Login Attempts (Control Enhancement)
     - Access Control
     -
   * - AC-8
     - System Use Notification
     - Access Control
     - SIMP displays a default banner prior to login. Implementations must customize that banner for their use.
   * - AC-9
     - Previous Logon (Access) Notification
     - Access Control
     - SIMP uses the pam\_lastlog.so module to display last login information.
   * - AC-9(1)
     - Previous Logon (Access) Notification (Control Enhancement)
     - Access Control
     - SIMP uses the pam\_lastlog.so module to display last login information.
   * - AC-9(2)
     - Previous Logon (Access) Notification (Control Enhancement)
     - Access Control
     - SIMP uses the pam\_lastlog.so module to display last login information, including the number of failed login attempts since the last logon.
   * - AC-9(3)
     - Previous Logon (Access) Notification (Control Enhancement)
     - Access Control
     -
   * - AC-10
     - Concurrent Session Control
     - Access Control
     - The default value for concurrent sessions in SIMP is 10 (/etc/security/limits.conf). Given the variety of system usage to include automated processes, it could impact functionality if this value were set lower. It can be tailored to a lower value if the implementation determines that number will not impact functionality.
   * - AC-11
     - Session Lock
     - Access Control
     - Terminal sessions do not enforce a session lock so this control is technically not implemented. However, it's mitigated by forcing inactive sessions to log out. If the gnome module is applied, SIMP locks a gnome session after 5 minutes.
   * - AC-14
     - Permitted Actions without Identification or Authentication
     - Access Control
     - SIMP provides several services that do not require authentication. Most require some form of identification. These are documented in the SIMP Security Concepts and is kept current for that version. Individual modules are not yet documented.
   * - AC-14(1)
     - Permitted Actions without Identification or Authentication (Control Enhancement)
     - Access Control
     - Justifications to those services that do not require Identification and Authentication can be found in the SIMP Security Concepts document.
   * - AC-16
     - Security Attributes
     - Access Control
     - New in SIMP 5.0 is the usage of MAC via SELinux. This is optional for each implementation and can be turned off at any time. All of the stock SIMP modules work with SELinux enabled and have the least restrictive MAC policies enforced. These policies assign each object a SELinux user, role, type, and level. These characteristics are used to define a context for each object.
   * - AC-16(1)
     - Security Attributes (Control Enhancement)
     - Access Control
     -
   * - AC-16(2)
     - Security Attributes (Control Enhancement)
     - Access Control
     -
   * - AC-16(3)
     - Security Attributes (Control Enhancement)
     - Access Control
     -
   * - AC-16(4)
     - Security Attributes (Control Enhancement)
     - Access Control
     - SeLinux user, role, type, and level are the security attributes that are associated with each object with SELinux enabled in SIMP.
   * - AC-16(5)
     - Security Attributes (Control Enhancement)
     - Access Control
     -
   * - AC-17
     - Remote Access
     -
     - By default, external connections are not allowed with the exception of SSH. This is documented in the SIMP user manual. Implementations have the ability to override this with the understanding that puppet controls Iptables.
   * - AC-17(1)
     - Remote Access (Control Enhancement)
     - Access Control
     - The extent of monitoring remote connections is done by auditd and syslog. The contents of the remote session is not logged. The keystrokes of users with sudosh shells are all logged.
   * - AC-17(2)
     - Remote Access (Control Enhancement)
     - Access Control
     - Remote access is limited to SSH. SSH (openssh on centos/rhel) provides both confidentiality and integrity of the remote session.
   * - AC-17(3)
     - Remote Access (Control Enhancement)
     - Access Control
     -
   * - AC-17(4)
     - Remote Access (Control Enhancement)
     - Access Control
     - This control is enforced via other access control mechanisms already covered in 800-53. Namely, AC-6. By default, SSH in SIMP will allow anyone to connect. Once identification and authentication is performed, access control to privileged commands is enforced as usual.
   * - AC-17(5)
     - Remote Access (Control Enhancement)
     - Access Control
     - Auditd provides logging of failed access attempts. It's up to the implementation to perform a level of inspection of these unauthorized events. Auditd does this by default. Other checks will ensure auditd is running and registered with puppet.
   * - AC-17(6)
     - Remote Access (Control Enhancement)
     - Access Control
     -
   * - AC-17(7)
     - Remote Access (Control Enhancement)
     - Access Control
     -
   * - AC-17(8)
     - Remote Access (Control Enhancement)
     - Access Control
     - This control is only met by defining all connections that SIMP allows internally and externally. For now, since this is a remote access control, it should suffice to continue to note that the only remote access protocol allowed by default is SSH.
   * - AC-18
     - Wireless Access
     - Access Control
     -
   * - AC-18(1)
     - Wireless Access (Control Enhancement)
     - Access Control
     -
   * - AC-18(2)
     - Wireless Access (Control Enhancement)
     - Access Control
     -
   * - AC-18(3)
     - Wireless Access (Control Enhancement)
     - Access Control
     -
   * - AC-18(4)
     - Wireless Access (Control Enhancement)
     - Access Control
     -
   * - AC-18(5)
     - Wireless Access (Control Enhancement)
     - Access Control
     -
   * - AC-19
     - Access Control for Mobile Devices
     - Access Control
     -
   * - AC-19(1)
     - Access Control for Mobile Devices (Control Enhancement)
     - Access Control
     -
   * - AC-19(2)
     - Access Control for Mobile Devices (Control Enhancement)
     - Access Control
     -
   * - AC-19(3)
     - Access Control for Mobile Devices (Control Enhancement)
     - Access Control
     -
   * - AC-19(4)
     - Access Control for Mobile Devices (Control Enhancement)
     - Access Control
     -
   * - AC-20
     - Use of External Information Systems
     - Access Control
     -
   * - AC-20(1)
     - Use of External Information Systems (Control Enhancement)
     - Access Control
     -
   * - AC-20(2)
     - Use of External Information Systems (Control Enhancement)
     - Access Control
     -
   * - AC-21
     - User-Based Collaboration and Information Sharing
     - Access Control
     -
   * - AC-21(1)
     - User-Based Collaboration and Information Sharing (Control Enhancement)
     - Access Control
     -
   * - AC-22
     - Publicly Accessible Content
     - Access Control
     -
   * - AU-1
     - Audit and Accountability Policy and Procedures
     - Audit and Accountability
     -
   * - AU-2
     - Auditable Events
     - Audit and Accountability
     - a. SIMP audit rules were built by using industry best practices gathered
          over the years. The heaviest reliance has been on the SCAP-Security
          Guide (SSG). SIMP aims for a balance between performance and
          operational needs so the settings are rarely an exact match from
          these guides. The list of events that audited are by auditd can be
          found in appendix of the Security Concepts document.
       b. Implementation Specific
       c. Rationale is for audit setting is provided in SSG. d. Threat
          information is specific to the implementation. Auditd and syslog
          facility can always be fine tuned for each implementation.
   * - AU-2(3)
     - Auditable Events (Control Enhancement)
     - Audit and Accountability
     - SIMP is constantly reviewing the audit rules for accuracy, relevance, and performance. Rules are added and in some cases removed as information becomes available.
   * - AU-2(4)
     - Auditable Events (Control Enhancement)
     - Audit and Accountability
     - Privileged user commands are logged using sudosh and auditd (sudo actions). By default, users in the administrators group can run sudosh. All of the key strokes (except things that are not echoed back to the screen like passwords) are logged to /var/log/sudosh.log and can be sent to syslog. If an implementation sets up specific sudo actions for other groups or users, those actions are logged with auditd.
   * - AU-3
     - Content of Audit Records
     - Audit and Accountability
     - The linux audit daemon contains event type, date/time, host, and outcome of events by default.
   * - AU-3(1)
     - Content of Audit Records (Control Enhancement)
     - Audit and Accountability
     - There are a number of events that are captured beyond the auditd. The SIMP syslog module captures additional log events from apache, ldap, puppet, messages.log, and secure.log.
   * - AU-3(2)
     - Content of Audit Records (Control Enhancement)
     - Audit and Accountability
     - By default, the SIMP syslog module logs locally. There is an option to send the syslog events to a central location. Instructions for implementing a syslog server are provided in the User Guide. Lastly, a combination of elasticsearch, logstash, and kibana (ELK) can be applied to filter, index, and search logs. Puppet modules are provided for the ELK stack
   * - AU-4
     - Audit Storage Capacity
     - Audit and Accountability
     - The audit partition is configured as a separation partition from the system files, reducing the likelihood of audit interfering with system operations. Implementations can change this but it's highly discouraged.
   * - AU-5
     - Response to Audit Processing Failures
     - Audit and Accountability
     - a. Implementation Specific.
       b. The audit.conf file configures the system to log to syslog when disk
          space becomes low. If the disk becomes full, the audit daemon will be
          suspended, but the system will remain active. This is contrary to
          some industry guidance to put the system into single user mode when
          disk space becomes an issue. Implementations may wish to change the
          default behaviour at the risk of stopping the system from
          functioning.
   * - AU-5(1)
     - Response to Audit Processing Failures (Control Enhancement)
     - Audit and Accountability
     - SIMP provides a warning (to syslog) when the disk has 75MB free. Each log file can be up to 30MB.
   * - AU-5(2)
     - Response to Audit Processing Failures (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-5(3)
     - Response to Audit Processing Failures (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-5(4)
     - Response to Audit Processing Failures (Control Enhancement)
     - Audit and Accountability
     - SIMP will not shut down a system by default. Implementation can configure this option at the own risk in the auditd.conf file.
   * - AU-6
     - Audit Review, Analysis, and Reporting
     - Audit and Accountability
     -
   * - AU-6(1)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-6(3)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     - The ELK modules provide implementations with one means to centralize, review, and recognize trends in SIMP logs.
   * - AU-6(4)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     - The ELK modules provide implementations with one means to centralize, review, and recognize trends in SIMP logs.
   * - AU-6(5)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     - The ELK modules provide implementations with one means to centralize, review, and recognize trends in SIMP logs. The logs sent to syslog can be customized to include logs from any application. They would then be in a central place for viewing and aggregation by users of the Kibana interface.
   * - AU-6(6)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-6(7)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-6(9)
     - Audit Review, Analysis, and Reporting (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-7
     - Audit Reduction and Report Generation
     - Audit and Accountability
     -
   * - AU-7(1)
     - Audit Reduction and Report Generation (Control Enhancement)
     - Audit and Accountability
     - While not true audit reduction, RedHat does allow someone with access to audit logs to perform filters using the journald. If audit logs are forwarded to a syslog server, it's not difficult for an admin to security officer to run batch filters against all of the audit records. As of SIMP 4.0.5, an optional Logstash, Kibana, and Elasticsearch modules can be applied. If applied, they provide centralized and indexed logs. An implementation can then perform searches against the logs or provide alerts to other parts of their infrastructure.
   * - AU-8
     - Time Stamps
     - Audit and Accountability
     - Auditd uses the system clock to time stamp audit events.
   * - AU-8(1)
     - Time Stamps (Control Enhancement)
     - Audit and Accountability
     - Time is an essential component of puppet. Therefore, NTPD is used to synchronize puppet clients with the puppet server. That default configuration can be changed to synchronize puppet each server/client with another time source.
   * - AU-9
     - Protection of Audit Information
     - Audit and Accountability
     - File system permissions and SELinux protect the content of /var/log/audit and /etc/audit/\*
   * - AU-9(1)
     - Protection of Audit Information (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-9(2)
     - Protection of Audit Information (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-9(3)
     - Protection of Audit Information (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-9(4)
     - Protection of Audit Information (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-10
     - Non-repudiation
     - Audit and Accountability
     -
   * - AU-10(1)
     - Non-repudiation (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-10(2)
     - Non-repudiation (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-10(3)
     - Non-repudiation (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-10(4)
     - Non-repudiation (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-10(5)
     - Non-repudiation (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-12(1)
     - Audit Generation (Control Enhancement)
     - Audit and Accountability
     -
   * - AU-11
     - Audit Record Retention
     - Audit and Accountability
     -
   * - AU-12
     - Audit Generation
     - Audit and Accountability
     - a. Auditd provides the audit generation capability and is running on all
          SIMP systems by default.
       b. The audit.rules files configures events that are audited.
       c. The audit.rules applies the list of audit rules defined in SIMP
          Security Concepts document.
   * - AU-12(1)
     - Audit Generation (Control Enhancement)
     - Audit and Accountability
     - Auditd stamps audit records with the system time. The system time is obtained from a central time source and synchronized between SIMP systems.
   * - AU-12(2)
     - Audit Generation (Control Enhancement)
     - Audit and Accountability
     - Auditd provides logging in standard formats. Additionally, logs that are sent through syslog adhere to that standard.
   * - AU-13
     - Monitoring For Information Disclosure
     - Audit and Accountability
     -
   * - AU-14
     - Session Audit
     - Audit and Accountability
     -
   * - AU-14(1)
     - Session Audit (Control Enhancement)
     - Audit and Accountability
     - Sessions that use the sudo shell have all keystrokes recorded. Those sessions can be viewed in text format or replayed to the screen
   * - IA-1
     - Identification and Authentication Policy and Procedures
     - Identification and Authentication
     -
   * - IA-2(1)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(2)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(3)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(4)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(5)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(6)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(7)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-2(8)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     - The authentication mechanisms used within SIMP are all resistant to replay attacks by default. Known vulnerabilities can occur in the protocols. As they are known, vendors release patches, which must them be applied by the implementation. Privileged accounts use the same protocols as unprivileged accounts.
   * - IA-2(9)
     - User Identification and Authentication (Organizational Users) (Control Enhancement)
     - Identification and Authentication
     - The authentication mechanisms used within SIMP are all resistant to replay attacks by default. Known vulnerabilities can occur in the protocols. As they are known, vendors release patches, which must them be applied by the implementation.
   * - IA-3
     - Device Identification and Authentication
     - Identification and Authentication
     - Identification of each puppet client occurs before an IP address can be assigned. This is controlled using DHCP (each client must have an address bound by MAC address). Devices identification and authentication with puppet occurs using SSL certificates. The clients must each have a SSL certificate installed to establish a valid session with the puppet master.
   * - IA-3(1)
     - Device Identification and Authentication (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-3(2)
     - Device Identification and Authentication (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-3(3)
     - Device Identification and Authentication (Control Enhancement)
     - Identification and Authentication
     - DHCP is used to statically define the IP addresses of each puppet client.
   * - IA-4
     - Identifier Management
     - Identification and Authentication
     - Local accounts expire 35 days after their passwords expire. There is no mechanism implemented to detect inactive LDAP accounts. Implementations might wish to mitigate this by regularly reviewing and removing unneeded accounts.
   * - IA-4(1)
     - Identifier Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-4(2)
     - Identifier Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-4(3)
     - Identifier Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-4(4)
     - Identifier Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-4(5)
     - Identifier Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-5
     - Authenticator Management
     - Identification and Authentication
     - C. Authenticator strength is enforced using pam\_crack\_lib.so. This works for user defined passwords on local and LDAP accounts. E. It's up to the implementation to change the values for the various passwords. F. Password history is set to 24 by default in SIMP and enforced with pam.G. For local accounts, password aging is set to 180 days. It's set to the same in LDAP, but enforced at the time of account creation using ldifs. LDAP subsequently uses PAM to enforce the aging. Key based passwordless logins do not enforce aging. Upon generation, server and puppet certificates can also be set to expire.H. Authenticators for local and LDAP account are protected using operating system access controls. The server certificates are also protected using operating system controls.
   * - IA-5(1)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     - a. Authenticator strength is enforced using pam\_crack\_lib.so. This
          works for user defined passwords on local and LDAP accounts.
          Administrators can bypass PAM and set weak passwords in LDAP. Under
          normal circumstances, users would be forced to change their password
          at login, at which point pam enforced complexity.
       b. Not enforced.
       c. Hashed passwords are built into linux (/etc/shadow and
          /etc/pam.d/system-auth pam\_unix.so). LDAP password changed by users
          are done through pam before getting placed in LDAP. Manual LDAP
          password are created using the slapasswd command.
       d. Password minimum and maximum lifetimes are enforced through
          /etc/login.defs and ldap.
       e. By default, the previous 24 passwords can not be reused.
   * - IA-5(2)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     - Puppet comes with a self contained public key infrastructure. Though just used for puppet, it operates as a full PKI. So the certificate path is validated.SSL certificates that are used for SSL and TLS also have certificate path validation built into the protocol.Note: SSH Keys are not considered PKI.
   * - IA-5(3)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-5(4)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     - Pam cracklib enforces password complexity rules on Redhat and CentOS. Additional tools to check authenticator strength can be used in operational settings.
   * - IA-5(5)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     - The simp-config utility gives each implementation an opportunity to change default passwords at build time. It's up to the implementation to change the values for the various passwords.
   * - IA-5(6)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     - Authenticators are protected with operating system access control and file permissions.
   * - IA-5(7)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     - Plaintext passwords are only used when application support no other means of providing a password.
   * - IA-5(8)
     - Authenticator Management (Control Enhancement)
     - Identification and Authentication
     -
   * - IA-6
     - Authenticator Feedback
     - Identification and Authentication
     - Plaintext passwords are not echoed back to the screen.
   * - IA-7
     - Cryptographic Module Authentication
     - Identification and Authentication
     - Redhat 7 and the several modules are being evaluated for FIPS 140 compliance. Implementations should check the FIPS site for updates on this evaluation. The SIMP team will also continue to evaluate the status and any relevant settings that need to be applied as a result of this evaluation.
   * - IA-8
     - Identification and Authentication (Non-Organizational Users)
     - Identification and Authentication
     -
   * - SC-1
     - System and Communications Protection Policy and Procedures
     - System and Communications Protection
     -
   * - SC-2
     - Application Partitioning
     - System and Communications Protection
     - The spirit of this control is providing logical separation so that users are not able to access administrative functions. There is no notion of partitioning within SIMP. There are access control enforcement that can be proven through tests on those controls. If this control is allocated to SIMP alone, it's unlikely it can be met. Since SIMP is the infrastructure that applications would use, showing that application users cannot access the SIMP environment is a better way to prove this control is met.
   * - SC-2(1)
     - Application Partitioning (Control Enhancement)
     - System and Communications Protection
     - The spirit of this control is providing logical separation so that users are not able to access administrative functions. There is no notion of partitioning within SIMP. There are access control enforcement that can be proven through tests on those controls. If this control is allocated to SIMP alone, it's unlikely it can be met. Since SIMP is the infrastructure that applications would use, showing that application users cannot access the SIMP environment is a better way to prove this control is met.
   * - SC-3
     - Security Function Isolation
     - System and Communications Protection
     - The spirit of this control is providing logical separation so that users are not able to access administrative functions. There is no notion of partitioning within SIMP. There are access control enforcement that can be proven through tests on those controls. If this control is allocated to SIMP alone, it's unlikely it can be met. Since SIMP is the infrastructure that applications would use, showing that application users cannot access the SIMP environment is a better way to prove this control is met.
   * - SC-3(1)
     - Security Function Isolation (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-3(2)
     - Security Function Isolation (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-3(3)
     - Security Function Isolation (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-3(4)
     - Security Function Isolation (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-3(5)
     - Security Function Isolation (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-4
     - Information In Shared Resources
     - System and Communications Protection
     - While difficult for the SIMP team to prove, object reuse has been part of previous versions of RedHat common criteria testing. That testing focusing on Files system objects, IPC objects and Memory objects. Any issues discovered within the platform that cause object reuse issues are likely to be address in security patches provided by the vendor.
   * - SC-4(1)
     - Information In Shared Resources (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-5
     - Denial of Service Protection
     - System and Communications Protection
     -
   * - SC-5(1)
     - Denial of Service Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-5(2)
     - Denial of Service Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-6
     - Resource Priority
     - System and Communications Protection
     -
   * - SC-7
     - Boundary Protection
     - System and Communications Protection
     - Most of this control deals with a separate boundary interface (FW etc.). There is a part of this control that deals with controlling network access at key internal boundary points. Since SIMP implements IPTables on all hosts (by default), each node might be considered an internal boundary. Note – internal boundaries are more likely implemented via vlans or internal layer 3 devices.
   * - SC-7(1)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(2)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(3)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(4)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(5)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     - Iptables, as configured by default, blocks all incoming traffic except for what is explicitly allowed.
   * - SC-7(6)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(7)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(8)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(9)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(10)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(11)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(12)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     - IPTables is the host based firewall implementation on RedHat/CentOS.
   * - SC-7(13)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(14)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(15)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(16)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(17)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-7(18)
     - Boundary Protection (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-8
     - Transmission Integrity
     - System and Communications Protection
     - With the exception of the services needed for kickstart, most communications within SIMP are protected by SSH or SSL. Implementations can add additional services or modules that do not use SSH or SSL. The SIMP Security Concepts document details the default allowed protocols and the mechanisms in place to protect them. It's also worth noting that the SIMP team has taken ever measure possible to remove encryption ciphers available to operating system applications. In the event this breaks an application, implementations might have to add those ciphers back.
   * - SC-8(1)
     - Transmission Integrity (Control Enhancement)
     - System and Communications Protection
     - With the exception of the services needed for kickstart, most communications within SIMP are protected by SSH or SSL. Implementations can add additional services or modules that do not use SSH or SSL. The SIMP Security Concepts document details the default allowed protocols and the mechanisms in place to protect them. It's also worth noting that the SIMP team has taken ever measure possible to remove encryption ciphers available to operating system applications. In the event this breaks an application, implementations might have to add those ciphers back.
   * - SC-8(2)
     - Transmission Integrity (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-9
     - Transmission Confidentiality
     - System and Communications Protection
     - With the exception of the services needed for kickstart, most communications within SIMP are protected by SSH or SSL. Implementations can add additional services or modules that do not use SSH or SSL. The SIMP Security Concepts document details the default allowed protocols and the mechanisms in place to protect them. It's also worth noting that the SIMP team has taken ever measure possible to remove encryption ciphers available to operating system applications. In the event this breaks an application, implementations might have to add those ciphers back.
   * - SC-9(1)
     - Transmission Confidentiality (Control Enhancement)
     - System and Communications Protection
     - With the exception of the services needed for kickstart, most communications within SIMP are protected by SSH or SSL. Implementations can add additional services or modules that do not use SSH or SSL. The SIMP Security Concepts document details the default allowed protocols and the mechanisms in place to protect them. It's also worth noting that the SIMP team has taken ever measure possible to remove encryption ciphers available to operating system applications. In the event this breaks an application, implementations might have to add those ciphers back.
   * - SC-9(2)
     - Transmission Confidentiality (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-10
     - Network Disconnect
     - System and Communications Protection
     -
   * - SC-11
     - Trusted Path
     - System and Communications Protection
     -
   * - SC-12
     - Cryptographic Key Establishment and Management
     - System and Communications Protection
     - In an operational setting, SIMP does not establish keys. It does come with the ability to create server keys using a custom application know as “FakeCA”. SSH keys can also be established using standard Unix command line tools. In an operational settings, both sets of keys should be obtained from valid key infrastructures. There is also a CA that puppet uses to generate and manage keys for puppet only.
   * - SC-12(1)
     - Cryptographic Key Establishment and Management (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-12(2)
     - Cryptographic Key Establishment and Management (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-12(3)
     - Cryptographic Key Establishment and Management (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-12(4)
     - Cryptographic Key Establishment and Management (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-12(5)
     - Cryptographic Key Establishment and Management (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-13
     - Use of Cryptography
     -
     - The forms of cryptography used are applied through SSH, SSL, and TLS. RedHat FIPs mode enabling is on the near term horizon for SIMP. Once enabled, it will be documented here and should allow implemtations to further explain how this control is being met. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS). The Security Concepts docucment provides additional details on default services/protocols that are used.
   * - SC-13(1)
     - Use of Cryptography (Control Enhancement)
     -
     - The forms of cryptography used are applied through SSH, SSL, and TLS. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS) that are documented in the Security Concepts document.
   * - SC-13(2)
     - Use of Cryptography (Control Enhancement)
     -
     - The forms of cryptography used are applied through SSH, SSL, and TLS. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS) that are documented in the Security Concepts document.
   * - SC-13(3)
     - Use of Cryptography (Control Enhancement)
     -
     -
   * - SC-13(4)
     - Use of Cryptography (Control Enhancement)
     -
     -
   * - SC-14
     - Public Access Protections
     - System and Communications Protection
     -
   * - SC-15
     - Collaborative Computing Devices
     - System and Communications Protection
     -
   * - SC-15(1)
     - Collaborative Computing Devices (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-15(2)
     - Collaborative Computing Devices (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-15(3)
     - Collaborative Computing Devices (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-16
     - Transmission of Security Attributes
     - System and Communications Protection
     -
   * - SC-16(1)
     - Transmission of Security Attributes (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-17
     - Public Key Infrastructure Certificates
     - System and Communications Protection
     - In an operational setting, SIMP does not establish keys. It does come with the ability to create server keys using a custom application know as “FakeCA”. SSH keys can also be established using standard unix command line tools. In an operational settings, both sets of keys should be obtained from valid key infrastructures.There is also a CA that puppet uses to generate and manage keys for puppet only.
   * - SC-18
     - Mobile Code
     - System and Communications Protection
     -
   * - SC-18(1)
     - Mobile Code (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-18(2)
     - Mobile Code (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-18(3)
     - Mobile Code (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-18(4)
     - Mobile Code (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-19
     - Voice Over Internet Protocol
     - System and Communications Protection
     -
   * - SC-20
     - Secure Name /Address Resolution Service (Authoritative Source)
     - System and Communications Protection
     -
   * - SC-20(1)
     - Secure Name /Address Resolution Service (Authoritative Source) (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-21
     - Secure Name /Address Resolution Service (Recursive or Caching Resolver)
     - System and Communications Protection
     -
   * - SC-21(1)
     - Secure Name /Address Resolution Service (Recursive or Caching Resolver) (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-22
     - Architecture and Provisioning for Name/Address Resolution Service
     - System and Communications Protection
     -
   * - SC-23
     - Session Authenticity
     - System and Communications Protection
     - The forms of cryptography used are applied through SSH, SSL, and TLS. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS) that are documented in the Security Concepts document.
   * - SC-23(1)
     - Session Authenticity (Control Enhancement)
     - System and Communications Protection
     - The forms of cryptography used are applied through SSH, SSL, and TLS. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS) that are documented in the Security Concepts document.
   * - SC-23(2)
     - Session Authenticity (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-23(3)
     - Session Authenticity (Control Enhancement)
     - System and Communications Protection
     - The forms of cryptography used are applied through SSH, SSL, and TLS. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS) that are documented in the Security Concepts document.
   * - SC-23(4)
     - Session Authenticity (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-24
     - Fail in Known State
     - System and Communications Protection
     - The forms of cryptography used are applied through SSH, SSL, and TLS. There are several unencrypted protocols used on the puppet server (Apache/YUM, DHCPD, TFTP, and DNS) that are documented in the Security Concepts document.
   * - SC-25
     - Thin Nodes
     - System and Communications Protection
     -
   * - SC-26
     - Honeypots
     - System and Communications Protection
     -
   * - SC-26(1)
     - Honeypots (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-27
     - Operating System-Independent Applications
     - System and Communications Protection
     -
   * - SC-28
     - Protection of Information at Rest
     - System and Communications Protection
     - Confidentiality of data at rest is achieved using the operating system access control. Integrity is only checked for critical operating system files. Implementations have the ability to extend the integrity checking of AIDE to include additional files that are not frequently changed.
   * - SC-28
     - Protection of Information at Rest (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-29
     - Heterogeneity
     - System and Communications Protection
     -
   * - SC-30
     - Virtualization Techniques
     - System and Communications Protection
     -
   * - SC-30(1)
     - Virtualization Techniques (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-30(2)
     - Virtualization Techniques (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-31
     - Covert Channel Analysis
     - System and Communications Protection
     -
   * - SC-31(1)
     - Covert Channel Analysis (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-32
     - Information System Partitioning
     - System and Communications Protection
     -
   * - SC-33
     - Transmission Preparation Integrity
     - System and Communications Protection
     -
   * - SC-34
     - Non-modifiable Executable Programs
     - System and Communications Protection
     -
   * - SC-34(1)
     - Non-modifiable Executable Programs (Control Enhancement)
     - System and Communications Protection
     -
   * - SC-34(2)
     - Non-modifiable Executable Programs (Control Enhancement)
     - System and Communications Protection
     -

Table: SIMP SCTM

SIMP SCTM Operational Controls
------------------------------

.. csv-table::
  :header: Control ID,Control Name,Control Family,SIMP Implementation Method
  :widths: 15 18 17 50
  :file: Appendix_SCTM_op_ctrls.csv

Table: SIMP SCTM

SIMP SCTM Management Controls
-----------------------------

.. list-table::
   :widths: 15 18 17 50
   :header-rows: 1

   * - Control ID
     - Control Name
     - Control Family
     - SIMP Implementation Method
   * - AT-1
     - Security Awareness and Training Policy and Procedures
     - Awareness and Training
     -
   * - AT-2(1)
     - Security Awareness (Control Enhancement)
     - Awareness and Training
     -
   * - AT-3
     - Security Training
     - Awareness and Training
     -
   * - AT-3(1)
     - Security Training (Control Enhancement)
     - Awareness and Training
     -
   * - AT-3(2)
     - Security Training (Control Enhancement)
     - Awareness and Training
     -
   * - AT-4
     - Security Training Records
     - Awareness and Training
     -
   * - AT-5
     - Contacts with Security Groups and Associations
     - Awareness and Training
     -
   * - CM-1
     - Configuration Management Policy and Procedures
     - Configuration Management
     -
   * - CM-2
     - Baseline Configuration
     - Configuration Management
     - SIMP has strictly enforced version control during development. The baseline files for SIMP are kept and maintained in a git repository. Files are packaged and a series of auto tests are performed on each release. Once released, there is a version number associated for distribution. Additionally, custom puppet modules are in the form of RPMs and have version numbers associated with them. All documentation is also built with source code.
   * - CM-2(1)
     - Baseline Configuration (Control Enhancement)
     - Configuration Management
     -
   * - CM-2(2)
     - Baseline Configuration (Control Enhancement)
     - Configuration Management
     - SIMP has strictly enforced version control during development. The baseline files for SIMP are kept and maintained in a git repository. Files are packaged and a series of auto tests are performed on the release. Once released, there is a version number associated for distribution. All documentation is also built with source code.
   * - CM-2(3)
     - Baseline Configuration (Control Enhancement)
     - Configuration Management
     - All old versions of SIMP remain in the code repository.
   * - CM-2(4)
     - Baseline Configuration (Control Enhancement)
     - Configuration Management
     -
   * - CM-2(5)
     - Baseline Configuration (Control Enhancement)
     - Configuration Management
     - a. SIMP provides a minimal list of packages and services installed. The
          minimal list of packages can be found in kickstart files and the
          appendix of this document. Additional packages are installed by each
          implementation or as SIMP modules are applied.
       b. It's not feasible to technically deny additional applications from
          being installed. There is nothing in SIMP that can stop and RPM from
          being applied. Applications that require network access to service
          activation must be registered with puppet.
   * - CM-2(6)
     - Baseline Configuration (Control Enhancement)
     - Configuration Management
     - As a project, SIMP is developmental only. The environments where it is tested is up to the implementation. Development testing is performed on SIMP in environments that have a code base frozen.
   * - CM-3
     - Configuration Change Control
     - Configuration Management
     -
   * - CM-3(1)
     - Configuration Change Control (Control Enhancement)
     - Configuration Management
     -
   * - CM-3(2)
     - Configuration Change Control (Control Enhancement)
     - Configuration Management
     -
   * - CM-3(3)
     - Configuration Change Control (Control Enhancement)
     - Configuration Management
     - Configuration changes in SIMP are automated using a combination of puppet, yum, and rsync. While not all files on an operating system are managed by those mechanisms, many are. Changes to critical files that are managed by puppet, revert back to their original state. These mechanisms were not meant to defeat an attack by a malicious insider.
   * - CM-3(4)
     - Configuration Change Control (Control Enhancement)
     - Configuration Management
     -
   * - CM-4
     - Security Impact Analysis
     - Configuration Management
     - All features or bugs in SIMP are vetted through the development process by being placed on the product backlog and discussed with the entire team. There is a security representative on the SIMP team that is part of that vetting process.
   * - CM-4(1)
     - Security Impact Analysis (Control Enhancement)
     - Configuration Management
     -
   * - CM-4(2)
     - Security Impact Analysis (Control Enhancement)
     - Configuration Management
     -
   * - CM-5
     - Access Restrictions for Change
     - Configuration Management
     - SIMP can only meet the enforcement part of this control. The remainder must be met by the environment that SIMP is implemented in. Changes to a SIMP based systems are enforced with built in Unix/LDAP groups. Only someone with sudo or sudosh access (usually an admin group) can apply changes to the environment
   * - CM-5(1)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     - SIMP can only meet the enforcement part of this control. The remainder must be met by the environment that SIMP is implemented in. Changes to a SIMP based systems are enforced with built in Unix/LDAP groups. Only someone with sudo or sudosh access (usually an admin group) can apply changes to the environment
   * - CM-5(2)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     -
   * - CM-5(3)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     - Redhat and Centos packages are signed with gpg keys. Those keys are vendor specific. Package installation occurs only when those gpgkeys are validate using the installed gpg public keys for the operating system. SIMP specific RPMS that were developed are signed using keys generate by the development team.
   * - CM-5(4)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     -
   * - CM-5(5)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     -
   * - CM-5(6)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     -
   * - CM-5(7)
     - Access Restrictions for Change (Control Enhancement)
     - Configuration Management
     - Most of the critical files that are managed by puppet cannot be permanently changed on a puppet client without disabling puppet and rsync. If they are changed, puppet will revert them back to their original state.
   * - CM-6
     - Configuration Settings
     - Configuration Management
     - Part “d” of this control is met my SIMP. The others are not. SIMP uses puppet to monitor changes to configuration settings. If changes to puppet controlled settings are manually made, they revert back to their original state.
   * - CM-6(1)
     - Configuration Settings (Control Enhancement)
     - Configuration Management
     - The puppet master is the central point of management for a SIMP system. While not required, the puppet master usually hosts a kickstart server so that clients are built the same every time.
   * - CM-6(2)
     - Configuration Settings (Control Enhancement)
     - Configuration Management
     - Puppet is not intended to be a security mechanism to prevent unauthorized changes to files. For files that are managed by puppet that changed, they will revert back to their original state. This control is really about protecting from unauthorized changes so access control to the puppet master should suffice to meet it. Changes to files are audited using auditd. Puppet changes are also audited. It's up to the implementation to perform altering on those changes.
   * - CM-6(3)
     - Configuration Settings (Control Enhancement)
     - Configuration Management
     - This control is not fully met by SIMP. It's important to point out that SIMP does provide logging of events to syslog. It's currently up to the implementation to alert on those events.
   * - CM-7
     - Least Functionality
     - Configuration Management
     - There isn't an explicit list of services that SIMP denies. Instead, it was built to provide only the essential functionality. Additional services get added only as needed.
   * - CM-7(1)
     - Least Functionality (Control Enhancement)
     - Configuration Management
     -
   * - CM-7(2)
     - Least Functionality (Control Enhancement)
     - Configuration Management
     - Applications can be installed, but new services will not run unless first registered with puppet. Additionally, puppet modules must be modified to ensure that IPtables opens up the necessary services. Minimally, for a service to remain active, it must be registered with puppet or the svckill.rb script will stop them.To be clear, there is nothing in SIMP that prevents the installation of RPMs (from the command line or YUM).
   * - CM-7(3)
     - Least Functionality (Control Enhancement)
     - Configuration Management
     - The registration process for ports, protocols, and services are handled via puppet.
   * - CM-8
     - Information System Component Inventory
     - Configuration Management
     -
   * - CM-8(1)
     - Information System Component Inventory (Control Enhancement)
     - Configuration Management
     -
   * - CM-8(2)
     - Information System Component Inventory (Control Enhancement)
     - Configuration Management
     - To the extent possible, puppet tracks clients that are within its control. It's not meant to be a true inventory mechanism.
   * - CM-8(3)
     - Information System Component Inventory (Control Enhancement)
     - Configuration Management
     -
   * - CM-8(4)
     - Information System Component Inventory (Control Enhancement)
     - Configuration Management
     -
   * - CM-8(5)
     - Information System Component Inventory (Control Enhancement)
     - Configuration Management
     -
   * - CM-8(6)
     - Information System Component Inventory (Control Enhancement)
     - Configuration Management
     -
   * - CM-9
     - Configuration Management Plan
     - Configuration Management
     -
   * - CM-9(1)
     - Configuration Management Plan (Control Enhancement)
     - Configuration Management
     -
   * - CP-1
     - Contingency Planning Policy and Procedures
     - Contingency Planning
     -
   * - CP-2
     - Contingency Plan
     - Contingency Planning
     -
   * - CP-2(1)
     - Contingency Plan (Control Enhancement)
     - Contingency Planning
     -
   * - CP-2(2)
     - Contingency Plan (Control Enhancement)
     - Contingency Planning
     -
   * - CP-2(3)
     - Contingency Plan (Control Enhancement)
     - Contingency Planning
     -
   * - CP-2(4)
     - Contingency Plan (Control Enhancement)
     - Contingency Planning
     -
   * - CP-2(5)
     - Contingency Plan (Control Enhancement)
     - Contingency Planning
     -
   * - CP-2(6)
     - Contingency Plan (Control Enhancement)
     - Contingency Planning
     -
   * - CP-3
     - Contingency Training
     - Contingency Planning
     -
   * - CP-3(1)
     - Contingency Training (Control Enhancement)
     - Contingency Planning
     -
   * - CP-3(2)
     - Contingency Training (Control Enhancement)
     - Contingency Planning
     -
   * - CP-4
     - Contingency Plan Testing and Exercises
     - Contingency Planning
     -
   * - CP-4(1)
     - Contingency Plan Testing and Exercises (Control Enhancement)
     - Contingency Planning
     -
   * - CP-4(2)
     - Contingency Plan Testing and Exercises (Control Enhancement)
     - Contingency Planning
     -
   * - CP-4(3)
     - Contingency Plan Testing and Exercises (Control Enhancement)
     - Contingency Planning
     -
   * - CP-6
     - Alternate Storage Site
     - Contingency Planning
     -
   * - CP-6(1)
     - Alternate Storage Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-6(2)
     - Alternate Storage Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-6(3)
     - Alternate Storage Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-7
     - Alternate Processing Site
     - Contingency Planning
     -
   * - CP-7(1)
     - Alternate Processing Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-7(2)
     - Alternate Processing Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-7(3)
     - Alternate Processing Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-7(4)
     - Alternate Processing Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-7(5)
     - Alternate Processing Site (Control Enhancement)
     - Contingency Planning
     -
   * - CP-8
     - Telecommunications Services
     - Contingency Planning
     -
   * - CP-8(1)
     - Telecommunications Services (Control Enhancement)
     - Contingency Planning
     -
   * - CP-8(2)
     - Telecommunications Services (Control Enhancement)
     - Contingency Planning
     -
   * - CP-8(3)
     - Telecommunications Services (Control Enhancement)
     - Contingency Planning
     -
   * - CP-8(4)
     - Telecommunications Services (Control Enhancement)
     - Contingency Planning
     -
   * - CP-9
     - Information System Backup
     - Contingency Planning
     - The BackupPC module is not currently available in SIMP 5.0.
   * - CP-9(1)
     - Information System Backup (Control Enhancement)
     - Contingency Planning
     -
   * - CP-9(2)
     - Information System Backup (Control Enhancement)
     - Contingency Planning
     -
   * - CP-9(3)
     - Information System Backup (Control Enhancement)
     - Contingency Planning
     -
   * - CP-9(5)
     - Information System Backup (Control Enhancement)
     - Contingency Planning
     -
   * - CP-9(6)
     - Information System Backup (Control Enhancement)
     - Contingency Planning
     -
   * - CP-10
     - Information System Recovery and Reconstitution
     - Contingency Planning
     - The BackupPC module is not currently available in SIMP 5.0.
   * - CP-10(1)
     - Information System Recovery and Reconstitution (Control Enhancement)
     - Contingency Planning
     -
   * - CP-10(2)
     - Information System Recovery and Reconstitution (Control Enhancement)
     - Contingency Planning
     -
   * - CP-10(3)
     - Information System Recovery and Reconstitution (Control Enhancement)
     - Contingency Planning
     -
   * - CP-10(4)
     - Information System Recovery and Reconstitution (Control Enhancement)
     - Contingency Planning
     -
   * - CP-10(5)
     - Information System Recovery and Reconstitution (Control Enhancement)
     - Contingency Planning
     -
   * - CP-10(6)
     - Information System Recovery and Reconstitution (Control Enhancement)
     - Contingency Planning
     -
   * - IR-1
     - Incident Response Policy and Procedures
     - Incident Response
     -
   * - IR-2
     - Incident Response Training
     - Incident Response
     -
   * - IR-2(1)
     - Incident Response Training (Control Enhancement)
     - Incident Response
     -
   * - IR-2(2)
     - Incident Response Training (Control Enhancement)
     - Incident Response
     -
   * - IR-3
     - Incident Response Testing and Exercises
     - Incident Response
     -
   * - IR-3(1)
     - Incident Response Testing and Exercises (Control Enhancement)
     - Incident Response
     -
   * - IR-4
     - Incident Handling
     - Incident Response
     -
   * - IR-4(1)
     - Incident Handling (Control Enhancement)
     - Incident Response
     -
   * - IR-4(2)
     - Incident Handling (Control Enhancement)
     - Incident Response
     - If an implementation chooses, they can leverage puppet's ability to reconfigure systems as part of incident response. While puppet is not intended to be a security product, its features can help provide security functionality such as dynamic reconfigurations.
   * - IR-4(3)
     - Incident Handling (Control Enhancement)
     - Incident Response
     -
   * - IR-4(4)
     - Incident Handling (Control Enhancement)
     - Incident Response
     -
   * - IR-4(5)
     - Incident Handling (Control Enhancement)
     - Incident Response
     -
   * - IR-5
     - Incident Monitoring
     - Incident Response
     -
   * - IR-5(1)
     - Incident Monitoring (Control Enhancement)
     - Incident Response
     -
   * - IR-6
     - Incident Reporting
     - Incident Response
     -
   * - IR-6(1)
     - Incident Reporting (Control Enhancement)
     - Incident Response
     -
   * - IR-6(2)
     - Incident Reporting (Control Enhancement)
     - Incident Response
     -
   * - IR-7
     - Incident Response Assistance
     - Incident Response
     -
   * - IR-7(1)
     - Incident Response Assistance (Control Enhancement)
     - Incident Response
     -
   * - IR-8
     - Incident Response Plan
     - Incident Response
     -
   * - MA-1
     - System Maintenance Policy and Procedures
     - Maintenance
     -
   * - MA-2
     - Controlled Maintenance
     - Maintenance
     -
   * - MA-2(1)
     - Controlled Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-2(2)
     - Controlled Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-3
     - Maintenance Tools
     - Maintenance
     -
   * - MA-3(1)
     - Maintenance Tools (Control Enhancement)
     - Maintenance
     -
   * - MA-3(2)
     - Maintenance Tools (Control Enhancement)
     - Maintenance
     -
   * - MA-3(3)
     - Maintenance Tools (Control Enhancement)
     - Maintenance
     -
   * - MA-3(4)
     - Maintenance Tools (Control Enhancement)
     - Maintenance
     -
   * - MA-4
     - Non-Local Maintenance
     - Maintenance
     - Remote maintenance can be performed on SIMP using SSH or direct console access. SSH sessions are tracked and logged using the security features built into SIMP. Console access requires someone to have access to the physical (or virtual) console along with the root password. Auditing of those actions also occurs in accordance with the configured audit policy. It's up to the implementation to decide how to distribute authentication information for remote maintenance.
   * - MA-4(1)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     - Remote maintenance can be performed on SIMP using SSH or direct console access. SSH sessions are tracked and logged using the security features built into SIMP. Console access requires someone to have access to the physical (or virtual) console along with the root password. Auditing of those actions also occurs in accordance with the configured audit policy. It's up to the implementation to decide how to distribute authentication information for remote maintenance
   * - MA-4(2)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-4(3)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-4(4)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-4(5)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-4(6)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     - Remote maintenance is performed using SSH. SSH inherently provides confidentiality and integrity of data while in transit.
   * - MA-4(7)
     - Non-Local Maintenance (Control Enhancement)
     - Maintenance
     -
   * - MA-5
     - Maintenance Personnel
     - Maintenance
     -
   * - MA-5(1)
     - Maintenance Personnel (Control Enhancement)
     - Maintenance
     -
   * - MA-5(2)
     - Maintenance Personnel (Control Enhancement)
     - Maintenance
     -
   * - MA-5(3)
     - Maintenance Personnel (Control Enhancement)
     - Maintenance
     -
   * - MA-5(4)
     - Maintenance Personnel (Control Enhancement)
     - Maintenance
     -
   * - MA-6
     - Timely Maintenance
     - Maintenance
     -
   * - MP-1
     - Media Protection Policy and Procedures
     - Media Protection
     -
   * - MP-2
     - Media Access
     - Media Protection
     -
   * - MP-2(1)
     - Media Access (Control Enhancement)
     - Media Protection
     -
   * - MP-2(2)
     - Media Access (Control Enhancement)
     - Media Protection
     -
   * - MP-4
     - Media Storage
     - Media Protection
     -
   * - MP-5
     - Media Transport
     - Media Protection
     -
   * - MP-5(1)
     - Media Transport (Control Enhancement)
     - Media Protection
     -
   * - MP-5(2)
     - Media Transport (Control Enhancement)
     - Media Protection
     -
   * - MP-5(3)
     - Media Transport (Control Enhancement)
     - Media Protection
     -
   * - MP-5(4)
     - Media Transport (Control Enhancement)
     - Media Protection
     -
   * - MP-6
     - Media Sanitization
     - Media Protection
     -
   * - MP-6(1)
     - Media Sanitization (Control Enhancement)
     - Media Protection
     -
   * - MP-6(2)
     - Media Sanitization (Control Enhancement)
     - Media Protection
     -
   * - MP-6(3)
     - Media Sanitization (Control Enhancement)
     - Media Protection
     -
   * - MP-6(4)
     - Media Sanitization (Control Enhancement)
     - Media Protection
     -
   * - MP-6(5)
     - Media Sanitization (Control Enhancement)
     - Media Protection
     -
   * - MP-6(6)
     - Media Sanitization (Control Enhancement)
     - Media Protection
     -
   * - PE-1
     - Physical and Environmental Protection Policy and Procedures
     - Physical and Environmental Protection
     -
   * - PE-2
     - Physical Access Authorizations
     - Physical and Environmental Protection
     -
   * - PE-2(1)
     - Physical Access Authorizations (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-2(2)
     - Physical Access Authorizations (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-2(3)
     - Physical Access Authorizations (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-3
     - Physical Access Control
     - Physical and Environmental Protection
     -
   * - PE-3(1)
     - Physical Access Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-3(2)
     - Physical Access Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-3(3)
     - Physical Access Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-3(4)
     - Physical Access Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-3(5)
     - Physical Access Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-3(6)
     - Physical Access Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-4
     - Access Control for Transmission Medium
     - Physical and Environmental Protection
     -
   * - PE-5
     - Access Control for Output Devices
     - Physical and Environmental Protection
     -
   * - PE-6
     - Monitoring Physical Access
     - Physical and Environmental Protection
     -
   * - PE-6(1)
     - Monitoring Physical Access (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-6(2)
     - Monitoring Physical Access (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-7
     - Visitor Control
     - Physical and Environmental Protection
     -
   * - PE-7(1)
     - Visitor Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-7(2)
     - Visitor Control (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-8
     - Access Records
     - Physical and Environmental Protection
     -
   * - PE-8(1)
     - Access Records (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-8(2)
     - Access Records (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-9
     - Power Equipment and Power Cabling
     - Physical and Environmental Protection
     -
   * - PE-9(1)
     - Power Equipment and Power Cabling (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-9(2)
     - Power Equipment and Power Cabling (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-10
     - Emergency Shutoff
     - Physical and Environmental Protection
     -
   * - PE-10(1)
     - Emergency Shutoff (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-11
     - Emergence Power
     - Physical and Environmental Protection
     -
   * - PE-11(1)
     - Emergence Power (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-11(2)
     - Emergence Power (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-12
     - Emergency Lighting
     - Physical and Environmental Protection
     -
   * - PE-12(1)
     - Emergency Lighting (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-13
     - Fire Protection
     - Physical and Environmental Protection
     -
   * - PE-13(1)
     - Fire Protection (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-13(2)
     - Fire Protection (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-13(3)
     - Fire Protection (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-13(4)
     - Fire Protection (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-14
     - Temperature and Humidity Controls
     - Physical and Environmental Protection
     -
   * - PE-14(1)
     - Temperature and Humidity Controls (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-14(2)
     - Temperature and Humidity Controls (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-15
     - Water Damage Protection
     - Physical and Environmental Protection
     -
   * - PE-15(1)
     - Water Damage Protection (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-16
     - Delivery and Removal
     - Physical and Environmental Protection
     -
   * - PE-17
     - Alternate Work Site
     - Physical and Environmental Protection
     -
   * - PE-18
     - Location of Information System Components
     - Physical and Environmental Protection
     -
   * - PE-18(1)
     - Location of Information System Components (Control Enhancement)
     - Physical and Environmental Protection
     -
   * - PE-19
     - Information Leakage
     - Physical and Environmental Protection
     -
   * - SI-1
     - System and Information Integrity Policy and Procedures
     - System and Information Integrity
     -
   * - SI-2(1)
     - Flaw Remediation (Control Enhancement)
     - System and Information Integrity
     - Patches that are part of the software base for SIMP are tested within the development environment. There is automated testing that is constantly being extended to test more features. There are times that patches to the base operating system (Centos or RedHat) are needed to resolve issues in SIMP. Those are also tested at build time, but require additional testing by implementations as patches are released from vendors. It's also important to note that SIMP is packaged and delivered decoupled with the operating system source files. It's up to the implementation to test vendor specific patches that are not part of the SIMP code base. Flaws are tracked using the software project management tool Redmine.
   * - SI-2(2)
     - Flaw Remediation (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-2(3)
     - Flaw Remediation (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-2(4)
     - Flaw Remediation (Control Enhancement)
     - System and Information Integrity
     - SIMP uses the yellowdog update manager (YUM) to deliver software patches to clients. Each installation usually has at least one YUM repository. There is also a cronjob running that runs once per day. It's the responsibility of the implementation to get patches to the yum server. Once they are there, the cron job will perform a yum update and the patches will be applied.
   * - SI-3
     - Malicious Code Protection
     - System and Information Integrity
     - SIMP has modules available for mcafee and ClamAV. The ClamAV. Implementations need need to provide their own version of the mcafee software for the module to work. That module comes with the ability to sync dat updates to clients via rsync. The module does NOT specify how often and what files systems should be scanned. SIMP also implements the open source tool chkrootkit that comes installed by default.
   * - SI-3(1)
     - Malicious Code Protection (Control Enhancement)
     - System and Information Integrity
     - The provided anti-virus modules are installed via puppet modules. Those modules include the ability to sycn data file updates via rsync. Therefore, all management of malicious code detection is done centrally.
   * - SI-3(2)
     - Malicious Code Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-3(3)
     - Malicious Code Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-3(4)
     - Malicious Code Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-3(5)
     - Malicious Code Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-3(6)
     - Malicious Code Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4
     - Information System Monitoring Tools and Techniques
     - System and Information Integrity
     -
   * - SI-4(1)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(2)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(3)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(4)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(5)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(6)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(7)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(8)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(9)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(10)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(11)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(12)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(13)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(14)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(15)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(16)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-4(17)
     - Information System Monitoring Tools and Techniques (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-5
     - System Alerts, Advisories, and Directives
     - System and Information Integrity
     - The only part of the control (a) that is met by SIMP, is the tracking of security alerts for products that are part of the code base. The development team subscribes to message boards for the main products (puppet) that are part of the packaging. RedHat/Centos advisories are also tracked out of necessity but since ALL the OS files are not part of SIMP delivery, patches are not our direct responsibility.
   * - SI-5(1)
     - System Alerts, Advisories, and Directives (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-6
     - Security Functionality Verification
     - System and Information Integrity
     - SIMP comes with an optional module to install and perform regular runs of the SCAP-Security-Guide (the checks for RHEL 7 are not yet complete/finalized). Doing so will report (for a user defined frequency) OVAL results of security settings of a host against SSG recommendations.
   * - SI-6(1)
     - Security Functionality Verification (Control Enhancement)
     - System and Information Integrity
     - SIMP comes with an optional module to install and perform regular runs of the SCAP-Security-Guide. Doing so will report (for a user defined frequency) OVAL results of security settings of a host against SSG recommendations.
   * - SI-6(2)
     - Security Functionality Verification (Control Enhancement)
     - System and Information Integrity
     - SIMP comes with an optional module to install and perform regular runs of the SCAP-Security-Guide. Doing so will report (for a user defined frequency) OVAL results of security settings of a host against SSG recommendations.
   * - SI-6(3)
     - Security Functionality Verification (Control Enhancement)
     - System and Information Integrity
     - SIMP comes with an optional module to install and perform regular runs of the SCAP-Security-Guide. Doing so will report (for a user defined frequency) OVAL results of security settings of a host against SSG recommendations.
   * - SI-7
     - Software and Information Integrity
     - System and Information Integrity
     - SIMP comes with AIDE installed. Puppet also serves the purpose of checking the integrity of files. During each client run, a change in file integrity means the file needs to be restored to its original state.
   * - SI-7(1)
     - Software and Information Integrity (Control Enhancement)
     - System and Information Integrity
     - AIDE baselines are not performed beyond initial install unless otherwise configured. Implementations can re-baseline the database.
   * - SI-7(2)
     - Software and Information Integrity (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-7(3)
     - Software and Information Integrity (Control Enhancement)
     - System and Information Integrity
     - AIDE is managed by puppet and is therefore centrally managed.
   * - SI-7(4)
     - Software and Information Integrity (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-8
     - Spam Protection
     - System and Information Integrity
     -
   * - SI-8(1)
     - Spam Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-8(2)
     - Spam Protection (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-9
     - Information Input Restrictions
     - System and Information Integrity
     -
   * - SI-10
     - Information Input Validation
     - System and Information Integrity
     -
   * - SI-11
     - Error Handling
     - System and Information Integrity
     -
   * - SI-13
     - Predictable Failure Prevention
     - System and Information Integrity
     -
   * - SI-13(1)
     - Predictable Failure Prevention (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-13(2)
     - Predictable Failure Prevention (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-13(3)
     - Predictable Failure Prevention (Control Enhancement)
     - System and Information Integrity
     -
   * - SI-13(4)
     - Predictable Failure Prevention (Control Enhancement)
     - System and Information Integrity
     -
   * - Control ID
     - Control Name
     - Control Family
     - SIMP Implementation Method
   * - Control ID
     - Control Name
     - Control Family
     - SIMP Implementation Method
   * - CA-1
     - Security Assessment and Authorization Policies
     - Security Assessment and Authorization
     -
   * - CA-2
     - Security Assessments
     - Security Assessment and Authorization
     -
   * - CA-2(1)
     - Security Assessments (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - CA-2(2)
     - Security Assessments (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - CA-3
     - Information System Connections
     - Security Assessment and Authorization
     -
   * - CA-3(1)
     - Information System Connections (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - CA-3(2)
     - Information System Connections (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - CA-5
     - Plan of Action and Milestones
     - Security Assessment and Authorization
     -
   * - CA-5(1)
     - Plan of Action and Milestones (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - CA-6
     - Security Authorization
     - Security Assessment and Authorization
     -
   * - CA-7
     - Continuous Monitoring
     - Security Assessment and Authorization
     -
   * - CA-7(1)
     - Continuous Monitoring (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - CA-7(2)
     - Continuous Monitoring (Control Enhancement)
     - Security Assessment and Authorization
     -
   * - Pl-1
     - Security Planning Policy and Procedures
     - Planning
     - The SIMP installation manual provides instructions for the installation of the product in a manner that is compliant with a multitude of security controls.
   * - PL-2
     - System Security Plan
     - Planning
     - Security Plans are provided for specific implementations. The SIMP team will continue to develop security documentation that can be used as s resource for implementation specific System Security Plans.
   * - PL-2(1)
     - System Security Plan (Control Enhancement)
     - Planning
     - TODO: Develop SIMP specific SSP.
   * - PL-2(2)
     - System Security Plan (Control Enhancement)
     - Planning
     -
   * - PL-4
     - Rules of Behavior
     - Planning
     -
   * - PL-4(1)
     - Rules of Behavior (Control Enhancement)
     - Planning
     -
   * - PL-5
     - Privacy Impact Assessment
     - Planning
     -
   * - PL-6
     - Security-Related Activity Planning
     - Planning
     -
   * - PS-1
     - Personnel Security Policy and Procedures
     - Planning
     -
   * - PS-2
     - Position Categorization
     - Planning
     -
   * - PS-3(2)
     - Personnel Screening (Control Enhancement)
     - Planning
     -
   * - RA-1
     - Risk Assessment Policy and Procedures
     - Risk Assessment
     -
   * - RA-2
     - Security Categorization
     - Risk Assessment
     -
   * - RA-3
     - Risk Assessment
     - Risk Assessment
     -
   * - RA-5
     - Vulnerability Scanning
     - Risk Assessment
     - The SIMP team performs a variety of security testing as part of the development process. Compliance and configuration checking is done using SSG. SIMP makes every effort to address problems discovered by these tools. Some configuration settings will not align with tools since the product was meant to be used for operational settings where some security features cause a loss in functionality. Implementations have the option of further hardening their system further at the risk of losing some functionality.
   * - RA-5(1)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     - SCAP-Security-Guide is the primary tool used to check for suspected configuration errors. Puppet also continues to protect clients against unwanted changes.
   * - RA-5(2)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     - SCAP-Security-Guide is the primary tool used to check for suspected configuration errors. Puppet also continues to protect clients against unwanted changes.
   * - RA-5(3)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     - Regular vulnerability scanning is performed during development of SIMP.
   * - RA-5(4)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     - Part of the vulnerability scanning process determines what information can be determined by a malicious outside user.
   * - RA-5(5)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     - The compliance tools require that privileged accounts be used to perform testing.
   * - RA-5(6)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     -
   * - RA-5(7)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     - Only part of this requirement is met. SIMP can detect when any software is installed via auditd and syslog. Services that are not registered with puppet will not operate without user intervention. Those changes are also audited. SIMP does not provide the ability to alert on those actions, however, Logstash filters or Elasticsearch queries can be applied if needed.
   * - RA-5(8)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     -
   * - RA-5(9)
     - Vulnerability Scanning (Control Enhancement)
     - Risk Assessment
     -
   * - SA-1
     - System and Services Acquisition Policy and Procedures
     - System and Service Acquisition
     -
   * - SA-2
     - Allocation of Resources
     - System and Service Acquisition
     -
   * - SA-3
     - Life Cycle Support
     - System and Service Acquisition
     -
   * - SA-4
     - Acquisitions
     - System and Service Acquisition
     -
   * - SA-4(1)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-4(2)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-4(3)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-4(4)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-4(5)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-4(6)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-4(7)
     - Acquisitions (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-5
     - Information System Documentation
     - System and Service Acquisition
     -
   * - SA-5(1)
     - Information System Documentation (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-5(2)
     - Information System Documentation (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-5(3)
     - Information System Documentation (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-5(4)
     - Information System Documentation (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-5(5)
     - Information System Documentation (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-6
     - Software Usage Restrictions
     - System and Service Acquisition
     -
   * - SA-6 (1)
     - Software Usage Restrictions
     - System and Service Acquisition
     -
   * - SA-7
     - User Installed Software
     - System and Service Acquisition
     -
   * - SA-8
     - Security Engineering Principles
     - System and Service Acquisition
     -
   * - SA-9
     - External Information System Services
     - System and Service Acquisition
     -
   * - SA-9(1)
     - External Information System Services (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-10
     - Developer Configuration Management
     - System and Service Acquisition
     -
   * - SA-10(1)
     - Developer Configuration Management (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-10(2)
     - Developer Configuration Management (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-11
     - Developer Security Testing
     - System and Service Acquisition
     -
   * - SA-11(1)
     - Developer Security Testing (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-11(2)
     - Developer Security Testing (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-11(3)
     - Developer Security Testing (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12
     - Supply Chain Protection
     - System and Service Acquisition
     -
   * - SA-12(1)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12(2)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12(3)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12(4)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12(5)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12(6)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-12(7)
     - Supply Chain Protection (Control Enhancement)
     - System and Service Acquisition
     -
   * - SA-13
     - Trustworthiness
     - System and Service Acquisition
     -
   * - SA-14
     - Critical Information System Components
     - System and Service Acquisition
     -
   * - SA-14(1)
     - Critical Information System Components (Control Enhancement)
     - System and Service Acquisition
     -

Table: Management Controls

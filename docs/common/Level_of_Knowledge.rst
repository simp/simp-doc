SIMP is designed for use by system administrators/users with a strong
background in Linux operating systems. The core technologies that require
prerequisite knowledge are:

* :term:`Puppet` - 5.5 or later
* :term:`Domain Name System` (DNS) - BIND 9
* :term:`Dynamic Host Configuration Protocol` (DHCP) - Internet Systems
  Consortium (ISC) DHCP
* :term:`Lightweight Directory Access Protocol` (LDAP) - :term:`OpenLDAP` for EL7 or :term:`389-DS` for EL8
* RedHat :term:`Kickstart`, including all technologies involved:
  :term:`Trivial File Transfer Protocol` (TFTP), :term:`PXE`, PXELinux, etc.
* The Apache HTTP Server
* The :term:`Yellowdog Updater, Modified` (YUM) package manager
* :term:`Rsyslog` 8+
* :term:`IPTables` (Internet Protocol Tables)/Firewalld, basic knowledge of
  the rules
* :term:`Auditd`, Basic knowledge of how the daemon works
* :term:`Advanced Intrusion Detection Environment` (AIDE), basic knowledge of
  the rules
* Basic :term:`X.509`-based :term:`PKI` Key Management

SIMP handles as much of the initial setup and management of these tools as
possible  However, you will need at least some understanding of them in order
to tailor a SIMP system to fit the desired environment. You will also need
a general understanding of how to control and manipulate these tools from the
:term:`command line interface` (CLI); SIMP does not provide a :term:`graphical
user interface` (GUI).

Knowledge of scripting and :term:`Ruby` programming will also help to further
customize a SIMP install but is not required for routine use.

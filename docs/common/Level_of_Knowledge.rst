SIMP is designed for use by system administrators or users with a strong
background using Linux operating systems. The core applications that
make up SIMP and require prerequisite knowledge are:

- :term:`Puppet` - 3.7 or later

- :term:`Domain Name System` (DNS) - BIND 9

- :term:`Dynamic Host Configuration Protocol` (DHCP) - Internet Systems Consortium (ISC) DHCP

- :term:`Lightweight Directory Access Protocol` (LDAP) - OpenLDAP

-  RedHat Kickstart (including all tools behind it) - :term:`Trivial File Transfer Protocol` (TFTP), PXELinux, etc.

-  Apache

-  :term:`Yellowdog Updater, Modified` (YUM)

-  Rsyslog Version 3+

-  :term:`Internet Protocol Tables` (IPtables) (Basic knowledge of the rules)

-  :term:`Auditd` (Basic knowledge of how the daemon works)

-  :term:`Advanced Intrusion Detection Environment` (AIDE) (Basic knowledge of the rules)

-  Basic :term:`X.509`-based :term:`PKI` Key Management

SIMP does as much initial setup and configuration of these tools as possible.
However, without at least some understanding, you will be unable to tailor a
SIMP system to fit the desired environment. A general understanding of how to
control and manipulate these tools from the :term:`command line interface`
(CLI) will be necessary, as SIMP does not come stock with a :term:`graphical user interface`
(GUI).

Knowledge of scripting and :term:`Ruby` programming will also help to further
customize a SIMP install but is not required for routine use.

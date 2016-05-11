Identification and Authentication (Organizational Users)
---------------------------------------------------------

The pam_ldap module ensures that the username is mapped to the uid portion of
the DN in LDAP.

The pam_ldap module is configured to tell the clients to ignore the following
user names, forcing them to be authenticated locally:

- root
- bin
- daemon
- adm
- lp
- mail
- operator
- nobody
- dbus
- ntp
- saslauth
- postfix
- sshd
- puppet
- stunnel
- nscd
- haldaemon
- clamav
- rpcuser
- rpc
- clam
- nfsnobody
- rpm
- nslcd
- avahi
- gdm
- rtkit
- pulse
- hsqldb
- radvd
- apache
- tomcat

There as an ldap account created for LDAP administration.  The username for that
account is ``LDAPAdmin``.

References: :ref:`IA-2`

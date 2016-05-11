Least Privilege
---------------

The OpenLDAP service runs under the ``ldap`` user and ``ldap`` group.  This is allows
directory permissions to limit the service's access to files/directories not
owned by the ``ldap`` user/group.  The ldap user does not have a valid login
shell.

The default LDAP server policy denies all users access to everything (default
deny).  Access to LDAP entries are explicitly added.

References: :ref:`AC-6`

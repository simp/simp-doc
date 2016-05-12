Access Enforcement
------------------

User password history (shadowLastChange) is written to the LDAP server. For this
to happen, the user is given write access to their own shadowLastChange entry in
LDAP.

References: :ref:`AC-3`

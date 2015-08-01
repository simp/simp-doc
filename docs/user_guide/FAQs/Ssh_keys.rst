SSH Keys in LDAP
================

This section provides guidance on managing SSH keys within the SIMP
environment.

LDAP Enabled
------------

When enabled, ssh keys are both stored and retrieved directly from LDAP.

See the ? chapter for more information on user management in LDAP.

Without LDAP
------------

If LDAP is not being used, use the */etc/ssh/local\_keys* directory for
all user keys.

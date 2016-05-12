Identification and Authentication
---------------------------------

SIMP uses the SSSD client to authenticate with the SIMP LDAP server.  The SSSD
client is configured to:

- Use LDAP
- Use autofs
- Use sudo
- Use SSH
- Enforce a minimum user ID of 500

References: :ref:`IA-2`

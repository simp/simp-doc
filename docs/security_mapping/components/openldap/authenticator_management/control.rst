Authenticator Management
------------------------

Authenticator strength is enforced using slapo-ppolicy overlay for LDAP. The
ppolicy overlay is then configured to use PAM cracklib to enforce complexity.
The SIMP cracklib settings ensure that passwords:

- Have at least four characters that are different from the previous password
- Do not repeat a character more than two times in a row
- Do not have the username (forward or reversed) in the password
- Have at lease one character from each class (upper, lower, number, special character)
- Have at least 14 characters
- Are not the same as any of the previous 24 passwords

The integration point between the remote LDAP server and PAM is the pam_ldap pam
module.  SIMP configures pam_ldap to point to the SIMP LDAP server and
communicates using TLS.

References: :ref:`IA-5 (1)(a)`, :ref:`IA-5 (1)(e)`

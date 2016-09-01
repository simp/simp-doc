Authenticator Management
------------------------

Authenticator strength is enforced using slapo-ppolicy overlay for LDAP. The
ppolicy overlay is then configured to use PAM cracklib to enforce complexity.

For the default password complexity rules see the
:ref:`faq-password-complexity` FAQ.

The integration point between the remote LDAP server and PAM is the pam_ldap pam
module. SIMP configures pam_ldap to point to the SIMP LDAP server and
communicates using TLS.

References: :ref:`IA-5 (1)(a)`, :ref:`IA-5 (1)(e)`

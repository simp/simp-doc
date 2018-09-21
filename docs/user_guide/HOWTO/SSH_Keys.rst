HOWTO Set up SSH Authorized Keys
================================

This section provides guidance on managing SSH keys within the SIMP
environment.

LDAP Enabled
------------

When enabled, ssh keys are both stored and retrieved directly from LDAP.

See Also: :ref:`Managing Users with LDAP <Managing LDAP Users>`

Without LDAP
------------

If not using LDAP, or in addition to LDAP, SSH authorized keys can be placed in
``/etc/ssh/local_keys/<USERNAME>``. This location can be changed by setting the
``ssh::server::conf::authorizedkeysfile`` parameter in :term:`Hiera` or your
:term:`ENC`.

See Also: :ref:`Managing Local/Service Users <local_user_management>`

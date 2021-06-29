.. _ug-user_management-ldap-openldap-manage_users:

Add a User to OpenLDAP
======================

.. include:: ldif_prep_header.inc

.. contents::
   :local:

Users can be added with or without a password. Follow the instructions
in the following sections.

.. NOTE::

   Every user must belong to a unique, primary group, but can optionally
   belong to one or more, secondary groups.

.. WARNING::

   This process should not be used to create users or groups for daemon
   processes unless the user has experience.


Add a User to OpenLDAP with a Password
--------------------------------------

To add a user with a password to the system, along with a unique group for
that user:

#. Login to the LDAP server as ``root``.
#. Use the ``slappasswd`` command to generate a password hash for a user.
#. Edit the ``/root/ldifs/add_user_with_password.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<username>,ou=Group,dc=your,dc=domain
      objectClass: posixGroup
      objectClass: top
      cn: <username>
      gidNumber: <Unique GID Number>
      description: "<Group Description>"

      dn: uid=<username>,ou=People,dc=your,dc=domain
      uid: <username>
      cn: <username>
      givenName: <First Name>
      sn: <Last Name>
      mail: <e-mail address>
      objectClass: inetOrgPerson
      objectClass: posixAccount
      objectClass: top
      objectClass: shadowAccount
      objectClass: ldapPublicKey
      shadowMax: 180
      shadowMin: 1
      shadowWarning: 7
      shadowLastChange: 10701
      sshPublicKey: <some SSH public key>
      loginShell: /bin/bash
      uidNumber: <some UID number above 1000>
      gidNumber: <GID number from above>
      homeDirectory: /home/<username>
      userPassword: <slappasswd generated SSHA hash>
      pwdReset: TRUE

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/add_user_with_password.ldif

Ensure that an administrative account is created as soon as the SIMP system has
been properly configured. Administrative accounts should belong to the
``administrators`` LDAP group (gidNumber 700). By default, Members of this
group can directly access a privileged shell via ``sudo su -``.

.. NOTE::

   The ``pwdReset: TRUE`` command causes the user to change the
   assigned password at the next login. This command is useful to
   pre-generate the password first and change it at a later time.

   This command appears to be broken in some versions of ``nss_ldap``.
   Therefore, to avoid future issues set ``shadowLastChange`` to a value
   around 10000.

.. WARNING::

   The initial password set for a user must conform to the password policy
   or the user will not be able to login and change his/her password, even
   though the password reset has been enabled by ``pwdReset: TRUE``.


Add a User to OpenLDAP without a Password
-----------------------------------------

To add a user without a password to the system, along with a unique group
for that user

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/add_user_no_password.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<username>,ou=Group,dc=your,dc=domain
      objectClass: posixGroup
      objectClass: top
      cn: <username>
      gidNumber: <Unique GID Number>
      description: "<Group Description>"

      dn: uid=<username>,ou=People,dc=your,dc=domain
      uid: <username>
      cn: <username>
      givenName: <First Name>
      sn: <Last Name>
      mail: <e-mail address>
      objectClass: inetOrgPerson
      objectClass: posixAccount
      objectClass: top
      objectClass: shadowAccount
      objectClass: ldapPublicKey
      sshPublicKey: <some SSH public key>
      loginShell: /bin/bash
      uidNumber: <some UID number above 1000>
      gidNumber: <GID number from above>
      homeDirectory: /home/<username>

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/add_user_no_password.ldif

Remove a User from OpenLDAP
===========================

To remove a user from the system, along with a unique group for that user:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/del_user.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<User UID>,ou=Group,dc=example,dc=domain
      changeType: delete

      dn: uid=<User UID>,ou=People,dc=example,dc=domain
      changeType: delete

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/del_user.ldif


Update a User's SSH Public Key in OpenLDAP
==========================================

To update an SSH public key:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/mod_sshkey.ldif`` shown below.

   .. code-block:: yaml

      dn: uid=<User UID>,ou=People,dc=your,dc=domain
      changetype: modify
      replace: sshPublicKey
      sshPublicKey: <User OpenSSH Public Key>

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldif/mod_sshkey.ldif


Force a Password Reset in OpenLDAP
==================================

To force a password reset for a user:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/force_password_reset.ldif`` shown below.

   .. code-block:: yaml

      dn: uid=<username>,ou=People,dc=your,dc=domain
      changetype: modify
      replace: pwdReset
      pwdReset: TRUE
      -
      replace: shadowLastChange
      shadowLastChange: 10101

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/force_password_reset.ldif

.. NOTE::

   The ``ldapmodify`` command is only effective when using the *ppolicy*
   overlay. In addition, the user's **shadowLastChange** must be changed to a
   value prior to the expiration date to force a :term:`PAM` reset.


Lock an LDAP Account in OpenLDAP
================================

To lock an LDAP account:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/lock_user.ldif`` shown below.

   .. code-block:: yaml

      dn: uid=<username>,ou=People,dc=your,dc=domain
      changetype: modify
      replace: pwdAccountLockedTime
      pwdAccountLockedTime: 000001010000Z
      -
      delete: sshPublicKey
      -
      replace: userPassword
      userPassword: !!

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/lock_user.ldif

.. NOTE::

   The ``ldapmodify`` command is only effective when using the *ppolicy*
   overlay.

Unlock an LDAP Account in OpenLDAP
==================================

To unlock an LDAP account:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/unlock_account.ldif`` shown below.

   .. code-block:: yaml

      dn: uid=<User UID>,ou=People,dc=your,dc=domain
      changetype: modify
      delete: pwdAccountLockedTime

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/unlock_account.ldif

.. NOTE::

   The ``ldapmodify`` command is only effective when using the *ppolicy*
   overlay.


Troubleshooting Issues in OpenLDAP
==================================

If a user's password is changed or the user changes it shortly after its initial
set up, the "Password too young to change" error may appear. In this situation,
apply the ``pwdReset:TRUE`` option to the user's account as described in
`Add a User to OpenLDAP with a Password`_.

.. _Managing LDAP Users:

Managing Users with Lightweight Directory Access Protocol (LDAP)
================================================================

.. contents::
  :local:

Prepare SIMP ldifs
------------------

SIMP natively uses OpenLDAP for user and group management. Actionable
copies of the :term:`LDAP` Data Interchange Format (.ldif) files can be found
on the system in the ``/usr/share/simp/ldifs`` directory.
Copy these files into ``/root/ldifs`` and fix their Distinguished Names:

.. code-block:: bash

  # mkdir /root/ldifs
  # cp /usr/share/simp/ldifs/* /root/ldifs
  # cd /root/ldifs
  # sed -i 's/dc=your,dc=domain/<your actual DN information>/g' \*.ldif

.. WARNING::
  Do not leave any extraneous spaces in LDIF files!

  Use `:set list` in vim to see hidden spaces at the end of lines.

  Use the following to strip out inappropriate characters:

.. code-block:: bash

  # sed -i \
      's/\\(^[[:graph:]]\*:\\)[[:space:]]\*\\ ([[:graph:]]\*\\) \\[[:space:]]\*$/\\1\\2/' \
      file.ldif

.. NOTE::
  Use the ``[`` and ``]`` characters to scroll horizontally when using ELinks.

Add a User
----------

Users can be added with or without a password. Follow the instructions
in the following sections.

.. NOTE::
   Every user must belong to a unique, primary group, but can optionally
   belong to one or more, secondary groups.

.. WARNING::
    This process should not be used to create users or groups for daemon
    processes unless the user has experience.

Add a User with a Password
^^^^^^^^^^^^^^^^^^^^^^^^^^

To add a user with a password to the system, along with a unique group for
that user:

1. Login to the LDAP server as ``root``.
2. Use the ``slappasswd`` command to generate a password hash for a user.
3. Edit the ``/root/ldifs/add_user_with_password.ldif`` shown below.

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

4. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/add_user_with_password.ldif

Ensure that an administrative account is created as soon as the SIMP system has
been properly configured. Administrative accounts should belong to the
``administrators`` LDAP group (gidNumber 700). Members of this LDAP group can
utilize sudo sudosh for privilege escalation.

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


Add a User without a Password
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To add a user without a password to the system, along with a unique group
for that user

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/add_user_no_password.ldif`` shown below.

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

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

   # ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
     -f /root/ldifs/add_user_no_password.ldif

Remove a User
-------------

To remove a user from the system, along with a unique group for that user:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/del_user.ldif`` shown below.

.. code-block:: yaml

   dn: cn=<User UID>,ou=Group,dc=example,dc=domain
   changeType: delete

   dn: uid=<User UID>,ou=People,dc=example,dc=domain
   changeType: delete

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/del_user.ldif

Additional Common LDAP Operations
---------------------------------

As described below, other useful operations can be executed using the
remaining LDIF files.

Add a Group
^^^^^^^^^^^

SIMP systems are preconfigured with two groups:

- ``administrators`` (700):  Group that has both sudosh and ssh privileges
- ``users`` (100): Group that does not have sudosh or ssh privileges

To add another group:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/add_group.ldif`` shown below.

.. code-block:: yaml

   dn: cn=<groupname>,ou=Group,dc=your,dc=domain
   objectClass: posixGroup
   objectClass: top
   cn: <groupname>
   gidNumber: <Unique GID number>
   description: "<Some useful group description>"

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/add_group.ldif

Remove a Group
^^^^^^^^^^^^^^

To remove a group:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/del_group.ldif`` shown below.

.. code-block:: yaml

  dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
  changetype: delete

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/del_group.ldif

Add Users to a Group
^^^^^^^^^^^^^^^^^^^^

To add users to a group:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/add_to_group.ldif`` shown below.

.. code-block:: yaml

  dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
  changetype: modify
  add: memberUid
  memberUid: <UID1>
  memberUid: <UID2>
  ...
  memberUid: <UIDX>

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/add_to_group.ldif

Remove Users from a Group
^^^^^^^^^^^^^^^^^^^^^^^^^

To remove users from a group:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/del_to_group.ldif`` shown below.

.. code-block:: yaml

  dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
  changetype: modify
  delete: memberUid
  memberUid: <UID1>
  memberUid: <UID2>
  ...
  memberUid: <UIDX>

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/del_from_group.ldif

Update a User's SSH Public Key
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To update an SSH public key:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/mod_sshkey.ldif`` shown below.

.. code-block:: yaml

  dn: uid=<User UID>,ou=People,dc=your,dc=domain
  changetype: modify
  replace: sshPublicKey
  sshPublicKey: <User OpenSSH Public Key>

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
  -f /root/ldif/mod_sshkey.ldif

Force a Password Reset
^^^^^^^^^^^^^^^^^^^^^^

To force a password reset for a user:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/force_password_reset.ldif`` shown below.

.. code-block:: yaml

   dn: uid=<username>,ou=People,dc=your,dc=domain
   changetype: modify
   replace: pwdReset
   pwdReset: TRUE
   -
   replace: shadowLastChange
   shadowLastChange: 10101

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/force_password_reset.ldif

.. NOTE::
    The ``ldapmodify`` command is only effective when using the *ppolicy*
    overlay. In addition, the user's **shadowLastChange** must be changed to a
    value prior to the expiration date to force a :term:`PAM` reset.

Lock an LDAP Account
^^^^^^^^^^^^^^^^^^^^

To lock an LDAP account:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/lock_user.ldif`` shown below.

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

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
    -f /root/ldifs/lock_user.ldif

.. NOTE::
    The ``ldapmodify`` command is only effective when using the
    *ppolicy* overlay.

.. _unlock-ldap-label:

Unlock an LDAP Account
^^^^^^^^^^^^^^^^^^^^^^

To unlock an LDAP account:

1. Login to the LDAP server as ``root``.
2. Edit the ``/root/ldifs/unlock_account.ldif`` shown below.

.. code-block:: yaml

  dn: uid=<User UID>,ou=People,dc=your,dc=domain
  changetype: modify
  delete: pwdAccountLockedTime

3. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

.. code-block:: bash

  # ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
   -f /root/ldifs/unlock_account.ldif

.. NOTE::
    The ``ldapmodify`` command is only effective when using the
    *ppolicy* overlay.

Troubleshooting Issues
----------------------

If a user's password is changed in LDAP or the user changes it shortly after
its initial set up, the "Password too young to change" error may appear. In this
situation, apply the ``pwdReset:TRUE`` option to the user's account as
described in `Add a User with a Password`_.

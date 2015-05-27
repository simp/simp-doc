.. _User_Management:

User Management
===============

The chapter explains how to manage users in the default SIMP
environment.

Managing Users with Lightweight Directory Access Protocol (LDAP)
----------------------------------------------------------------

SIMP natively uses OpenLDAP for user and group management. Actionable
copies of the LDAP Data Interchange Format (.ldif) files can be found on
the system in the */usr/share/doc/simp-<Version>/ldifs* directory.

Users cannot have any extraneous spaces in .ldif files.
   
.. code-block:: bash

   Type `:set list` in vim to see hidden spaces at the end of lines.

   Type `sed -i 's/\\(^[[:graph:]]\*:\\)[[:space:]]\*\\
   ([[:graph:]]\*\\) \\[[:space:]]\*$/\\1\\2/' file.ldif` 
   to strip out inappropriate characters.


**Note**

Use the **[** and **]** characters to scroll right when using
ELinks.

Add Users
---------

Users can be added with or without a password. Follow the instructions
in the following sections.

    **Warning**

    This process should not be used to create users or groups for daemon
    processes unless the user has experience.

Adding Users With a Password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To add a user to the system, :term:`Secure Shell (SSH)` to the LDAP server and use the
**slappasswd** command to generate a password hash for a user.

Create a */root/ldifs* directory and add the following information to
the */root/ldifs/adduser.ldif* file. Replace the information within < >
with the installed system's information.

Example ldif to add a user

.. code-block:: Ruby

           dn: uid=<User UID>,ou=People,dc=your,dc=domain
           uid: <User UID>
           cn: <User UID>
           objectClass: account
           objectClass: posixAccount
           objectClass: top
           objectClass: shadowAccount
           objectClass: ldapPublicKey
           shadowMax: 90
           shadowMin: 1
           shadowWarning: 7
           shadowLastChange: 10167
           pwdReset: TRUE
           sshPublicKey: <User SSH Public Key>
           loginShell: /bin/bash
           uidNumber: <User UID Number>
           gidNumber: <User Primary GID>
           homeDirectory: /home/<User UID>
           userPassword: <Password Hash from slappasswd>
           
.. code-block:: bash

  Type `ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f /root/ldifs/adduser.ldif` .

Ensure that an administrative account is created as soon as the SIMP
system has been properly configured. Administrative accounts should
belong to the *administrators*\ LDAP group (gidNumber 700). Members of
this LDAP group can utilize sudo sudosh for privilege escalation.

    **Note**

    The **pwdReset: TRUE** command causes the user to change the
    assigned password at the next login. This command is useful to
    pre-generate the password first and change it at a later time.

    This command appears to be broken in some versions of *nss\_ldap*.
    Therefore, to avoid future issues set *shadowLastChange* to a value
    around 10000.

Adding Users Without a Password
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Create a */root/ldifs* directory and add the following information to
the */root/ldifs/adduser.ldif* file. Replace the information within < >
with the installed system's information.

Example ldif example to add a user

.. code-block:: Ruby

           dn: uid=<User UID>,ou=People,dc=your,dc=domain
           uid: <User UID>
           cn: <User UID>
           objectClass: account
           objectClass: posixAccount
           objectClass: top
           objectClass: shadowAccount
           objectClass: ldapPublicKey
           sshPublicKey: <User SSH Public Key>
           loginShell: /bin/bash
           uidNumber: <User UID Number>
           gidNumber: <User Primary GID>
           homeDirectory: /home/<User UID>
           

.. code-block:: bash

  Type `ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
   -f /root/ldifs/adduser.ldif` .

Wait for the **akeys.pl** command to run (hourly) prior to the user
being able to log in with the assigned keys.

Remove Users
------------

To remove a user, create a */root/ldifs/removeuser.ldif* file. Add the
information below to the file and replace the text within < > with the
installed system's information.

Example ldif to remove a user

.. code-block:: Ruby

              dn: cn=<User UID>,ou=Group,dc=example,dc=domain
              changeType: delete

              dn: uid=<User UID>,ou=People,dc=example,dc=domain
              changeType: delete
            
.. code-block:: bash

  Type **ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f /root/ldifs/removeuser.ldif**.

Additional .ldif File Commands
------------------------------

Other useful commands for .ldif files can be found below. Before using
these commands, ensure that the */root/ldifs* directory has been
created.

Changing a Password
~~~~~~~~~~~~~~~~~~~

To change a password, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to change password

.. code-block:: Ruby

           dn: uid=<User UID>,ou=People,dc=your,dc=domain
           changetype: modify
           replace: userPassword
           userPassword: <Hash from slappasswd>
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

Adding a Group
~~~~~~~~~~~~~~

To add a group, add the following information to the */root/ldifs/<.ldif
File>* file. Replace the information below within < > with the installed
system's information.

Example ldif to add a group

.. code-block:: Ruby

           dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
           objectClass: posixGroup
           objectClass: top
           cn: <Group Name>
           gidNumber: <GID>
           description: "Some Descriptive Text"
           
.. code-block:: bash

  Type `ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

Removing a Group
~~~~~~~~~~~~~~~~

To remove a group, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to remove a group

.. code-block:: Ruby

           dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
           changetype: delete
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

Adding Users to a Group
~~~~~~~~~~~~~~~~~~~~~~~

To add users to a group, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to add to a group

.. code-block:: Ruby

           dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
           changetype: modify
           add: memberUid
           memberUid: <UID1>
           memberUid: <UID2>
           ...
           memberUid: <UIDX>
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

Removing Users from a Group
~~~~~~~~~~~~~~~~~~~~~~~~~~~

To remove users from a group, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to remove an user from a group

.. code-block:: Ruby

           dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
           changetype: modify
           delete: memberUid
           memberUid: <UID1>
           memberUid: <UID2>
           ...
           memberUid: <UIDX>
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

Updating an SSH Public Key
~~~~~~~~~~~~~~~~~~~~~~~~~~

To update an SSH public key, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to update SSH public key

.. code-block:: Ruby

           dn: uid=<User UID>,ou=People,dc=your,dc=domain
           changetype: modify
           replace: sshPublicKey
           sshPublicKey: <User OpenSSH Public Key>
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

Forcing a Password Reset
~~~~~~~~~~~~~~~~~~~~~~~~

To force a password reset, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to reset user's shadowLastChange

.. code-block:: Ruby

           dn: uid=<User UID>,ou=People,dc=your,dc=domain
           changetype: modify
           replace: pwdReset
           pwdReset: TRUE
           -
           replace: shadowLastChange
           shadowLastChange: 10000
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif\_file>` .

**Note**

    The **ldapmodify** command is only effective when using the
    *ppolicy* overlay. In addition, the user's *shadowLastChange* must
    be changed to a value prior to the expiration date to force a 
    :term:`Pluggable Authentication Modules (PAM)` reset.

Unlocking an LDAP Account
~~~~~~~~~~~~~~~~~~~~~~~~~

To unlock an LDAP account, add the following information to the
*/root/ldifs/<.ldif File>* file. Replace the information below within <
> with the installed system's information.

Example ldif to Unlock LDAP Account

.. code-block:: Ruby

           dn: uid=<User UID>,ou=People,dc=your,dc=domain
           changetype: modify
           delete: pwdAccountLockedTime
           
.. code-block:: bash

  Type `ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain"
  -f <.ldif File>` .

    **Note**

    The **ldapmodify** command is only effective when using the
    *ppolicy* overlay.

Troubleshooting Issues
======================

If a user's password is changed in LDAP or the user changes it shortly
after its initial setup, the "Password too young to change" error may
appear. In this situation, apply the **pwdReset:TRUE** command to the
user's account as described Add Users with a Password section.

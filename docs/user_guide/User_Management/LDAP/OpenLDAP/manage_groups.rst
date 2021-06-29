.. _ug-user_management-ldap-openldap-manage_groups:

Add a Group to OpenLDAP
=======================

.. include:: ldif_prep_header.inc

.. contents::
   :local:

SIMP systems are preconfigured with two groups:

- ``administrators`` (700):  Group that has ssh and privilege escalation privileges
- ``users`` (100): Group that does not have ssh or privilege escalation privileges

To add another group:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/add_group.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<groupname>,ou=Group,dc=your,dc=domain
      objectClass: posixGroup
      objectClass: top
      cn: <groupname>
      gidNumber: <Unique GID number>
      description: "<Some useful group description>"

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapadd -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/add_group.ldif

Remove a Group
--------------

To remove a group:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/del_group.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
      changetype: delete

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/del_group.ldif

Add Users to a Group
--------------------

To add users to a group:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/add_to_group.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
      changetype: modify
      add: memberUid
      memberUid: <UID1>
      memberUid: <UID2>
      ...
      memberUid: <UIDX>

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
      -f /root/ldifs/add_to_group.ldif

Remove Users from a Group
-------------------------

To remove users from a group:

#. Login to the LDAP server as ``root``.
#. Edit the ``/root/ldifs/del_to_group.ldif`` shown below.

   .. code-block:: yaml

      dn: cn=<Group Name>,ou=Group,dc=your,dc=domain
      changetype: modify
      delete: memberUid
      memberUid: <UID1>
      memberUid: <UID2>
      ...
      memberUid: <UIDX>

#. Type the following, substituting your DN information for
   ``dc=your,dc=domain``:

   .. code-block:: bash

      ldapmodify -Z -x -W -D "cn=LDAPAdmin,ou=People,dc=your,dc=domain" \
        -f /root/ldifs/del_from_group.ldif

.. _ug-user_management-ldap-389_ds-manage_groups:

Group Management in 389-DS
=========================

List 389-DS Groups
------------------

You can list all groups in the default SIMP :term:`389-DS` instance by running:

.. code-block:: shell

   dsidm accounts group list

If running a SIMP-generated default instance, you should see the usual ``users``
and ``administrators`` groups.

Add a Group to 389-DS
---------------------

To add a group to 389-DS, you can either run ``dsidm posixgroup create`` and it
will prompt you for input or you can provide most parameters at the command line
as follows:

.. code-block:: shell

   dsidm accounts posixgroup create --cn alice --gidNumber 1000

.. NOTE::

   Note the use of ``posixgroup`` instead of ``group`` when adding groups.

   * ``posixgropup`` => POSIX-style groups generally used for system accounts.
   * ``group`` => Regular LDAP groups which may be useful for external services.

Remove a Group from 389-DS
--------------------------

To remove our `alice` group, run the following command:

.. code-block:: shell

   dsidm accounts group delete "<DN>"

It will prompt you to type ``Yes I am sure`` to confirm deletion.

To get the DN for the group run:

.. code-block:: shell

   dsidm accounts group get alice | head -1 | cut -f2- -d' '

Get Information about a 389-DS Group
------------------------------------

Use the following command to get information about a specific group:

.. code-block:: shell

   dsidm accounts group get alice

Add a User to a 389-DS Group
----------------------------

Use the following command to add a user to a group:

.. code-block:: shell

   dsidm accounts group add_member "<DN>"

You can get the DN of a user by running:

.. code-block:: shell

   dsidm accounts user get <username> | head -1 | cut -f2- -d' '

It is important to note that, by default, referential integrity is **not**
preserved between users and groups. This means that you will need to manually
remove users from groups if you decide to delete a user.

If you want to change this behavior, you can enable the Referential Integrity
Postoperation plug-in manually. However, this has ramifications in clustered
environments so please read the `related documentation`_ before proceeding.

.. _related documentation: https://directory.fedoraproject.org/docs/389ds/design/referint-replication-design.html

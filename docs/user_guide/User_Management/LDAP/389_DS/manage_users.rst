.. _ug-user_management-ldap-389_ds-manage_users:

User Management in 389-DS
=========================

List 389-DS Users
-----------------

You can list all users in the default SIMP :term:`389-DS` instance by running:

.. code-block:: shell

   dsidm accounts user list

Add a User to 389-DS
--------------------

To add a user to 389-DS, you can either run ``dsidm user create`` and it will
prompt you for input or you can provide most parameters at the command line as
follows:

.. code-block:: shell

   dsidm accounts user create --uid alice --cn "Alice User" --displayName 'Alice' \
     --uidNumber 1000 --gidNumber 1000 --homeDirectory /home/alice

Remove a User from 389-DS
-------------------------

To remove our `alice` user, run the following command:

.. code-block:: shell

   dsidm accounts user delete <DN>

It will prompt you to type ``Yes I am sure`` to confirm deletion.

To get the DN for the user run:

.. code-block:: shell

   dsidm accounts user get alice | head -1 | cut -f2- -d' '

Add a Password to a 389-DS User
-------------------------------

You may notice that this user has been created without a password. The command
line options do not provide this capability so a password will need to be added
afterwards.

.. NOTE::

  No matter which of the following methods you choose, the user will be prompted
  to change their password at the next login by default.

Interactive Reset
^^^^^^^^^^^^^^^^^

To be prompted for the user credentials, you can run the following:

.. code-block:: shell

   dsidm accounts account reset_password "<DN>"

To obtain the ``DN`` run:

.. code-block:: shell

   dsidm accounts user get alice | head -1 | cut -f2- -d' '

Direct Reset
^^^^^^^^^^^^

If you want to set the user's password directly, first generate the password
using ``pwdhash``:

.. code-block:: shell

   pwdhash -D /etc/dirsrv/slapd-accounts "<plain_text_password>"

Then run the following, pasting the output of the previous command into
``<GENERATED_HASH>``:

.. code-block:: shell

   dsidm accounts user modify alice add:userPassword:<GENERATED HASH>


Add a SSH Public Key to a 389-DS User
-------------------------------------

You can use the following command to add a SSH key to a 389-DS user:

.. code-block:: shell

   dsidm accounts user modify alice add:nsSshPublicKey:"<ssh-rsa AAA...>"

Remove a SSH Public Key from a 389-DS User
------------------------------------------

You can use the following command to remove a SSH key from a 389-DS user:

.. code-block:: shell

   dsidm accounts user modify alice delete:nsSshPublicKey:"<ssh-rsa AAA...>"

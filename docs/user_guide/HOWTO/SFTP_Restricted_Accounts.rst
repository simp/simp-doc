HOWTO Enable SFTP Restricted Accounts
=====================================

This section describes the method for restricting an account to
:term:`SSH File Transfer Protocol` (SFTP) access only.

Add a User
----------

Create a user account based on the following example.

.. code-block:: ruby

  user { "foo":
    uid => <UID>,
    gid => <GID>,
    shell => <Path to SFTP Server>
  }

On a SIMP system, shell would be: ``"/usr/libexec/openssh/sftp-server"``


Modify ``/etc/shells``
----------------------

To modify ``/etc/shells`` to include the shell information provided in the
previous user account example, add ``common::shells`` in Hiera, and add
``/usr/libexec/openssh/sftp-server`` to the list.

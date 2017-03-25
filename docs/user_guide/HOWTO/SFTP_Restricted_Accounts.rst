HOWTO Enable SFTP Restricted Accounts
=====================================

This section describes the method for restricting an account to
:term:`SSH File Transfer Protocol` (SFTP) access only.

Add a User
----------

Create a user account based on the following example.

.. code-block:: puppet

  user { "foo":
    uid   => <UID>,
    gid   => <GID>,
    shell => '/usr/libexec/openssh/sftp-server'
  }

Modify ``/etc/shells``
----------------------

To allow your user to use the ``sftp-server`` application as a shell, you will
need to add custom shell to ``useradd::shells`` in :term:`Hiera` as shown
below.

.. code:: yaml

   useradd::shells:
     - /usr/libexec/openssh/sftp-server

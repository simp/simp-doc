Least Privilege
---------------

The stunnel service runs under the ``stunnel`` user and ``stunnel`` group.  This allows
directory permissions to limit the service's access to files/directories not
owned by the ``stunnel`` user/group. The stunnel user does not have a valid login
shell.

References: :ref:`AC-6`

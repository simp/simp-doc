Least Privilege
---------------

The Apache service runs under the ``apache`` user and ``apache`` group.  This is allows
directory permissions to limit the service's access to files/directories not
owned by the ``apache`` user/group. The apache user does not have a valid login
shell.

References: :ref:`AC-6`

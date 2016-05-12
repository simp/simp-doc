Least Privilege
---------------

The named service runs under the ``named`` user and ``named`` group.  This is allows
directory permissions to limit the service's access to files/directories not
owned by the ``apache`` user/group. The ``named`` user does not have a valid login
shell.

References: :ref:`AC-6`

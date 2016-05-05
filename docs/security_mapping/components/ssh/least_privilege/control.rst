Least Privilege
---------------

The SSH service runs under the ``ssh`` user and ``ssh`` group.  This is allows
directory permissions to limit the service's access to files/directories not
owned by the ``ssh`` user/group. The ssh user does not have a valid login
shell.

X11 forwarding over SSH is explicitly disallowed. This limits the exposure of
the SSH server to networks outside of the control of SIMP.

References: :ref:`AC-6`

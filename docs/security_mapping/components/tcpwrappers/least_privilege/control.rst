Least Privilege
----------------

SIMP configures tcpwrappers to deny all, meaning that a service will be denied
access to the TCP stack unless it is explicitly allowed.  Each SIMP module that
needs access to the TCP stack has an entry added to the host.allow file using
this tcpwrappers module.

References: :ref:`AC-6`

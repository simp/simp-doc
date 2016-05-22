Access Enforcement
------------------

The puppet master uses a whitelist to determine which puppet clients can connect
to the puppet master.  The certificate of the connecting client must match the
fully qualified domain name of the system.  If it doesn't, then the connection
is denied.

References: :ref:`AC-3`

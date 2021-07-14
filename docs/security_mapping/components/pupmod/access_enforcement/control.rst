Access Enforcement
------------------

The :term:`Puppet Server` uses a whitelist to determine which puppet clients can
connect via the network.  The certificate of the connecting client must match
the fully qualified domain name of the system as resolved via :term:`DNS`.  If
it does not then the connection is denied.

References: :ref:`AC-3`

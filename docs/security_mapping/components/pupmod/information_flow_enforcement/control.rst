Information Flow Enforcement
----------------------------

The `pupmod` :term:`puppet module` listens on ports 8140 and 8141 by default and
makes these ports available via the system firewall.

Port 8140 is the :term:`Puppet Server` port and 8141 is the certificate
authority port.  The connecting source IPs are limited to the value of
``$trusted_nets``, which for most installs, is the local network.

References: :ref:`AC-4`

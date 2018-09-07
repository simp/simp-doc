Information Flow Enforcement
----------------------------

The pupmod module explicitly opens up ports 8140 and 8141
using IPTables rules.  Port 8140 is the puppet master port and 8141 is the
certificate authority port.  The connecting source IPs are limited to the value of
``$trusted_nets``, which for most installs is the local network.

References: :ref:`AC-4`

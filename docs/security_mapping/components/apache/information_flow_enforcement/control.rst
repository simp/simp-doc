Information Flow Enforcement
-----------------------------

The Apache module explicitly opens up ports 80 and 443 for the root web servers by
using IPTables rules.  The connecting source IPs are limited to the value of
``$client_nets``, which for most installs is the local network.

References: :ref:`AC-4`

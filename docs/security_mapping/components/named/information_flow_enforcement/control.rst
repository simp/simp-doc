Information Flow Enforcement
-----------------------------

The named module explicitly opens TCP and UDP ports 53 for the DNS by
using IPTables rules.  The connecting source IPs are limited to the value of
``$client_nets`` which for most installs is the local network.

References: :ref:`AC-4`

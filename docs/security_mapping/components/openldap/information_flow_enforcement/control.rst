Information Flow Enforcement
----------------------------

Since TCPWrappers has a default deny policy in place, a specific entry is added
to allow all hosts to connect to the slapd service.

The OpenLDAP module explicitly opens up ports 389 (LDAP) and 636 (LDAPS)
using IPTables rules.  The connecting source IPs are limited to the value of
``$client_nets`` which for most installs is the local network.

References: :ref:`AC-4`

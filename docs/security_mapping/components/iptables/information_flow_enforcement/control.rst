Information Flow Enforcement
----------------------------

IPTables is installed and running on all SIMP clients. IPtables controls the
flow of inbound traffic by limiting IP addresses, protocols, and port numbers.

The default IPTables rules:

- Allow all outbound traffic
- Allow ping
- Allow traffic from established connections
- Drop broadcast traffic
- Drop multicast traffic
- Drop all other traffic

References: :ref:`AC-4`, :ref:`CM-7b.`

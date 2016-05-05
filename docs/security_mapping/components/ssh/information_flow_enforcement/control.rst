Information Flow Enforcement
-----------------------------

The SSH module explicitly opens up port 22 for the SSH server by
using IPTables rules.

Since TCPWrappers has a default deny policy in place, a specific entry is added
to allow all hosts to connect to the SSH service.

References: :ref:`AC-4`

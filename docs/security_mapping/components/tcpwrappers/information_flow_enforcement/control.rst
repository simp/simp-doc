Information Flow Enforcement
----------------------------

TCP Wrappers is enabled on SIMP systems.  TCP Wrappers is a host-based
networking ACL system, used to filter access to IP addresses.
It allows host or subnetwork IP addresses, names and/or ident query
replies, to be used as tokens on which to filter for access control
purposes.

TCP Wrappers uses the ``/etc/hosts.allow`` and the ``/etc/hosts.deny`` files to
configure the access control.

References: :ref:`AC-4`

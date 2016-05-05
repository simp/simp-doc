Boundary Protection
--------------------

The SIMP IPTables module adds an IPtables rule that will prevent external IP
addresses from being able to send spoofed packets to your system.  This applies
to IPv6 traffic.  IPv4 spoofing is prevented using the rp_filter sysctl setting.

References: :ref:`SC-7`

Boundary Protection
-------------------

The ``simp::sysctl`` class uses the kernel's sysctl ``rp_filter`` (reverse path)
setting to drop spoofed IPv4 packets.

It also enables the use of ``tcp_syncookies`` to resist SYN flood attacks.

Finally, several classes in the ``simp`` module enable :term:`IPTables` in a
deny-by-default mode.

References: :ref:`SC-7`

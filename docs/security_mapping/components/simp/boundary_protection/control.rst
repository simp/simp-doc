Boundary Protection
-------------------

The ``simp::sysctl`` class can use the kernel's sysctl ``rp_filter`` (reverse
path) setting to drop spoofed IPv4 packets.

It can also enable the use of ``tcp_syncookies`` to resist SYN flood attacks.

As of ``simp-simp`` 11.0.0, ``simp::sysctl`` applies these settings only when
the ``simp:defaults`` compliance profile is enforced (see
:ref:`SIMP Compliance Engine`), or when the following are set in :term:`Hiera`:

.. code-block:: yaml

   ---
   simp::sysctl::net__ipv4__conf__all__rp_filter: 1
   simp::sysctl::net__ipv4__conf__default__rp_filter: 1
   simp::sysctl::net__ipv4__tcp_syncookies: 1

Finally, several classes in the ``simp`` module enable :term:`IPTables` in a
deny-by-default mode.

References: :ref:`SC-7`

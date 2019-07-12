IPTables
--------

By default, the SIMP system drops all **incoming** connections to the server,
save port ``22``. Port ``22`` is allowed from **all** external sources since
there is no safe way to restrict this that will not lock users out of freshly
installed systems in many cases.

The default SIMP :term:`IPTables` start-up sequence has been set to *fail
safe*. This means that if the IPTables rules cannot cleanly apply, the system
will only allow port ``22`` into the system for SSH troubleshooting and
recovery.

There are many examples of how to use the ``simp-iptables`` module in the source
code; the ``simp-simp_apache`` module is a particularly good example.  This
module can be found in your SIMP Puppet environment or, if SIMP is installed
via ISO or RPM, at ``/usr/share/simp/modules/simp_apache``.

You can also reference the :term:`Defined Types` in the ``simp-iptables``
module, itself, to understand their purpose and choose the best option.

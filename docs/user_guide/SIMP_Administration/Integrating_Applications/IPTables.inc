IPTables
--------

By default, the SIMP system drops all **incoming** connections to the server,
save port 22. Port 22 is allowed from **all** external sources since there is
no safe way to restrict this that will not lock users out of freshly installed
systems in some cases.

The default SIMP :term:`IPTables` start-up sequence has been set to *fail
safe*. This means that if the IPTables rules cannot cleanly apply, the system
will only allow port 22 into the system for SSH troubleshooting and recovery.

There are many examples of how to use the IPTables module in the source code;
the Apache module at ``/etc/puppetlabs/code/environments/simp/modules/simp_apache``
is a particularly good example. You can also reference the Defined Types in the
IPTables Puppet module to understand their purpose and choose the best option.

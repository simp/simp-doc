Automated Change Implementation
-------------------------------

The most prominent tool in the SIMP architecture is Puppet.  Puppet is a
client/server tool where managed nodes run the Puppet agent application.
One or more servers run the :term:`Puppet Server` application in the form of
a web-based service using :term:`TLS` encrypted connections.

The  Puppet agent sends facts to the Puppet Server and request a
catalog. The master compiles and returns that node's catalog, using several
sources of information it has access to.

Once it receives a catalog, Puppet agent applies it by checking each resource
the catalog describes. If it finds any resources that are not in their desired
state, it makes any changes necessary to correct them. After applying the
catalog, the agent submits a report to the Puppet Server.

Puppet clients have a scheduled job configured to run the puppet agent every 30
minutes by default.

References: :ref:`CM-3 (3)`

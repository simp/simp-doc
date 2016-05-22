Automated Change Implementation
-------------------------------

The most prominent tool in the SIMP architecture is Puppet.  Puppet is a
client/server tool where managed nodes run the Puppet agent application.
One or more servers run the Puppet master application in the form of
Puppet Server.

The  Puppet agent sends facts to the Puppet master and request a
catalog. The master compiles and returns that node’s catalog, using several
sources of information it has access to.

Once it receives a catalog, Puppet agent applies it by checking each resource
the catalog describes. If it finds any resources that are not in their desired
state, it makes any changes necessary to correct them. After applying the
catalog, the agent submits a report to the Puppet master.

Puppet clients have a cron job configured to run the puppet agent every 30
minutes.

References: :ref:`CM-3 (3)`

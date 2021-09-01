.. _ht-disconnect-puppetdb:

HOWTO Disconnect PuppetDB from the Puppet Server
================================================

This section covers how to disconnect :term:`PuppetDB` from the
:term:`Puppet Server` for troubleshooting, or to allow SIMP to repair the
configuration.

Run the following script on the :term:`Puppet Server` to stop :program:`puppetdb` and
restart the :program:`puppetserver` process without the connection.

.. code-block:: shell

   puppet resource service puppetdb ensure=stopped

   # The following line assumes the puppet server configuration directory is
   # /etc/puppetlabs/puppet (the default).
   mv /etc/puppetlabs/puppet/routes.yaml /etc/puppetlabs/puppet/routes.yaml.backup

   puppet config set --section server storeconfigs false
   puppet config set --section server reports store
   puppet config set --section main storeconfigs false

   puppet resource service puppetserver ensure=stopped
   puppet resource service puppetserver ensure=running

.. Note::

   When :program:`puppet` is next run on the Puppet server, it will reconfigure the
   :program:`puppetserver` and :program:`puppetdb` to reconnect and restart the appropriate
   services.  If you are debugging an issue, you may want to run :command:`puppet agent --disable`
   to prevent Puppet from resetting the system while you are debugging.

.. _ht-disconnect-puppetdb:

How to Disconnect PuppetDB from the Puppet Server
=================================================

This section covers how to disconnect :term:`PuppetDB` from the
:term:`Puppet Server` for troubleshooting or to allow SIMP to repair the
configuration.

Run the following script on the :term:`Puppet Master` to stop ``puppetdb`` and
restart the ``puppetserver`` process without the connection.

.. code-block:: shell

  puppet resource service puppetdb ensure=stopped

  # The following line assumes the puppet server configuration directory is
  # /etc/puppetlabs/puppet (the default).
  mv /etc/puppetlabs/puppet/routes.yaml /etc/puppetlabs/puppet/routes.yaml.backup

  puppet config set --section master storeconfigs false
  puppet config set --section main storeconfigs false

  puppet resource service puppetserver ensure=stopped
  puppet resource service puppetserver ensure=running

.. Note::

  When ``puppet`` is run on the Puppet master again, it will reconfigure the
  ``puppetserver`` and ``PuppetDB`` to reconnect and restart the appropriate
  services.  If you are debugging an issue, you may want to run ``puppet agent
  --disable`` to prevent Puppet from resetting the system while you are
  debugging.


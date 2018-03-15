.. _ht-disable-puppetdb:

How to temporarily disable Puppetdb
===================================

`Puppetdb`_ is a storage service for Puppet-produced data. It is not required to run
puppetserver.  This section covers how to disconnect puppetdb from the puppetserver
temporarily for troubleshooting.

.. _Puppetdb: https://puppet.com/blog/introducing-puppetdb-put-your-data-to-work

Run the following script on the puppetserver to stop puppetdb and restart puppetserver without it.

.. code-block:: shell

  puppet resource service puppetdb ensure=stopped
  pkill -9 -f puppetdb
  # The following line assumes the puppet server configuration directory is
  # /etc/puppetlabs/puppet (the default).
  mv /etc/puppetlabs/puppet/routes.yaml /etc/puppetlabs/puppet/routes.yaml.backup
  puppet config set --section master storeconfigs false
  puppet config set --section main storeconfigs false
  # use service puppetserver restart on Centos/RedHat 6
  systemctl restart puppetserver


.. Note::

  When puppet is run on the puppetserver again it will reconfigure it to use
  puppetdb and restart the puppetdb service. Run ``puppet agent --disable`` to stop puppet
  from updating your system while you are debugging.


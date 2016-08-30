HOWTO Change Puppet Masters
===========================

It may be necessary to change the Puppet Master. To point a particular
client to a new Puppet Master, follow the steps in the sections below.

On the Client
-------------

Enter the following changes into the */etc/puppet/puppet.conf* file.

Code Changes on Client to Switch Puppet Masters

.. code-block:: ruby

  server = new.puppet.master.fqdn
  ca_server = new.puppet.master.fqdn
  ca_port = 8141


To remove all files and sub-directories in the ``/var/lib/puppet/ssl``
directory, type ``cd /var/lib/puppet/ssl``. Then type ``rm -rf ./*``.

Assuming the new Puppet Master has been set up to properly accept the
client, type ``puppet agent --test`` to run a full Puppet run while
pointing to the new server.

If all goes well, the client will now be synchronized with the new
Puppet Master. If not, refer to the SIMP Server Installation section of
the SIMP Install Guide and ensure that the new Puppet Master was set up
properly.

On the Old Puppet Master
------------------------

Remove or comment out all items for the client node in the ``/etc/puppet/environments/simp/hieradata/hosts`` space.

To run ``puppet agent`` in *noop* mode to ensure that there are no
inadvertent errors, type ``puppet agent --test --noop``.

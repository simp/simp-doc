HOWTO Change Puppet Servers
===========================

It may be necessary to change the Puppet Server. To point a particular
client to a new Puppet Server, follow the steps in the sections below.

.. NOTE::

   All commands in this section should be run as the ``root`` user.

On the Old Puppet Server
------------------------

Collect the Client's Server-Side Artifacts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Until SIMP implements a shared Puppet data store (expected 2017-Q2), you will
need to manually copy some artifacts from the old server to the new server

To do this, run:

.. code-block:: shell

   $ find `puppet config --section master print vardir`/simp -name "*<client-fqdn>*" -exec tar --selinux --xattrs -rpvf <client-fqdn>_transfer.tar {} \;
   $ find /var/simp/environments -name "*<client-fqdn>*" -exec tar --selinux --xattrs -rpvf <client-fqdn>_transfer.tar {} \;

Then, pull all of the relevant Hiera configuration for the node:

.. code-block:: shell

   $ find /etc/puppetlabs/code/environments -name "<client-hostname>.yaml" -exec tar --selinux --xattrs -rpvf <client-hostname>_transfer.tar {} \;
   $ find /etc/puppetlabs/code/environments -name "<client-fqdn>.yaml" -exec tar --selinux --xattrs -rpvf <client-hostname>_transfer.tar {} \;

Remove all of the node specific Hiera data:

.. code-block:: shell

   $ find /etc/puppetlabs/code/environments -name "<client-fqdn>.yaml" --delete

.. NOTE::

   You may have Hiera YAML files with the short name of the host still in place
   but those are too dangerous to automatically delete since they may match
   multiple hosts.

Reload the ``puppetserver`` process:

.. code-block:: shell

   $ puppetserver_reload

On the New Puppet Server
------------------------

.. WARNING::

   This assumes that the new Puppet Server is set up identically to the old
   Puppet Server. If it is not, you will need to verify that the artifacts in
   the ``tar`` file are correctly placed.

Unpack the ``<client-hostname>_tansfer.tar`` archive onto the system:

.. code-block:: shell

   tar --selinux --xattrs -C / -xvf <client-hostname>_transfer.tar

Reload the ``puppetserver`` process:

.. code-block:: shell

   puppetserver_reload

On Each Client
--------------

.. WARNING::

   Make sure you are running these commands **on the client**. If you run them
   on the server, you have a very high risk of making your Puppet
   infrastructure inoperable.

Update the Puppet Config
^^^^^^^^^^^^^^^^^^^^^^^^

Enter the following changes into ``/etc/puppetlabs/puppet/puppet.conf``.

.. code-block:: ini

  server = new.puppet.master.fqdn
  ca_server = new.puppet.master.fqdn
  ca_port = 8141

Remove the Client Puppet Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To remove all legacy SSL material, run ``rm -rf `puppet config --section agent ssldir```

Run Puppet
^^^^^^^^^^

Assuming the new Puppet Server has been set up to properly accept the
client, execute a full Puppet run using ``puppet agent --test``.

If everything was done properly, the client will now be synchronized with the
new Puppet Server.

If you find issues, refer to the :ref:`Client_Management` section of the
documentation and ensure that the new Puppet Server was set up properly.

.. _ug-howto-change-puppet-masters:

HOWTO Change Puppet Masters
===========================

It may be necessary to change the Puppet master. To point a particular
client to a new Puppet master, follow the steps in the sections below.

.. NOTE::

   All commands in this section should be run as the ``root`` user.

On the Old Puppet Master
------------------------

Collect the Client's Server-Side Artifacts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Until SIMP implements a shared Puppet data store, you will need to manually
copy some artifacts from the old server to the new server.

To do this, run:

.. code-block:: shell

   # find `puppet config --section master print vardir`/simp -name "*<client-fqdn>*" -exec tar --selinux --xattrs -rpvf <client-fqdn>_transfer.tar {} \;
   # find /var/simp/environments -name "*<client-fqdn>*" -exec tar --selinux --xattrs -rpvf <client-fqdn>_transfer.tar {} \;

Then, pull all of the relevant Hiera configuration for the node:

.. code-block:: shell

   # find /etc/puppetlabs/code/environments -name "<client-hostname>.yaml" -exec tar --selinux --xattrs -rpvf <client-hostname>_transfer.tar {} \;
   # find /etc/puppetlabs/code/environments -name "<client-fqdn>.yaml" -exec tar --selinux --xattrs -rpvf <client-hostname>_transfer.tar {} \;

Remove all of the node specific Hiera data:

.. code-block:: shell

   # find /etc/puppetlabs/code/environments -name "<client-fqdn>.yaml" --delete

.. NOTE::

   You may have Hiera YAML files with the short name of the host still in place
   but those are too dangerous to automatically delete since they may match
   multiple hosts.

Reload the ``puppetserver`` process:

.. code-block:: shell

   # puppetserver_reload

On the New Puppet Master
------------------------

.. WARNING::

   This assumes that the new Puppet master is set up identically to the old
   Puppet master. If it is not, you will need to verify that the artifacts in
   the ``tar`` file are correctly placed.

Unpack the ``<client-hostname>_transfer.tar`` archive onto the system:

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

Remove the Client Puppet Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To remove all legacy SSL material, run ``rm -rf `puppet config --section agent ssldir```

Update the Puppet Config
^^^^^^^^^^^^^^^^^^^^^^^^
.. NOTE::
  If upgrading from SIMP 4 or 5 to SIMP 6 you will need to upgrade your puppet agent
  to the Puppet 4.0 agent before it can connect to the new Puppet master.  A fix is being
  worked under SIMP-3049.  If you installed from the ISO, the simp repo on the SIMP 6
  server contains the correct rpm.  Point to the correct repo and run
  ``yum install puppet-agent``.  This will also remove the old version.

Enter the following changes into ``/etc/puppetlabs/puppet/puppet.conf``.

.. code-block:: ini

  server = new.puppet.master.fqdn
  ca_server = new.puppet.master.fqdn
  ca_port = 8141

Run Puppet
^^^^^^^^^^

Assuming the new Puppet master has been set up to properly accept the
client, execute a full Puppet run using ``puppet agent --test``.

If everything was done properly, the client will now be synchronized with the
new Puppet master.

If you find issues, refer to the :ref:`Client_Management` section of the
documentation and ensure that the new Puppet master was set up properly.

.. _ug-howto-change-puppet-masters:

HOWTO Move a Client to a new Puppet Server
==========================================

The following provides details on how to move an client to a new :term:`Puppet Server`.

.. NOTE::

   All commands in this section should be run as the ``root`` user.

On the Old Puppet Server
------------------------

The following procedures will archive the agent's artifacts from all environments, copy them to the new
Puppet Server, and clean out the agent's :term:`Hiera` data.

Archive the agent's artifacts from all environments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Archive the agent's artifacts from all SIMP :term:`Secondary Environments <SIMP Secondary Environment>`:

  .. code-block:: shell

     find /var/simp/environments -name "*<agent-fqdn>*" -exec tar --selinux --xattrs -rpvf <agent-fqdn>_transfer.tar {} \;

2. Archive the agent's data from all SIMP :term:`Writable Environments <SIMP Writable Environment>`:

  .. code-block:: shell

     find `puppet config --section server print vardir`/simp -name "*<agent-fqdn>*" -exec tar --selinux --xattrs -rpvf <agent-fqdn>_transfer.tar {} \;


3. Archive the agent's Hiera data from all :term:`Puppet Environments`:

  .. WARNING::

     If you deploy your agents' Hiera data from a :term:`Control Repository <Control Repo>` on
     the new Puppet server, ensure the agent's Hiera data is in the relevant
     branches and **skip this step.**

  .. code-block:: shell

     find /etc/puppetlabs/code/environments/*/{data,hieradata} -name "<agent-hostname>.yaml" -exec tar --selinux --xattrs -rpvf <agent-hostname>_transfer.tar {} \;
     find /etc/puppetlabs/code/environments/*/{data,hieradata} -name "<agent-fqdn>.yaml" -exec tar --selinux --xattrs -rpvf <agent-hostname>_transfer.tar {} \;

  4. Copy :file:`<agent-hostname>_transfer.tar` to the new Puppet server.


Remove agent-specific Hiera data from all environments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. WARNING::

   **Skip this section** if you deploy your agents' Hiera data from
   a :term:`Control Repository`

1. Remove agent-specific Hiera data from all environments

  .. code-block:: shell

     find /etc/puppetlabs/code/environments -name "<agent-fqdn>.yaml" --delete

  .. NOTE::

     You may have Hiera YAML files with the short name of the host still in
     place, but those are too dangerous to automatically delete since they may
     match multiple hosts.

2. Reload the :program:`puppetserver` process after removing the agent's Hiera data:

  .. code-block:: shell

     puppetserver reload

On the New Puppet Server
------------------------

.. WARNING::

   This assumes that the new Puppet server is set up identically to the old
   Puppet server. If it isn't, you will need to verify that the artifacts in
   the ``tar`` file are correctly placed.

1. Unpack the :file:`<agent-hostname>_transfer.tar` archive onto the system:

  .. code-block:: shell

     tar --selinux --xattrs -C / -xvf <agent-hostname>_transfer.tar

2. Reload the :program:`puppetserver` process:

  .. code-block:: shell

     puppetserver reload

On The Agent
------------

.. IMPORTANT::

   Make sure you are running these commands **on the agent**. If you run them
   on the server, there is a **very high risk** they will make your Puppet
   infrastructure inoperable.

Remove the Agent Puppet Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To remove all legacy SSL files, run:

.. code-block:: shell

   rm -rf $(puppet config print --section agent ssldir)

Update the Puppet Config
^^^^^^^^^^^^^^^^^^^^^^^^

Update :file:`/etc/puppetlabs/puppet/puppet.conf` with the following changes:

.. code-block:: ini

   server = new.puppet.server.fqdn
   ca_server = new.puppet.server.fqdn
   ca_port = 8141

Run Puppet
^^^^^^^^^^

Assuming the new Puppet server has been set up to properly accept the
agent, execute a full Puppet run using :command:`puppet agent --test`.

On the new puppet server you will need to sign off the certificate for the new client
using :command:`puppetserver ca cert sign <new client name`.

If everything was done properly, the agent will now be synchronized with the
new Puppet server.

If you find issues, refer to the :ref:`cm-setting-up-the-client` and
:ref:`cm-troubleshoot-puppet-issues` sections of the documentation, and ensure
that the new Puppet CA is set up properly to trust the Puppet agent.

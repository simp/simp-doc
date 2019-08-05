.. _Examples for Deploying SIMP Environments:

Examples for Deploying SIMP Environments
----------------------------------------

.. _Deploy an Environment Using Local Module Repositories:

Create and Deploy a Test Environment Using Local Module Repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This example uses the :ref:`Local deployment scenario<ug-local_module_deployment_scenario>`
It steps through creating a new :term:`SIMP Omni-Environment`,
deploying the SIMP modules and  add nodes to the new environemnt.

It assumes:

* You have upgraded SIMP-packaged Puppet module RPMs via ``yum``.
* You have updated some of your own Puppet modules.
* You want to create a ``test`` Puppet environment with both sets of the changes.
* You want to assign a subset of nodes to this new environment to test the
  changes.



#. **Create the new SIMP Omni-Environment**

   Run the ``simp`` command to create a :term:`SIMP Omni-Environment`
   called  ``test`` for which the SIMP :term:`secondary <SIMP Secondary Environment>` and
   :term:`writable <SIMP Writable Environment>` environments are linked to the
   corresponding directories in the existing ``production`` environment.  This
   will allow the two environments to use common passwords and PKI certificates.

   .. code-block:: sh

      simp environment new test --link production

   This will

   * Create the Puppet environment ``test``
   * Copy over the entire ``production`` Puppet environment over to the ``test``
     environment.
   * Link the ``test`` secondary  environment to the ``production`` secondary
     environment.
   * Link the ``test`` writable  environment to the ``production`` writable
     environment.

#. **Update the Puppetfile Pair**

   Edit /etc/puppetlabs/code/environment/test/Puppetfile and add/update
   any of you own  puppet modules that you are updating.

   Then update /etc/puppetlabs/code/environment/test/Puppetfile.simp using the
   following:

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/test
      simp puppetfile generate > Puppetfile.simp


#. **Deploy the Modules**

   Use r10k to update the modules in the test environment:

   .. code-block:: sh

      ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
      --puppetfile /etc/puppetlabs/code/environments/test/Puppetfile \
      --moduledir /etc/puppetlabs/code/environments/test/modules' )

#. **Assign nodes to the new environment**

   Since the secondary and writable environments for the ``production``
   and ``test`` SIMP Omni-Environments are linked in this example, you can move
   nodes between these two environments without generating new certificates.

   You can assign the node to the new environment by changing the ``environment``
   setting in the node's ``/etc/puppetlabs/puppet/puppet.conf`` file, or, if you are
   using :ref:`the ENC supplied the simp-util module<howto-simp-enc>`, adding a line
   like the following to ``/etc/puppetlabs/puppet/environments.yaml``.

   .. code-block:: yaml

      node.my.domain: test

#. **Apply node configuration**

   To apply the configuration for a node in the ``test`` environment, simply run

   .. code-block:: sh

      puppet agent -t

   You should see a message similar to the following at the beginning of
   the log for the Puppet run:

  ``Info: Using configured environment 'test'``

   .. NOTE::

      Once you have deployed a Puppet environment, you can edit the modules in that
      Puppet environment directly to debug issues you may be having and run puppet
      agent -t to deploy those changes.  However, the next time you use ``r10k``
      to deploy the modules, it will **overwrite** any local changes, unless the module
      is defined as ``:local => true`` in the Puppetfile.  It is recommended that you
      use ``git`` to manage your work flow.


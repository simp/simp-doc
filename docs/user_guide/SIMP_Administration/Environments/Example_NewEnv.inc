Example: Create and Deploy a New Environment
--------------------------------------------

This section steps through creating a new  :term:`SIMP Omni-Environment`,
deploying the ``simp`` modules and configuring an :term:`ENC` to move nodes
to the new environment.

See :ref:`ug-sa-simp-environments` for more information on the
:term:SIMP OMNI Environment.

This example uses the ``Local`` deployment scenario.  See :ref:`ug-deployment_scenarios`
for more information on deployment scenarios.

Description of Example
^^^^^^^^^^^^^^^^^^^^^^

You have installed, via yum, updates to the SIMP RPMs and have
also updated some of your own git modules to test.
You want to create a ``test`` environment and a set of those nodes to
that this environment for testing.

Remember, if you install updates to simp modules via RPM, they are not installed in
any environment until you update the Puppetfile for that environment and deploy it.

Create the :term:`SIMP Omni-Environment`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In this example the new environment, called ``test``, is created linking the
secondary and writable environments to the existing production environment.
This will enable the environments to use common passwords and certificates.
Run the command

.. code-block:: sh

   simp environment new test --link production

This has created the Puppet environment ``test`` and copied over the  Puppet files,
hiera data and modules from the production environment.  The secondary and writable
environments have been linked to the production environments.

Update Puppetfile and Deploy modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Replace ``<name of your environment>`` with ``test`` for this example.

.. include:: Environments/Update_and_Deploy_Local_Environment.inc

Add nodes to the New Environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Since the secondary and writable enviroments for production and test are linked
in this example you can move nodes between them without generating new certificates.

.. include:: Environments/ENC_setup.inc

.. NOTE::

   Once you have deployed an environment you can edit the modules directly without
   affecting the other environments, but when you run r10k again, it will overwrite
   any local changes unless the module is defined as ``:local => true`` in the
   Puppetfile.  It is recommended that you use ``git`` to manage your work flow.

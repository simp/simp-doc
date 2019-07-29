.. _example_create_and_deploy_new_env:

Example: Create and Deploy a New Environment
--------------------------------------------

This section steps through creating a new :term:`SIMP Omni-Environment`,
deploying the SIMP modules and configuring an
:term:`External Node Classifier (ENC) <ENC>` to assign nodes to the new
environment.

See :ref:`ug-sa-simp-environments` for more information on the
:term:`SIMP Omni-Environment`.

This example uses the :ref:`Local deployment scenario<ug-local_module_deployment_scenario>`
and a simple, SIMP-provided ENC.  It assumes:

* You have upgraded SIMP-packaged Puppet module RPMs via ``yum``.
* You have updated some of your own Puppet modules.
* You want to create a ``test`` Puppet environment with both sets of the changes.
* You want to assign a subset of nodes to this new environment to test the
  changes.

.. admonition:: REMINDER

   As of SIMP 6.4, if you install updates to SIMP modules via RPM, they are
   **not** installed in any Puppet environment until you update the
   :term:`Puppetfile` for that environment and then deploy it.

STEP 1: Create the new :term:`SIMP Omni-Environment`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In this example we create a new environment, ``test``, for which the
SIMP :term:`secondary <SIMP Secondary Environment>` and
:term:`writable <SIMP Writable Environment>` environments are linked to the
corresponding directories in the existing ``production`` environment.  This
will allow the two environments to use common passwords and PKI certificates.

Run the following command:

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

STEP 2: Update the Puppetfile pair and deploy modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. include:: Environments/Update_and_Deploy_Local_Environment.inc

STEP 3: Add nodes to the new environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Since the secondary and writable environments for the ``production``
and ``test`` SIMP Omni-Environments are linked, in this example, you can move
nodes between these two environments without generating new certificates.

.. include:: Environments/ENC_setup.inc

.. NOTE::

   Once you have deployed a Puppet environment, you can edit the modules in that
   Puppet environment, directly, to debug issues you may be having.  However, the
   next time you use ``r10k`` to deploy the modules, it will **overwrite** any
   any local changes, unless the module is defined as ``:local => true`` in the
   Puppetfile.  It is recommended that you use ``git`` to manage your work flow.

STEP 4: Apply node configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To apply the configuration for a node in the ``test`` environment, simply run
``puppet agent -t`` on that node, or wait for that node's Puppet ``cronjob`` to
execute. You should see a message similar to the following at the beginning of
the log for the Puppet run:

  ``Info: Using configured environment 'test'``


.. _ug-environments-deploying-from-local-repositories:

Deploying an Environment from Local Git Repositories
----------------------------------------------------

When you are using the :ref:`Local deployment
scenario<ug-sa-env-deployment-scenarios--local>`
the following steps can be used to update and deploy puppet modules from
the local git repositories to an existing environment.

The examples in the following steps use an enviroment called :file:`test`.

Update SIMP Local Git Repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   Add any new or updated SIMP module rpms to the yum repositories using
   your sites procedures. Use :code:`yum install  <module name>` to
   install the module.  This will update the local git repositories.

Update the Environment
^^^^^^^^^^^^^^^^^^^^^^

.. include:: ../common/Update_and_Deploy_Local_Environment.inc

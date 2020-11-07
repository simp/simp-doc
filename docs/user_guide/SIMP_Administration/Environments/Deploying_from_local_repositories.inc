.. _ug-environments-deploying-from-local-repositories:

Deploying an Environment from Local Git Repositories
----------------------------------------------------

An existing environment may need to be deployed to add new modules or
update additional ones.  This example uses an enviroment named ``test``.


#. Update SIMP local Repositories.

   Add any new or updated SIMP module rpms to the yum repositories using
   your sites procedures.

   Use yum to install/update the SIMP modules.

   .. code-block:: sh

      # The following are examples.
      # To install the modules in simp-extras
      yum install simp-extras
      # To update a single module
      yum install pupmod-simp-simp_nfs

   This will install/update the puppet modules in the local Git repositories.

.. include:: ../common/Update_and_Deploy_Local_Environment.inc

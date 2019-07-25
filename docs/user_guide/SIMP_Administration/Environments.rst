.. _Deploying SIMP Environments:

Deploying SIMP Environments
===========================

SIMP fully supports `Puppet Environments`_, and extends the practice by
providing parallel environment directories to manage data that does not belong
in a control repository.

This section describes how SIMP interacts with Puppet environments, and how to
integrate SIMP environments into your site's deployment strategy.

.. include:: Environments/SIMP_Environments.inc
.. include:: Environments/Deployment_Scenarios.inc
.. include:: Environments/Local_Module_Repositories.inc
.. include:: Environments/SIMP_CLI_Environment_Management.inc
..
  TODO:

  Here are some probable topics to consider

 .. include::  Environments/Deploying_Simp_Environments.inc
 .. include::  Environments/Deploying_from_local_repositories.inc
 .. include::  Environments/Deploying_from_local_repositories/The_simp_environment_command.inc
 .. include::  Environments/Deploying_from_local_repositories/The_simp_puppetfile_command.inc

 // optional, covers later deployment scenarios
 .. include::  Environments/Deploying_from_local_repositories.inc

.. _Puppet Environments: https://docs.puppet.com/puppet/latest/environments.html


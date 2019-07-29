.. _ug-deployment_scenarios:


Deployment Scenarios
--------------------

.. |r10k_local_repos__no_control_repo| image:: ../../images/diagrams/deployment_scenarios/r10k_local_repos__no_control_repo.png
    :alt: r10k + local, SIMP RPM-delivered module repositories

.. |r10k__control_repos| image:: ../../images/diagrams/deployment_scenarios/r10k__control_repos.png
    :alt: r10k + Control Repository

.. |enterprise__automated_deploy| image:: ../../images/diagrams/deployment_scenarios/enterprise__automated_deploy.png
    :alt: PE Code Manager + Control Repository

.. _ug-local_module_deployment_scenario:

Local: r10k + SIMP RPM-delivered module repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. NOTE::

  This is the initial deployment scenario immediately after
  :ref:`gsg-installing_simp_from_an_iso`.

|r10k_local_repos__no_control_repo|

Under this scenario, *local* Puppet admins:

* deploy Puppet modules from an existing Puppet environment directory
* use the command ``r10k puppetfile install`` to deploy the modules defined in
  the local ``Puppetfile``
* reference module repos from Git repositories on the local filesystem
  via ``file://`` urls in the ``Puppetfile``.

Professional: r10k + Control Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

|r10k__control_repos|

.. NOTE::

   Administrators of all control repository scenarios are assumed to have enough
   git and r10k (or Code Manager) expertise to manage their own infrastructure.
   This includes arranging their own external git-hosting service, managing their
   own control repository, and updating their modules repositories with the
   contents delivered on the local filesystem by SIMP Puppet module RPMs.

Under this scenario, Puppet admins:

* maintain a Puppet control repository from an external git service
* deploy entire Puppet environment directories (one environment for each
  repository branch) using ``r10k deploy environment [ENVIRONMENT]``.

  * This might be done manually from the command line, or triggered
    automatically (e.g., from a webhook).

Enterprise: PE Code Manager + Control Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

|enterprise__automated_deploy|

Under this scenario, Puppet admins:

* maintain a Puppet control repository from an external git service
* deploy entire Puppet environment directories (one environment for each
  repository branch) using PE Code Manager.

  * This is triggered automatically from a Code Manager webhook.

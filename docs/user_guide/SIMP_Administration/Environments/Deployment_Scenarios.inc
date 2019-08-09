.. _ug-sa-env-deployment-scenarios:

.. ### Using some unicode
.. include:: <isonum.txt>
.. include:: <isopub.txt>

..  ### Replacement text (to keep the grid-style tables manageable):

.. |local_ds__link|     replace:: :ref:`Local module repos <ug-sa-env-deployment-scenarios--local>`
.. |man_crepo_ds__link| replace:: :ref:`Manual r10k <ug-sa-env-deployment-scenarios--controlrepo>`
.. |wh_crepo_ds__link|  replace:: :ref:`Webhook + r10k <ug-sa-env-deployment-scenarios--controlrepo>`
.. |pe_crepo_ds__link|  replace:: :ref:`PE Code Manager <ug-sa-env-deployment-scenarios--cr+pe-codemanager>`
.. |local_mods__link|   replace:: Local :ref:`(from RPMs) <ug-sa-env-local-module-repositories>`
.. |remote_mods__link|  replace:: Remote (Git hosting service)
.. |r10k_de|  replace:: ``r10k deploy environment``
.. |r10k_pi|  replace:: ``r10k puppetfile install``
.. |r10k_pcd| replace:: ``puppet code deploy``

..  ### Images

.. |r10k_local_repos__no_control_repo| image:: ../../images/diagrams/deployment_scenarios/r10k_local_repos__no_control_repo.png
    :alt: r10k + local, SIMP RPM-delivered module repositories
.. |r10k__control_repo__manual| image:: ../../images/diagrams/deployment_scenarios/r10k__control_repo__manual.png
    :alt: r10k + Control Repository (manual deploy)
.. |r10k__control_repo__webhook| image:: ../../images/diagrams/deployment_scenarios/r10k__control_repo__webhook.png
    :alt: r10k + Control Repository (webhook deploy)
.. |enterprise__automated_deploy| image:: ../../images/diagrams/deployment_scenarios/enterprise__automated_deploy.png
    :alt: PE Code Manager + Control Repository

Deployment Scenarios
--------------------

SIMP supports deploying code using r10k or Code Manager, even when its Puppet
modules are installed from RPMs.  However, deployment scenarios vary widely.
The optimal practices will depend on your preferences, infrastucture, and
deployment architecture:

.. table:: Deployment Scenarios
   :widths: 7 10 10 3 7 3

   +------------+----------------------+------------+-------------+---------------------+---------+
   | Deployment | Variation            | r10k       | Control     | Module Repos        | Auto?   |
   | Scenario   |                      | command    | Repository? |                     |         |
   +============+======================+============+=============+=====================+=========+
   | Local      | |local_ds__link|     | |r10k_pi|  | No          | |local_mods__link|  |         |
   +------------+----------------------+------------+-------------+---------------------+---------+
   | Control    | |man_crepo_ds__link| | |r10k_de|  | Yes         | |remote_mods__link| |         |
   | Repository +----------------------+------------+             |                     +---------+
   |            | |wh_crepo_ds__link|  | |r10k_de|  |             |                     | |check| |
   |            +----------------------+------------+             |                     +---------+
   |            | |pe_crepo_ds__link|  | |r10k_pcd| |             |                     | |check| |
   +------------+----------------------+------------+-------------+---------------------+---------+

.. _ug-sa-env-deployment-scenarios--local:

Local deployment scenario
^^^^^^^^^^^^^^^^^^^^^^^^^

The :dfn:`Local deployment scenario` requires the least preparation or
infrastructure: it doesn't require a control repository, a remote Git-hosting
service, or even a functioning network.

The local deployment scenario is recommended for:

* Systems that don't have access to a git-hosting service
* Users who prefer to treat the SIMP framework as a vendor-packaged appliance
* Users who prefer to avoid maintaining a control repository

The local deployment scenario requires:

* A SIMP master that has been installed from ISO or RPM
* SIMP RPM-delivered :ref:`Local Module Git Repositories <ug-sa-env-local-module-repositories>`
* The ``simp`` command
* No additional compile masters.

.. IMPORTANT::
   The local deployment scenario is only suitable for a single monolithic SIMP
   master.

.. NOTE::
   If you need to scale beyond a single master, you should :ref:`deploy
   from a control repository <ug-sa-env-deployment-scenarios--controlrepo>`.

Deploying from Local module repos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is the initial deployment scenario immediately after
:ref:`gsg-installing_simp_from_an_iso`.

|r10k_local_repos__no_control_repo|

Under this scenario, a *local* r10k user will:

* Use the command ``r10k puppetfile install`` to deploy the modules defined in
  the local ``Puppetfile``
* Deploy Puppet modules into an existing Puppet environment directory

This is is the only scenario variation that deploys Puppet *modules* into an
existing Puppet environment directory, instead of deploying a complete
environment.

The ``Puppetfile`` references modules using ``file://`` urls, from Git
repositories on the local filesystem


.. _ug-sa-env-deployment-scenarios--controlrepo:

Control Repository deployment scenarios
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. NOTE::

   Administrators of all control repository scenarios are assumed to have enough
   git and r10k (or Code Manager) expertise to manage their own infrastructure.
   This includes:

     * Managing a :term:`Control Repository`
     * Maintaining :term:`git` repositories in a remote git-hosting service
     * Keeping remote Puppet module repositories up-to-date with the
       versions that SIMP RPMs deliver to the local filesystem.

Manual ``r10k deploy``
~~~~~~~~~~~~~~~~~~~~~~

|r10k__control_repo__manual|

Under this scenario, Puppet admins:

* Maintain a Puppet control repository from an external git service
* Manually Deploy entire :term:`Puppet environment` directories (one per
  branch):

  * ``r10k deploy environment [ENVIRONMENT]``
  * ``r10k puppet code deploy [ENVIRONMENT]`` (Puppet Enterprise)
* This might be done manually from the command line, or triggered
  automatically (e.g., from a webhook).


.. _ug-sa-env-deployment-scenarios--webhook+controlrepo:

Webhook-triggered ``r10k deploy``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

|r10k__control_repo__webhook|

Under this scenario, Puppet admins:


* Maintain a Puppet control repository from an external git service
* Deploy entire Puppet environment directories (one environment for each
  repository branch) using PE Code Manager.

  * This is triggered automatically from a Code Manager webhook.

.. _ug-sa-env-deployment-scenarios--cr+pe-codemanager:

PE Code Manager
^^^^^^^^^^^^^^^

|enterprise__automated_deploy|

Under this scenario, Puppet admins:


* Maintain a Puppet control repository from an external git service
* Deploy entire Puppet environment directories (one environment for each
  repository branch) using PE Code Manager.

  * This is triggered automatically from a Code Manager webhook.

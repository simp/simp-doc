.. _ug-sa-env-deployment-scenarios:

.. ### Using some unicode

.. include:: <isonum.txt>
.. include:: <isopub.txt>

..  ### Replacement text (to keep the grid-style tables manageable):

.. |local_dsc__link|    replace:: :ref:`Local module repos <ug-sa-env-deployment-scenarios--local>`
.. |crepo_dsc__link|    replace:: :ref:`Manual r10k <ug-sa-env-deployment-scenarios--controlrepo>`
.. |wh_crepo_dsc__link| replace:: :ref:`Webhook + r10k <ug-sa-env-deployment-scenarios--controlrepo>`
.. |pe_crepo_dsc__link| replace:: :ref:`PE Code Manager <ug-sa-env-deployment-scenarios--cr+pe-codemanager>`
.. |local_mods__link|   replace:: Local :ref:`(from RPMs) <ug-sa-env-local-module-repositories>`
.. |remote_mods__link|  replace:: Remote (Git hosting service)
.. |r10k_de| replace:: ``r10k deploy environment``
.. |r10k_pi| replace:: ``r10k puppetfile install``
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
modules are installed from RPMs.  However, site practices vary widely—the
optimal practices for deploying SIMP will vary, depending on your deployment
arcitecture.

.. table:: Deployment Scenarios
   :widths: 7 10 10 3 7 3

   +------------+----------------------+------------+-------------+---------------------+---------+
   | Deployment | Variation            | r10k       | Control     | Module Repos        | Auto?   |
   | Scenario   |                      | command    | Repository? |                     |         |
   +============+======================+============+=============+=====================+=========+
   | Local      | |local_dsc__link|    | |r10k_pi|  | No          | |local_mods__link|  |         |
   +------------+----------------------+------------+-------------+---------------------+---------+
   | Control    | |crepo_dsc__link|    | |r10k_de|  | Yes         | |remote_mods__link| |         |
   | Repository +----------------------+------------+             |                     +---------+
   |            | |wh_crepo_dsc__link| | |r10k_de|  |             |                     | |check| |
   |            +----------------------+------------+             |                     +---------+
   |            | |pe_crepo_dsc__link| | |r10k_pcd| |             |                     | |check| |
   +------------+----------------------+------------+-------------+---------------------+---------+

.. _ug-sa-env-deployment-scenarios--local:

Local deployment scenarios
^^^^^^^^^^^^^^^^^^^^^^^^^^

Local deployment scenarios will work out-of-the-box on systems where SIMP
has been installed by ISO or RPM.  You don't need a control repository, a
remote Git-hosting service, or even a functioning network

relies on SIMP RPM-created Puppet module Git repository

Deploying local (SIMP RPM-delivered) Puppet modules
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is the initial deployment scenario immediately after
:ref:`gsg-installing_simp_from_an_iso`.

|r10k_local_repos__no_control_repo|

Under this scenario, *local* Puppet admins:

* Deploy Puppet modules from an existing Puppet environment directory
* Use the command ``r10k puppetfile install`` to deploy the modules defined in
  the local ``Puppetfile``
* Reference module repos from Git repositories on the local filesystem
  via ``file://`` urls in the ``Puppetfile``.

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

Manual deployment
~~~~~~~~~~~~~~~~~

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

Webhook deploying from Code Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
|r10k__control_repo__webhook|

Under this scenario, Puppet admins:


* Maintain a Puppet control repository from an external git service
* Deploy entire Puppet environment directories (one environment for each
  repository branch) using PE Code Manager.

  * This is triggered automatically from a Code Manager webhook.

.. _ug-sa-env-deployment-scenarios--cr+pe-codemanager:

Deploying from Code Manager
^^^^^^^^^^^^^^^^^^^^^^^^^^^
|enterprise__automated_deploy|

Under this scenario, Puppet admins:


* Maintain a Puppet control repository from an external git service
* Deploy entire Puppet environment directories (one environment for each
  repository branch) using PE Code Manager.

  * This is triggered automatically from a Code Manager webhook.

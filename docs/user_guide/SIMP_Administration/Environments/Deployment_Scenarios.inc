.. _ug-sa-env-deployment-scenarios:

.. ### Using some unicode
.. include:: <isonum.txt>
.. include:: <isopub.txt>

..  ### Replacement text (to keep the grid-style tables manageable):

.. |local_ds__link|      replace:: :ref:`Local        <ug-sa-env-deployment-scenarios--local>`
.. |crepo_ds__link|      replace:: :ref:`Control Repo <ug-sa-env-deployment-scenarios--controlrepo>`

.. |local_dsv__link|     replace:: :ref:`Local module repos <ug-sa-env-deployment-scenarios--local--variant>`
.. |man_crepo_dsv__link| replace:: :ref:`Manual r10k        <ug-sa-env-deployment-scenarios--crepo+manual-r10k--variant>`
.. |wh_crepo_dsv__link|  replace:: :ref:`Webhook + r10k     <ug-sa-env-deployment-scenarios--crepo+webhook--variant>`
.. |pe_crepo_dsv__link|  replace:: :ref:`PE Code Manager    <ug-sa-env-deployment-scenarios--cr+pe-codemanager--variant>`

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

SIMP supports deploying Puppet code using r10k or Code Manager, even when its
modules are installed from RPMs.  However, deployment strategies vary widely,
based on a site's infrastructure, existing policies, and administrative
considerations.
SIMP broadly groups these strategies into high-level
:dfn:`deployment scenarios`, with some important variations:

.. # COMING SOON:
   |bolt_ds__link|
     Deploying Puppet environments on remote systems using :term:`Puppet bolt`

.. table:: Deployment Scenarios and Variations
   :widths: 7 10 10 3 7 3

   +----------------------+-----------------------+------------+-------------+---------------------+--------------+
   | Deployment           | Variation             | r10k       | Control     | Module Repos        | Autodeploy?  |
   | Scenario             |                       | command    | Repository? |                     |              |
   +======================+=======================+============+=============+=====================+==============+
   | |local_ds__link|     | |local_dsv__link|     | |r10k_pi|  | No          | |local_mods__link|  |              |
   +----------------------+-----------------------+------------+-------------+---------------------+--------------+
   | |crepo_ds__link|     | |man_crepo_dsv__link| | |r10k_de|  | Yes         | |remote_mods__link| |              |
   |                      +-----------------------+------------+             |                     +--------------+
   |                      | |wh_crepo_dsv__link|  | |r10k_de|  |             |                     | |check|      |
   |                      +-----------------------+------------+             |                     +--------------+
   |                      | |pe_crepo_dsv__link|  | |r10k_pcd| |             |                     | |check|      |
   +----------------------+-----------------------+------------+-------------+---------------------+--------------+

Regardless of the deployment scenario and variation employed, it is important
to ensure that each Puppet environment is associated with an appropriate
:term:`SIMP Omni-environment`, or that the data provided by SIMP's "Extra"
environments are handled by alternate means.

.. _ug-sa-env-deployment-scenarios--local:

Local deployment scenario
^^^^^^^^^^^^^^^^^^^^^^^^^
The :dfn:`Local deployment scenario` deploys Puppet **modules** from
:ref:`ug-sa-env-local-module-repositories` into an existing Puppet
environment's ``modules/`` directory.

This scenario requires the least preparation: it doesn't require a control
repository, a remote git-hosting service, or even a functioning network.  It
also works automatically on any monolithic SIMP master (6.4.0+) that was
installed from ISO or RPM.  In fact, the initial run of ``simp config`` uses
this method to deploy the system's first modules into the production
environment.

The local deployment scenario is recommended for:

* Systems that don't have access to a git-hosting service
* Users who prefer to treat the SIMP framework as a vendor-packaged appliance
* Users who prefer to avoid maintaining a control repository

It requires:

* SIMP RPM-delivered :ref:`Local Module Git Repositories <ug-sa-env-local-module-repositories>`
* The ``simp`` command
* A pre-existing Puppet environment directory (``simp environment new``)
* A simp Puppetfile (``simp puppetfile generate``)
* No additional compile masters.

.. IMPORTANT::
   The local deployment scenario is only suitable for a single monolithic SIMP
   master.

.. NOTE::
   If you need to scale beyond a single master, you should :ref:`deploy
   from a control repository <ug-sa-env-deployment-scenarios--controlrepo>`.


.. _ug-sa-env-deployment-scenarios--local--variant:

Deploying from Local module repos
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This is the initial deployment scenario immediately after
:ref:`gsg-installing_simp_from_an_iso`.


|r10k_local_repos__no_control_repo|

Under this scenario, a *local* r10k user will:

* ``cd`` into an existing Puppet environment directory
* Use the command ``r10k puppetfile install`` to deploy the modules defined in
  the local ``Puppetfile`` (which may include a separate ``Puppetfile.simp``)

Notes:

* The Puppet environment directory and ``Puppetfile`` must exist before ``r10k
  puppetfile install`` is run
* The modules in the ``Puppetfile`` are defined with ``file://`` URLs, and pull from
  Git repositories on the local file system


.. _ug-sa-env-deployment-scenarios--controlrepo:

Control Repository deployment scenario
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :dfn:`Control Repository deployment scenario` deploys complete Puppet
**environments** from a remotely-hosted :term:`Control Repository`.

SIMP administrators must ensure that each deployed Puppet environment is
associated with an appropriate :term:`SIMP Omni-environment` (or doesn't need
to be).  A good starting point is to review the article,
:ref:`howto-setup-a-simp-control-repository`

The Control repository deployment scenario is recommended for:

* Environments that manage their infrastructure as code
* Infrastructures that need to scale beyond a single master (e.g., compile masters)
* Administrators who are comfortable with git and Puppet control repositories
* Puppet Enterprise users

It requires:

* A remote git-hosting service
* A (remotely-hosted) :term:`Control Repository`
* :term:`r10k` and/or :term:`Code Manager`, configured to deploy from the Control Repository
* Keeping all remote Puppet module repositories (and the control repository's
  ``Puppetfile``) up-to-date as SIMP RPMs deliver new module versions to the
  master's local filesystem

Depending on the variation, it may also require:

* (optional) a local webhook on the Puppet master, configured to trigger the code deploy
* (optional) a webhook on the git-hosting service to trigger environment
  deployments on the Puppet master after a control repository branch is updated

.. IMPORTANT::

   Control repository deployment scenario administrators are expected to:

   * Have sufficient expertise to understand and manage the requirements above
   * Review and understand the documentation at
     :ref:`howto-setup-a-simp-control-repository`
   * Understand the relationship between a deployed Puppet environment and the
     larger :term:`SIMP Omni-environment`


.. _ug-sa-env-deployment-scenarios--crepo+manual-r10k--variant:

Manual ``r10k deploy``
~~~~~~~~~~~~~~~~~~~~~~

|r10k__control_repo__manual|

Under this scenario:

* Puppet admins maintain a Puppet control repository (and Puppet module
  repositories) in an external git-hosting service.

* An r10k user logs into the Puppet master and manually deploys entire
  :term:`Puppet environment` directories (one per branch) from the command
  line:

  * ``r10k deploy environment [ENVIRONMENT]``
  * ``r10k puppet code deploy [ENVIRONMENT]`` (Puppet Enterprise)


.. _ug-sa-env-deployment-scenarios--crepo+webhook--variant:

Webhook-triggered ``r10k deploy``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

|r10k__control_repo__webhook|

Under this scenario, Puppet admins:

* Maintain a Puppet control repository (and Puppet module repositories) in an
  external git-hosting service
* Deploy a Puppet environment by pushing updates to a control repository branch

When a control repository branch is updated, it automatically
triggers an environment deployment on the Puppet master:

  * The Git-hosting service uses a webhook to notify the Puppet master about
    the branch that was changed
  * The webhook on the Puppet master triggers r10k to deploy the branch as an
    environment

.. _ug-sa-env-deployment-scenarios--cr+pe-codemanager--variant:

PE Code Manager
^^^^^^^^^^^^^^^

|enterprise__automated_deploy|

Under this scenario, Puppet admins:

* Maintain a Puppet control repository (and Puppet module repositories) in an
  external git-hosting service
* Deploy a Puppet environment by pushing updates to a control repository branch

When a control repository branch is updated, it automatically
triggers an environment deployment on the Puppet master:

  * The Git-hosting service uses a webhook to notify the PE master/MoM about
    the branch that was changed
  * The webhook on the Puppet master triggers PE Code Manager to deploy the branch
    as a staged environment
  * The PE File Sync service distributes the staged environment to any compile
    masters, and promotes the code from staging to live use.

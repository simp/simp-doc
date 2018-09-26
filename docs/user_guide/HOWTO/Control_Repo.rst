.. _howto-setup-a-simp-control-repository:

HOWTO Set up a SIMP Control Repository
======================================

A control repository contains the modules, hieradata, and roles/profiles
required in a Puppet infrastructure.  Managing the control repo with GIT allows
sysadmins to utilize a workflow when updating and developing their
infrastructure.

.. NOTE::

  Refer to Puppet, Inc.'s `control repository documentation`_ for more
  information.

SIMP distributes a partial control repository:

* On the filesystem of an installed SIMP system:

.. code-block:: bash

  # tree -L 1 /usr/share/simp/environments/simp/
  /usr/share/simp/environments/simp/
  ├── environment.conf
  ├── FakeCA
  ├── hieradata
  ├── manifests
  └── modules

* In our `environment repository`_ :

.. code-block:: bash

  # tree -L 1 src/assets/simp-environment/environments/simp
  src/assets/simp-environment/environments/simp
  ├── environment.conf
  ├── hieradata/
  └── manifests/

To begin creating your control repository, make a directory, say ``r10k_production``,
and copy in the contents of the ``simp-environment-skeleton`` or
``environments/simp`` from a live system, depending on your needs.

Modules are defined in a Puppetfile.  We keep up-to-date Puppetfiles in the
base of our `simp-core repository`_.  For best results, download
``Puppetfile.stable`` to the base of the ``r10k_production`` directory, using the
following snippet:

.. code-block:: bash

  # curl -o Puppetfile https://github.com/simp/simp-core/blob/<release>/Puppetfile.stable

.. NOTE::

  The example Puppetfile is labeled stable, meaning that the versions of the
  modules it contains are the ones contained in the last SIMP release.  You can
  go to any previous release and download a Puppetfile with references to older
  modules from the git history of the simp-core repo.

Our Puppetfile pulls down every dependency SIMP needs, including more than just
Puppet modules.  Remove non-Puppet modules by editing the downloaded Puppetfile
and erasing the lines ``moduledir 'src'`` to ``moduledir 'src/puppet/modules``.

If want your data layer to be SIMP-like, create a ``hiera.yaml`` file at the
base of the ``r10k_production`` directory, and add the following content:

.. NOTE::

  For more information about data in SIMP, see the
  :ref:`Classification and Data` documentation.

.. code-block:: yaml

  ---

  # This is the default hiera.yaml file
  # Feel free to modify the hierarchy to suit your needs but please
  # leave the simp* entries in place at the bottom of the list
  :backends:
    - 'yaml'
    - 'json'
  :hierarchy:
    - 'hosts/%{trusted.certname}'
    - 'hosts/%{facts.fqdn}'
    - 'hosts/%{facts.hostname}'
    - 'domains/%{facts.domain}'
    - '%{facts.os.family}'
    - '%{facts.os.name}/%{facts.os.release.full}'
    - '%{facts.os.name}/%{facts.os.release.major}'
    - '%{facts.os.name}'
    - 'hostgroups/%{::hostgroup}'
    - 'default'
    - 'compliance_profiles/%{::compliance_profile}'
    - 'simp_config_settings'
    - 'scenarios/%{::simp_scenario}'
  :logger: 'puppet'
  # When specifying a datadir:
  # # 1) Make sure the directory exists
  # # 2) Make sure the directory reflects the hierarchy
  :yaml:
    :datadir: '/etc/puppetlabs/code/environments/%{::environment}/hieradata'
  :json:
    :datadir: '/etc/puppetlabs/code/environments/%{::environment}/hieradata'

Run ``git init .`` at the base of the ``r10k_production`` directory and commit
changes to a ``production`` branch.  Push the ``production`` branch to a
repository of your choosing.

.. _control repository documentation: https://docs.puppet.com/pe/latest/cmgmt_control_repo.html
.. _environment repository: https://github.com/simp/simp-environment-skeleton
.. _simp-core repository: https://github.com/simp/simp-core

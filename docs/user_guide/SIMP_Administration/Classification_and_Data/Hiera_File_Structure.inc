Hiera
-----

There are three `Hiera Configuration Layers`_ in hiera 5. To take advantage of
environmental data layer, SIMP configures hiera at the environment level.
The global level configuration file still exists but its hierarchy is empty in a
fresh system.

``/etc/puppetlabs/code/environments/<environment name>/hiera.yaml`` - Hiera's config file,
used to control hiera data lookups.

The hiera data directory layout created in a fresh system is:

.. code-block:: bash

   /etc/puppetlabs/code/environments/simp/data/
   ├── default.yaml
   ├── hostgroups/
   ├── hosts/
   ├── scenarios/
   └── simp_config_settings.yaml

- ``data/simp_config_settings.yaml`` - Contains the variables needed to configure SIMP. Added by ``simp config``.
- ``data/scenarios/`` - Directory containing SIMP Scenarios, set in ``manifests/site.pp``.
- ``data/hosts/`` - By populating this directory with host name files, you can assign parameters to specific hosts. Heira looks for the name in following formats::

   * %{trusted.certname}.yaml
   * %{facts.fqdn}.yaml
   * %{facts.hostname}.yaml

- ``data/hostgroups/`` - The hostgroup of a node can be computed in `site.pp`. Nodes assigned to hostgroup `$hostgroup` will read Hiera from a file named `<hostgroup>.yaml` in this directory.
- ``data/default.yaml`` - Settings that should be applied to all systems in the environment.

It is recommended to not change or update the scenarios or simp_config_settings.yaml
files.  These files are the last in the hierachy and settings can be overridden in
the other hiera files.

The default hierarchy defined in the `Hiera configuration file`_, hiera.yaml, contains other files/directories that can be added to the environment's data directory.
An example of the file is shown in `Hiera in SIMP`_

.. _Hiera Configuration Layers: https://puppet.com/docs/puppet/5.5/hiera_intro.html#hiera-s-three-config-layers

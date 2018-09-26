Hiera
-----

.. code-block:: bash

   /etc/puppetlabs/code/environments/simp/hieradata/
   ├── CentOS -> RedHat/
   ├── compliance_profiles/
   ├── default.yaml
   ├── hostgroups/
   ├── hosts/
   ├── RedHat/
   ├── scenarios/
   └── simp_config_settings.yaml

- ``hieradata/hosts/`` - By populating this directory with some.host.name.yaml file, you can assign parameters to host some.host.name
- ``hieradata/domains/`` - Same principal as hosts, but domain names.
- ``hieradata/Redhat/`` - RedHat-specific Hiera settings.
- ``hieradata/CentOS/`` - CentOS-specific Hiera settings, symlinked to ``hieradata/Redhat/``.
- ``hieradata/hostgroups/`` - The hostgroup of a node can be computed in `site.pp`. Nodes assigned to hostgroup `$hostgroup` will read Hiera from a file named `<hostgroup>.yaml` in this directory.
- ``hieradata/default.yaml`` - Settings that should be applied to the entire infrastructure.
- ``hieradata/simp_config_settings.yaml`` - Contains the variables needed to configure SIMP. Added by ``simp config``.
- ``hieradata/scenarios/`` - Directory containing SIMP Scenarios, set in ``manifests/site.pp``.

``/etc/puppetlabs/puppet/hiera.yaml`` - Hiera's config file, used to control the
hierarchy of your backends. The order of the files above mirrors that order in
the distributed hiera.yaml.

Hiera
-----

There are three `Hiera Configuration Layers`_ in Hiera 5.  SIMP configures
Hiera at the environment level, via
``/etc/puppetlabs/code/environments/<environment name>/hiera.yaml``.  The
global level configuration file at ``/etc/puppetlabs/puppet/hiera.yaml`` still
exists but its hierarchy is empty in a fresh system and probably should remain
that way for general usage.

The default ``hiera.yaml`` for a SIMP environment is shown in `Hiera in SIMP`_
and its syntax is explained in `Hiera configuration file`_.  The corresponding
Hiera data directory layout in a fresh SIMP system (or a new environment created
by ``simp environment new``) is as follows:

.. code-block:: bash

   /etc/puppetlabs/code/environments/production/data/
   ├── default.yaml
   ├── hostgroups/
   ├── hosts/
   ├── scenarios/
   └── simp_config_settings.yaml

* ``data/simp_config_settings.yaml``

  * Contains the variables needed to configure SIMP. Added by ``simp config``.

* ``data/scenarios/``

  * Directory containing settings for SIMP Scenarios.  The scenario selected is
    set in ``manifests/site.pp``.  This is done automatically for you when you
    run ``simp config``.

* ``data/hosts/``

  * Settings for specific hosts.  By populating this directory with host name
    files, you can assign parameters to specific hosts. Based on SIMP's
    ``hiera.yaml``, Hiera looks for the name in following formats:

    * ``%{trusted.certname}.yaml``
    * ``%{facts.fqdn}.yaml``
    * ``%{facts.hostname}.yaml``

* ``data/hostgroups/``

  * Settings for specific groups of hosts.  The
    ``hostgroup`` of a node is computed using a top-scope ``$hostgroup``
    variable. By default, this is set via SIMP's top-level ``site.pp``. Then,
    nodes assigned to ``$hostgroup`` will read Hiera from a file named
    ``<hostgroup>.yaml`` in this directory.

* ``data/default.yaml``

  * Settings that should be applied to all systems in
    the environment.

.. IMPORTANT::

   Do not change or update the scenarios YAML files or
   ``simp_config_settings.yaml``.  If you want to change any of the settings
   contained in these files for one or more hosts, override the settings in
   Hiera files earlier in the hierarchy, instead.

.. _Hiera Configuration Layers: https://puppet.com/docs/puppet/5.5/hiera_intro.html#hiera-s-three-config-layers

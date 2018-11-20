Hiera in SIMP
-------------

SIMP users are expected to make extensive use of Hiera to set
parameters, particularly those that are deep within the code.

The default Hiera hierarchy used by SIMP defined at the environment level
in the `Hiera configuration file`_ and looks like the following:

.. code:: yaml

  ---
  version: 5
  defaults:
    datadir: data
    data_hash: yaml_data

  hierarchy:

    - name: Per-node data
      paths:
      - "hosts/%{trusted.certname}.yaml"
      - "hosts/%{facts.fqdn}.yaml"
      - "hosts/%{facts.hostname}.yaml"

    - name: Per-domain data
      paths:
      - "domains/%{facts.domain}.yaml"

    - name: Per-OS data
      paths:
      - "%{facts.os.family}.yaml"
      - "%{facts.os.name}/%{facts.os.release.full}.yaml"
      - "%{facts.os.name}/%{facts.os.release.major}.yaml"
      - "%{facts.os.name}.yaml"

    - name: Per-hostgroup data - See site.pp for more information
      paths:
      - "hostgroups/%{hostgroup}.yaml"

    - name: General data
      paths:
      - "default.yaml"
      - "common.yaml"

    - name: SIMP specific data - Please do not modify
      paths:
      - "simp_config_settings.yaml"
      - "scenarios/%{simp_scenario}.yaml"


.. WARNING::

  This may not be accurate for your version of SIMP, please check your local
  Hiera settings!

The rest of this document will use this hierarchy as a reference.


.. _Hiera configuration file: https://puppet.com/docs/puppet/5.5/hiera_config_yaml_5.html

Hiera in SIMP
-------------

As mentioned, SIMP users are expected to make extensive use of Hiera to set
parameters, particularly those that are deep within the code.

The default Hiera hierarchy used by SIMP looks like the following:

.. code:: yaml

   ---
   :backends:
     - 'yaml'
   :hierarchy:
     # Most specific
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
     # Least specific
   :logger: 'puppet'
   :yaml:
     :datadir: '/etc/puppetlabs/code/environments/%{::environment}/hieradata'

.. WARNING::

   This may not be accurate for your version of SIMP, please check your local
   Hiera settings!

The rest of this document will use this hierarchy as a reference.

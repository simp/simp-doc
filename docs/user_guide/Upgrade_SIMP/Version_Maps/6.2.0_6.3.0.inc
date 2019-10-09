.. _upgrade-6.2.0-to-6.3.0:

Upgrading from SIMP-6.2.0 to SIMP-6.3.0
---------------------------------------

The main purpose of the SIMP 6.3 release is to upgrade to Puppet 5/Hiera 5. You
should familiarize yourself with the `Puppet 5 release notes`_ and the
`Hiera 5 upgrade`_ documents.

You can follow :ref:`ug-incremental-upgrades` for upgrading with only the
exception noted below in Upgrade the Puppetserver section.

After the system is upgraded you can enable the environment layer, by giving
each environment its own hiera.yaml (recommended).  General instructions are
given in the Updating Hiera section below.

Upgrade the Puppetserver
^^^^^^^^^^^^^^^^^^^^^^^^

After you have disabled the puppet agent on the puppetserver and before you
have upgraded the puppetserver to puppet 5 or updated the simp modules, make
the following two edits to the puppet configuration files.  If this is skipped
the puppet server will not restart after it is upgraded to Puppet 5.

File:
   /etc/puppetlabs/puppetserver/services.d/ca.cfg
Add line:
   puppetlabs.trapperkeeper.services.watcher.filesystem-watch-service/filesystem-watch-service

File:
   /etc/puppetlabs/puppetserver/conf.d/web-routes.conf
Add line:
   "puppetlabs.trapperkeeper.services.metrics.metrics-service/metrics-webservice": "/metrics"

Also note that you may see the following warnings multiple times during the upgrade::

  Warning: Setting trusted_server_facts is deprecated.
     (location: /opt/puppetlabs/puppet/lib/ruby/vendor_ruby/puppet/settings.rb:1169:in `issue_deprecation_warning')
  Warning: Setting ca is deprecated.
      (location: /opt/puppetlabs/puppet/lib/ruby/vendor_ruby/puppet/settings.rb:1169:in `issue_deprecation_warning')


This message is innocuous, but unavoidable unless you are comfortable with
:ref:`ug-ts-puppet-disabling-depwarnings`.

Restart PuppetDB
^^^^^^^^^^^^^^^^

At this point, all packages should be updated and the ``puppetserver`` process
has probably restarted. If you've been using your system with :term:`puppetdb`
for a while, then you will want to restart the ``puppetdb`` process to ensure a
successful database migration.

.. code-block:: bash

   /opt/puppetlabs/bin/puppet resource service puppetdb ensure=stopped
   /opt/puppetlabs/bin/puppet resource service puppetdb ensure=running

Updating Hiera
^^^^^^^^^^^^^^

One of the major features of :term:`Hiera` 5 is the addition of the
`environment layer`_.  You can `enable the Hiera environment layer`_ with the
following steps.

The new ``hiera.yaml`` file is installed by
``simp-environment-5.3.0-0.<dist>.noarch.rpm`` into
``/usr/share/simp/environments/simp``.  This hiera.yaml file uses ``data`` as
the directory instead of ``hieradata``.  (If you have made changes to the hiera
hierarchy make sure to incorporate these into the file.)

For every environment, copy the new hiera.yaml to the environment and then
rename the hieradata directory to data.

.. code-block:: bash

   cp /usr/share/simp/environments/simp/hiera.yaml /etc/puppetlabs/code/environments/<environment>/
   cd /etc/puppetlabs/code/environments/<environment>/
   chmod 640 ./hiera.yaml
   chown root:puppet ./hiera.yaml
   mv hieradata data


When all the environments are converted, update the global ``hiera.yaml`` file:

.. code-block:: bash

   #  Copy an empty hiera.yaml file to the global level.
   cd /etc/puppetlabs/puppet
   unlink hiera.yaml
   rm hiera.yaml.simp
   cp hiera.yaml.rpmnew hiera.yaml
   chown root:puppet hiera.yaml
   chmod 640 hiera.yaml

If the ``.rpmnew`` file does not exist the new global ``hiera.yaml`` file should look like:

.. code-block:: ruby

  version 5
  # defaults:
  # data_hash: yaml_data
  # hierarchy:
  # - name: Common
  # data_hash: yaml_data
  hierarchy: []

.. _Hiera 5 upgrade: https://puppet.com/docs/puppet/5.5/hiera_migrate.html
.. _Puppet 5 release notes: https://puppet.com/docs/puppet/5.5/release_notes.html
.. _enable the Hiera environment layer: https://puppet.com/docs/puppet/5.5/hiera_migrate.html#enable-the-environment-layer-for-existing-hiera-data
.. _environment layer: https://puppet.com/docs/puppet/latest/hiera_intro.html#hiera-s-three-config-layers

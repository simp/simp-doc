.. _ht-enable-puppetdb:

HOWTO Enable PuppetDB
=====================

.. IMPORTANT::

   Do not attempt to manage PuppetDB on Puppet Enterprise hosts!

As of SIMP 6.6.0, :term:`puppetdb` is no longer enabled or managed by default.

Use the following YAML as a guide to the :term:`hiera` settings required to enable the management of :program:`puppetdb`.

.. code-block:: yaml

   ---
   # Include the puppetdb class
   simp::server::classes:
     - simp::puppetdb

   ## SIMP Required Settings

   # Let pupmod::master::base handle this.
   puppetdb::master::config::restart_puppet: false

   # Set the PuppetDB port
   puppetdb::master::config::puppetdb_port: 8139

   ## Optional Settings

   # The version of PuppetDB that should be installed
   puppetdb::globals::version: 'latest'

   # Set up node report storage
   puppetdb::master::config::manage_report_processor: true
   puppetdb::master::config::enable_reports: true

See the :pupmod:`puppetlabs/puppetdb` documentation for details on all of the settings that affect
the configuration of :program:`puppetdb`.

Version-Specific Upgrade Instructions
=====================================

Upgrading from SIMP-6.0.0 to SIMP-6.1.0
---------------------------------------

Update Puppetserver auth.conf
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Legacy auth.conf, ``/etc/puppetlabs/puppet/auth.conf``, has been deprecated.
``pupmod-simp-pupmod`` will back up legacy puppet auth.conf after upgrade.

The puppetserver's auth.conf is now managed by Puppet. You will need to
re-produce any custom work done to legacy auth.conf in the new auth.conf, via
the ``puppet_authorization::rule`` define.  The stock rules are managed in
``pupmod::master::simp_auth``.

Set up ClamAV DAT Files Updates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Given the wide spacing of SIMP releases, the team determined that it was
ineffective for us to maintain the ``simp-rsync-clamav`` RPM with upstream
ClamAV DAT file updates.

From this point forward, SIMP will not ship with updated ClamAV DAT files and
we highly recommend updating your DAT files from the authoritative upstream
sources.  See the `ClamAV Virus Database FAQ`_ for instructions on how to
automatically update these files.

Prepare system for PostgreSQL upgrade
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP 6.1.0 updates the ``puppetdb`` Puppet module to 6.0.0, which,
by default, requires a PostgreSQL upgrade from 9.4 to 9.6.  Although
the appropriate PostgreSQL RPMs will be installed via the ``puppetdb``
6.0.0 Puppet module, the Puppet catalog compilation after the SIMP 6.1.0
upgrade will fail, because the PostgreSQL 9.6 server cannot start
while the PostgreSQL 9.4 server is still running.  You must manually
stop the PostgreSQL 9.4 server prior to compiling the SIMP 6.1.0
Puppet manifests.

In addition, you may want to

* Migrate the data contained in the 9.4 database.  This data is *not*
  automatically imported into the 9.6 database by the ``puppetdb``
  Puppet module.  See `Upgrading a PostgreSQL Cluster`_ for detailed
  instructions.

* Prevent future conflicts between the two PostgreSQL versions by
  performing one of the following actions:

  * Disabling the automatic start of the PostgreSQL 9.4 server at
    boot time.
  * Configuring the PostgreSQL 9.4 server to use different ports
    than the 9.6 server.
  * Uninstalling the PostgreSQL 9.4 RPMs.

  Unless you need the PostgreSQL 9.4 server for another application
  running on the PuppetDB server, removing the 9.4 RPMs is advised.

.. _ClamAV Virus Database FAQ: https://www.clamav.net/documents/clamav-virus-database-faq
.. _Upgrading a PostgreSQL Cluster: https://www.postgresql.org/docs/9.6/static/upgrading.html

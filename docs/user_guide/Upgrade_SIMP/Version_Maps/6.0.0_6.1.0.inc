.. _upgrade-6.0.0-to-6.1.0:

Upgrading from SIMP-6.0.0 to SIMP-6.1.0
---------------------------------------

.. contents:: :local:
   :depth: 3

.. IMPORTANT::

   It is *highly recommended* that you read the information in this section
   in its entirety.

Upgrade Script
^^^^^^^^^^^^^^

There were several issues found during the SIMP 6.0.0 to 6.1.0 upgrade that
necessitated the creation of an upgrade script that is to be run on your
:term:`SIMP Server` systems.

.. NOTE::

   No changes are required on your clients for the upgrade to succeed.

The upgrade script,
:file:`/usr/share/simp/upgrade_scripts/upgrade_simp_6.0.0_to_6.1.0`,
will assist with the upgrade from ``6.0.0`` to ``6.1.0``, taking into account
all of the specific issues. This script is available in the
:package:`simp-utils-6.1.0` package provided by SIMP 6.1.0 or the
:github:`simp-utils` repository.

As always, back up your system prior to upgrading!

.. NOTE::

   This script assumes that you are upgrading from the SIMP RPMs!

   If you have chosen some other installation method, you will need to follow
   the general steps outlined in the script.

To perform the upgrade, with ``root`` permissions:

#. Upgrade the :package:`simp-utils` package, *only*, by executing
   :command:`yum update -y simp-utils`.
#. Run the script: :command:`/usr/share/simp/upgrade_scripts/upgrade_simp_6.0.0_to_6.1.0`

.. NOTE::

   This script will:

   #. Run a :command:`yum -y update`.
   #. Reinstall :package:`simp-gpgkeys` and :package:`pupmod-simp-timezone` due
      to RPM issues.
   #. Stop and uninstall the PostgresSQL 9.4 server to prevent
      :package:`postgresql` upgrade issues.
   #. Restart the :program:`puppetserver` process.
   #. Run :command:`puppet agent -t`.

      * Some systems have shown issues with the :package:`postgresql` upgrade
        during this step.

Update auth.conf
^^^^^^^^^^^^^^^^

The legacy :file:`auth.conf`, :file:`/etc/puppetlabs/puppet/auth.conf`, has
been deprecated. :package:`pupmod-simp-pupmod` will back up the legacy
:file:`auth.conf` after the upgrade.

The Puppet Server's :file:`auth.conf` is now managed by Puppet. You will need
to reproduce any custom work done to legacy :file:`auth.conf` via the new
:code:`puppet_authorization::rule` define.  The stock rules are managed in
:code:`pupmod::master::simp_auth`.

Set up ClamAV DAT Files Updates
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Given the wide spacing of SIMP releases, the team determined that it was
ineffective for us to maintain the :package:`simp-rsync-clamav` RPM with
upstream ClamAV DAT file updates.

From this point forward, SIMP will not ship with updated ClamAV DAT files and
we highly recommend updating your DAT files from the authoritative upstream
sources.  See the `ClamAV Virus Database FAQ`_ for instructions on how to
automatically update these files.

Prepare System for PostgreSQL Upgrade
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

During the Puppet-managed upgrade, from PostgreSQL 9.4 to PostgreSQL 9.6,
the PostgreSQL 9.4 data is not automatically imported into the 9.6 database.
If for any reason you need to retain this data, which normally is quite
transitory, see `Upgrading a PostgreSQL Cluster`_ for detailed instructions.

.. _ClamAV Virus Database FAQ: https://docs.clamav.net/faq/faq-cvd.html
.. _Upgrading a PostgreSQL Cluster: https://www.postgresql.org/docs/9.6/upgrading.html

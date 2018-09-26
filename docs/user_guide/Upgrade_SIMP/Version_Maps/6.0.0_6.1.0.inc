.. _upgrade-6.0.0-to-6.1.0:

Upgrading from SIMP-6.0.0 to SIMP-6.1.0
---------------------------------------

.. IMPORTANT::

   It is *highly recommended* that you read the information in this section
   in its entirety.

Upgrade Script
^^^^^^^^^^^^^^

There were several issues found during the SIMP 6.0.0 to 6.1.0 upgrade that
necessitated the creation of an upgrade script that is to be run on your SIMP
Puppet masters.

.. NOTE::

   No changes are required on your clients for the upgrade to succeed.

The upgrade script, ``/usr/share/simp/upgrade_scripts/upgrade_simp_6.0.0_to_6.1.0``,
will assist with the upgrade from ``6.0.0`` to ``6.1.0``, taking into account
all of the specific issues. This script is available in the ``simp-utils-6.1.0``
package provided by SIMP 6.1.0 or the `simp-utils repository`_.

As always, back up your system prior to upgrading!

.. NOTE::

   This script assumes that you are upgrading from the SIMP RPMs!

   If you have chosen some other installation method, you will need to follow
   the general steps outlined in the script.

To perform the upgrade, with ``root`` permissions:

#. Upgrade the ``simp-utils`` package, *only*, by executing ``yum update -y simp-utils``.
#. Run the script: ``/usr/share/simp/upgrade_scripts/upgrade_simp_6.0.0_to_6.1.0``

.. NOTE::

   This script will:

   #. Run a ``yum -y update``.
   #. Reinstall ``simp-gpgkeys`` and ``pupmod-simp-timezone`` due to RPM issues.
   #. Stop and uninstall the PostgresSQL 9.4 server to prevent ``postgresql``
      upgrade issues.
   #. Restart the ``puppetserver`` process.
   #. Run ``puppet agent -t``.

      * Some systems have shown issues with the ``postgresql`` upgrade
        during this step.

Update auth.conf
^^^^^^^^^^^^^^^^

The legacy ``auth.conf``, ``/etc/puppetlabs/puppet/auth.conf``, has been
deprecated. ``pupmod-simp-pupmod`` will back up the legacy ``auth.conf`` after
the upgrade.

The Puppet master's ``auth.conf`` is now managed by Puppet. You will need to
reproduce any custom work done to legacy ``auth.conf`` via the new
``puppet_authorization::rule`` define.  The stock rules are managed in
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

Prepare System for PostgreSQL Upgrade
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

During the Puppet-managed upgrade, from PostgreSQL 9.4 to PostgreSQL 9.6,
the PostgreSQL 9.4 data is not automatically imported into the 9.6 database.
If for any reason you need to retain this data, which normally is quite
transitory, see `Upgrading a PostgreSQL Cluster`_ for detailed instructions.

.. _ClamAV Virus Database FAQ: https://www.clamav.net/documents/clamav-virus-database-faq.html
.. _Upgrading a PostgreSQL Cluster: https://www.postgresql.org/docs/9.6/static/upgrading.html
.. _simp-utils repository: https://github.com/simp/simp-utils

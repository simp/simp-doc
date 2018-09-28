.. _ug-upgrade-simp:

Upgrading SIMP
==============

This section describes both the general, recommended upgrade procedures
as well as any version-specific upgrade procedures.

.. IMPORTANT::

   To minimize upgrade problems in your production environment, we
   strongly recommend that you:

   * Carefully read the CHANGELOG for the SIMP version to which you are
     upgrading, as well as the Changelogs for any interim versions
     you are skipping over.
   * Test your upgrades in a development environment before deploying
     to a production environment.
   * Backup any critical server data/configurations prior to executing
     the upgrade to a production environment.
   * On each managed server, ensure you have a local user with ``su``
     and ``ssh`` privileges to prevent lockout.

.. toctree::
   :maxdepth: 2

   Upgrade_SIMP/General_Upgrade_Instructions
   Upgrade_SIMP/Version_Specific_Upgrade_Instructions

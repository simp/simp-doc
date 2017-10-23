.. _ug-upgrade-simp:

Upgrading SIMP
==============

SIMP follows Semantic Versioning 2.0.0 and has the following versioning
structure: ``X.Y.Z``, where

* ``X`` indicates breaking changes
* ``Y`` indicates new features
* ``Z`` indicates bug fixes.

This section describes both the general, recommended upgrade procedures
for ``X``, ``Y``, or ``Z`` releases, as well as any version-specific
upgrade procedures.

.. IMPORTANT::

   To minimize upgrade problems in your production environment, we
   strongly recommend you

   * Carefully read the Changelog for the version to which you are
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

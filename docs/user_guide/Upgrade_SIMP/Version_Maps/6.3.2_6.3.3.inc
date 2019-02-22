.. _upgrade-6.3.2-to-6.3.3:

Upgrading from SIMP-6.3.2 to SIMP-6.3.3
---------------------------------------

The 6.3.3 release of SIMP is a bugfix release focusing on a specific issue with
the updated ``sssd`` module that may cause account lockouts on upgrade
depending on how your LDAP accounts have been maintained.

You can follow :ref:`ug-incremental-upgrades` for upgrading.

.. WARNING::

   If you are upgrading from 6.2.0 to 6.3.X, please read
   :ref:`upgrade-6.2.0-to-6.3.0` with care.

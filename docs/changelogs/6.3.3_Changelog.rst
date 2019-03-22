.. _changelog:
.. _changelog-6.3.3:

SIMP Community Edition (CE) 6.3.3-0
===================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak

.. WARNING::

   Please see the :ref:`changelog-6.2.0` Changelog for general information,
   upgrade guidance, and compatibility notes.

This is a bug fix release in the 6.3.X series of SIMP to address the following
issues:

  * `SIMP-6152`_: Change a new default that was introduced by a bug fix in
    :ref:`changelog-6.3.2` to SSSD that caused accounts with old
    ``shadowLastChange`` entries in LDAP to be unable to login to systems.

Fixed Bugs
----------

pupmod-simp-sssd
^^^^^^^^^^^^^^^^

* Change the ``sssd::provider::ldap::ldap_access_order`` defaults to
  ``['ppolicy','pwd_expire_policy_renew']`` by default to prevent accidental
  system lockouts on upgrade.

Known Bugs
----------

Upgrading from previous SIMP 6.X versions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are known issues when upgrading from Puppet 4 to Puppet 5.  Make sure you
read the :ref:`ug-upgrade-simp` before attempting an upgrade.

Tlog
^^^^

Tlog currently has `a bug where session information may not be logged`_. The
immediate mitigation to this is the fact that `pam_tty_audit` is the primary
mode of auditing with ``tlog`` and/or ``sudosh`` being in place for a better
overall tracking and behavior analysis experience.

Tlog has `a second bug where the application fails if a user does not have a TTY`_.
This has been mitigated by the SIMP wrapper script simply bypassing ``tlog`` if
a TTY is not present.


.. _SIMP-6152: https://simp-project.atlassian.net/browse/SIMP-6152
.. _a bug where session information may not be logged: https://github.com/Scribery/tlog/issues/228
.. _a second bug where the application fails if a user does not have a TTY: https://github.com/Scribery/tlog/issues/227

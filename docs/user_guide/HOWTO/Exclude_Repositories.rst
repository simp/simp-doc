.. _Exclude_Repos:

HOWTO Exclude YUM Repositories
==============================

By default, SIMP applies updates from all available repositories on a
nightly basis via the ``simp::yum`` class.

The purpose of running a ``yum update`` on a nightly basis is to ensure that
all critical updates are applied to the systems in a timely manner.
Additionally, this places the package management burden on the repositories
themselves instead of on a large number of resources in the Puppet
infrastructure.

This section provides guidance on how to include or exclude specific
repositories from the nightly YUM updates.

Enabling and Disabling Specific Repositories
--------------------------------------------

The ``simp::yum::schedule::repos`` and ``simp::yum::schedule::disable``
parameters in the ``simp`` module control which repositories are enabled for
nightly updating. Both parameters must be specified as Arrays of Strings and
follow the syntax for the ``--enablerepo`` and ``--disablerepo`` options for
``yum``.

Restricting the Update Repositories
-----------------------------------

``simp::yum::schedule::repos`` is used to specify an Array of
repository names from which updates are provided. This defaults to ``all``
repos and, if specified, no other repositories will be used.

* For example, you could use this capability to do something like set up a
  security-only repository and target the nightly updates to security updates
  only.

Disabling Specific Repositories
-------------------------------

``simp::yum::schedule::disable`` is used to specify an array of
repositories from which updates are not provided; all other repositories
will be used.

Disabling all Nightly Updates
-----------------------------

If you wish to completely disable the nightly update capability, simply set
``simp::yum::auto_update`` to ``false``.

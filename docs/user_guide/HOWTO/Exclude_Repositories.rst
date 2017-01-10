.. _Exclude_Repos:

HOWTO Exclude YUM Repositories
==============================

By default, SIMP applies updates from all available repositories on a
nightly basis. This ensures that bug fixes and security updates are
applied to all systems without minute management in Puppet manifests.
This section provides guidance on how to include or exclude specific
repositories from nightly YUM updates.

Methodology
-----------

The ``simp::yum::schedule::repos`` and ``simp::yum::schedule::disable``
variables in the simp module control which repositories are
enabled for nightly updating. Both variables must be specified in array
format.

``simp::yum::schedule::repos`` is used to specify an array of
repositories from which updates are provided; no other repositories will
be used.

``simp::yum::schedule::disable`` is used to specify an array of
repositories from which updates are not provided; all other repositories
will be used.

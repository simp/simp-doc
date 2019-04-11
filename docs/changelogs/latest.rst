.. _changelog-6.4.0:

SIMP Community Edition (CE) 6.4.0-Alpha
=======================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak

This release is known to work with:

  * CentOS 6.10 x86_64
  * CentOS 7.0 1810 x86_64
  * OEL 6.10 x86_64
  * OEL 7.6 x86_64
  * RHEL 6.10 x86_64
  * RHEL 7.6 x86_64


.. WARNING::

   REALLY SUPER IMPORTANT STUFF GOES HERE

Breaking Changes
----------------

.. todo::

   ADD BREAKING CHANGES

Significant Updates
-------------------

Module RPM Installation
^^^^^^^^^^^^^^^^^^^^^^^

The ``simp-adapter`` has been redesigned to create and maintain local Git
repositories for Puppet modules installed via SIMP-packaged RPMs, in lieu
of (optionally) auto-updating ``/etc/puppetlabs/code/environments/simp``.
This change allows SIMP users on isolated networks to manage one or more
Puppet environments easily, using R10K or Code Manager.  The use of
R10K/Code Manager, in turn, provides Puppet module installation that aligns
with current, industry-wide, best practices.


Security Announcements
----------------------

.. todo::

   ADD SECURITY ANNOUNCEMENTS

RPM Updates
-----------

simp-adapter 1.0.0
^^^^^^^^^^^^^^^^^^

Beginning with ``simp-adapter`` 1.0.0, the (optional) auto-update to the
``simp`` Puppet environment has been replaced with creation/maintenance of
a local Git repository for each Puppet module that SIMP packages as an RPM.
The ``simp_adapter``'s ``simp_rpm_helper`` now ensures that each Puppet
module is imported from its RPM installation location,
``/usr/share/simp/modules/<module name>``, into a local, SIMP-managed,
Git repository, ``/usr/share/simp/git/puppet_modules/<owner>-<module name>``.
The name of the repository is the top-level ``name`` field from the module's
``metadata.json``.

The specific behavior of ``simp_rpm_helper`` during RPM operations is as follows:

* Upon module RPM install/upgrade/downgrade, the ``simp_rpm_helper``

  - Updates the master branch of the repository to be the contents of the RPM,
    excluding any empty directories.
  - Adds a Git tag to the repository that matches the version number in the
    module's ``metadata.json`` file, as necessary.  If the tag for the version
    already exists but doesn't match the contents of the RPM, ``simp_rpm_helper``
    will **overwrite** the tag with the correct content.

* Upon module RPM erase, the ``simp_rpm_helper``  does **NOT** remove the local
  module Git repo, but leaves it intact, in case it is still being used
  by R10K or Code Manager for an active Puppet environment.


Removed Modules
---------------

Elasticsearch-Logstash-Grafana (ELG) Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following modules were removed because they are significantly out of
date and, in some cases, only work with application versions that are no
longer supported:

* pupmod-elastics-elasticsearch
* pupmod-elastics-logstash
* pupmod-puppet-grafana
* pupmod-simp-simp_elasticsearch
* pupmod-simp-simp_logstash
* pupmod-simp-simp_grafana


Fixed Bugs
----------

.. todo::

   NOTE BUGS FIXED

pupmod-simp-igotfixed
^^^^^^^^^^^^^^^^^^^^^

* Information about what got fixed

New Features
------------

.. todo::

   NOTE FEATURES ADDED

pupmod-simp-igotawesome
^^^^^^^^^^^^^^^^^^^^^^^

* Information about the new hotness

Known Bugs
----------

.. todo::

   NOTE KNOWN BUGS

.. _file bugs: https://simp-project.atlassian.net

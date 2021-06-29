.. _changelog-latest:
.. _changelog-6.6.0:

SIMP Community Edition (CE) 6.6.0
=================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak


OS compatibility
----------------

.. contents::
  :depth: 2
  :local:

This release is known to work with:

  * CentOS 7.0 2003 x86_64
  * CentOS 8.2 2004 x86_64
  * OEL 7.8 x86_64
  * OEL 8.2 x86_64
  * RHEL 7.8 x86_64
  * RHEL 8.2 x86_64


Important OS compatibility limitations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

EL6 support has been removed
""""""""""""""""""""""""""""

EL6 is no longer supported by SIMP CE.

If you need support for EL6 systems, please consider purchasing commercial
support.

.. _changelog-6.6.0-breaking-changes:

Breaking Changes
----------------

.. contents::
  :depth: 2
  :local:

TBD

.. _changelog-6.6.0-significant-updates:

Significant Updates
-------------------

.. contents::
  :depth: 2
  :local:

EL8 SIMP Client Node Support
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This release provides full support for both EL8 server and client systems.

One of the biggest changes was the deprecation of :term:`OpenLDAP` in EL8.

SIMP has replaced the native :term:`LDAP` capabilities with :term:`389-DS`.

Existing infrastructures will not be affected on upgrade but new environments
will need to correctly configure their environment for the target LDAP server.

.. todo::

   Add links to the appropriate documentation sections

Puppet 7 Support
^^^^^^^^^^^^^^^^

All SIMP Puppet modules now work with both Puppet 6 and Puppet 7.

Puppet 5 support has been dropped due to end-of-life.


.. _changelog-6.6.0-security-anouncements:

Security Announcements
----------------------

.. contents::
  :depth: 2
  :local:

.. _changelog-6.6.0-rpm-updates:

RPM Updates
-----------

Puppet RPMs
^^^^^^^^^^^

.. todo::

   Update the RPM list

The following Puppet RPMs are packaged with the SIMP 6.6.0 ISOs:

+-----------------------------+---------+
| Package                     | Version |
+=============================+=========+
| :package:`puppet-agent`     | FIXME   |
+-----------------------------+---------+
| :package:`puppet-bolt`      | FIXME   |
+-----------------------------+---------+
| :package:`puppetdb`         | FIXME   |
+-----------------------------+---------+
| :package:`puppetdb-termini` | FIXME   |
+-----------------------------+---------+
| :package:`puppetserver`     | FIXME   |
+-----------------------------+---------+

Removed Puppet Modules
----------------------

.. contents::
  :depth: 2
  :local:

.. _changelog-6.6.0-fixed-bugs:

Fixed Bugs
----------

.. contents::
  :depth: 2
  :local:

pupmod-simp-todo
^^^^^^^^^^^^^^^^

* TODO

.. _changelog-6.6.0-new-features:

New Features
------------

.. contents::
  :depth: 2
  :local:

pupmod-simp-todo
^^^^^^^^^^^^^^^^

* TODO

Known Bugs and Limitations
--------------------------

Below are bugs and limitations known to affect this release. If you discover
additional problems, please `submit an issue`_ to let use know.

.. contents::
  :depth: 2
  :local:

.. _changelog-6.6.0-el8-client-limitations:

Special considerations with EL8 clients
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

unpack_dvd does not (re-)create modular repos for EL8 dnf repos (:jira:`SIMP-8614`)
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

EL8 introduces `modular package repositories
<https://docs.pagure.org/modularity/>`_. When unpacking an EL8 ISO to populate
a yum repository, SIMP 6.6.0's :program:`unpack_dvd` script does not recognize
or correctly package repository modules.  Consequently, EL8 Puppet agents
applying catalogs that require modular EL8 packages may encounter errors like
the following:

.. code-block:: none

   Error: /Stage[main]/Simp_apache::Install/Package[httpd]/ensure: change from 'purged' to 'latest' failed: Could not update: Execution of '/usr/bin/dnf -d 0 -e 1 -y install httpd' returned 1: No available modular metadata for modular package 'httpd-2.4.37-21.module_el8.2.0+382+15b0afa8.x86_64', it cannot be installed on the system
   Error: No available modular metadata for modular package


.. _submit an issue: https://simp-project.atlassian.net
.. _simp-project.com: https://simp-project.com

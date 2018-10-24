SIMP Community Edition (CE) 6.3.0-Beta
======================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak

This release is known to work with:

  * CentOS 6.10 x86_64
  * CentOS 7.0 1804 x86_64
  * OEL 6.10 x86_64
  * OEL 7.5 x86_64
  * RHEL 6.10 x86_64
  * RHEL 7.5 x86_64


.. WARNING::

   Puppet 4 is no longer supported as of SIMP 6.3. Users can continue with the
   SIMP 6.2 release and can obtain commercial support if further Puppet 4
   support is required.

   From this point on, all components are tested againt Puppet 5.

   Puppet 4 might work but there are no guarantees over time.

Breaking Changes
----------------

TBD

Significant Updates
-------------------

Security Announcements
----------------------

TBD

RPM Updates
-----------

ELG Stack
^^^^^^^^^

* The rpms for Elasticsearch, Logstash and Grafana (ELG) will no longer be delivered with the
  SIMP iso. Updates in the same major version of Elasticsearch and Logstash have
  been shown to randomly corrupt data and are therefore too dangerous to potentially drop
  into upstream updates repos by default. Users must now download their own ELG packages
  from their preferred repositories

Removed Modules
---------------

pupmod-simp-freeradius
^^^^^^^^^^^^^^^^^^^^^^

* There was not enough time to get the ``freeradius`` components updated
  sufficiently for Puppet 5 prior to release. This module may reappear in
  future releases if there is significant demand.

Fixed Bugs
----------

TBD

New Features
------------



TBD

Known Bugs
----------

TBD

.. _file bugs: https://simp-project.atlassian.net

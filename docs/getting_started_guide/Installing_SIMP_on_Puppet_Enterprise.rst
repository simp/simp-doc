.. _install-simp-on-pe:

Installing SIMP on Puppet Enterprise
====================================

.. contents:: Contents:
  :local:

Overview
^^^^^^^^^^^^^

As of 6.0, SIMP can now be installed on top of an existing
Puppet Enterprise installation. Once installed SIMP will 
configure itself to add the necessary firewall rules, and service
level exclusions based only on changes made to the PE Console.

Installing SIMP on top of Puppet Enterprise is a multi-step process

System Requirements
^^^^^^^^^^^^^^^^^^^

The following versions of Puppet Enterprise are currently supported:

============  ==============  =============================================
 PE Version    SIMP Version    Notes
============  ==============  =============================================
 2016.4        6.0
 2016.5        6.0             SIMP on PE HA requires manual configuration
 2017.1        6.0             SIMP on PE HA requires manual configuration
============  ==============  =============================================

The following yum repositories are required for SIMP modules, that are 
currently not being managed by SIMP


Disclaimer
^^^^^^^^^^

SIMP will attempt to make minor compliance changes to Puppet
Enterprise on certain versions. However we have not made major changes
to bring it in-line with FIPS 140-2 to ensure that SIMP users who have
a valid support license do not invalidate their support by using SIMP.

XXX TODO Insert further information relevant to the disclaimer

Pre-install tasks
^^^^^^^^^^^^^^^^^


.. include:: Creating_A_SIMP_Control_Repo.rst

XXX TODO Setting hiera simp_options::server_distribution

XXX TODO Node Classification recommendations



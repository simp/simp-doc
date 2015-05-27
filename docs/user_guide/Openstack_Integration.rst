OpenStack Integration
=====================

This chapter explains how to integrate OpenStack IceHouse in the SIMP
environment.

OpenStack In SIMP - IceHouse
----------------------------

OpenStack is a collection of IaaS cloud computing services aimed at
creating a free and open source platform for cloud development and
deployment. It is designed in a modular fasion, encompassing the
following components:

-  **Cinder**\ Block Storage Service

-  **Glance**\ VM Image Service

-  **Keystone**\ Identity Service

-  **Nova**\ Compute Service

-  **Horizon**\ Dashboard Service

-  **Neutron**\ Networking Service

-  **Swift**\ Object Storage Service

-  **Ceilometer**\ Metrics

-  **Heat**\ Templating

Integration of OpenStack into SIMP provides an easily scalable, secture
cloud infrastructure for the end user. Currently, SIMP supports
OpenStack IceHouse for CentOS 6.5, with the exception of Swift.

Each OpenStack module has been encapsulated into SIMP as a Puppet
module, for rapid deployment. The following modules have been integrated
into SIMP for OpenStack support:\ **Puppetlabs Apache, Cinder, Glance,
INIFile, Keystone, MYSQL, Nova, Qpid, Horizon, Memcached, OpenVSwitch,
Heat, Ceilometer, Neutron, and OpenStack.**

Deployment
----------

The premise of OpenStack deployment is to create a matrix of control and
compute nodes. The control nodes retain all identity, network, database,
and communication services; compute nodes run virtualization services.

Each OpenStack SIMP puppet module provides, for itself, the necessary
configuration and security required to run inside SIMP. All OpenStack
modules are abstracted inta a single module, called 'openstack'. This
module contains pre-loaded configurations for OpenStack deployment,
including the framework for basic control and compute nodes.

Please note that each OpenStack puppet module (keystone, glance, cinder,
nova, etc.) can run independent of the supplied 'openstack' module and
manifests. That means you, the end user, can opt to use the example
manifests directly or not at all. Create your own manifests for specific
site deployments.

To deploy, you must install the pupmod-puppetlabs-openstack module
(which will chain-install all dependencies).
/etc/puppet/modules/openstack/simp has example hieradata yaml files for
compute and control nodes.

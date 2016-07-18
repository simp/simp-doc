.. _Rsyslog:

Centralized Rsyslog
===================

SIMP provides a pre-built set of classes within the *rsyslog* module for
enabling centralized logging within the infrastructure.

There are no provisions here for setting up shared storage or deduplication.
This is inherently not a use case that Rsyslog is well designed for and we
suggest that you look at an alternative. We have incorporated the combination
of :ref:`Elasticsearch, Logstash, and Grafana` (ELG) into the SIMP ecosystem as a
well-known, Open Source, software collection.

Enable the Server
-----------------

To enable the pre-built log server, add the following example code to
the designated centralized logging node.

Code to Enable the Server Logging Examples

.. code-block:: yaml

  classes :
    - 'simp::rsyslog::stock'


Enable the Clients
------------------

To have clients send data to the server, make the following changes to
the ``/etc/puppet/environments/simp/hieradata/simp_def.yaml`` file.

Code to Enable the Client Logging Examples:

.. code-block:: ruby

  log_server="fqdn.of.your.log.server"

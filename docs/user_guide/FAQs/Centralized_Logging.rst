Centralized Logging
===================

SIMP provides a pre-built set of classes within the *rsyslog* module for
enabling centralized logging within the infrastructure.

After completing these steps, run Puppet on the server and clients, or
wait until after the next run to see logs start to flow.

Enable the Server
-----------------

To enable the pre-built log server, add the following example code to
the designated logging node.

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

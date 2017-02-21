.. _Rsyslog:

Centralized Rsyslog
===================

SIMP provides a pre-built set of classes within the *rsyslog* module for enabling centralized logging within the infrastructure.

There are no provisions here for setting up shared storage or deduplication.
This is inherently not a use case that Rsyslog is well designed for and we
suggest that you look at an alternative. We have incorporated the combination
of :ref:`Elasticsearch, Logstash, and Grafana` (ELG) into the SIMP ecosystem
as a well-known, Open Source, software collection.


Preparation
-----------


The ``simp_rsyslog`` Profile Module
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A profile module, `simp_rsyslog <https://github.com/simp/pupmod-simp-simp_rsyslog>`_,
is provided to help configure systems for logging.

Configuration of the simp_rsyslog is done using hiera.  See :ref:`Hiera` for
information on how SIMP configures and uses hiera.

The simp_rsyslog class is included on systems if the simp or simp-lite
:ref:`scenarios <simp scenarios>` are used and by default configures local
logging.  If scenarios are not being used include the the simp_rsyslog class
on all systems including the log server. To do this add the following to the
appropriate hiera files:

.. code-block:: yaml

  classes :
    - 'simp_rsyslog'


The default hiera directory is
``/etc/puppetlabs/code/environments/simp/hieradata`` and generally the
following files will be used::

  default.yaml                   # contains settings that will affect all systems
  hosts/<FQDN_of_LOGSERVER>.yaml # contains setting for that affect only the server.

What is Logged
^^^^^^^^^^^^^^

The simp_rsyslog module uses the following parameters

.. code-block:: yaml

  simp_rsyslog::default_logs     # A hash of the security relevant logs to be collected.
  simp_rsyslog::log_collection   # Use this hash to add logs to the default set.

There are also booleans available to enable collection of certain logs like
openldap. See the simp_rsyslog module for more details.

Set Log Servers
^^^^^^^^^^^^^^^

The list of log servers are usually set during ``simp config``, and placed in
the ``simp_config_settings.yaml`` hiera file. If this value needs to be
changed, you can either run ``simp config`` again or set the values below in
the ``default.yaml``.

.. code-block:: yaml

  simp_options::syslog::log_servers:
    - 'logserver1.fullyqualified.domain'
    - 'logserver2.fullyqualified.domain'
  simp_options::syslog::failover_log_servers:
    - 'failoverserver1.fullyqualified.domain'
    - 'failoverserver2.fullyqualified.domain'

Failover logservers are optional.

The default settings can be over written by setting the following hiera
settings for host groups or individual servers

.. code-block:: yaml

  simp_rsyslog::logservers
  simp_rsyslog::failover_log_servers

.. WARNING::
  If log forwarding is enabled on your log server, make sure you override the
  log server settings to NOT include itself. This will cause looping and will
  fill the disks on the system very quickly with repeated messages.

.. NOTE::
  It is common in big environments to use aliases or to cluster servers so
  determining the name a server is using for logging is not straight forward.
  Because of this simp could not check reliably to see if the host was
  forwarding to itself.


TLS
^^^

If encryption is going to be used, make sure the certificates are in place.
See the pupmod-simp-pki module to understand how SIMP modules distribute
certificates or, if SIMP is not being used to distribute certificates, the
naming convention used for pki variables in modules.


Enable the Client
-----------------

To set up the clients enter the following settings in the default.yaml or
similiar hiera file to reach all clients:

.. code-block:: yaml

  #If using TLS
  simp_rsyslog::forward_logs: true
  rsyslog::enable_tls_logging: true

or

.. code-block:: yaml

  #If not using TLS
  simp_rsyslog::forward_logs: true
  rsyslog::pki: false
  rsyslog::enable_tls_logging: false


Enable the Server
-----------------

To set up the server enter the following in the server's hiera file:

.. code-block:: yaml

  # If using TLS
  simp_rsyslog::is_server: true
  simp_rsyslog::forward_logs: false
  rsyslog::tls_tcp_server: true


or

.. code-block:: yaml

  # If NOT using TLS
  simp_rsyslog::is_server: true
  simp_rsyslog::forward_logs: false
  rsyslog::tcp_server: true
  rsyslog::tls_tcp_server: false

After puppet has run on all the systems, the logs from the clients will be stored in
``/var/log/hosts/<client name>`` directory on the log server.  simp_rsyslog
also sets up logrotation for these files by default.

Forwarding logfiles from a log server
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the log server needs to forward the logs to another server set
``forward_logs`` to true and remember to set the list of logservers so it does
not include the current server in its list.  For example for a server using TLS:

.. code-block:: yaml

  simp_rsyslog::is_server: true
  simp_rsyslog::forward_logs: true
  rsyslog::tls_tcp_server: true
  simp_rsyslog::logservers:
    - 'someotherlogserver.that.is.not.me'
  simp_rsyslog::failover_log_servers:
    - 'someotherfailoverserver.that.is.not.me'

This will forward its own logs and the client logs it receives on to another server.



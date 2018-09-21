.. _Rsyslog:

Centralized Rsyslog
===================

SIMP provides a pre-built set of classes within the *rsyslog* module for
enabling centralized logging within the infrastructure.

There are no provisions here for setting up shared storage or deduplication.
This is inherently not a use case that Rsyslog is well designed for and we
suggest that you look at an alternative. We have incorporated the combination
of :ref:`Elasticsearch, Logstash, and Grafana` (ELG) into the SIMP ecosystem as
a well-known, Open Source, software collection.

.. NOTE::

   For an overview of how to use Hiera to manage class parameters, please see
   :ref:`Classification and Data`.

Preparation
-----------

The ``simp_rsyslog`` Profile Module
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A profile module, `simp_rsyslog <https://github.com/simp/pupmod-simp-simp_rsyslog>`_,
is provided to help configure systems for logging.

The ``simp_rsyslog`` class is included on systems if the ``simp`` or
``simp_lite`` :ref:`scenarios <simp scenarios>` are used and by default
configures local logging.

If scenarios are not being used, include the ``simp_rsyslog`` class on all
systems including the log server. If you are using the default SIMP install,
you can add it to the ``simp::classes`` array. Otherwise, you will need to use
a standard Puppet ``include`` mechanism.

What is Logged
^^^^^^^^^^^^^^

The ``simp_rsyslog`` module uses the following parameters:

.. code-block:: yaml

  simp_rsyslog::default_logs     # A Hash of the default system logs to be collected
  simp_rsyslog::log_collection   # Use this Hash to add logs to the default set

There are also Booleans available to enable collection of certain logs, such as
those from OpenLDAP. See the ``simp_rsyslog`` module for more details.

The Log Hash Format
"""""""""""""""""""

The Hashes mentioned above are complex in nature but provide a clean interface to most
aspects of log collection targeted to most users.

The :term:`Puppet Data Type` representation of the Hashes is as follows:

.. code-block:: ruby

   Hash[
     Enum[
       'programs',
       'facilities',
       'msg_starts',
       'msg_regex'
     ],
     Array[String]
   ]

This means that you can have a ``Hash``, with any of the keys ``programs``,
``facilities``, ``msg_starts``, or ``msg_regex`` followed by an ``Array`` of
``Strings``.

Using the following example ``Hash``:

.. code-block:: ruby

   {
     'programs'   => [ 'sudo' ],
     'facilities' => [ 'cron.*' ],
     'msg_starts' => [ 'IMPORTANT:' ],
     'msg_regex'  => [ '*bad_guys*' ]
   }

The ``programs`` line would match the following due to the highlighted section:

* 2017-03-14T15:26:53.589793+00:00 sample.host.name **sudo**: test_user : TTY=pts/0 ; PWD=/home/test_user ; USER=root ; COMMAND=/bin/sudosh

The ``facilities`` line would match the following because the listed facility is ``cron``:

* 2017-03-14T15:26:53.589793+00:00 sample.host.name CROND[31415]: (root) CMD (run-parts /etc/cron.hourly)

The ``msg_starts`` line would match the following due to the highlighted section:

* 2017-03-14T15:26:53.589793+00:00 sample.host.name kernel: **IMPORTANT:** This is an important message

The ``msg_regex`` line would match the following due to the highlighted section:

* 2017-03-14T15:26:53.589793+00:00 sample.host.name kernel: This system was prodded by **bad_guys** and should be watched

Set Log Servers
^^^^^^^^^^^^^^^

The list of log servers are usually set during ``simp config``, and placed in
the ``simp_config_settings.yaml`` :term:`Hiera` file.

If this value needs to be changed, either ``simp config`` can be run again or
the values below can be overridden in ``default.yaml``:

.. code-block:: yaml

  simp_options::syslog::log_servers:
    - 'logserver1.fullyqualified.domain'
    - 'logserver2.fullyqualified.domain'
  simp_options::syslog::failover_log_servers:
    - 'failoverserver1.fullyqualified.domain'
    - 'failoverserver2.fullyqualified.domain'

If you list more than one primary log server your logs will be forwarded to
**all** of the log servers in the array.

Failover log servers are optional.

.. WARNING::
   If log forwarding is enabled on your log server, make sure you override the
   log server settings to NOT include itself. This will cause looping and will
   fill the disks on the system very quickly with repeated messages.

.. NOTE::
   It is common in big environments to use :term:`DNS` aliases or to cluster
   servers so determining the name a server is using for logging is not
   straightforward. Because of this SIMP cannot reliably determine if a host
   is forwarding to itself.

TLS
^^^

If encryption is going to be used, make sure the certificates are in place.
See the :ref:`Certificates` documentation to understand how SIMP modules
distribute certificates.

If SIMP is not being used to distribute certificates, the naming convention
used for PKI variables can be found in ``rsyslog::config/pki``.

Enable the Client
-----------------

To set up the clients enter the following settings in the default.yaml or
similar :term:`Hiera` file to reach all clients:

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

To set up the server enter the following in the server's :term:`Hiera` file:

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

After ``puppet`` has run on all the systems, the logs from the clients will be
stored in ``/var/log/hosts/<client name>`` directory on the log server.

``simp_rsyslog`` also sets up log rotation for these files by default using the
``logrotate`` module.

Forwarding Log Files from a Log Server
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the log server needs to forward logs to another server, edit its :term:`Hiera` file.
Set ``simp_rsyslog::forward_logs`` to ``true`` and  make sure that the
``log_servers`` array used on the relevant node does not include itself in the
list. For example for a server using TLS:

.. code-block:: yaml

  simp_rsyslog::is_server: true
  simp_rsyslog::forward_logs: true
  rsyslog::tls_tcp_server: true
  simp_options::syslog::log_servers:
    - 'some-other-log-server.that.is.not.me'
  simp_options::syslog::failover_log_servers:
    - 'some-other-failover-server.that.is.not.me'

This will forward the server's own logs, and all received client logs, to the
specified servers.

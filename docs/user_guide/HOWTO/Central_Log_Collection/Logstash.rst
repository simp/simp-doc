.. _Elasticsearch, Logstash, and Grafana:

Elasticsearch, Logstash, and Grafana
====================================

This chapter provides instruction for getting a basic configuration of
:term:`Logstash` working in a SIMP environment.

If these instructions do not work for you, please take a look at the README in
the `simp_logstash` profile module, particularly the acceptance tests in the
``spec/acceptance`` directory.

Known Issues
------------
#. Per Elasticsearch, you may have issues retaining existing data, when
   you upgrade from Elasticsearch 2.X to 5.X.  See the
   `Elasticsearch Upgrade Guide`_ for detailed instructions on how
   to safely upgrade, *before* you upgrade SIMP's :term:`ELG` stack.

#. The current ``simp_grafana`` module, version 1.0.4, only works if
   ``simp_options::ldap`` is set to ``true``.

#. SIMP's Grafana dashboards have not been updated to work with the
   latest ELG stack.

The ``simp_grafana`` and SIMP Grafana dashboard issues will be
addressed in upcoming releases of these components.

Obtaining the Required Packages
-------------------------------

Because SIMP's :term:`ELG` profile modules are optional components in the SIMP
infrastructure, the ELG packages are not included in the SIMP distribution.

You will need to proceed to the vendor sites to obtain the required RPMs and
**put them in an accessible** :term:`YUM` **repository**. The SIMP modules were
designed with the assumption that you would be using a repository for all of
your installations.

The following versions have been tested against the SIMP ELG Stack:

+------------------------+---------+
| Package                | Version |
+========================+=========+
| elasticsearch_         | 5.6     |
+------------------------+---------+
| elasticsearch-curator_ | 5.0     |
+------------------------+---------+
| logstash_              | 5.6     |
+------------------------+---------+
| grafana_               | 4.2     |
+------------------------+---------+

Logstash
--------

`Logstash`_ is an Open Source tool that provides a means for SIMP
implementations to have logs and events collected, filtered, and forwarded
to another host. SIMP comes with three separate but related modules:

* **simp_logstash:**

  * SIMP profile module that installs the RPMs and configuration needed
    for log inputs, filters, and outputs.
  * Uses the `logstash module`_.

* **simp_elasticsearch:**

  * SIMP profile module that installs the RPMs and configuration needed
    for Elasticsearch.
  * Uses the `elasticsearch module`_.

* **simp_grafana:**

  * SIMP profile module that installs the RPMs and configuration needed
    for the Grafana web interface.
  * Uses the `grafana module`_.

.. WARNING::
   The simp_logstash class is incompatible with the SIMP
   ``simp_rsyslog::server`` class!

   You cannot enable both of them on the same sever.

Logstash Architecture
---------------------

The Logstash architecture is quite straightforward. It takes inputs from
various sources, optionally applies filters, and outputs the results to a
specified target. It's likely that you can already forward logs to Logstash and
output them in a useful format as part of your existing architecture.

Logstash filters can manipulate logs after ingest and before output.  Examples
of existing filters include fixing logs to split/combine lines, adding fields,
normalizing time stamps, and adding GeoIP fields. Depending on the type of log
manipulation that is desired, there is likely a filter and
`Logstash documentation`_ that already exists.

SIMP Logstash Architecture
--------------------------

Combining the simp_logstash_, simp_elasticsearch_, and simp_grafana_
modules provides a functioning log collection, reduction, and search
capability. Unless scale dictates otherwise, these three modules can easily be
applied to a single host.

The intent of providing Logstash in SIMP is to replace the default
:ref:`Rsyslog` server with a capability that is easier to search and analyze
over time. Once your Logstash server is set up, you simply need to direct your
hosts to forward logs to your Logstash server. In a default SIMP configuration,
this can be done by setting the ``$simp_options::syslog::log_servers`` variable
in :term:`Hiera`.

It is up to each implementation to define and apply filters that meet their
local requirements. While multiple Logstash output targets may be defined,
simp_logstash_ only defines the Elasticsearch output by default. Please see
the Elasticsearch Puppet module for details on how to define additional output
targets.

The following diagram depicts the standard SIMP data flow through the Logstash
system.

.. image:: ../../../images/Logstash.png
   :scale: 35%
   :alt: Logstash Data Flow
   :align: center

SIMP Logstash Deployment
------------------------

Logstash, SIMP, and Security
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The provided SIMP modules for Logstash, Elasticsearch, and Grafana have been
built with connection security in mind. Overriding these settings could
adversely affect the security of the logging infrastructure. The following list
describes the security features in place with the default SIMP module settings:

.. WARNING::
   The native (Java) Elasticsearch connections, e.g., node-to-node
   connections, are not encrypted!

   This will be remedied in SIMP in the future, as sufficient methods
   are found. Presently, you can look at the `SIMP IPSec`_ implementation
   to encrypt communication between your Elasticsearch nodes.
   Alternatively, you can purchase a subscription to the Elasticsearch
   Security plugin as part of Elasticsearch X-Pack.

* **User Name and Password Protection for Grafana:**
    The Grafana web can be
    exposed to a defined list of hosts. If you are connecting to Grafana from
    anything other than the localhost, a user name and password is required for
    authentication. Both :term:`LDAP` and local database users are supported.  By
    default, only an admin account is created.  SIMP will automatically generate
    that password.

* **Syslog over Stunnel:**
    The default behavior in SIMP is to encrypt syslog
    traffic using native :term:`TLS` in rsyslog.  The logstash syslog
    configuration is set up to listen on a stunnel port, which then forwards to
    the local logstash syslog listener.  Unencrypted traffic is also supported
    for network devices.

* **Limiting Web Actions:**
    The Grafana module restricts what HTTP commands a
    user can perform on the Elasticsearch data store. Full **POST** action must
    be given to the Logstash nodes and some nodes may require **DELETE**
    capabilities. Logstash hosts should be tightly controlled so that
    administrative users cannot modify data inside of Elasticsearch with
    carefully crafted commands. This is one reason that we use syslog on the
    local hosts.

.. IMPORTANT::
   The Puppet modules for Logstash, Grafana, and Elasticsearch contain dozens
   of variables that may be manipulated.

   You should read each product's documentation and ensure you understand any
   setting that is changed from the default SIMP values. Changes can affect
   both security and functionality of the system.

Logstash Setup
--------------

Logstash System Requirements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The storage requirements for Logstash and Elasticsearch vary depending on how
long you plan on keeping logs. When using Elasticsearch, the logs are formatted
for Elasticsearch and stored in ``/var/elasticsearch``. You can also configure
how many days of data you wish to keep in Elasticsearch
``(keep_days => '99')``. Therefore, you should ensure you have enough space on
``/var`` to keep your defined number of days worth of logs.

As you grow your Elasticsearch cluster to handle increasing log loads, you will
want to ensure that your ``keep_days`` is set to handle your entire cluster
appropriately.

.. NOTE::
   You should have at least 4G of memory available on any Elasticsearch node.

.. IMPORTANT::
   It is not advised to install the ELG stack on your Puppet management
   infrastructure as both tend to use large amounts of system resources.

Recommended SIMP Logstash Setup
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following example can be applied to a single host with a large ``/var``
volume and 4GB of memory.

You can extend and replicate this setup on as many systems as necessary to
provide ingest and dashboard redundancy. Alternatively, you can split Grafana
and Logstash to allow greater resource dedication.

We do recommend that you have an Elasticsearch node on the Logstash system to
reduce the likelihood that Logstash will hang when trying to find a
non-existent storage node.

Optimization of your Elasticsearch infrastructure depends on many factors and
should be handled once you decide how far your system is going to expand.
Please be aware that scaling is highly dependent on how your actually use your
cluster in production.

We would recommend a search on `Elasticsearch Scaling`_ prior to setting up
your initial cluster.

The following configuration assumes Logstash and one Elasticsearch node
are collocated on one host, ``es1.<your domain>``:

.. code-block:: yaml

  ---
  # Add these settings to your Logstash node

  ## Set up Logstash ##

  # Listen on unencrypted UDP for legacy network devices
  #
  simp_logstash::input::syslog::listen_plain_udp


  # Send all output to the local Elasticsearch instance
  #
  simp_logstash::outputs :
    - 'elasticsearch'

  # Keep 30 days of logs
  #
  simp_logstash::clean::keep_days: '30'

  ## Set up Elasticsearch ##

  # Make this unique per cluster!  The elasticsearch service
  # for the cluster will be named
  #
  #    elasticsearch-<cluster_name>
  #
  simp_elasticsearch::cluster_name : 'some_unique_cluster_name'

  # The default value for simp_elasticsearch::bind_host assumes
  # an Elasticsearch host only has one interface. If this is not
  # true, set this to the appropriate value for each Elasticsearch
  # host in your system.
  #
  simp_elasticsearch::bind_host : "%{::ipaddress}"

  # This needs to be a list of *all* of the Elasticsearch nodes in the
  # cluster, (including the host with Logstash and Elasticsearch).
  # This is done to restrict communications to only trusted nodes
  #
  # Any node not entered here will not be connected to and will not
  # be allowed to communicate with the cluster.
  #
  simp_elasticsearch::unicast_hosts :
    - "es1.%{::domain}:9300"

  # Add your Grafana hosts to the apache ACL.
  simp_elasticsearch::http_method_acl :
    'limits' :
      'hosts' :
        'grafana.%{::domain}' : 'defaults'

  # Turn off client SSL verification *only* if you are connecting
  # to Grafana.  Otherwise, the default setting of 'require'
  # is best!
  #
  simp_elasticsearch::simp_apache::ssl_verify_client: 'none'


  ## Classes that you need to include for this setup

  classes:
    - 'simp_elasticsearch'
    - 'simp_logstash'
    # Include this if you wish to auto-purge your Elasticsearch records
    - 'simp_logstash::clean'

Deploying Additional Elasticsearch Nodes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When more than one Elasticsearch node are to be deployed in your system,
configuration of these nodes may be more easily handled using a group
match to pull your :term:`Hiera` settings. To do this, you should add
the following to your ``site.pp`` file for your environment.

.. code-block:: ruby

  if $trusted['certname'] =~ /es\d+\.your\.domain/ {
    $hostgroup = 'elasticsearch'
  }

Then, ensure that a file called 'elasticsearch.yaml' is present in the
``/etc/puppetlabs/code/environments/simp/hieradata/hostgroups/``
directory and contains the following content.

.. code-block:: yaml

  ---
  # All nodes running elasticsearch in your cluster should use
  # these settings.

  simp_elasticsearch::cluster_name: 'some_unique_cluster_name'

  # Remember, this must be the *complete* list of Elasticsearch nodes.
  #
  simp_elasticsearch::unicast_hosts :
    - "es1.%{::domain}:9300"
    - "es2.%{::domain}:9300"
    - "es3.%{::domain}:9300"
    - "es4.%{::domain}:9300"

  classes:
    - 'simp_elasticsearch'

Make sure you point your clients to the Logstash server by setting the
``$simp_options::syslog::log_servers`` variable to the FQDN of the
Logstash server in :term:`Hiera`.  You will also need to set
``simp_rsyslog::forward_logs: true`` and
``rsyslog::enable_tls_logging: true``,
to ensure logs are sent to Logstash Stunnel listener.

Deploying Grafana
^^^^^^^^^^^^^^^^^

Now that you have a functional logging setup, you will probably want to deploy a
GUI to provide the ability to generate user dashboards as well as dynamic log
analysis.

The SIMP team chose to support the Open Source `Grafana`_ project due to its
built-in authentication and access control support.  While the Grafana is great
at visualizing data, it can be challenging to explore your logs.  You could
easily point `Kibana`_ or another tool of your choosing at your
`Elasticsearch`_ cluster. You could also install Kibana alongside Grafana.
Since Kibana does not offer (free and open source) access control, you can
configure Kibana to listen to local host only and tightly control who can SSH
to your Kibana node.

.. NOTE::
   By default, the Grafana administrative password is randomly set using
   `simplib passgen()`_. You can use the :ref:`simp passgen` command to obtain
   the password for your environment.

.. NOTE::
   The ``rubygem-toml`` package must be present on your Puppet compile servers
   for the Grafana Puppet module to function properly.

   Starting with SIMP version 6.2, the ``pupmod-simp-simp_grafana`` rpm will
   automatically install this gem, by pulling in ``rubygem-puppetserver-toml``
   as an RPM dependency.

   If you do not install this via Kickstart, you will need two runs of Puppet
   to complete the Grafana installation since the TOML Ruby Gem will not be
   able to be installed prior to Puppet loading.

.. WARNING::
   Do **not** point Grafana directly at your Elasticsearch node unless you have
   a single-node deployment.

   Grafana has the ability to put **extreme** loads on your Elasticsearch
   infrastructure with poorly formed queries and should be connected to a node
   that is not used for ingest. This also helps prevent any vulnerabilities
   in Grafana from providing direct access to your Elasticsearch
   infrastructure.

Targeting your Grafana host or hostgroup, apply the following :term:`Hiera`
settings.

.. code-block:: yaml

  ---
  # Array of networks that are allowed to access your Grafana dashboard.
  # Uses the standard SIMP 'simp_options::trusted_nets' semantics.
  #
  # In this case, instead of using the default of
  # ``simp_options::trusted_nets``, we are allowing everyone in and
  # trusting that Grafana will do properly authenticate users using
  # the LDAP configured via the ``simp_options::ldap`` parameters.

  simp_grafana::trusted_nets:
    - 'ALL'

  classes:
    - 'simp_grafana'

After your Puppet run, you should be able to connect to port ``8443`` on your
Grafana host and authenticate with the administrative user.

Grafana LDAP Integration
````````````````````````

SIMP uses Grafana roles and maps them to :term:`LDAP` groups to provide access
control.

When you apply the SIMP Grafana class, Grafana will be configured for LDAP
authentication (assuming you are using SIMP LDAP).  The table below describes
the Grafana roles.

.. list-table:: Grafana Roles
   :widths: 15 30 55
   :header-rows: 1

   * - Grafana Role
     - SIMP LDAP Role
     - Permissions
   * - Viewer
     - simp_grafana_viewers
     - Can only view dashboards, not save / create them.
   * - Read Only Editors
     - simp_grafana_editors_ro
     - Can edit graphs and queries but not save dashboards.
   * - Editor
     - simp_grafana_editors
     - Can view, update and create dashboards.
   * - Admin
     - simp_grafana_admins
     - Everything an Editor can plus edit and add data sources and
       organization users.

All the system administrator needs to do is to create the LDAP groups
and assign users to those groups.  An example ``ldif`` for creating
the viewers group is as follows:

.. code-block:: ruby

   dn: cn=simp_grafana_viewers,ou=Group,dc=your,dc=domain
   objectClass: posixGroup
   objectClass: top
   cn: simp_grafana_viewers
   gidNumber: <Unique GID number>
   description: "Grafana Viewers"

An ``ldif`` such as the one below could then be used to add users
to that group:

.. code-block:: ruby

  dn: cn=simp_grafana_viewers,ou=Group,dc=your,dc=domain
  changetype: modify
  add: memberUid
  memberUid: <UID1>
  memberUid: <UID2>
  ...
  memberUid: <UIDX>

More information on managing LDAP users can be found in the
:ref:`User_Management` section.  Refer to the ``simp_grafana`` module for
additional information on using the Puppet module to manage Grafana LDAP
configuration.

Grafana Dashboards
``````````````````
SIMP can optionally install default Grafana dashboards, contained in
the ``simp_grafana`` RPM.  To install the dashboards in Grafana, set
``simp_grafana::simp_dashboards: true`` in the Hiera configuration for
your Grafana node.  The dashboards will reside in
``/var/lib/grafana/dashboards`` and will be read-only. If you want to
modify any of them, via the Grafana GUI, you must first save a copy of
each dashboard you want to customize.

.. _Elasticsearch: https://www.elastic.co/products/elasticsearch
.. _elasticsearch: https://www.elastic.co/products/elasticsearch
.. _elasticsearch-curator: https://www.elastic.co/products/elasticsearch
.. _elasticsearch module: https://github.com/elastic/puppet-elasticsearch
.. _Elasticsearch scaling: https://www.elastic.co/guide/en/elasticsearch/guide/master/_scale_horizontally.html
.. _Elasticsearch Upgrade Guide: https://www.elastic.co/guide/en/elasticsearch/reference/current/setup-upgrade.html
.. _Grafana: https://grafana.com/
.. _grafana: https://grafana.com/
.. _grafana module: https://github.com/voxpupuli/puppet-grafana
.. _Kibana: https://www.elastic.co/products/kibana
.. _Logstash: https://www.elastic.co/products/logstash
.. _logstash: https://www.elastic.co/products/logstash
.. _Logstash documentation: https://www.elastic.co/guide/en/logstash/current/index.html
.. _logstash module: https://github.com/elastic/puppet-logstash
.. _simp_elasticsearch: https://github.com/simp/pupmod-simp-simp_elasticsearch
.. _simp_logstash: https://github.com/simp/pupmod-simp-simp_logstash
.. _simp_grafana: https://github.com/simp/pupmod-simp-simp_grafana
.. _SIMP IPSec: https://github.com/simp/pupmod-simp-libreswan
.. _simplib passgen(): https://github.com/simp/pupmod-simp-simplib/blob/master/lib/puppet/parser/functions/passgen.rb

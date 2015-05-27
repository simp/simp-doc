Logstash
========

This chapter gives instruction for getting a basic configuration of
Logstash working in a SIMP environment.

Logstash
--------

`Logstash <http://logstash.net/>`__ is an open source tool that provides
a means for SIMP implementations to have logs and events collected,
searched, and forwarded (filtered or unfiltered) to another host. SIMP
comes with three separate but related modules. The modules are:

-  **Logstash:**\ Installs the RPMs and configuration needed for log
   inputs, filters, and outputs.

-  **Kibana:**\ Installs the RPMs and configuration needed for the
   Kibana 3 web interface.

-  **Elasticsearch:**\ Installs the RPMs and configuration needed for
   Elasticsearch.

    **Warning**

    The Logstash class is incompatible with the SIMP
    rsyslog::stock::server class! You cannot enable both of them on the
    same sever.

Logstash Architecture
---------------------

The overall model for Logstash is very simple. It takes inputs from
various sources, optionally applies filters, and outputs the results to
a specified target. It's likely that you can already forward logs to
Logstash and output them in a useful format as part of your existing
architecture.

Logstash filters can manipulate logs after ingest and before output.
Examples of existing filters include fixing logs to split/combine lines,
adding fields, normalizing time stamps, and adding GeoIP fields.
Depending on the type of log manipulation that is desired, there is
likely a filter and `associated
documentation <http://logstash.net/docs/1.1.10/>`__ that already exists.

Logstash SIMP Architecture
--------------------------

Applying the SIMP Logstash, Elasticsearch, and Kibana modules provides
an implementation with a functioning log reduction and search
capability. Unless scale dictates otherwise, these three modules can
easily be applied to a single host.

The intent of providing Logstash in SIMP is to replace the default
Rsyslog server with a capability that is easier to search and analyze
over time. Once your Logstash server is set up, you simply need to
direct your hosts to forward logs to your Logstash server. In a default
SIMP configuration, this can be done by setting the $log\_server
variable in hiera.

    **Note**

    SIMP does **NOT** apply any filters to the logs by default.

It is up to each implementation to define and apply filters that meet
their local requirements. While multiple output targets may be defined,
SIMP only defines the Elasticsearch output by default. Please see the
Elasticsearch Puppet module for details on how to define additional
output targets.

SIMP Logstash Fow
-----------------

Logstash, SIMP, and Security
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The provided SIMP modules for Logstash, Elasticsearch, and Kibana have
been built with connection security in mind. Overriding these settings
could adversely effect the security of the logging infrastructure. The
following list describes the security features in place with the default
SIMP module settings:

    **Warning**

    The native (Java) Elasticsearch connections are not encrypted! This
    will be remedied in the future as sufficient methods are found.

-  **User Name and Password Protection for Kibana:**\ The Kibana web can
   be exposed to a defined list of hosts. If you are connecting to
   Kibana from anything other than the localhost, a user name and
   password is required for authentication. Both LDAP and local database
   users are supported.

-  **Syslog over Stunnel:**\ The default behavior in SIMP is to encrypt
   syslog traffic over Stunnel. This remains the case with Logstash.
   Unencrypted traffic is also supported for network devices.

-  **Limiting Web Actions:**\ The Kibana module restricts what HTTP
   commands a user can perform on the Elasticsearch data store. Full
   POST action must be given to the Logstash nodes and some nodes may
   require DELETE capabilities. Logstash hosts should be tightly
   controlled so that administrative users cannot modify data inside of
   Elasticsearch with carefully crafted commands. This is one reason
   that we use syslog on the local hosts.

    **Important**

    The Puppet modules for Logstash, Kibana, and Elasticsearch contain
    dozens of variables that may be manipulated. You should read each
    product's documentation and ensure you understand any setting that
    is changed from the default SIMP values. Changes can effect both
    security and functionality of the system.

Logstash Setup
--------------

Logstash System Requirements
----------------------------

The storage requirements for Logstash and Elasticsearch vary depending
on how long you plan on keeping logs. If you use the settings in ?, then
your logs are not being filtered and are being sent to Elasticsearch.
When using Elasticsearch, the logs are formatted for Elasticsearch and
stored in /var/elasticsearch. You can also configure how many days of
data you wish to keep in Elasticsearch (keep\_days => '99'). Therefore,
you should ensure you have enough space on /var to keep your defined
number of days worth of logs.

As you grow your Elasticsearch cluster to handle increasing log loads,
you will want to ensure that your keep\_days is set to handle your
entire cluster appropriately.

    **Note**

    You should have at least 4G of memory available on any Elasticsearch
    node.

    **Important**

    You should NOT install Logstash, Elasticsearch, nor Kibana on your
    Puppet master. There will likely be conflicts with Apache and
    resource limitations.

Logstash Module Recommended SIMP Setup
--------------------------------------

The following example manifest can be applied to a single host with a
large /var volume and 4GB of memory.

.. code-block:: Ruby

          ---
          # Add these settings to only your Logstash node.

          apache::ssl::sslverifyclient: %{hiera('kibana::ssl_verify_client')}

          kibana::redirect_web_root: true
          kibana::ssl_allowroot: %{hiera('client_nets')}
          kibana::ssl_verify_client: 'none'
          # You can add more groups under ldap_groups if you want others
          # to be able to access your Kibana instance.
          #
          # Remember, whitespace matters!
          #
          kibana::method_acl:
            'method':
              'ldap':
                'enable': true
            'limits':
              'users':
                'valid-user': 'defaults'
              'ldap_groups':
                'cn=administrators,ou=Group,dc=your,dc=domain': 'defaults'

          logstash::simp::keep_days: '30'

          elasticsearch::simp::manage_httpd: 'conf'

          classes:
            - 'logstash::simp'
            - 'kibana'
              

In the case of the Elasticsearch node setup below, it may be better to
use a group match to pull your Hiera settings. To do this, you should
add the following to a file like /etc/puppet/manifests/nodegroups.pp

.. code-block:: Ruby

          if $trusted['certname'] =~ /es\d+\.your\.domain/ {
            $hostgroup = 'elasticsearch'
          }
            

Then, ensure that a file called 'elasticsearch.yaml' is present in the
/etc/puppet/hieradata/hostgroups directory and contains the following
content.

.. code-block:: Ruby

          ---
          # All nodes running elasticsearch in your cluster should use
          # these settings.
          elasticsearch::simp::cluster_name: 'a_unique_hard_to_guess_name'
          # This can be no more than the total number of ES nodes that you
          # have in your cluster.
          elasticsearch::simp::replicas: '2'
          elasticsearch::simp::java_install: true

          classes:
            - 'elasticsearch::simp'
              

Make sure you point your clients to the Logstash server by setting the
'log\_server' variable to the fqdn of the Logstash server in hiera. This
is further covered in ?.

Using LogStash and ElasticSearch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

With the default settings applied, you should be able to connect to port
443 on your Kibana host. If connecting from localhost, you will not be
prompted for a password. If you are connecting from an external host, a
valid LDAP account with that user being defined in the Kibana Class is
needed. The page is SSL protected so use https://<hostname>/kibana

With the web interface up, you now have the ability to search logs.

There are several resources available to help with searching. The Kibana
`Overview Page <http://www.elasticsearch.org/overview/kibana/>`__ and
`Elasticsearch Guide <http://www.elasticsearch.org/guide/>`__ are a good
place to start. You should also visit the main `Logstash
page <http://logstash.net/>`__ to see demonstrations and read their tips
for searching logs.

.. image:: ../images/Logstash.svg

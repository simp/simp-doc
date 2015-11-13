Installation_Miscellaney
========================

This sections provides a list of variables that are configurable during
the install.

.. _List of Installation Variables:

List of Installation Variables
------------------------------

+-------------------------------------------+------------------+-------------------+-----------------+
|Description                                |  Default Setting | Puppet Variable   | Section         |
+===========================================+==================+===================+=================+
| Enable FIPS-140-2 compliance              | enabled          |  use_fips         | FIPS            |
+-------------------------------------------+------------------+-------------------+-----------------+
| Do you want to set up network interface   |                  | none -            | SYSTEM          |
| - use DHCP or Static for NIC              | static           | The device is     |                 |
| - Hostname of server                      | puppet.change.me | confgured         |                 |
| - IP Address of server                    | none             | none              |                 |
| - Netmask                                 | none             | none              |                 |
| - Default gateway                         | none             | none              |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| Your DNS server                           | IP this install  | dns::servers      |  DNS            |
+-------------------------------------------+------------------+-------------------+-----------------+
| The search domain for DNS.                | change.me        | dns::search       |  DNS            |
+-------------------------------------------+------------------+-------------------+-----------------+
| Subnet used for clients managed by the    | subnet of IP     | client_nets       |  PUPPET         |
| puppet server                             | this install     |                   |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| NTP servers.                              | none             | ntpd::servers     |  NTP            |
+-------------------------------------------+------------------+-------------------+-----------------+
| IP addr of primary log server (rsyslog)   | none             | log_servers       |  RSYSLOG        |
+-------------------------------------------+------------------+-------------------+-----------------+
| IP address of failover log server.        | none             | failover_log\     | RSYSLOG         |
|                                           |                  | _server           |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| Yum server for simp modules.              | IP this install  | simp::yum\        | YUM             |
|                                           |                  | ::servers         |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| Turn on the audit deamon?                 | true             | use_auditd        | SYSTEM          |
+-------------------------------------------+------------------+-------------------+-----------------+
| Turn on iptable deamon?                   | true             | use_iptables      | SYSTEM          |
+-------------------------------------------+------------------+-------------------+-----------------+
| The default system run level              | 3                | common::runlevel  | SYSTEM          |
+-------------------------------------------+------------------+-------------------+-----------------+
| Do you want to set SELINUX to enforcing?  | true             | selinux::ensure   |  SYSTEM         |
+-------------------------------------------+------------------+-------------------+-----------------+
| Set a grub password on the puppet server? | true             | set_grub_password |   GRUB          |
+-------------------------------------------+------------------+-------------------+-----------------+
| Make puppet server the master yum server? | true             | is_master_yum\    | YUM             |
|                                           |                  | _server           |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| The FQDN of the puppet server.            | puppet.change.me | puppet::server    | PUPPET          |
+-------------------------------------------+------------------+-------------------+-----------------+
| Puppet servers IP address.                | current IP       | puppet::server\   | PUPPET          |
|                                           |                  | ::ip              |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| FQDN of Puppet Certificate Authority (CA) | puppet server    | puppet::ca        | PUPPET          |
+-------------------------------------------+------------------+-------------------+-----------------+
| The port Puppet CA will listen on.        | 8141             | puppet::ca_port   | PUPPET          |
+-------------------------------------------+------------------+-------------------+-----------------+
| The DNS name of puppet database server.   | puppet server    | puppetdb::master\ | PUPPET          |
|                                           |                  | ::config\         |                 |
|                                           |                  | ::puppetdb_server |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| The port used by the puppet database      | 8139             | puppetdb::master\ | PUPPET          |
| server                                    |                  | ::config\         |                 |
|                                           |                  | ::puppetdb_port   |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| Do you want to use LDAP?                  |  true            | use_ldap          | PUPPET          |
+-------------------------------------------+------------------+-------------------+-----------------+
| LDAP Server Base Distinquish Name (DN)    | generate from    |                   | LDAP            |
|                                           | puppetsrv name   | ldap::basedn      |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| The LDAP Bind Distiquished name.          | generate from    |                   | LDAP            |
|                                           | LDAP base DN     | ldap::bind_dn     |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| LDAP Bind password                        | yes              | ldap::bind_hash   | LDAP            |
+-------------------------------------------+------------------+-------------------+-----------------+
| LDAP Sync Distiquished name.              | generate from    |                   | LDAP            |
|                                           | LDAP base DN     | ldap::sync_dn     |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| LDAP Sync password                        | yes              | ldap::sync_pw     | LDAP            |
+-------------------------------------------+------------------+-------------------+-----------------+
| The LDAP root DN.                         | generated from   |                   |                 |
|                                           | the ldap::basedn | ldap::root_dn     | LDAP            |
+-------------------------------------------+------------------+-------------------+-----------------+
| LDAP root password                        |  no              | ldap::root_hash   | LDAP            |
| This password is used for manually        |                  |                   |                 |
| updating LDAP, you will want to set it    |                  |                   |                 |
| your self.                                |                  |                   |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| The URI for your LDAP server.             | ldap:://         |                   |                 |
|                                           | <puppetsrvFQDN>  | ldap::master      | LDAP            |
+-------------------------------------------+------------------+-------------------+-----------------+
| The directory that will hold files used   | /var/simp/rsync/ |                   | RSYNC           |
| to sync oprational directories            | OSTYPE/MJRREL    |                   |                 |
+-------------------------------------------+------------------+-------------------+-----------------+
| The server that remote syncs              | 127.0.0.1        |  rsync::server    | RSYNC           |
+-------------------------------------------+------------------+-------------------+-----------------+
| Maximum rsync timeout in seconds          | 1                |  rsync::timeout   | RSYNC           |
+-------------------------------------------+------------------+-------------------+-----------------+

Configuration
-------------

This briefly describes what is being configured in the different
sections indicated in the table above.

You may make changes to the default settings in `` `puppet config print environmentpath`/simp/hieradata/simp_def.yaml` `` or one of the other yaml files in the hieradata directory.

These :term:`Hiera` files can be used after initial set up to change settings.
The :ref:`Hiera` section gives an introduction of using Hiera in SIMP.

FIPS
++++

- Turning on and off :term:`FIPS` mode sets kernel parameters and systems
  environment variables to ensure the system is FIPS 140-2 compliant.
- FIPS is on by default. If you ever want to have your system to beFIPS
  compliant, you will want to ensure that the system is built with this
  enabled. It may easily be disabled once the system is built.

GRUB
++++

.. only:: simp_4

  -  Grub password in ``/etc/grub.conf``

.. only:: not simp_4

  -  Grub password in ``/etc/grub2.cfg``

DNS
+++

- The /etc/resolv.conf
- The :term:`DNS` server capabilities are not configured by this.

SYSTEM
++++++

- Basic network setup.
- Startup files in /etc/init.d.
- Configuration files under /etc/sysconfig.
- Rsyslog settings.

PUPPET
++++++

- Autosigning in ``*/etc/puppet/autosign.conf``
- File Serving in ``*/etc/puppet/fileserver.conf``
- Puppet server and :term:`Certificate Authority` (CA) information in ``/etc/puppet/puppet.conf``
- Server certificates for the puppet host (Fake CA)

LDAP
++++

- If you select use_ldap and set this server as your :term:`LDAP`
  server, OpenLDAP Puppet will enable the LDAP service on this server
  and all clients will be set to reference it for authentication.
- If you select use_ldap and set another server as your LDAP server,
  then the clients (including this server) will use the specified server
  instead.
- If you choose not to use LDAP the system is set up to use traditional
  local authentication only.

RSYNC
+++++

- The puppet server is configured to rsync data directories for services
  like :term:`DNS`, :term:`DHCP` or :term:`TFTP`.


YUM
+++

-  Base :term:`YUM` repositories for :term:`RPM` updates.

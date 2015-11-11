Appendix B
==========

This sections provides a list of variables that are configurable during the install.

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

Table: Sample values for SIMP install


Configuration
-------------

This briefly describes what is being configured in the different sections indicated in the table above. 

If you make changes to the default settings these are usually set in

.. only:: not simp_4

  -  ``/etc/puppet/environments/simp/hieradata/simp_def.yaml``

.. only:: simp_4

  -  ``/etc/puppet/hieradata/simp_def.yaml``

or one of the other yaml files in the hieradata directory.

These Hiera files can be used after initial set up to change settings.  The :ref:`Hiera` section gives an introduction to SIMPS use of Hiera.

FIPS

- Turning on and off FIPS mode sets kernel parameters and systems environment variables to ensure the system is FIPS-140-2 compliant. This has compatability issues and you should understand FIPS if you are turning this on.
- This can be reset after installation without rebuilding.

GRUB

-  Grub password in ``/boot/grub/grub.conf``

DNS

- The /etc/resolv.conf
- The DNS server is not set up by this.  There are instructions for that.

SYSTEM

-  Basic network setup under /etc/sysconfig. Puppet does not control network card settings and these can be changed through system utilities.
-  Start up files in /etc/init.d
-  Configuration files under /etc
-  Rsyslog settings for the server and clients.

PUPPET

-  Autosigning in ``*/etc/puppet/autosign.conf``
-  Fileserving in ``*/etc/puppet/fileserver.conf``
-  Puppet server and Certificate Authority (CA) information in ``/etc/puppet/puppet.conf``
-  Server certificates for the puppet server itself (Fake CA)
-  There are instructions for installing Official PKI certificates after initial configuration.

LDAP

- if you select use_ldap and set this server as LDAP, open ldap is configured on this server and puppet is configured to point clients to this server for authentication.
- if you select use_ldap and point to another server /etc/slapd files are configured to point you to the LDAP server and puppet is set up to point any clients to that server.
- if you select not to use LDAP the system is set up to use local authenticaton.

RSYNC

- The puppet server is configured to rsync data directories for services like DNS, DHCP or TFTP.   


YUM

-  Base YUM repositories 




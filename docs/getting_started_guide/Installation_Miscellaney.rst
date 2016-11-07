Installation Miscellany
=======================

.. _List of Installation Variables:

Installation Variables
------------------------------
This section describes the list of variables that are configurable during
the install. These variables are written to
``/etc/puppet/environments/simp/hieradata/simp_def.yaml`` by ``simp config``
and are derived from user input.

+-------------------------------------------+--------------------+----------+
|Description                                |Variable            | Category |
+===========================================+====================+==========+
| Enable FIPS-140-2 compliance;             | use_fips           | FIPS     |
| *true* or *false*                         |                    |          |
+-------------------------------------------+--------------------+----------+
| Network interface to use                  | network::interface | NETWORK  |
+-------------------------------------------+--------------------+----------+
| Whether to set up the network interface;  | network::setup_nic | NETWORK  |
| *true* or *false*                         |                    |          |
+-------------------------------------------+--------------------+----------+
| Whether to use DHCP for the network;      | dhcp               | NETWORK  |
| *dhcp* to enable DHCP, *static* otherwise |                    |          |
+-------------------------------------------+--------------------+----------+
| FQDN of server                            | hostname           | NETWORK  |
+-------------------------------------------+--------------------+----------+
| IP address of server                      | ipaddress          | NETWORK  |
+-------------------------------------------+--------------------+----------+
| Netmask of the system                     | netmask            | NETWORK  |
+-------------------------------------------+--------------------+----------+
| Default gateway                           | gateway            | NETWORK  |
+-------------------------------------------+--------------------+----------+
| List of DNS servers for the managed hosts |\dns::servers       | DNS      |
+-------------------------------------------+--------------------+----------+
| Search domain for DNS                     |\dns::search        | DNS      |
+-------------------------------------------+--------------------+----------+
| Subnet used for clients managed by the    | client_nets        | PUPPET   |
| puppet server                             |                    |          |
+-------------------------------------------+--------------------+----------+
| NTP servers                               | ntpd::servers      |  NTP     |
+-------------------------------------------+--------------------+----------+
| IP address of primary log server          | log_servers        | RSYSLOG  |
+-------------------------------------------+--------------------+----------+
| IP address of failover log server         |failover_log_server | RSYSLOG  |
+-------------------------------------------+--------------------+----------+
| Yum server for simp modules               | simp::yum::servers | YUM      |
+-------------------------------------------+--------------------+----------+
| Whether to use the audit daemon;          | use_auditd         | SYSTEM   |
| *true* or *false*                         |                    |          |
+-------------------------------------------+--------------------+----------+
| Whether to use the iptables daemon;       | use_iptables       | SYSTEM   |
| *true* or *false*                         |                    |          |
+-------------------------------------------+--------------------+----------+
| Default system run level; 1-5             | simplib::runlevel  | SYSTEM   |
+-------------------------------------------+--------------------+----------+
| SELINUX mode to use;                      | selinux::ensure    | SYSTEM   |
| *enforcing*, *permissive*, or *disabled*  |                    |          |
+-------------------------------------------+--------------------+----------+
| Whether to set a GRUB password on the     | set_grub_password  | GRUB     |
| server; *true* or *false*                 |                    |          |
+-------------------------------------------+--------------------+----------+
| GRUB password hash                        | grub::password     | GRUB     |
+-------------------------------------------+--------------------+----------+
| Whether puppet server will be a yum       |is_master_yum\      | YUM      |
| server; *true* or *false*                 |_server             |          |
+-------------------------------------------+--------------------+----------+
| FQDN of the puppet server                 |puppet::server      | PUPPET   |
+-------------------------------------------+--------------------+----------+
| Puppet servers IP address                 |puppet::server::ip  | PUPPET   |
+-------------------------------------------+--------------------+----------+
| FQDN of Puppet Certificate Authority (CA) |puppet::ca          | PUPPET   |
+-------------------------------------------+--------------------+----------+
| Port Puppet CA will listen on             |puppet::ca_port     | PUPPET   |
+-------------------------------------------+--------------------+----------+
| DNS name of puppet database server        |puppetdb::master\   | PUPPET   |
|                                           |::config\           |          |
|                                           |::puppetdb_server   |          |
+-------------------------------------------+--------------------+----------+
| Port used by the puppet database          |puppetdb::master\   | PUPPET   |
| server                                    |::config\           |          |
|                                           |::puppetdb_port     |          |
+-------------------------------------------+--------------------+----------+
| Whether to use LDAP; *true* or *false*    |use_ldap            | PUPPET   |
+-------------------------------------------+--------------------+----------+
| LDAP Server Base Distinguished Name       |\ldap::base_dn      | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP Bind Distinguished Name              |\ldap::bind_dn      | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP Bind password                        |\ldap::bind_pw      | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP Bind password hash                   |\ldap::bind_hash    | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP Sync Distinguished Name              |\ldap::sync_dn      | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP Sync password                        |\ldap::sync_pw      | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP Sync password hash                   |\ldap::sync_hash    | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP root Distinguished Name              |\ldap::root_dn      | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP root password hash                   |\ldap::root_hash    | LDAP     |
+-------------------------------------------+--------------------+----------+
| LDAP master URI                           |\ldap::master       | LDAP     |
+-------------------------------------------+--------------------+----------+
| List of OpenLDAP server URIs              |\ldap::uri          | LDAP     |
+-------------------------------------------+--------------------+----------+
| List of SSSD domains                      |\ldap::master       | SYSTEM   |
+-------------------------------------------+--------------------+----------+
| Root location of files to be distributed  |rsync::base         | RSYNC    |
| via rsync                                 |                    |          |
+-------------------------------------------+--------------------+----------+
| Rsync server; typically *127.0.0.1* for   | rsync::server      | RSYNC    |
| rsync over stunnel, which is the default  |                    |          |
| protocol stack for this capability        |                    |          |
+-------------------------------------------+--------------------+----------+
| Maximum rsync timeout in seconds          | rsync::timeout     | RSYNC    |
+-------------------------------------------+--------------------+----------+

.. _simp config Actions:

simp config Actions
-------------------

In addition to creating ``simp_defs.yaml``, ``simp config`` also
performs a limited set of actions in order to prepare the system for
bootstrapping.

+---------------+--------------------------------------------------------------+
|Category       |Actions Performed                                             |
+===============+==============================================================+
|FIPS           | When the user selects to enable :term:`FIPS`,                |
|               | ``simp config`` will set the Puppet digest algorithm to      |
|               | *sha256* to prevent any Puppet-related actions executed by   |
|               | ``simp config`` from using MD5 checksums. Note that this is  |
|               | **not** all that must be done to enable FIPS. The complete   |
|               | set of actions required to to enable FIPS is handled by      |
|               | ``simp bootstrap``.                                          |
+---------------+-----------------+--------------------------------------------+
|Network        | - When the user selects to configure the network interface,  |
|               |   ``simp config`` uses Puppet to set the network interface   |
|               |   parameters in system networking files and to bring up the  |
|               |   interface.                                                 |
|               | - ``simp config`` sets the hostname.                         |
+---------------+--------------------------------------------------------------+
|GRUB           |.. only:: simp_4                                              |
|               |                                                              |
|               |  When the user selects to set the GRUB password              |
|               |  ``simp config`` will set the password in                    |
|               |  ``/etc/grub.conf``.                                         |
|               |                                                              |
|               |.. only:: not simp_4                                          |
|               |                                                              |
|               |  When the user selects to set the GRUB password,             |
|               |  ``simp config`` will set the password in                    |
|               |  ``/etc/grub2.cfg``.                                         |
+---------------+--------------------------------------------------------------+
|Certificates   | If no certificates for the host are found in                 |
|               | ``/etc/puppet/environments/simp/keydist``, ``simp config``   |
|               | will use the FakeCA to generate certificates needed by SIMP  |
|               | for the host.  These certificates are independent of the     |
|               | certificates managed by Puppet, itself.                      |
+---------------+--------------------------------------------------------------+
|System Hiera   | If a hosts yaml file in                                      |
|               | ``/etc/puppet/environments/simp/hieradata/hosts`` does not   |
|               | already exist, ``simp config`` will create one from a SIMP   |
|               | template.                                                    |
+---------------+--------------------------------------------------------------+
|YUM Update     |.. only:: simp_4                                              |
|               |                                                              |
|               |  ``simp config`` updates the appropriate YUM Updates         |
|               |  repository contained at                                     |
|               |  ``/srv/www/yum/OSTYPE/MAJORRELEASE/ARCH``.                  |
|               |                                                              |
|               |.. only:: not simp_4                                          |
|               |                                                              |
|               |  ``simp config`` updates the appropriate YUM Updates         |
|               |  repository contained at                                     |
|               |  ``/var/www/yum/OSTYPE/MAJORRELEASE/ARCH``.                  |
+---------------+--------------------------------------------------------------+
|Puppet         | - Updates ``/etc/puppet/autosign.conf``.                     |
|               | - Updates ``/etc/puppet/puppet.conf``, after creating a      |
|               |   backup of the existing file. This update will include      |
|               |   FIPS-related settings, as appropriate.                     |
|               | - Updates ``/etc/hosts`` to ensure puppet server entries     |
|               |   exist.                                                     |
+---------------+--------------------------------------------------------------+
|LDAP           | ``simp config`` adds or removes the ``simp::ldap_server``    |
|               | setting from the hosts YAML file in                          |
|               | ``/etc/puppet/environments/simp/hieradata/hosts``, based on  |
|               | whether the user opts to use or not use LDAP, respectively.  |
+---------------+--------------------------------------------------------------+

.. todo simp bootstrap Actions

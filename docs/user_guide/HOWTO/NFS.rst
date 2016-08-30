HOWTO Configure NFS
===================

.. contents:: This chapter describes multiple configurations of NFS including:
  :local:

All implementations are based on ``pupmod-simp-nfs`` and ``pupmod-simp-simp``.

Exporting Non-Home Directories
------------------------------

**Goal:** Export ``/srv/nfs_share`` on the server, mount as ``/mnt/nfs`` on the
client.

default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

  nfs::server:            "your.server.fqdn"
  nfs::server::client_ips: "%{alias('client_nets')}"
  nfs::simp_iptables:      true
  nfs::simp_krb5:          false

Server
^^^^^^

In ``site/manifests/nfs_server.pp``:

.. code-block:: puppet

  class site::nfs_server {
    include '::nfs'

    file { '/srv/nfs_share':
      ensure => 'directory',
      owner  => 'root',
      group  => 'root',
      mode   => '0644'
    }

    nfs::server::export { 'nfs4_root':
      client      => ['*'],
      export_path => '/srv/nfs_share',
      sec         => ['sys'],
    }

    File['/srv/nfs_share'] -> Nfs::Server::Export['nfs4_root']
   }

In ``hosts/<your_server_fqdn>.yaml``:

.. code-block:: puppet

  nfs::is_server: true

  classes:
    - 'site::nfs_server'

Client
^^^^^^

In ``site/manifests/nfs_client.pp``:

.. code-block:: puppet

  class site::nfs_client {
    include '::nfs'

    file { '/mnt/nfs':
      ensure => 'directory',
      mode => '755',
      owner => 'root',
      group => 'root'
    }

    mount { "/mnt/nfs":
      ensure  => 'mounted',
      fstype  => 'nfs4',
      device  => '<your_server_fqdn>:/srv/nfs_share',
      options => 'sec=sys'
    }

    File['/mnt/nfs'] -> Mount['/mnt/nfs']
   }

In ``hosts/<your_client_fqdn>.yaml``:

.. code-block:: puppet

  nfs::is_server: false

  classes:
    - 'site::nfs_client'


Exporting home directories
--------------------------

**Goal:** Export home directories for LDAP users.

Utilize three stock classes from ``pupmod-simp-simp``:

  #. ``simp::export_home`` : Configures an NFS server to share centralized home
     directories using NFSv4
  #. ``simp::home_client`` : Configures an NFS client to point at the server
     created by ``simp::export_home``.
  #. ``simp::create_home_dirs`` : Optional hourly cron that binds to a LDAP
     server, ``ldap::uri`` by default, and creates a NFS home directory for all
     users in the LDAP server. Also expires any home directories for users that
     no longer exist in LDAP.

.. note::
   The NFS deamon may take time to reload after module application.  If your
   users do not have home directories immediately after application or it takes
   a while to log in, don't panic!

.. note::
   Any users logged onto a host at the time of module application will not have
   their home directories re-mounted until they log out and log back in.

default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

  nfs::server:             "your.server.fqdn"
  nfs::server::client_ips: "%{alias('client_nets')}"
  nfs::simp_iptables:      true
  nfs::simp_krb5:          false

Server
^^^^^^

.. code-block:: yaml

  nfs::is_server: true
  simp::nfs::export_home::create_home_dirs: true

  classes:
    - 'simp::nfs::export_home'
    - 'simp::nfs::home_client'

Client
^^^^^^

.. code-block:: yaml

  nfs::is_server: false

  classes:
    - 'simp::nfs::home_client'


Enabling Stunnel
----------------

If you wish to encrypt your NFS data using stunnel, set the following in
``default.yaml``:

.. code-block:: yaml

  nfs::use_stunnel : true


Enabling krb5
-------------

.. warning::

  This functionality is incomplete. See ticket SIMP-1400 in our
  `JIRA Bug Tracking`_ . Until that ticket is resolved, it is
  HIGHLY recommended you continue to use stunnel for encrypted
  nfs traffic.

default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

  classes:
    - 'krb5::keytab'

  nfs::server:             "your.server.fqdn"
  nfs::server::client_ips: "%{alias('client_nets')}"
  nfs::simp_iptables:      true
  nfs::secure_nfs:         true
  simp_krb5:               true


  krb5::kdc::auto_keytabs::global_services:
    - 'nfs'


Server
^^^^^^

.. code-block:: yaml

  nfs::is_server: true
  simp::nfs::export_home::create_home_dirs: true

  classes:
    - 'simp::nfs::export_home'
    - 'simp::nfs::home_client'
    - 'krb5::kdc'

Client
^^^^^^

.. code-block:: yaml

  nfs::is_server: false

  classes:
    - 'simp::nfs::home_client'


.. _JIRA Bug Tracking: https://simp-project.atlassian.net/

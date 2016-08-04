Configuring NFS
===============

This chapter describes multiple instantiations of NFS including:

  1. Exporting non-home directories
  2. Exporting home directories
  3. Enabling Stunnel
  4. Coming soon: Enabling Kerberos

All implementations are based on pupmod-simp-nfs and pupmod-simp-simp.

Exporting Non-Home Directories
------------------------------

Directive: Export /srv/nfs_share on the server, mount as /mnt/nfs on the client.

## Default.yaml

.. code-block:: yaml

   nfs::server :            "your.server.fqdn"
   nfs::server::client_ips: "%{alias('client_nets')}"
   nfs::simp_iptables:      true
   nfs::simp_krb5:          false

## Server

In site/manifests/nfs_server.pp:

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

In hosts/<your_server_fqdn>.yaml:

.. code-block:: puppet

   nfs::is_server: true

   classes:
     - 'site::nfs_server'

## Client

In site/manifests/nfs_client.pp:

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

In hosts/<your_client_fqdn>.yaml:

.. code-block:: puppet

   nfs::is_server: false

   classes:
     - 'site::nfs_client'


Exporting home directories
--------------------------

Directive: Export LDAP user's home directories.

Utilize three stock-classes from pupmod-simp-simp:
  1. simp::export_home      : Configures an NFS server to share centralized home
    directories, in the NFSv4 way.
  2. simp::home_client      : Configures an NFS client to point at the server
    created by simp::export_home.
  3. simp::create_home_dirs : Sets up an hourly cron to bind with an LDAP server,
    ldap::uri by default, to store all user's home directory paths, and create
    those which don't exist.  This functionality is optional.

.. note::
   The NFS deamon may take time to reload after module application.  If your
   users do not have home directories immediately after application or it
   takes a WHILE to log in, don't panic!

.. note::
   Any users logged onto a host at the time of module application will not have
   their home directories re-mounted until they log out and log back in.

## Default.yaml

.. code-block:: yaml

   nfs::server :            "your.server.fqdn"
   nfs::server::client_ips: "%{alias('client_nets')}"
   nfs::simp_iptables:      true
   nfs::simp_krb5:          false

## Server

.. code-block:: yaml

   nfs::is_server: true
   simp::nfs::export_home::create_home_dirs: true

   classes:
     - 'simp::nfs::export_home'
     - 'simp::nfs::home_client'

## Client

.. code-block:: yaml

   nfs::is_server: false

   classes:
     - 'simp::nfs::home_client'


Enabling Stunnel
----------------

If you wish to encrypt your NFS data over stunnel, in default.yaml set:

.. code-block:: yaml

   nfs::use_stunnel : true

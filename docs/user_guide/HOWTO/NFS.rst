HOWTO Configure NFS
===================

.. contents:: This chapter describes multiple configurations of NFS including:
  :local:

All implementations are based on ``pupmod-simp-nfs``, ``pupmod-simp-simp_nfs``,
and ``pupmod-simp-simp``.

.. NOTE::

  pupmod-simp-simp_nfs is not a core module, and may need to be installed prior
  to applying the following manifests.


Exporting Non-Home Directories
------------------------------

**Goal:** Export ``/var/nfs_share`` on the server, mount as ``/mnt/nfs`` on the
client.


default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

  nfs::server: "your.server.fqdn"

Server
^^^^^^

In ``site/manifests/nfs_server.pp``:

.. code-block:: puppet

  class site::nfs_server (
    Boolean          $kerberos     = simplib::lookup('simp_options::kerberos', { 'default_value' => false }),
    Simplib::Netlist $trusted_nets = simplib::lookup('simp_options::trusted_nets', { 'default_value' => ['127.0.0.1'] }),
  ){
    include '::nfs'

    $nfs_security = $kerberos ? { true => 'krb5p', false => 'sys' }

    file { '/var/nfs_share':
      ensure => 'directory',
      owner  => 'root',
      group  => 'root',
      mode   => '0644'
    }

    nfs::server::export { 'nfs4_root':
      clients     => $trusted_nets,
      export_path => '/var/nfs_share',
      sec         => [$nfs_security],
      require     => File['/var/nfs_share']
    }
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

  class site::nfs_client (
    Boolean $kerberos = simplib::lookup('simp_options::kerberos', { 'default_value' => false }),
  ){
    include '::nfs'

    $nfs_security = $kerberos ? { true => 'krb5p', false =>  'sys' }

    file { '/mnt/nfs':
      ensure => 'directory',
      mode => '755',
      owner => 'root',
      group => 'root'
    }

    mount { "/mnt/nfs":
      ensure  => 'mounted',
      fstype  => 'nfs4',
      device  => '<your nfs server>:/var/nfs_share',
      options => "sec=${nfs_security}",
      require => File['/mnt/nfs']
    }
  }

In ``hosts/<your_client_fqdn>.yaml``:

.. code-block:: yaml

  nfs::is_server: false

  classes:
    - 'site::nfs_client'


Exporting home directories
--------------------------

**Goal:** Export home directories for LDAP users.

Utilize the SIMP profile module ``simp_nfs``:

  #. ``simp_nfs``: Manages client and server configurations for managing nfs
     home directories.
  #. ``simp_nfs::export_home::create_home_dirs``: Optional hourly cron that
     binds to a LDAP server, ``ldap::uri`` by default, and creates a NFS home
     directory for all users in the LDAP server. Also expires any home
     directories for users that no longer exist in LDAP.

.. NOTE::

   The NFS deamon may take time to reload after module application.  If your
   users do not have home directories immediately after application or it takes
   a while to log in, don't panic!

.. NOTE::

   Any users logged onto a host at the time of module application will not have
   their home directories re-mounted until they log out and log back in.


default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

  nfs::is_server: false
  simp_nfs::home_dir_server: <your nfs server>

  classes:
    - simp_nfs


Server
^^^^^^

.. code-block:: yaml

  nfs::is_server: true
  simp_nfs::export_home::create_home_dirs: true

  classes:
    - simp_nfs::export::home


Enabling/Disabling Stunnel
--------------------------

Stunnel is a means to encrypt your NFS data.

Enable
^^^^^^

If simp_options::stunnel is set to true, you need only specify the following,
in the server's yaml file:

.. NOTE::

  The following is set to prevent a cyclical connection of stunnel to itself,
  in the event the server is a client of itself.

.. code-block:: yaml

  nfs::client::stunnel::nfs_server: <your nfs server>


If simp_options::stunnel is set to false and you don't wish to globally enable
stunnel, you will also need to set the following, in default.yaml:

.. code-block:: yaml

  nfs::stunnel: true

Disable
^^^^^^^

If simp_options::stunnel is set to true, but you don't want your NFS traffic to
go through stunnel, set the following, in default.yaml:

.. code-block:: yaml

  nfs::stunnel: false

If simp_options::stunnel is set to false, stunnel is already disabled.

Enabling krb5
-------------

.. WARNING::

  This functionality is incomplete. See ticket SIMP-1400 in our
  `JIRA Bug Tracking`_ . Until that ticket is resolved, it is
  HIGHLY recommended you continue to use stunnel for encrypted
  nfs traffic.

In addition to the code above, add the following code:

default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

  classes:
    - 'krb5::keytab'

  nfs::secure_nfs: true
  simp_options::krb5: true

  krb5::kdc::auto_keytabs::global_services:
    - 'nfs'


Server
^^^^^^

.. code-block:: yaml

  classes:
    - 'krb5::kdc'


Clients
^^^^^^^

.. code-block:: yaml

  nfs::is_server: false

  classes:
    - 'simp_nfs'


.. _JIRA Bug Tracking: https://simp-project.atlassian.net/

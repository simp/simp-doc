HOWTO Configure NFS
===================

.. contents:: This chapter describes multiple configurations of NFS including:
   :local:

All implementations are based on the ``simp-nfs``, ``simp-simp_nfs``,
and ``simp-simp`` modules.

For ease of explanation, examples in this section use the concept of a
:term:`site profile` and are namespaced accordingly.  The manifests are in a
module called ``site``.  If using a different site profile, change the
directory and the namespace in the examples.

.. NOTE::

   ``simp-simp_nfs`` and ``simp-nfs`` are not core modules, and their
   corresponding packages, ``pupmod-simp-simp_nfs`` and ``pupmod-simp-nfs``,
   may need to be installed prior to following this guide.

Known Issues
------------

.. WARNING::

  A number of issues may render NFS inoperable.  Please read through the known
  issues below before deploying into your environment.

Stunnel and Autofs
^^^^^^^^^^^^^^^^^^

The ``autofs`` packages that were released with CentOS 6.8 (`autofs-5.0.5-122`_)
and CentOS 7.3 (`autofs-5.0.7-56`_) worked properly over a :term:`stunnel`
connection.

The releases shipped with CentOS 6.9 (**5.0.5-132**)  and with CentOS 7.4 (**5.0.7-69**)
prevent any connection from happening to the local ``stunnel`` process and
break mounts to remote systems over ``stunnel`` connections.

The releases that ship with CentOS 6.10 (**5.0.5-139**) and CentOS 7.6
(**5.0.7-99**) have fixed the issue.

To use :term:`NFS` over ``stunnel`` and ``automount`` directories the old
package must be used or you must update to the latest release.

To determine what version of ``autofs`` is installed, run ``automount -V``.

To force the package to the desired version:

* Make sure the package is available via your package-management facility then
  set the package version in :term:`Hiera`:

In :term:`EL` 7:

.. code-block:: puppet

   ---
   autofs::autofs_package_ensure: '5.0.7-99'

In :term:`EL` 6

.. code-block:: puppet

   ---
   autofs::autofs_package_ensure: '5.0.5-139'

* Alternatively, ensure that the latest packages are available and set the
  following:

.. code-block:: puppet

   ---
   autofs::autofs_package_ensure: 'latest'


The associated bug reports can be found at:

- CentOS 6  https://bugs.centos.org/view.php?id=13575.
- CentOS 7  https://bugs.centos.org/view.php?id=14080.

Autofs Option in ``nfs::client::mount``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``autofs`` option in ``nfs::client::mount`` resource currently only works
with indirect wild-card mounts.  For all other ``autofs`` options use the
``autofs`` module directly.

SIMP-2944 in `JIRA Bug Tracking`_.

Kerberos and Home Directories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``simp-krb5`` module is not fully integrated with NFS home directories at
this time.

SIMP-1407 in `JIRA Bug Tracking`_.

Exporting Directories
---------------------

**Goal:** Export ``/var/nfs_share`` on the server, mount as ``/mnt/nfs`` on the
client.

.. NOTE::

   If anything in this section does not make sense, there is a full working
   example of how to export NFS home directories in the ``simp_nfs`` module.

Server
^^^^^^

Create a manifest in your :term:`site profile`. In this example the
site profile module is ``site`` and the manifest ``nfs_server.pp``

``site/manifests/nfs_server.pp``:

.. code-block:: puppet

   class site::nfs_server (
     Stdlib::AbsolutePath                             $data_dir     = '/var/nfs_share',
     Simplib::Netlist                                 $trusted_nets = simplib::lookup('simp_options::trusted_nets', { 'default_value' => ['127.0.0.1'] }),
     Array[Enum['none','sys','krb5','krb5i','krb5p']] $sec          = ['sys']
   ){
     include '::nfs::server'

     file { $data_dir:
       ensure => 'directory',
       owner  => 'root',
       group  => 'root',
       mode   => '0644'
     }

     if !$::nfs::stunnel {
       nfs::server::export { 'nfs_share':
         clients     => $trusted_nets,
         export_path => $data_dir,
         sec         => $sec,
         require     => File[$data_dir]
       }
     }
     else {
       # Stunnel needs to point at the local host
       nfs::server::export { 'nfs_share':
         clients     => ['127.0.0.1'],
         export_path => $data_dir,
         sec         => $sec,
         require     => File[$data_dir]
       }
     }
   }

In ``hosts/<your_server_fqdn>.yaml``:

.. code-block:: puppet

   nfs::is_server: true

   simp::classes:
     - 'site::nfs_server'

Client
^^^^^^

Create a manifest in your :term:`site profile`.
In this example the site profile module  is ``site`` and the manifest ``nfs_client.pp``

.. code-block:: puppet

   class site::nfs_client (
    Simplib::Host                                    $nfs_server,
    Enum['none','sys','krb5','krb5i','krb5p']        $sec = 'sys'
  ){

     $_mnt_point = '/mnt/nfs'

     file { "${_mnt_point}":
       ensure => 'directory',
       mode   => '755',
       owner  => 'root',
       group  => 'root'
     }

     nfs::client::mount { "${_mnt_point}":
       nfs_server  => $nfs_server,
       remote_path => '/var/nfs_share',
       sec         => $sec,
       at_boot     => true,
       autofs      => false,
       require     => File["${_mnt_point}"]
     }
  }

In ``hosts/<your_client_fqdn>.yaml``:

.. code-block:: yaml

   nfs::is_server: false
   site::nfs_client::nfs_server: <your nfs server>

   simp::classes:
     - 'site::nfs_client'

.. WARNING::

   Non-wildcard indirect autofs mounts configured via ``nfs::client::mount``
   are not working properly at this time. See SIMP-2944 in our
   `JIRA Bug Tracking`_.  You may wish to manually configure the mount via
   ``autofs::map::master``, and ``autofs::map::entry`` instead.

.. NOTE::

   The ``simp_nfs`` module contains a further example that includes the use of
   a NFS root on the server and indirect autofs with wildcards on the client.

.. _Exporting_Home_Directories:

Exporting Home Directories
--------------------------

**Goal:** Export home directories for LDAP users.

Utilize the SIMP profile module ``simp_nfs``:

#. ``simp_nfs``: Manages client and server configurations for managing NFS home
   directories.
#. ``simp_nfs::create_home_dirs``: Optional hourly cron job that binds to a
   :term:`LDAP` server, ``simp_options::ldap::uri`` by default, and creates a
   NFS home directory for all users in the LDAP server. Also expires any home
   directories for users that no longer exist in LDAP.

.. NOTE::

   Any users logged onto a host at the time of module application will not have
   their home directories re-mounted until they log out and log back in.

.. NOTE::

   The simp_nfs module utilizes an NFS root mount which must be used to export
   any further directories from this server.
   See :ref:`Additional_Directories` for and example of how to do this.

Client
^^^^^^

The following block of code should be entered in the Hiera YAML files of all
systems that need to mount home directories.  The ``default.yaml`` file will
affect all systems.

.. code-block:: yaml

   nfs::is_server: false
   simp_nfs::home_dir_server: <your nfs server>

   simp::classes:
     - simp_nfs

Server
^^^^^^

.. code-block:: yaml

   nfs::is_server: true
   simp_nfs::export_home::create_home_dirs: true

   simp::classes:
     - simp_nfs::export::home

.. _Additional_Directories:

Exporting additional directories on the NFS home server
-------------------------------------------------------

**Goal:** Export ``/var/nfs/share1`` located on the server which is also
sharing home directories set up by the ``simp-simp_nfs`` module.  Mount the
share to ``/share`` on client systems.

The ``simp-simp_nfs`` module utilizes a NFS root share.  Any directories
shared out in addition to the home directories must be mounted to the NFS root
and shared from there.  To see how the NFS root is created see the
``simp_nfs::export::home`` documentation.

The following example assumes you have set up the home server already following
the instructions in the previous section.

Server
^^^^^^

Create a manifest in your :term:`site profile`. In this example the site
profile module is ``site`` and the manifest ``nfs_server.pp``

``site/manifest/nfs_server.pp``;

.. code-block:: puppet

   class site::nfs_server (
   #  Make sure the data_dir is the same as in simp_nfs.
   Stdlib::Absolutepath                             $data_dir     = '/var',
   Simplib::Netlist                                 $trusted_nets = simplib::lookup('simp_options::trusted_nets', { 'default_value' => ['127.0.0.1'] }),
   Array[Enum['none','sys','krb5','krb5i','krb5p']] $sec = ['sys'],
   ) {

   #
   #  Exporting directories from the home directory server when
   #  using the simp_nfs module.
   #
     include '::nfs::server'

   # Create the directory where the data exists.
     file { '/var/nfs/share1':
       ensure => 'directory',
       mode   => '0755',
       owner  => 'root',
       group  => 'root'
     }

   # Create a mount point under the nfs root created in simp_nfs.
     file { "${data_dir}/nfs/exports/share1":
       ensure => 'directory',
       mode   => '0755',
       owner  => 'root',
       group  => 'root'
     }

   # Mount the share to the nfs_root created in simp_nfs.
     mount { "${data_dir}/nfs/exports/share1":
       ensure   => 'mounted',
       fstype   => 'none',
       device   => "/var/nfs/share1",
       remounts => true,
       options  => 'rw,bind',
       require  => [
         File["${data_dir}/nfs/exports/share1"],
         File['/var/nfs/share1']
       ]
     }

   # Export the directory
     if !$::nfs::stunnel {
       nfs::server::export { 'share1':
         clients     => nets2cidr($trusted_nets),
         export_path => "${data_dir}/nfs/exports/share1",
         rw          => true,
         sec         => $sec
       }
     } else {
         nfs::server::export { 'share1':
         clients     => ['127.0.0.1'],
         export_path => "${data_dir}/nfs/exports/share1",
         rw          => true,
         sec         => $sec,
         insecure    => true
       }
     }
   }

Include this manifest in the servers Hiera file.

.. code-block:: yaml

   ---
   simp::classes:
     - site::nfs_server
     - simp_nfs

   nfs::is_server: true

Client
^^^^^^

Create a manifest in your :term:`site profile`. In this example the site
profile module is ``site`` and the manifest ``nfs_client.pp``

``site/manifests/nfs_client.pp``

.. code-block:: puppet

   class site::nfs_client (
     Simplib::Host                      $nfs_server,
     Enum['sys','krb5','krb5i','krb5p'] $sec           = 'sys',
   ){

     include nfs

     $mount_point = '/share'

     # Since it the nfs server uses a nfs_root, you onlt put the path
     # relative to the root.
     $remote_path = '/share1'


     if getvar('::nfs::client::is_server') {
       $_target = '127.0.0.1'
     }
     else {
       $_target = $nfs_server
     }

     file { "${mount_point}":
       ensure => 'directory',
       mode   => '0755',
       owner  => 'root',
     }

     nfs::client::mount { "${mount_point}":
       nfs_server         => $nfs_server,
       remote_path        => "${remote_path}",
       nfs_version        => 'nfs4',
       sec                => $sec,
       autofs             => false,
       at_boot            => true,
     }
   }

Then include this manifest in Hiera for any system that should mount this
share.

.. code-block:: yaml

   ---
   simp::classes:
     - site::nfs_client

   nfs::is_server: false
   site::nfs_client::nfs_server: server21.simp.test


Enabling/Disabling Stunnel
--------------------------

Stunnel is a means to encrypt your NFS data during transit.

Enable
^^^^^^

If ``simp_options::stunnel`` is set to ``true``, you need only specify the
following, in the server's :term:`YAML` file:

.. NOTE::

   The following is set to prevent a cyclical connection of stunnel to itself,
   in the event the server is a client of itself.

.. code-block:: yaml

   nfs::client::stunnel::nfs_server: <your nfs server>

If ``simp_options::stunnel`` is set to ``false`` and you do not wish to
globally enable ``stunnel``, you will also need to set the following, in
``default.yaml``:

.. code-block:: yaml

   nfs::stunnel: true

Disable
^^^^^^^

If ``simp_options::stunnel`` is set to ``true``, but you do not want your NFS
traffic to go through ``stunnel``, set the following, in ``default.yaml``:

.. code-block:: yaml

   nfs::stunnel: false

If ``simp_options::stunnel`` is set to ``false`` then ``stunnel`` is already
disabled.

Enabling Kerberos
-----------------

.. WARNING::

   This functionality is incomplete. It does not work with home directories.
   See ticket SIMP-1407 in our `JIRA Bug Tracking`_ .

In addition to the sharing code (not the ``stunnel`` code) above, add the
following:

default.yaml
^^^^^^^^^^^^

.. code-block:: yaml

   simp::classes:
     - 'krb5::keytab'

   nfs::secure_nfs: true
   simp_options::krb5: true

   krb5::kdc::auto_keytabs::global_services:
     - 'nfs'

Server
^^^^^^

.. code-block:: yaml

   simp::classes:
     - 'krb5::kdc'

Clients
^^^^^^^

.. code-block:: yaml

   nfs::is_server: false

   simp::classes:
     - 'simp_nfs'

.. _JIRA Bug Tracking: https://simp-project.atlassian.net/
.. _autofs-5.0.5-122: http://vault.centos.org/6.8/os/x86_64/Packages/autofs-5.0.5-122.el6.x86_64.rpm
.. _autofs-5.0.7-56: http://vault.centos.org/7.3.1611/os/x86_64/Packages/autofs-5.0.7-56.el7.x86_64.rpm

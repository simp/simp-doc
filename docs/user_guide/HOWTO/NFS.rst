.. _ug-howto-configure-nfs:

HOWTO Configure NFS
===================

.. contents:: This chapter describes multiple configurations of NFS.  Topics include:
   :local:

All implementations are based on the ``simp-nfs``, ``simp-simp_nfs``,
and ``simp-simp`` modules.

.. WARNING::

   ``simp-nfs`` version 7.0.0 and ``simp-autofs`` version 7.0.0 had major
   breaking changes in the pursuit of fixing long-standing bugs. General usage
   as noted in this document remains largely the same, but there are many
   changes to the API, some of which are nuanced.  For example, parameters that
   used to accept hostnames now require IP addresses in support of firewalld.
   Please check your settings carefully on upgrade.

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

   A number of issues may render NFS inoperable. Please read through the known
   issues below before deploying into your environment.

Stunnel and Autofs
^^^^^^^^^^^^^^^^^^

The ``autofs`` package that was released with CentOS 7.3 (`autofs-5.0.7-56`_)
worked properly over a :term:`stunnel` connection.

The release shipped with with CentOS 7.4 (**5.0.7-69**) prevents any connection
from happening to the local ``stunnel`` process and breaks mounts to remote systems
over ``stunnel`` connections.

The release that ship with CentOS 7.6 (**5.0.7-99**) has fixed the issue.

To use :term:`NFS` over ``stunnel`` and ``automount`` directories with old
CentOS 7 releases, you must use the appropriate ``autofs`` package.

To determine what version of ``autofs`` is installed, run ``automount -V``.

To force the package to the desired version:

* Make sure the package is available via your package-management facility then
  set the package version in :term:`Hiera`:

.. code-block:: yaml

   autofs::autofs_package_ensure: '5.0.7-99'

* Alternatively, ensure that the latest packages are available and set the
  following:

.. code-block:: yaml

   autofs::autofs_package_ensure: 'latest'


The associated bug report can be found at:

- CentOS 7  https://bugs.centos.org/view.php?id=14080.

Limited Kerberos Support
^^^^^^^^^^^^^^^^^^^^^^^^

SIMP's NFS modules provide limited support for Kerberos and will not be
discussed here.

* See the `README`_ for ``simp-nfs`` for information about the Kerberos
  support it provides and its integration with the ``simp-krb5`` module.
* The ``simp-krb5`` module is not fully integrated with NFS home directories at
  this time.  See `SIMP-1407`_ for details.


Exporting Directories
---------------------

**Goal:**

  * Export */var/nfs_share* on the server, for read-only access.
  * Mount as */mnt/nfs* on the client.

Server
^^^^^^

Create a manifest in your :term:`site profile`. In this example the manifest
in the ``site`` module is *nfs_server.pp*.

In *site/manifests/nfs_server.pp*:

.. code-block:: puppet

   class site::nfs_server (
     Stdlib::AbsolutePath                             $data_dir     = '/var/nfs_share',
     Simplib::Netlist                                 $trusted_nets = simplib::lookup('simp_options::trusted_nets', { 'default_value' => ['127.0.0.1'] }),
     Array[Enum['none','sys','krb5','krb5i','krb5p']] $sec          = ['sys']
   ){
     include nfs::server

     file { $data_dir:
       ensure => 'directory',
       owner  => 'root',
       group  => 'root',
       mode   => '0755'
     }

     if !$nfs::stunnel {
       nfs::server::export { 'nfs_share':
         clients     => $trusted_nets,
         export_path => $data_dir,
         sec         => $sec,
         require     => File[$data_dir]
       }
     }
     else {
       nfs::server::export { 'nfs_share':
         # From the NFS server's perspective, the stunneled connections will
         # come from the local host
         clients     => [ '127.0.0.1' ],
         export_path => $data_dir,
         insecure    => true,
         sec         => $sec,
         require     => File[$data_dir]
       }
     }
   }

In *data/hosts/<your NFS server FQDN>.yaml*:

.. code-block:: yaml

   nfs::is_server: true

   simp::classes:
     - 'site::nfs_server'

Client
^^^^^^

The client can be configured to mount statically, automatically with a direct
autofs mount, or automatically with an indirect autofs mount. Examples for all
three types will be shown in this section.  The indirect autofs mount example
will configure autofs for an indirect mount that uses key substitution.


.. NOTE::

   If the NFS client is also a NFS server, there is autodetect logic in
   ``nfs::client::mount`` that attempts to detect that the client has both
   roles and, when this condition is detected, sets the NFS server's IP to
   ``127.0.0.1`` for efficiency. This detection logic has limitations. So,
   if you know the client is also the NFS server, it is better to disable
   the auto-detection by setting ``nfs::client::mount::autodetect_remote``
   to ``false``, and then set the ``nfs::client::mount::nfs_server`` to
   ``127.0.0.1``.

Static Mount
""""""""""""

Create a manifest in your :term:`site profile`.  In this example the manifest
in the ``site`` modules is *nfs_client.pp*.

In *site/manifests/nfs_client.pp*:

.. code-block:: puppet

   class site::nfs_client (
    Simplib::Ip                               $nfs_server,
    Enum['none','sys','krb5','krb5i','krb5p'] $sec = 'sys'
  ){

     $_mnt_point = '/mnt/nfs'

     file { $_mnt_point:
       ensure => 'directory',
       mode   => '755',
       owner  => 'root',
       group  => 'root'
     }

     nfs::client::mount { $_mnt_point:
       nfs_server  => $nfs_server,
       remote_path => '/var/nfs_share',
       sec         => $sec,
       autofs      => false,
       # The mount point must already exist in a static mount
       require     => File[$_mnt_point]
     }
  }

In *data/hosts/<your NFS client FQDN>.yaml*:

.. code-block:: yaml

   site::nfs_client::nfs_server: <your NFS server IP>

   simp::classes:
     - 'site::nfs_client'

Direct autofs Mount
"""""""""""""""""""

Create a manifest in your :term:`site profile`.  In this example the manifest
in the ``site`` module is *nfs_client.pp*.

In *site/manifests/nfs_client.pp*:

.. code-block:: puppet

   class site::nfs_client (
    Simplib::Ip                               $nfs_server,
    Enum['none','sys','krb5','krb5i','krb5p'] $sec = 'sys'
  ){

     nfs::client::mount { '/mnt/nfs':
       nfs_server  => $nfs_server,
       remote_path => '/var/nfs_share',
       sec         => $sec
     }
  }

In *data/hosts/<your NFS client FQDN>.yaml*:

.. code-block:: yaml

   site::nfs_client::nfs_server: <your NFS server IP>

   simp::classes:
     - 'site::nfs_client'

Indirect autofs Mount with Key Substitution
"""""""""""""""""""""""""""""""""""""""""""

Create a manifest in your :term:`site profile`.  In this example the manifest
in the ``site`` module is *nfs_client.pp*.

In *site/manifests/nfs_client.pp*:

.. code-block:: puppet

   class site::nfs_client (
    Simplib::Ip                               $nfs_server,
    Enum['none','sys','krb5','krb5i','krb5p'] $sec = 'sys'
  ){

     nfs::client::mount { '/mnt/nfs':
       nfs_server              => $nfs_server,
       remote_path             => '/var/nfs_share',
       sec                     => $sec,
       autofs_indirect_map_key => '*',
       autofs_add_key_subst    => true
     }
  }

In *data/hosts/<your NFS client FQDN>.yaml*:

.. code-block:: yaml

   site::nfs_client::nfs_server: <your NFS server IP>

   simp::classes:
     - 'site::nfs_client'

.. _Exporting_Home_Directories_For_LDAP_Users:

Exporting Home Directories for LDAP Users
-----------------------------------------

**Goal:** Export home directories for LDAP users.

Utilize the SIMP profile module ``simp-simp_nfs``:

#. ``simp-simp_nfs``: Manages client and server configurations for managing NFS
   home directories.
#. ``simp_nfs::create_home_dirs``: Enables an optional hourly cron job that
   binds to a :term:`LDAP` server, ``simp_options::ldap::uri`` by default, and
   creates a NFS home directory for all users in the LDAP server. It also
   expires any home directories for users that no longer exist in LDAP.

.. NOTE::

   Any users logged onto a host at the time of module application will not have
   their home directories re-mounted until they log out and log back in.

.. NOTE::

   The ``simp-simp_nfs module`` utilizes an NFS root share which must be used
   to export any further directories from this server using NFSv4. This is
   because NFSv4 exports must exist in a single pseudo filesystem.
   See :ref:`Additional_Directories` for an example of how to do this.

Client
^^^^^^

The following should be entered in the Hiera YAML files of all servers that
need to mount home directories.  Use *data/default.yaml* if you want to mount
the home directories on all servers.

.. code-block:: yaml

   simp_nfs::home_dir_server: <your NFS server IP>

   simp::classes:
     - simp_nfs

Server
^^^^^^

The following should be entered in the Hiera YAML file of the NFS server.

In *data/hosts/<your NFS server FQDN>.yaml*:

.. code-block:: yaml

   nfs::is_server: true
   simp_nfs::export::home::create_home_dirs: true

   simp::classes:
     - simp_nfs::export::home

.. _Additional_Directories:

Exporting Additional Directories on the NFS Home Server
-------------------------------------------------------

**Goal:**

* Export */var/nfs/share1* for read-write access to the 'administrators'
  group

  * This directory is located on the NFS server which is also sharing home
    directories for LDAP users.
  * The home directory share is set up by the ``simp-simp_nfs`` module.

* Statically mount the share to */share* on client systems.

The ``simp-simp_nfs`` module exports home directories under a root NFS
share directory. Because NFSv4 exports exist in a single pseudo filesystem,
each directory below that NFS share should be a bind mount to a directory on
the NFS server.

The following example assumes you have set up the home server already following
the instructions in the previous section and will be creating a bind mount under
the root NFS share directory.

Server
^^^^^^

Create a manifest in your :term:`site profile`. In this example the manifest
and the ``site`` module is *nfs_server.pp*.

In *site/manifest/nfs_server.pp*:

.. code-block:: puppet

   class site::nfs_server (
     #  Make sure the data_dir is the same as in simp_nfs.
     Stdlib::Absolutepath                             $data_dir     = '/var',
     Simplib::Netlist                                 $trusted_nets = simplib::lookup('simp_options::trusted_nets', { 'default_value' => ['127.0.0.1'] }),
     Array[Enum['none','sys','krb5','krb5i','krb5p']] $sec = ['sys'],
   ) {

     #
     #  Exporting directories from the home directory server configured
     #  with the simp_nfs module.
     #
     include nfs::server

     # Create the directory where the data exists.
     file { '/var/nfs/share1':
       ensure => 'directory',
       mode   => '0775',
       owner  => 'root',
       group  => 'administrators'
     }

     # Create a mount point under the NFS root share created in simp_nfs.
     file { "${data_dir}/nfs/exports/share1":
       ensure => 'directory',
       mode   => '0775',
       owner  => 'root',
       group  => 'administrators'
     }

     # Bind mount the share to the NFS root share created in simp_nfs.
     mount { "${data_dir}/nfs/exports/share1":
       ensure   => 'mounted',
       fstype   => 'none',
       device   => '/var/nfs/share1',
       remounts => true,
       options  => 'rw,bind',
       require  => [
         File["${data_dir}/nfs/exports/share1"],
         File['/var/nfs/share1']
       ]
     }

     # Export the directory
     if !$nfs::stunnel {
       nfs::server::export { 'share1':
         clients     => $trusted_nets,
         export_path => "${data_dir}/nfs/exports/share1",
         rw          => true,
         sec         => $sec
       }
     } else {
         nfs::server::export { 'share1':
         # From the NFS server's perspective, the stunneled connections will
         # come from the local host
         clients     => ['127.0.0.1'],
         export_path => "${data_dir}/nfs/exports/share1",
         rw          => true,
         sec         => $sec,
         insecure    => true
       }
     }
   }

In *data/hosts/<your NFS server FQDN>.yaml*:

.. code-block:: yaml

   nfs::is_server: true

   simp::classes:
     - site::nfs_server
     - simp_nfs

Client
^^^^^^

Create a manifest in your :term:`site profile`. In this example the manifest
in the ``site`` module is *nfs_client.pp*.

In *site/manifests/nfs_client.pp*:

.. code-block:: puppet

   class site::nfs_client (
     Simplib::Host                      $nfs_server,
     Enum['sys','krb5','krb5i','krb5p'] $sec = 'sys',
   ){

     include nfs

     $mount_point = '/share'

     # Only need the path relative to the root of the NFSv4 pseudo filesystem.
     $remote_path = '/share1'


     file { $mount_point:
       ensure => 'directory',
       mode   => '0775',
       owner  => 'root',
       group  => 'administrators'
     }

     nfs::client::mount { $mount_point:
       nfs_server  => $nfs_server,
       remote_path => $remote_path,
       sec         => $sec,
       autofs      => false,
       at_boot     => true,
       # The mount point must already exist in a static mount
       require     => File[$mount_point]
     }
   }

Then include this manifest in Hiera for any system that should mount this
share.

.. code-block:: yaml

   site::nfs_client::nfs_server: <your NFS server IP>

   simp::classes:
     - site::nfs_client


Enabling/Disabling Stunnel
--------------------------

Stunnel is a means to encrypt your NFS data during transit.

Enable
^^^^^^

If ``simp_options::stunnel`` is set to ``true``, ``stunnel`` will be enabled.

If ``simp_options::stunnel`` is set to ``false`` and you do not wish to
globally enable ``stunnel``, you will need to set the following, in
*data/default.yaml*:

.. code-block:: yaml

   nfs::stunnel: true

Disable
^^^^^^^

If ``simp_options::stunnel`` is set to ``true``, but you do not want your NFS
traffic to go through ``stunnel``, set the following, in *data/default.yaml*:

.. code-block:: yaml

   nfs::stunnel: false

If ``simp_options::stunnel`` is set to ``false`` then ``stunnel`` is already
disabled.

.. _README: https://github.com/simp/pupmod-simp-nfs/blob/7.0.0/README.md
.. _SIMP-1407: https://simp-project.atlassian.net/browse/SIMP-1407
.. _autofs-5.0.7-56: http://vault.centos.org/7.3.1611/os/x86_64/Packages/autofs-5.0.7-56.el7.x86_64.rpm

.. _Infrastructure-Setup:

HOWTO Manage Workstation Infrastructures
========================================

This chapter describes how to manage client workstations with a SIMP
system including GUIs, repositories, virtualization, Network File System
(NFS), printing, and Virtual Network Computing (VNC).

User Workstation Setup
----------------------

Below is an example class,
``/etc/puppet/environments/simp/modules/site/manifests/workstation.pp``, that could be used to
set up a user workstation.

.. code-block:: ruby

  class site::workstation {
    include 'site::gui'
    include 'site::repos'
    include 'site::virt'
    include 'site::automount'
    include 'site::print::client'

    # Make sure everyone can log into all nodes.
    # If you want to change this, simply remove this line and add
    # individual entries to your nodes as appropriate
    pam::access::manage { "Allow Users":
      comment => 'Allow all users in the "users" group to access the system from anywhere.',
      users   => '(users)',
      origins => ['ALL']
    }

    # General Use Packages
    package { [
      'pidgin',
      'git',
      'control-center-extra',
      'gconf-editor',
      'evince',
      'libreoffice-writer',
      'libreoffice-xsltfilter',
      'libreoffice-calc',
      'libreoffice-impress',
      'libreoffice-emailmerge',
      'libreoffice-base',
      'libreoffice-math',
      'libreoffice-pdfimport',
      'bluefish',
      'gnome-media',
      'pulseaudio',
      'file-roller',
      'inkscape',
      'gedit-plugins',
      'planner'
    ]: ensure => 'latest'
    }
  }


Graphical Desktop Setup
-----------------------

Below is an example manifest called
``/etc/puppet/environments/simp/modules/site/manifests/gui.pp`` for setting up a graphical
desktop on a user workstation.

.. code-block:: ruby

            class site::gui {
              include 'xwindows::gdm'
              include 'windowmanager::gnome'
              include 'vnc::client'

               # Compiz Stuff
              package { [
                'fusion-icon',
                'emerald-themes',
                'compiz-fusion-extras',
                'compiz-fusion-extras-gnome',
                'vinagre'
              ]:
                ensure => 'latest'
              }
            }


Workstation Repositories
------------------------

Below is an example manifest called
``/etc/puppet/environments/simp/modules/site/manifests/repos.pp`` for setting up workstation
repositories.

.. code-block:: ruby

            class site::repos {
              # Whatever local yumrepo statements you need for installing
              # your packages and keeping your systems up to date
            }


Virtualization on User Workstations
-----------------------------------

Below is an example manifest called
``/etc/puppet/environments/simp/modules/site/manifests/virt.pp`` for allowing virtualization
on a user workstation.

.. code-block:: ruby

            # We allow users to run VMs on their workstations.
            # If you don't want this, just don't include this class.
            # If this is installed, VM creation and management is still limited by PolicyKit

            class site::virt {
              include 'libvirt::kvm'
              include 'libvirt::ksm'
              include 'network::redhat'

              network::redhat::add_eth { "em1":
                bridge => 'br0',
                hwaddr => $::macaddress_em1
              }

              network::redhat::add_eth { "br0":
                net_type => 'Bridge',
                hwaddr => $::macaddress_em1,
                require => Network::Redhat::Add_eth["em1"]
              }

              common::swappiness::conf { 'default':
                high_swappiness => '80',
                max_swappiness => '100'
              }

              # If 80% of memory is used, flush caches.
              exec { 'flush_cache_himem':

                command => '/bin/echo 1 > /proc/sys/vm/drop-caches',
                onlyif => inline_template("/bin/<%= memoryfree.split(/\s/)[0].
                to_f/memorysize.split(/\s/)[0].to_f < 0.2 ? true : false %>")
              }

              package { 'virt-manager': ensure => 'latest' }
            }


Network File System
-------------------

Below is an example manifest called
``/etc/puppet/environments/simp/modules/site/automount.pp`` for Network File System setup.

.. code-block:: ruby

            #If you are not using NFS, you do not need to include this.

            class site::automount {
              include '::autofs'

              file { '/net':
                ensure => 'directory',
                mode   => '0755'
              }

              #A global share
              Autofs::map::master { ‘share’:
                mount_point => ‘/net’,
                map_name    => ‘/etc/autofs/share.map’
              }

              #Map the share
              autofs::map::entry { ‘share’:
                options  => ‘-fstype=nfs4, port=2049.soft’,
                location => “${::nfs_server}:/share’,
                Target   => ‘share’
              }
            }


Printer Setup
-------------

Below are example manifests for setting up a printing environment.

Setting up a Print Client
~~~~~~~~~~~~~~~~~~~~~~~~~

Below is an example manifest called
``/etc/puppet/environments/simp/modules/site/manifests/print/client.pp`` for setting up a
print client.

.. code-block:: ruby

            class site::print::client inherits site::print::server {
              polkit::local_authority { 'print_support':
                identity                 => ['unix_group:*'],
                action                   => 'org.opensuse.cupskhelper.mechanism.*',
                section_name       => 'Allow all print management permissions',
                result_any            => 'yes',
                result_interactive => 'yes',
                result_active         => 'yes'
              }

              package { 'cups-pdf': ensure => 'latest' }
              package { 'cups-pk-helper': ensure => 'latest' }
              package { 'system-config-printer': ensure => 'present' }
            }


Setting up a Print Server
~~~~~~~~~~~~~~~~~~~~~~~~~

Below is an example manifest called
``/etc/puppet/environments/simp/modules/site/manifests/print/server.pp`` for setting up a
print server.

.. code-block:: ruby

            class site::print::server {

              # Note, this is *not* set up for being a central print server.
              # You'll need to add the appropriate IPTables rules for that to work.
              package { 'cups': ensure => 'latest' }

              service { 'cups':
                enable     => 'true',
                ensure     => 'running',
                hasrestart => 'true',
                hasstatus  => 'true',
                require    => Package['cups']
              }
            }


VNC Setup
---------

:term:`Virtual Network Computing` (VNC) is a tool that is used to manage desktops and workstations remotely
through the standard setup or a proxy.

VNC Standard Setup
~~~~~~~~~~~~~~~~~~

.. note::

    You must have the ``pupmod-simp-vnc`` RPM installed to use VNC on your
    system!

To enable remote access via VNC on the system, include ``vnc::server``
in Hiera for the node.

The default VNC setup that comes with SIMP can only be used over SSH and
includes three default settings:

+---------------+------------------------------------+
|Setting Type   |Setting Details                     |
+===============+====================================+
|Standard       | Port: 5901                         |
|               |                                    |
|               | Resolution: 1024x768@16            |
+---------------+------------------------------------+
|Low Resolution | Port: 5902                         |
|               |                                    |
|               | Resolution: 800x600@16             |
+---------------+------------------------------------+
|High Resolution| Port: 5903                         |
|               |                                    |
|               | Resolution: 1280x1024@16           |
+---------------+------------------------------------+

Table: VNC Default Settings

To connect to any of these settings, SSH into the system running the VNC
server and provide a tunnel to ``127.0.0.1:<VNC Port>``. Refer to the SSH
client's documentation for specific instructions.

To set up additional VNC port settings, refer to the code in
``/etc/puppet/environments/simp/modules/vnc/manifests/server.pp``
for examples.

.. important::

    Multiple users can log on to the same system at the same time with
    no adverse effects; however, none of these sessions are persistent.

    To maintain a persistent VNC session, use the ``vncserver``
    application on the remote host. Type ``man vncserver`` to reference
    the manual for additional details.

VNC Through a Proxy
~~~~~~~~~~~~~~~~~~~

The section describes the process to VNC through a proxy. This setup
provides the user with a persistent VNC session.

.. important::

    In order for this setup to work, the system must have a VNC server
    (``vserver.your.domain``), a VNC client (``vclnt.your.domain``), and a
    proxy (``proxy.your.domain``). A ``vuser`` account must also be set up
    as the account being used for the VNC. The ``vuser`` is a common user
    that has access to the server, client, and proxy.

Modify Puppet
+++++++++++++

If definitions for the machines involved in the VNC do not already exist
in Hiera, create an ``/etc/puppet/environments/simp/hieradata/hosts/vserv.your.domain.yaml``
file. In the client hosts file, modify or create the entries shown in
the examples below. These additional modules will allow vserv to act as
a VNC server and vclnt to act as a client.

VNC Server node

.. code-block:: yaml

  # vserv.your.domain.yaml
  classes:
    - 'windowmanager::gnome'
    - 'mozilla::firefox'
    - 'vnc::server'


VNC client node

.. code-block:: yaml

  # vclnt.your.domain.yaml
  classes:
    - 'windowmanager::gnome'
    - 'mozilla::firefox'
    - 'vnc::client'


Run the Server
++++++++++++++

As ``vuser`` on ``vserv.your.domain``, type ``vncserver``.

The output should mirror the following:

  New 'vserv.your.domain:<Port Number> (vuser)' desktop is vserv.your.domain:<Port Number>

Starting applications specified in ``/home/vuser/.vnc/xstartup`` Log file
is ``/home/vuser/.vnc/vserv.your.domain:<Port Number>.log``

.. note::

    Remember the port number; it will be needed to set up an SSH tunnel.

Set up an SSH Tunnel
++++++++++++++++++++

Set up a tunnel from the client (vclnt), through the proxy server
(proxy), to the server (vserv). The table below lists the steps to set
up the tunnel.


1. On the workstation, type ssh -l vuser -L 590***<Port Number>*:localhost:590***<Port Number>***proxy.your.domain**

  .. note:: This command takes the user to the proxy.

2. On the proxy, type ssh -l vuser -L 590***<Port Number>*:localhost:590***<Port Number>***vserv.your.domain**

  .. note:: This command takes the user to the VNC server.

Table: Set Up SSH Tunnel Procedure

.. note::

    The port number in 590\ *<Port Number>* is the same port number as
    previously described. For example, if the *<Port Number>* was 6,
    then all references below to 590\ *<Port Number>* become 5906.

Set Up Clients
++++++++++++++

On ``vclnt.your.domain``, type ``vncviewer localhost:590\ ***<Port
Number>***`` to open the Remote Desktop viewer.

Troubleshooting VNC Issues
~~~~~~~~~~~~~~~~~~~~~~~~~~

If nothing appears in the terminal window, X may have crashed. To
determine if this is the case, type ``ps -ef | grep XKeepsCrashing``

If any matches result, stop the process associated with the command and
try to restart ``vncviewer`` on ``vclnt.your.domain``.

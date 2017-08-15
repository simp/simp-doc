HOWTO Configure SNMPD
=====================

.. contents::
  This document gives details on using the ``pupmod-simp-simp_snmpd`` profile
  to configure the snmpd daemon.
  Simple instructions to  configure the snmpd daemon using the ``pupmod-simp-simp_snmpd``
  profile module are described in its README file.  Those should be read before this.

.. NOTE::

  ``pupmod-simp-simp_snmpd`` and the ``puppet-snmp``  module it requires
  are not core modules and may need to be installed prior to following
  this guide.


SNMPD configuration:
--------------------

The ``pupmod-simp-simp_snmpd`` profile module configures the snmpd daemon
to use snmp v3 with User-based Security Model (USM) with View-based Access
Control Model (VACM).
The snmpd.conf man page describes these in detail.

The snmpd configuration files are in /etc/snmp.

Simp_snmpd configures the snmpd.conf file to look in two directories for files ending
in ``.conf``.  These are /etc/snmp/simp_snmpd.d and /etc/snmp/user_snmpd.d.
The files in the simp directory are managed by puppet.  The user can add
any configuration they want to the user directory to further configure
the snmpd daemon.  Some settings, like views and groups are cumulative.  In the
case where they are not, the settings in the user directory will override
settings in the simp directory.

The simp_snmpd profile module disables the snmptrapd daemon by default.
The daemon can be enabled but this module does configure the snmptrap
daemon.  If the snmptrapd needs to run on the same machine, the variables
simp_snmpd::trap_service_ensure and simp_snmpd::trap_service_startatboot
need to be set appropriately and any configuration files need to be placed
in /etc/snmp/user_trapd.d with a .conf extension.

The following sections  describe some of the settings in simp module and how
to change them via hiera.  They can also be changed in the simp_snmpd resource call.

Agent Addresses and Firewall
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As you noticed from the README file, by default, simp_snmpd,
only configures the daemon to listen on the local interface.
The ``simp_snmpd::agentaddress`` is used to configure what interfaces
the snmpd daemon will listen on.  It is an Array of Strings.
These strings should be listening addresses as defined in the man page for snmpd
under the ``LISTENING ADDRESS`` section.  (IPV6 addresses must be bracketed).

The following is an example :nd there are many more examples of the string to add
in the man page:

.. code-block:: yaml

  ---
  simp_snmpd::agentaddress:
    - upd:161
    - tcp:%{facts.fqdn}:161

If firewall is turned on it will parse the array of listening addresses to
determine what ports should be opened.  It does not, at this time do anything
for ipx or pvc.  It uses ``simp_snmpd::trusted_nets`` to determine who can access
the ports.  If you want to restrict access to snmpd to just certain machines
set this value to restrict it.

If the agent address is set in a conf file in the user directory and not by
hiera or in the simp_snmpd resource call, firewall will not know about it and the
firewall will not be opened by simp_snmpd.

Access
^^^^^^
The simp_snmpd profile module configures snmp v3 USM access with VACM access
controls.  Read the man page on the snmpd.conf file for description of these
configuration items.

The profile module, by default, installs two users snmpd_ro and snmp_rw.
snmp_ro is configured for read only access to system view.
snmp_rw is configurated with read/write access to everything.
The passwords are auto generated and stored on the puppet server in the
passgen directory which is located at
``/opt/puppetlabs/server/data/puppetserver/simp/environments/production/simp_autofile/gen_passwd``.

The access configuration is in the /etc/snmp/simp_snmpd.d/access.conf file.

To create the access.conf file, the profile modules uses a set of HASHES.
The default hashes are in the ``data/common.yaml`` file.
These hashes are merged with any hash you defined in the hiera files on the
puppetserver.
Merging is described in ` Puppet docs <https://docs.puppet.com/puppet/4.10/hiera_merging.html>`_

To remove something from the default hash add the name of object with no keys.

.. NOTE::
  Once a user is created, to remove it or change the password the snmpusm
  utility must be used or the /var/lib/net-simp directory removed and
  puppet run again.  Changing the password in the hash or removing the
  keys will not change the password of an existing user or remove it.

The full hash and the full set of keys is defined below and an example is
given after this to show how to remove a user and change group.

The hashes used to create users, views, group and give access are:

User Hash
"""""""""
.. code-block:: yaml

  simp_snmpd::v3_users_hash
    username:
      authtype: MD5|SHA
      privtype: DES|AES
      privpass: 'your priv password'
      authpass: 'your auth password'

If authtype or privtype is missing it will use the default from
the simp_snmpd modude, $defauthtype and $defprivtype.

If either of the passwords are missing it will generate on using
passgen.

View Hash
"""""""""

.. code-block:: yaml

  simp_snmpd::view_hash:
    viewname:
      included: [array of oids to include]
      excluded: [array of oids to exclude]

One or both of included/excluded needs to be specified.  Any number
of oids can be listed.

It will create one view line for each oid in the list with exclude
or include.

Group Hash
""""""""""
.. code-block:: yaml

  simp_snmpd::group_hash:
    groupname:
      model: The security model to use (default to defsecuritymodel)
      secname: [array of user names to include in this group]

It does not verify the user exists.

Access Hash
"""""""""""
.. code-block:: yaml

  simp_snmpd::access _hash:
    accessname:
      vread: view to use for reading access (default none)
      vwrite: view to use for write access (default none)
      vnotify: view to use for notify (default none)
      level:  priv|auth|noauth (default is defsecuritylevel)
      model: the model to use (default is defsecuritymodel)
      context: context to use (default "")
      prefx:  prefix for the context exact| prefix (default exact)
      groups: [array of groups to create this access for]

It does not verify the group exists.
The access name is just a place holder.

For all hashes:
Anything with a default does not need to be included in the hash.

.. NOTE:: Any views, groups or access lines set up in user conf files
   will be in addition to anything anything configured in the hash.

Example
"""""""
If you do not want the default user or any of the views, groups or access created
you can pass and empty hash and it will ignore that setting. :

.. code-block:: yaml

  ---
  simp_snmpd::v3_user_hash:
   snmp_ro:
   myuser:
     authpass: 'HardToBreakPassword'
     privpass: 'OtherPassword'
  simp_snmpd::group:
   readonly_group:
     secname: myuser

The above example will not create the snmp_ro user and add myuser.
(If the snmp_ro user is already created it will not delete it.)
It will also override the default definition of readonly_group.
See "Merging data from Multiple source" in https://docs.puppet.com
for more information on merging.

The quickest way to delete users or change the password is to configure
the hashes and the remove the /var/lib/net-simp directory, stop the
snmpd daemon and run puppet.

Client
^^^^^^

By default net-snmp-utils  and it dependancies are not installed.
This includes snmpd utilities like snmpget, snmpset, snmpwalk , etc.
Set ``simp_snmpd::manage_client`` to true to install these.

.. code-block:: yaml

  simp_snmpd::manage_client: true

If these are installed, it will configure the defaults security model
and level, authentication and privacy types.  It does not
configure a default password.

Rsync MIBS and DLMODS
^^^^^^^^^^^^^^^^^^^^^
Rsync can be used to push out custom MIBS and dynamically loaded shared
objects (dlmod).

Rsync in simp_snmpd copies MIBS by default into the default mib directory
used by net-snmp.  To copy them somewhere else set ``simp_snmpd::rsync_mibs_dir``
to the fully qualified path.  It will copy the files to a mibs
directory under that path and add this directory to the mibs path.

DLMODS are copied the same way as MIBS using the rsync_dlmod_dir as
the destination and creating a dlmod directory.  In order to load them
you must add the name of the module to the ``isimp_snmpd::dlmods`` list.
This will create a dlmod.conf file in the simp snmpd.  The '.so'
extension will be added.  See the ``Dynamically Loadable Modules``
modules section in man page for snmpd.conf.

.. code-block:: yaml

  ---
  simp_snmpd::rsync_dlmod: true
  simp_snmpd::rsync_mibs: true
  simp_snmpd::dlmods:
    - mymodulename

.. _JIRA Bug Tracking: https://simp-project.atlassian.net/


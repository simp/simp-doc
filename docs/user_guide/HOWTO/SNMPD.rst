HOWTO Configure SNMPD
=====================

This document details how to use the ``pupmod-simp-simp_snmpd`` profile
to configure the SNMP daemon.

Simple instructions to configure the snmpd daemon using the
``pupmod-simp-simp_snmpd`` profile module are described in its README file.

.. NOTE::

  ``pupmod-simp-simp_snmpd`` and ``puppet-snmp`` are not core modules and may
  need to be installed prior to following this guide.


SNMPD Configuration
-------------------

There are two primary configuration directories:

  * ``/etc/snmp/simp_snmpd.d``

    * Files managed by ``puppet``

  * ``/etc/snmp/user_snmpd.d``

    * Files not managed by ``puppet``
    * Extended configurations should be placed here
    * Settings in this directory will override settings in the ``simp_snmpd.d``
      directory

``snmptrapd`` is disabled by default.  The daemon can be enabled, but
``pupmod-simp-simp_snmpd`` will not configure it.  If you need to run
``snmptrapd``, set ``simp_snmpd::trap_service_ensure`` and
``simp_snmpd::trap_service_startatboot`` appropriately, and place any
configuration files in ``/etc/snmp/user_trapd.d``, with a ``.conf`` extension.

Agent Addresses and Firewall
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, ``pupmod-simp-simp_snmpd`` configures ``snmpd`` to listen on the
local interface.  Use ``simp_snmpd::agentaddress`` to toggle what interfaces
snmpd will listen on.

.. NOTE::

  ``simp_snmpd::agentaddress`` is an array of strings, that should follow the
  format defined in the man page for snmpd, under the ``LISTENING ADDRESS``
  section.


The following is an example agent address:

.. code-block:: yaml

  ---
  simp_snmpd::agentaddress:
    - upd:161
    - tcp:%{facts.fqdn}:161


If ``simp_options::firewall`` is turned on, ``pupmod-simp-simp_snmpd``  will
parse the array of listening addresses to determine what ports should be
opened.  It does not, at this time, do anything for ipx or pvc.
``simp_snmpd::trusted_nets`` is used to determine what networks can access
the ports.

.. NOTE::

  If the agent address is set in a conf file in the user directory, but not in
  Hiera or in the simp_snmpd resource call, ``pupmod-simp-simp_snmpd`` will not
  open the ports in the firewall.


Access
^^^^^^

``pupmod-simp-simp_snmpd`` configures ``SNMP v3``, with

  * User-based Security Model (USM)
  * View-based Access Control Model (VACM).


The profile module, by default, installs two users:

  * ``snmp_ro`` is configured for read only access to system view
  * ``snmp_rw`` is configured for read/write access to everything

User passwords are auto-generated and stored on the Puppet master in the
passgen directory:

``/opt/puppetlabs/server/data/puppetserver/simp/environments/production/simp_autofile/gen_passwd``.

Access is configured by ``/etc/snmp/simp_snmpd.d/access.conf``

  * To create the ``access.conf`` file, the profile modules uses a set of
    hashes.
  * The default hashes are in the ``data/common.yaml`` file.
  * These hashes are merged with any hash you defined in the Hiera files on the
    Puppet master.  Merging is described in
    `Puppet docs <https://docs.puppet.com/puppet/4.10/hiera_merging.html>`_
  * To remove something from the default hash add the name of object with no
    keys

.. NOTE::

  To remove a user, or modify their password, the ``snmpusm`` utility must be
  used, or remove ``/var/lib/net-simp`` and run ``puppet``.  Changing the
  password in the hash or removing the keys will not change the password of an
  existing user.


Example hashes used to create users, views, group and give access:

User Hash
"""""""""

.. code-block:: yaml

  simp_snmpd::v3_users_hash
    username:
      authtype: MD5|SHA
      privtype: DES|AES
      privpass: 'your priv password'
      authpass: 'your auth password'


* If authtype or privtype is missing, it will use the modules ``$defauthtype``
  and ``$defprivtype``
* If either of the passwords are missing, it will be automatically generated
  using passgen

View Hash
"""""""""

.. code-block:: yaml

  simp_snmpd::view_hash:
    viewname:
      included: [array of oids to include]
      excluded: [array of oids to exclude]


* One or both of included, excluded needs to be specified.  Any number of OIDs
  can be listed
* It will create one view line for each oid in the list with exclude or include

Group Hash
""""""""""

.. code-block:: yaml

  simp_snmpd::group_hash:
    groupname:
      model: The security model to use (default to defsecuritymodel)
      secname: [array of user names to include in this group]


* It does not verify the user exists

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


* It does not verify the group exists
* The access name is just a place holder
* For all hashes, anything with a default does not need to be included in the
  hash

.. NOTE::

   Any views, groups, or access lines set up in user conf files will be in
   addition to anything configured in the hash.


Remove Values from Default Hash
"""""""""""""""""""""""""""""""

If you do not want the default user, or any of the views, groups, or access
created, you can pass and empty hash and it will ignore that setting:

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


* The above example will not create the snmp_ro user and add myuser. If the
  snmp_ro user is already created it will not delete it.
* It will override the default definition of readonly_group
* The quickest way to delete users or change the password is to configure
  the hashes and the remove the /var/lib/net-simp directory, stop the
  snmpd daemon, and run ``puppet``.


Client
^^^^^^

By default, ``net-snmp-utils`` and it dependencies are not installed, including
snmpd utilities like snmpget, snmpset, snmpwalk. Set
``simp_snmpd::manage_client`` to ``true`` to install them:

.. code-block:: yaml

  simp_snmpd::manage_client: true


.. NOTE::

  After installation, the default security model, level, authentication, and
  privacy types will be configured.  No default passwords will be configured.


Rsync MIBS and DLMODS
^^^^^^^^^^^^^^^^^^^^^

Rsync can be used to push out custom ``MIBS`` and dynamically loaded shared
objects, or ``dlmod``.

By default, rsync will copy ``MIBS`` into the directory used by ``net-snmp``.
To copy them elsewhere, set ``simp_snmpd::rsync_mibs_dir``
to the fully qualified path.

.. NOTE::

  The module will rsync the files to a ``MIBS`` directory under that path and
  add the directory to the ``MIBS`` path.

``DLMODS`` are copied the same way as ``MIBS``, using the ``rsync_dlmod_dir``
as the destination, creating a dlmod directory.  In order to load dlmods, you
must add the name of the module to the ``isimp_snmpd::dlmods`` list. This will
create a ``dlmod.conf`` file in ``simp_snmpd``.  The ``.so`` extension will be
added.  See the ``Dynamically Loadable Modules`` section in man page of
``snmpd.conf`` for more information.

Below is an example showing how to activate rsync of ``MIBS`` and ``dlmods``:

.. code-block:: yaml

  ---
  simp_snmpd::rsync_dlmod: true
  simp_snmpd::rsync_mibs: true
  simp_snmpd::dlmods:
    - mymodulename


.. _JIRA Bug Tracking: https://simp-project.atlassian.net/


HOWTO Enable Redundant LDAP
===========================

This section describes how to set up redundant OpenLDAP servers in SIMP.  These
servers are also referred to as "slave" servers.


Set up the Master
-----------------

The easiest way to setup an ldapmaster is to set it up on the puppet master
using `simp config` during the initial configuration of the Puppet server. This
is done by answering "YES" to "use ldap?" query when you run simp config and
answering the basic questions it asks you.  If it is not desirable to have the
ldap server on the Puppet server a redundant LDAP server can be set up on an
alternate server and promoted to master using the directions below.

However if there is already a working infrastructure and you want to move to
openldap you can do the following:

Configure the following settings in ``simp_def.yaml`` in the hiera directory:

.. code-block:: yaml

  # === ldap ===
  # Whether or not to use LDAP on this system.
  # If you disable this, modules will not attempt to use LDAP where possible.
  simp_options::ldap: true

  # The Base DN of the LDAP server
  simp_options::ldap::base_dn: "dc=your,dc=domain"

  # LDAP Bind Distinguished Name
  simp_options::ldap::bind_dn: "cn=hostAuth,ou=Hosts,%{hiera('ldap::base_dn')}"

  # The LDAP bind password
  simp_options::ldap::bind_pw: "MyRandomlyGeneratedLargePassword"

  # The salted LDAP bind password hash
  simp_options::ldap::bind_hash: "{SSHA}9nByVJSZFBe8FfMkar1ovpRxJLdB0Crr"

  #
  simp_options::ldap::sync_dn: "cn=LDAPSync,ou=Hosts,%{hiera('ldap::base_dn')}"

  # The LDAP sync password
  simp_options::ldap::sync_pw: "MyOtherRandomVeryLargePassword"

  # The SSHA hash for ldap::sync_pw
  simp_options::ldap::sync_hash: "{SSHA}VlgYUmRzyuuKZXM3L8RT28En/eqtuTUO"

  # The LDAP root DN.
  simp_options::ldap::root_dn: "cn=LDAPAdmin,ou=People,%{hiera('ldap::base_dn')}"

  # The LDAP root password hash.
  # If you set this with simp config, type the password and the hash will be
  # generated for you.'
  simp_options::ldap::root_hash: "{SSHA}GSCDnNF6KMXBf1F8eIe5xvQxVJou3zGu"

  # This is the LDAP master in URI form (ldap://server)
  simp_options::ldap::master: ldap://ldap_server1.your.domain

  # === ldap::uri ===
  # List of OpenLDAP servers in URI form (ldap://server)
  simp_options::ldap::uri:
    - ldap://ldap_server1.your.domain

  # === sssd::domains ===
  # A list of domains for SSSD to use.
  # `simp config` will automatically populate this field with `FQDN` if
  # `use_fqdn` is true, otherwise it will comment out the field.
  #
  sssd::domains:
    - LDAP


Add the `simp::ldap_server` class into the yaml file for the ldap server in
Hiera (`hieradata/hosts/ldap_server1.your.domain.yaml`):

.. code-block:: yaml

  classes :
    - 'simp::server::ldap'

Leave any other classes that are there if they are needed.  Run the puppet
agent on the ldap server until it runs cleanly. Run the agent on the puppet
server.  Once all the other clients update against the Puppet server, they will
be able to authenticate against the LDAP server.  Adding users and groups is
described in the :ref:`User_Management`.

.. NOTE::

 Information on how the create salted ({SSHA}) passwords can be found at the
 `OpenLDAP site <http://www.openldap.org/faq/data/cache/347.html>`__.


Set up the Redundant(Slave) Servers
-----------------------------------

Default Settings
~~~~~~~~~~~~~~~~

Once the master is ready, LDAP slave nodes can be configured to replicate data
from the master. These servers are read-only, and modifications cannot be made
to LDAP entries while the master is down.

Slave nodes can be configured via hiera by using `simp_options::simp::ldap_server::is_slave`,
setting the replication id, and adding the `simp_options::simp::ldap_server` class.  This
will set up your redundant server using the defaults. To do these three things,
add the following lines to the
``hieradata/hosts/ldap_server2.your.domain.yaml`` file:

.. code-block:: yaml

  simp::server::ldap:is_slave: true
  simp::server::ldap:rid: "888"

  classes :
    - 'simp::server::ldap'

.. _URI:

To make other clients aware of this server, add the redundant server's URI to
lists of URIs in the ``hieradata/simp_def.yaml`` file:

.. code-block:: yaml

  # === ldap::uri ===
  # List of OpenLDAP servers in URI form (ldap://server)
  simp_options::ldap::uri:
    - ldap://ldap_server1.your.domain
    - ldap://ldap_server2.your.domain

.. NOTE::

 To see the defaults for LDAP replication in SIMP, review the parameters passed
 to the module ``openldap/manifests/server/syncrepl.pp``. These parameters are
 used to add the replication settings to the ``syncrepl.conf`` file.
 Definitions can be found in the syncrepl.conf (5) man page.


Custom Replication Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~

If settings other than the defaults are needed, create a manifest under
`site::` and use the `openldap::server::syncrepl` class with the necessary
parameters.

In this example, the site profile is called `site::ldapslave` and the RID of
the server is `999` (these can be changed).  One setting, `sizelimit`, is being
overwritten but you can overwrite any number of them.

.. code-block:: puppet

  class site::ldapslave {

    include 'simp::server::ldap'

    # custom settings:
    openldap::server::syncrepl { '999':
      sizelimit  => '5000',
    }
  }

The name of the `openldap::server::syncrepl` instance must be a unique replication id.

Place this file in the `site::` module's  `manifests/` directory using the name
`ldapslave.pp`.   Include this class from the slave server's hiera .yaml file:

.. code-block:: yaml

  classes :
    - 'site::ldapslave'


Lastly, add the server to the URI_ listing in `simp_def.yaml` so all the
clients know about it once they have updated from the puppet master.

Promote a Slave Node
--------------------

Slave nodes can be promoted to act as the LDAP master node. To do this, change
the node classifications of the relevant hosts.  For a node with the default
settings, just remove the ``simp::ldap_server::is_slave : true`` from the
server's hiera .yaml file and change the setting for the master ldap in the
``simp_def.yaml``.

.. code-block:: yaml

  # This is the LDAP master in URI form (ldap://server)
  simp_options::ldap::master: ldap://ldap_server1.your.domain

For a redundant server set up using custom settings, remove the call to the
custom class and replace it with the call to the site::ldap_server class in the
servers yaml file and set the master setting in the ``simp_def.yaml`` file as
shown above.

In both cases, if the current master is not down, make sure it has completed
replication before changing the settings.  Once the settings are changed, run
puppet agent -t on the ldap server. After the next Puppet run on all the hosts
the server will be promoted to master and all the slaves will point to it.

Remove a Node or Demote a Master
--------------------------------

To demote a master, simply configure it as slave in either of the
configurations above after the new master has been configured and put in place,
then run the puppet agent.  Lastly, manually remove the active database from
the server. (Check the setting ``openldap::server::conf::directory`` setting
for the location of the files.)

To remove an LDAP server, first remove the server from the URI_  settings in
``simp_def.yaml``.  Give the clients time to update from the puppet server so
they do not attempt to call it.  Then remove relevant settings from it's hiera
.yaml file and run the puppet agent.

Troubleshooting
---------------

If the system is not replicating, it is possible that another user has updated
the ``simp_options::ldap::sync_pw`` and ``simp_options::ldap::sync_hash`` entries in the
``/etc/puppetlabs/code/environments/simp/simp_def.yaml`` file without also updating the
value in LDAP itself; this is the most common issue reported by users.

Currently, SIMP cannot self-modify the LDAP database directly; therefore, the
LDAP Administrator needs to perform this action. Refer to the
:ref:`User_Management` chapter for more information on manipulating entries in
OpenLDAP.

The example below shows the changes necessary to update the
``simp_options::ldap::sync`` information in LDAP.

Update ``simp_options::ldap::sync`` Information in LDAP Examples

.. code-block:: yaml

  dn: cn=LDAPSync,ou=People,dc=your,dc=domain
  changetype: modify
  replace: userPassword
  userPassword: <Hash from simp_options::ldap::sync_hash>


Further Information
--------------------

The `OpenLDAP site <http://www.openldap.org/doc/admin24/intro.html>`__ contains more information on configuring and maintaining Open LDAP servers.


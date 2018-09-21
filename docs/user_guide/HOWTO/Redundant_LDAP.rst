HOWTO Enable Redundant LDAP
===========================

This section describes how to set up redundant OpenLDAP servers in SIMP. These
servers are also referred to as "slave" servers.

Set up the Master
-----------------

The easiest way to set up an LDAP master is to set it up on the Puppet master
using ``simp config`` during the initial configuration of the Puppet master.
This is done by answering "yes" when asked if you want to use LDAP during your
initial ``simp config`` run and answering the basic questions it asks you. If
it is not desirable to have the LDAP server on the Puppet master, a LDAP server
can be set up on an alternate server by including the ``simp::server::ldap`` on
the node of your choice.

.. NOTE::

   If you use another node, you may want to re-run ``simp config`` and answer
   the questions with this new LDAP master server in mind.

If you do not want to run ``simp config`` again, you will need to configure the
following settings in :term:`Hiera`:

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

  # The DN of the LDAP sync user
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
  simp_openldap::server::conf::rootpw: "{SSHA}GSCDnNF6KMXBf1F8eIe5xvQxVJou3zGu"

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

Add the ``simp::server::ldap`` class into the yaml file for the LDAP server in
Hiera, for example: ``hieradata/hosts/ldap_server1.your.domain.yaml``:

.. code-block:: yaml

  classes :
    - 'simp::server::ldap'

Leave any other classes that are there if they are needed. Run the Puppet
agent on the LDAP server until it runs cleanly. Run the agent on the Puppet
server. Once all the other clients update against the Puppet master, they will
be able to authenticate against the LDAP server. Adding users and groups is
described in the :ref:`User_Management`.

.. NOTE::

 Information on how the create salted ({SSHA}) passwords can be found at the
 `OpenLDAP site <http://www.openldap.org/faq/data/cache/347.html>`__.

Set up the Redundant (Slave) Servers
------------------------------------

Default Settings
~~~~~~~~~~~~~~~~

Once the LDAP master is ready, LDAP slave nodes can be configured to replicate
data from the master. These servers are read-only, and modifications cannot be
made to LDAP entries while the master is down.

Slave nodes can be configured via Hiera by setting
``simp::server::ldap::is_slave`` to ``true``, setting the
replication id (RID) , and adding the ``simp::server::ldap``
class. This will set up your redundant server using the defaults. To do these
three things, add the following lines to the
``hieradata/hosts/ldap_server2.your.domain.yaml`` file:

.. code-block:: yaml

  simp_openldap::server::conf::rootpw: "{SSHA}GSCDnNF6KMXBf1F8eIe5xvQxVJou3zGu"
  simp::server::ldap::is_slave: true
  simp::server::ldap::rid: 888

  classes :
    - 'simp::server::ldap'

.. _URI:

To make other clients aware of this server, add the redundant server's URI to
lists of URIs in the ``hieradata/default.yaml`` file:

.. code-block:: yaml

  # === ldap::uri ===
  # List of OpenLDAP servers in URI form (ldap://server)
  simp_options::ldap::uri:
    - ldap://ldap_server1.your.domain
    - ldap://ldap_server2.your.domain

.. NOTE::

   To see the defaults for LDAP replication in SIMP, review the parameters
   passed to the module ``simp_openldap/manifests/server/syncrepl.pp``. These
   parameters are used to add the replication settings to the ``syncrepl.conf``
   file. Definitions can be found in the syncrepl.conf (5) man page.

Custom Replication Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~

If settings other than the defaults are needed, create a manifest under
``site`` and use the ``simp_openldap::server::syncrepl`` class with the necessary
parameters.

In this example, the site profile is called ``site::ldap_slave`` and the RID of
the server is ``999`` (these can be changed). One setting, ``sizelimit``, is
being overwritten but you can overwrite any number of them.

.. code-block:: puppet

  class site::ldap_slave {

    include 'simp::server::ldap'

    # custom settings:
    simp_openldap::server::syncrepl { '999':
      sizelimit  => '5000',
    }
  }

The name of the ``simp_openldap::server::syncrepl`` instance must be a unique
replication id.

Place this file in the ``site`` module's  ``manifests/`` directory using the name
`ldapslave.pp`. Include this class from the slave server's Hiera YAML file:

.. code-block:: yaml

  classes :
    - 'site::ldap_slave'


Lastly, add the server to the URI_ listing in ``default.yaml`` so all the
clients know about it once they have updated from the Puppet master.

Promote a Slave Node
--------------------

Slave nodes can be promoted to act as the LDAP master node. To do this, change
the node classifications of the relevant hosts. For a node with the default
settings, just remove the ``simp::server::ldap::is_slave : true`` from the
server's Hiera YAML file and change the setting for the master LDAP in Hiera.
This setting is needed by all LDAP servers. (It defaults to the Puppet master
if it is not set.)

.. code-block:: yaml

  # This is the LDAP master in URI form (ldap://server)
  simp_options::ldap::master: ldap://ldap_server2.your.domain

For a redundant server setup using custom settings, remove the call to the
custom class and replace it with the call to the ``site::ldap_server`` class in
the servers yaml file and set the master setting in the Hiera as shown above.

In both cases, if the current master is not down, make sure it has completed
replication before changing the settings. Once the settings are changed, run
``puppet agent -t`` on the LDAP server. After the next Puppet run on all the
hosts the server will be promoted to master and all the slaves will point to
it.

Remove a Node or Demote a Master
--------------------------------

To demote a master, simply configure it as slave in either of the
configurations above after the new master has been configured and put in place.
Then run the Puppet agent. Lastly, manually remove the active database from
the server. (Check the setting ``simp_openldap::server::conf::directory``
setting for the location of the files.)

To remove an LDAP server, first remove the server from the
``simp_options::ldap::uri`` settings in Hiera. Give the clients time to update
from the Puppet master so they do not attempt to call it. Then remove relevant
settings from it's hiera.yaml file and run the Puppet agent.

Troubleshooting
---------------

If the system is not replicating, it is possible that another user has updated
the ``simp_options::ldap::sync_pw`` and ``simp_options::ldap::sync_hash``
entries in Hiera file without also updating the value in LDAP itself;
this is the most common issue reported by users. If simp config was used to
set up the server these values are in the ``simp_config_settings.yaml`` file.

Currently, SIMP cannot self-modify the LDAP database directly; therefore, the
LDAP Administrator needs to perform this action. Refer to the
:ref:`User_Management` chapter for more information on manipulating entries in
LDAP.

The example below shows the changes necessary to update the
``simp_options::ldap::sync`` information in LDAP.

Update ``simp_options::ldap::sync`` Information in LDAP Examples:

.. code-block:: yaml

  dn: cn=LDAPSync,ou=People,dc=your,dc=domain
  changetype: modify
  replace: userPassword
  userPassword: <Hash from simp_options::ldap::sync_hash>

Further Information
--------------------

The `OpenLDAP site <http://www.openldap.org/doc/admin24/intro.html>`__ contains
more information on configuring and maintaining OpenLDAP servers.

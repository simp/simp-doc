Redundant LDAP
==============

This section describes how to set up redundant OpenLDAP servers in SIMP.

The version of OpenLDAP in RHEL5 only supports *syncrepl*. Multi-master
replication has been added in a more recent version of OpenLDAP but is
not currently supported in SIMP. *Syncrepl* is optimal for :term:`Wide Area Network (WAN)` situations
and is the SIMP default.

Set up the Master
-----------------

If the standard *puppet\_servers.pp* file in SIMP is being used, the
user has a working master server. If not, the following example
demonstrates how to use the SIMP *openldap* module to create a server
using the *puppet\_servers.pp* file .

Source Code for Using an OpenLDAP Server openldap

.. code-block:: Ruby

            # These are some common variables.
            # See /etc/puppet/manifests/vars.pp for the stock version.

            $ldap_master = 'ldap://ldapmaster.your.domain'

            class ldap_common {
              include 'openldap::slapd_pki'

              openldap::slapd::conf { 'default':
                suffix => 'dc=your,dc=domain',
                rootdn => 'dn=LDAPAdmin,ou=People,dc=your,dc=domain',
                rootpw => '{SSHA}$klskf$asoghaagasgasgaggawawg',
                tlsCertificateFile => "/etc/pki/public/${fqdn}.pub",
                tlsCertificateKeyFile => "/etc/pki/private/${fqdn}.pem",
                client_nets => [ '1.2.3.4/16' ]
              }
            }

            class ldap_master inherits ldap_common {
              include 'openldap::slapo::syncprov'

              openldap::slapo::syncprov::conf { "default": }
            }

            node ldapmaster {
              include 'ldap_master'
            }
          
.. _Redundant_LDAP-Replicants:

Set up the Replicated Servers
-----------------------------

Once the master is ready, LDAP slave nodes must be configured to
replicate data from the master. The example below shows an the code that
should be added to the slave node in Puppet. The actual order of which
gets done first is irrelevant; the replicated servers will attempt to
contact the master until they are successful.

Source Code to Configure an LDAP Slave Node replication

.. code-block:: Ruby

            class ldap_repl inherits ldap_common {
              include 'openldap::slapd::syncrepl'

              openldap::slapd::syncrepl::conf { "111":
                provider => $ldap_master,
                syncrepl_retry => '60 10 600 +',
                searchbase => 'dc=your,dc=domain',
                starttls => 'critical',
                bindmethod => 'simple',
                binddn => 'cn=LDAPSync,ou=People,dc=your,dc=domain',
                credentials => '<plain text password>',
                updateref => $ldap_master
              }
            }

            node ldaprepl1 {
              include "ldap_repl"
            }

            node ldaprepl2 {
              include "ldap_repl"
            }
          

Promote a Slave Node
--------------------

Slave nodes can be promoted to act as the LDAP master node. To do this,
change the node classifications of the relevant hosts. The following
example shows the promotion of the *ldaprepl1* server to the master
server.

Source Promoting a Slave Node LDAP

.. code-block:: Ruby

            # Change the common ldap server variable to promote the slave node.

            $ldap_master = 'ldap://ldaprepl1.your.domain'

            node ldapmaster {
              # include 'ldap_master'
            }

            node ldaprepl1 {
              # include 'ldap_repl'
              include 'ldap_master'
            }
          

After the next Puppet run on all hosts, *ldaprepl1* will be promoted to
the master and all slave nodes will point to it.

Troubleshooting
---------------

If the system is not replicating, it is possible that another user has
updated the *$ldap\_sync\_passwd* and *$ldap\_sync\_hash* entries in the
*/etc/puppet/manifests/vars.pp* file without also updating the value in
LDAP itself; this is the most common issue reported by users.

Currently, SIMP cannot self-modify the LDAP database directly;
therefore, the LDAP Administrator needs to perform this action. Refer to
the :ref:`User_Management` chapter for more information on manipulating entries in OpenLDAP.

The example below shows the changes necessary to update the
*$ldap\_sync* information in LDAP.

Update $ldap\_sync Information in LDAP Examples

.. code-block:: Ruby

            dn: cn=LDAPSync,ou=People,dc=your,dc=domain
            changetype: modify
            replace: userPassword
            userPassword: <Hash from $ldap_sync_hash>
            

Master Node Demotion
~~~~~~~~~~~~~~~~~~~~

In the event that multiple master nodes have been set up, it may be
necessary to demote one or more of them to slave instances. To do this,
add the replication code shown in the previous section titled :ref:`Redundant_LDAP-Replicants` to the
manifest of the master node being demoted.

Once this is complete, manually remove the active database from the LDAP
server being demoted and then run Puppet. The SIMP team is working to
enable SIMP to handle this transition automatically in the future.

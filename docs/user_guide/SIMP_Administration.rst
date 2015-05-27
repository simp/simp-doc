SIMP Administration
===================

This chapter provides basic guidance on how to administer a SIMP
environment.

    **Warning**

    While working with the system, keep in mind that Puppet does not
    work well with capital letters in host names. Therefore, they should
    not be used.

Nightly Updates
---------------

All SIMP systems are configured, by default, to do a YUM update of the
entire system on a nightly basis.

The configuration pulls updates from all repositories that the system is
aware of. To change this behavior, refer to the :ref:`Exclude_Repos` FAQ section. This
configuration is also helpful because it is easier to manage symlinks in
YUM repositories than it is to manage individual package minutia for
every single package on every system.

The general technique is to put packages that all systems will receive
into the *Updates* repository provided with SIMP. Any packages that will
only go to specific system sets will then be placed into adjunct
repositories under */var/www/yum* and the user will point specific
systems at those repositories using the *yumrepo* Puppet type. Any
common packages can be symlinked or hard linked between repositories for
maximum space utilization.

Sudosh
------

By default, a SIMP system uses :term:`Sudosh` to enable logging of sudo sessions to
*Rsyslog*. To open a sudo session as *root* (or any other user), type
**su -** as simp, or **sudo sudosh** as anyone else, instead of **sudo
su**.

The logs are stored in */var/log/sudosh.log*. Sessions can be replayed
by typing **sudosh-syslog-replay**.

User Accounts
-------------

By default, users can add local users to a system or use LDAP to
administer users.

It is recommended that LDAP is used for adding all regular users so that
there is no conflict with multiple system updates and synchronization.
For more information on managing LDAP users, refer to the :ref:`User_Management` chapter.

It is also possible that there will be users that are local to the
system. To have these users follow the normal password expiration
conventions set on the system, use the native Puppet user and group
types.

To have a user that does not expire, look at the
*/etc/puppet/localusers* file to enable these users across the systems.
The comments in the file provide instructions on generating entries for
the desired systems. It is hoped that future versions of Puppet will
support the modification of password expiration values via the native
types and that the *localusers* file will be retired.

Certificate Management
----------------------

This section describes the two different types of certificates used in a
SIMP system and how to manage them. For information on initial
certificate setup, refer to the :ref:`Certificates` section of the Client Management
chapter.

Server Certificates
-------------------

Server certificates are the standard PKI certificates assigned either by
an official CA or generated using the FakeCA utility offered by SIMP.
They can be found in the */etc/pki/* directory of both the client and
server systems. These certificates are set to expire annually. To change
this, edit the following files with the number of days for the desired
lifespan of the certificates:

    **Note**

    This assumes that the user has generated Certificates with the
    FakeCA provided by SIMP. If offical certificates are being used,
    these settings must be changed within the official CA, not on the
    SIMP system.

-  */etc/puppet/Config/FakeCA/CA*

-  */etc/puppet/Config/FakeCA/ca.cnf*

-  */etc/puppet/Config/FakeCA/default\_altnames.cnf*

-  */etc/puppet/Config/FakeCA/default.cnf*

-  */etc/puppet/Config/FakeCA/user.cnf*

In addition, any certificates that have already been created and signed
will have a config file containing all of its details in
*/etc/puppet/Config/FakeCA/output/conf/*.

    **Important**

    Editing any entries in the above mentioned config files will not
    affect the existing certificates. To make changes to an existing
    certificate it must be re-created and signed.

Below is an example of how to change the expiration time from one year
(the default) to five years for any newly created certificate.

.. code-block:: Bash

            for file in $(grep -rl 365 /etc/puppet/Config/FakeCA/)
            do
              sed -i 's/365/1825/' $file
            done
            

Puppet Certificates
-------------------

Puppet certificates are issued and maintained strictly within Puppet.
They are different from the server certificates and should be managed
with the **puppet cert** tool. For the complete documentation on the
**puppet cert** tool, visit the `Puppet Labs cert
manual <http://docs.puppetlabs.com/man/cert.html>`__ detailing its
capabilities. On a SIMP system, these certificates are located in the
*/var/lib/puppet/ssl/* directory and are set to expire every five years.

Applications
------------

This section describes how to add services to the servers. To perform
this action, it is important to understand how to use IPtables and what
the *svckill.rb* script does on the system.

IPTables
--------

By default, the SIMP system locks down all incoming connections to the
server save port 22. Port 22 is allowed from all external sources since
it is expected that the user will want to be able to SSH into the
systems from the outside at all times.

The default alteration for the IPtables start-up script is such that it
will "fail safe". This means that if the IPtables rules are incorrect,
the system will not open up the IPtables rule set completely. Instead,
the system will deny access to all ports except port 22 to allow for
recovery via SSH.

There are many examples of how to use the IPtables module in the source
code; the Apache module at */etc/puppet/modules/apache* is a
particularly good example. In addition, look at the definitions in the
IPtables module to understand their purpose and choose the best option.
Refer to the `IPtables page of the Developers
Guide <../../developers_guide/rdoc/classes/iptables.html>`__ for a good
summary and example code (HTML version only).

svckill.rb
----------

To ensure that the system does not run more services than are required,
the *svckill.rb* script has been implemented to stop any service that is
not properly defined in the Puppet catalogue.

To prevent services from stopping, refer to the instructions in the 
:ref:`Services_Dying` FAQ section.

GUI
---

SIMP was designed as a minimized system, but it is likely that the user
will want to have a GUI on some of the systems. Refer to the :ref:`Infrastructure-Setup` section
for information on setting up GUIs for the systems.

General Administration
======================

This chapter provides basic guidance on how to administer a SIMP environment.

.. WARNING::

    While working with the system, keep in mind that Puppet does not work well
    with capital letters in host names. Therefore, they should not be used.


Nightly Updates
---------------

All SIMP systems are configured, by default, to do a YUM update of the entire
system on a nightly basis.

The configuration pulls updates from all repositories that the system is aware
of. To change this behavior, refer to the :ref:`Exclude_Repos` HOWTO section.
This configuration is also helpful because it is easier to manage symlinks in
YUM repositories than it is to manage individual package minutia for every
single package on every system.

The general technique is to put packages that all systems will receive into
the ``Updates`` repository provided with SIMP. Any packages that will only go
to specific system sets will then be placed into adjunct repositories under
``/var/www/yum`` and the user will point specific systems at those
repositories using the ``yumrepo`` Puppet type. Any common packages can be
symlinked or hard linked between repositories for maximum space utilization.


Extending the Native Framework
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, SIMP stores :term:`YUM` information in the following directories:

 - ``/var/www/yum``

The base SIMP repository is in ``/var/www/yum/SIMP`` and it is highly
unlikely that you would want to modify anything in this directory.

With the standard configuration, access to the yum repository is restricted to
the networks contained in the ``simp_options::trusted_nets`` variable in
:term:`Hiera`.  For this section, we will assume that this is sufficient for
your needs.


The Operating System Repos
^^^^^^^^^^^^^^^^^^^^^^^^^^

The default location for the OS repositories is
``/var/www/yum/<OSTYPE>/<MAJORRELEASE>/x86_64``.

An ``Updates`` repository has been configured in this space. All OS updates
should be placed within this directory.

You should run the following in the ``Updates`` directory after **any** package
addition or removal within that directory.

.. code-block:: bash

  $ createrepo .
  $ chown -R root.apache ./*
  $ find . -type f -exec chmod 640 {} \;
  $ find . -type d -exec chmod 750 {} \;
  $ yum clean all
  $ yum makecache


Adding a Custom Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^

For this example, we are going to assume that you have a repository named
``foo`` that you would like to expose to your systems. You would perform the
following commands to enable this repository on the server:

  .. code-block:: bash

     $ cd /var/www/yum
     $ mkdir foo
     $ cd foo
     $ -- copy all RPMs into the folder
     $ createrepo .
     $ chown -R root.apache ./*
     $ find . -type f -exec chmod 640 {} \;
     $ find . -type d -exec chmod 750 {} \;


By placing the ``basepath`` of the repository within the default path served by
Apache, it will be exposed to all networks in ``simp_options::trusted_nets``.  To modify the
package set in any repository at any time, re-run:

.. code-block:: bash

  $ cd /some/repository/
  $ cp /some/packages /some/repository/
  $ createrepo .
  $ chown -R root.apache ./*
  $ find . -type f -exec chmod 640 {} \;
  $ find . -type d -exec chmod 750 {} \;
  $ yum clean all
  $ yum makecache

.. _ug-configuring-the-clients:


Configuring the Clients
^^^^^^^^^^^^^^^^^^^^^^^

Now that you've added this directory, you're obviously going to want to add it
to one or more client nodes.

The best way to do this is to make it part of your site configuration.  You
**can** make it part of your module, but you will need to wrap it in a define so
that the server can be modified. This ends up being not too much better than
just adding it to each node manually.

To add it to the client node, you should use the puppet ``yumrepo`` native type.
You can find more information on the type on the `Puppet Type Reference`_ on the
Internet.

At a glance, it would look like the following (assuming you are doing this one
on the server configured as ``simp_options::yum::server`` in :term:`Hiera`):

.. code-block:: ruby

  yumrepo { example:
    baseurl         => "http://your.server.fqdn/yum/example",
    enabled         => 1,
    enablegroups    => 0,
    gpgcheck        => 0,
    keepalive       => 0,
    metadata_expire => 3600,
    tag             => "firstrun"
  }


Working Outside the SIMP Framework
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SIMP framework fully supports `Puppet Environments`_ and, by default,
installs into an environment named ``simp``. This environment is symlinked to
the ``production`` environment by default but that symlink will **not** be
overwritten on update so you may freely update the environment to meet your
needs.


Session auditing
----------------

By default, a SIMP system uses :term:`Sudosh` to enable logging of sudo
sessions to ``Rsyslog``. To open a sudo session as ``root`` (or any other
user), type ``su -`` as simp, or ``sudo sudosh`` as anyone else, instead of
``sudo su``.

The logs are stored in ``/var/log/sudosh.log``. Sessions can be replayed by
typing ``sudosh-syslog-replay``.


User Accounts
-------------

By default, users can add local users to a system or use LDAP to administer
users.

It is recommended that LDAP is used for adding all regular users so that there
is no conflict with multiple system updates and synchronization.  For more
information on managing LDAP users, refer to the :ref:`User_Management`
chapter.

It is also possible that there will be users that are local to the system. To
have these users follow the normal password expiration conventions set on the
system, use the native Puppet user and group types.


Certificate Management
----------------------


This section describes the two different types of certificates used in a SIMP
system and how to manage them. For information on initial certificate setup,
refer to the :ref:`Certificates` section of the Client Management chapter.


Server Certificates
^^^^^^^^^^^^^^^^^^^

Server certificates are the standard PKI certificates assigned either by an
official CA or generated using the FakeCA utility offered by SIMP.  They can be
found in the ``/etc/pki/simp`` directory of both the client and server systems.
These certificates are set to expire annually. To change this, edit the
following files with the number of days for the desired lifespan of the
certificates:

.. NOTE::

    This assumes that the user has generated Certificates with the
    FakeCA provided by SIMP. If official certificates are being used,
    these settings must be changed within the official CA, not on the
    SIMP system.

-  ``/etc/puppetlabs/code/environments/simp/FakeCA/CA``

-  ``/etc/puppetlabs/code/environments/simp/FakeCA/ca.cnf``

-  ``/etc/puppetlabs/code/environments/simp/FakeCA/default\_altnames.cnf``

-  ``/etc/puppetlabs/code/environments/simp/FakeCA/default.cnf``

-  ``/etc/puppetlabs/code/environments/simp/FakeCA/user.cnf``

In addition, any certificates that have already been created and signed will
have a config file containing all of its details in
``/etc/puppetlabs/code/environments/simp/FakeCA/output/conf/``.

.. IMPORTANT::

    Editing any entries in the above mentioned config files will not
    affect the existing certificates. To make changes to an existing
    certificate it must be re-created and signed.

Below is an example of how to change the expiration time from one year (the
default) to five years for any newly created certificate.

.. code-block:: bash

  for file in $(grep -rl 365 /etc/puppetlabs/code/environments/simp/FakeCA/)
  do
    sed -i 's/365/1825/' $file
  done


Puppet Certificates
^^^^^^^^^^^^^^^^^^^

Puppet certificates are issued and maintained strictly within Puppet.  They are
different from the server certificates and should be managed with the
``puppet cert`` tool. For the complete documentation on the ``puppet cert``
tool, visit the `Puppet Inc. cert manual <https://docs.puppet.com/puppet/latest/man/cert.html>`__
detailing its capabilities. On a SIMP system, these certificates are located in
the ``/etc/puppetlabs/puppet/ssl`` directory and are set to expire every five years.


The SIMP Utility
----------------

The SIMP server provides a command line utility called ``simp`` that is a
simple interface into some SIMP-specific settings and subsystems.

The best source of information on the capabilities of this tool are the help
page which can be accessed via ``simp help``.

.. _simp passgen:


simp passgen
^^^^^^^^^^^^

Throughout the SIMP codebase, you may find references to the ``passgen()``
function. This function will auto-generate passwords and store them in the
``simp_autofiles/gen_passwd`` space in the root of the simp Environment on the
Puppet server. For more information, see the `passgen()`_ documentation.


Integrating Applications
------------------------

This section describes how to add services to the servers. To perform this
action, it is important to understand how to use IPTables and what the
``svckill.rb`` script does on the system.


IPTables
^^^^^^^^

By default, the SIMP system locks down all incoming connections to the server,
save port 22. Port 22 is allowed from all external sources since it is expected
that the user will want to be able to SSH into the systems from the outside at
all times.

The default alteration for the :term:`IPTables` start-up script is such that it will
"fail safe". This means that if the IPTables rules are incorrect, the system
will not open up the IPTables rule set completely. Instead, the system will
deny access to all ports except port 22 to allow for recovery via SSH.

There are many examples of how to use the IPTables module in the source code;
the Apache module at ``/etc/puppetlabs/code/environments/simp/modules/apache`` is a
particularly good example. In addition, look at the definitions in the IPTables
module to understand their purpose and choose the best option.  Refer to the
`IPTables page of the Developers Guide <../../developers_guide/rdoc/classes/iptables.html>`__
for a good summary and example code (HTML version only).


svckill.rb
^^^^^^^^^^

To ensure that the system does not run more services than are required, the
``svckill.rb`` script has been implemented to stop any service that is not
properly defined in the Puppet catalog.

To prevent services from stopping, refer to the instructions in the
:ref:`Services_Dying` Troubleshooting section.


GUI
^^^

SIMP was designed as a minimized system, but it is likely that the user will
want to have a GUI on some of the systems. Refer to the
:ref:`Infrastructure-Setup` section for information on setting up GUIs for the
systems.

.. _Puppet Type Reference: https://docs.puppet.com/puppet/latest/type.html
.. _Puppet Environments: https://docs.puppet.com/puppet/latest/environments.html
.. _passgen(): https://github.com/simp/pupmod-simp-simplib/blob/master/lib/puppet/parser/functions/passgen.rb

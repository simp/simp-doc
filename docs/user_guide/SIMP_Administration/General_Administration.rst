General Administration
======================

.. WARNING::

      While working with the system, keep in mind that Puppet does not work well
      with capital letters in host names. Therefore, they should not be used.

The SIMP Environment
--------------------

SIMP fully supports `Puppet Environments`_ and, by default, installs into an
environment named ``simp``. This environment is symlinked to the ``production``
environment by ``simp config`` but that symlink will **not** be overwritten on
update so you may freely change or replace the symlink to meet your needs.

There are a couple of paths on the system that are environment related.

/var/simp
^^^^^^^^^

This space holds all static, non-Puppet created files. It is generally used for
large binary items that will be centrally delievered via rsync and for files
that are too dangerous to add to a version control system. These include things
like the SIMP rsync materials and the Infrastructure keys.

This space is environment aware and you will note that there is an
``environments`` directory under ``/var/simp`` with, by default, the ``simp``
environment represented. If you add new environments, you will need to
replicate the appropriate structure from the ``simp`` environment into your
custom environment.

This space also holds FakeCA. See `Infrastructure Certificates`_.

.. NOTE::

    For more information on the SIMP rsync structure, please see
    :ref:`HOWTO Work with the SIMP Rsync Shares`

/opt/puppetlabs/server/data/puppetserver/simp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This space holds all non-static, Puppet **server** created files. This is used
by both ``passgen()`` and the ``krb5`` Puppet module for storing dynamically
generated server-side content.

Like ``/var/simp`` this space is also environment aware but you should never
need to manually adjust anything in this directory space.


Nightly Updates
---------------

All SIMP systems are configured, by default, to do a YUM update of the entire
system on a nightly basis. When the update task runs, it will pull **ALL**
updates that the system is aware of.

.. NOTE::

    Refer to the :ref:`Exclude_Repos` HOWTO for additional configuration
    information.

SIMP chose this as the default because it is easier to manage symlinks in YUM
repositories than it is to manage individual package minutia for all packages
across the environment.

To use this effectively, packages that all systems will receive should be
placed into the ``Updates`` repository provided with SIMP. Any packages that
will only go to specific system sets should then be placed into adjunct
repositories under ``/var/www/yum`` and the user will point specific systems at
those repositories using the ``yumrepo`` Puppet Type. Any common packages can
be either symlinked or hard linked between repositories for efficiency.


Changing the Default Repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default, SIMP stores :term:`YUM` information in the following directories:

 - ``/var/www/yum``

The base SIMP repository is in ``/var/www/yum/SIMP`` and it is highly unlikely
that you would want to modify anything in this directory.

By default, access to the YUM repository is restricted to the networks
contained in the ``simp_options::trusted_nets`` parameter. For this section, we
will assume that this is sufficient.


The Operating System Repos
^^^^^^^^^^^^^^^^^^^^^^^^^^

The default location for the :term:`Operating System` (OS) repositories, on the
Puppet server, is ``/var/www/yum/<OSTYPE>/<MAJORRELEASE>/x86_64``.

An ``Updates`` repository has been configured in this space. All OS updates
should be placed within this directory.

You should run the following in the ``Updates`` directory after **ANY** package
addition or removal within that directory.

.. code-block:: bash

   $ createrepo .
   $ chown -R root.apache ./*
   $ find . -type f -exec chmod 640 {} \;
   $ find . -type d -exec chmod 750 {} \;


Adding a Custom Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^

For this section, we will assume that you have a repository named ``foo`` that
you would like to expose to your systems. To do this, perform the following:

   .. code-block:: bash

       $ cd /var/www/yum
       $ mkdir foo
       $ cd foo
       $ -- copy all RPMs into the folder
       $ createrepo .
       $ chown -R root.apache ./*
       $ find . -type f -exec chmod 640 {} \;
       $ find . -type d -exec chmod 750 {} \;

.. NOTE::

    For more information on managing YUM repos, please see the
    `Red Hat local repository Documentation`_.


Configuring the Clients
^^^^^^^^^^^^^^^^^^^^^^^

Now that you've added this repository, you're going to want to add it to your
clients.

The best way to do this is to make it part of your site profile. You **can**
make it part of your module, but you will need to wrap it in a Defined Type so
that the server parameter can be modified.

To add it to your clients, use the puppet ``yumrepo`` Type. You can find more
information in the `Puppet Type Reference`_.

The following is a basic ``yumrepo`` example:

.. code-block:: ruby

  yumrepo { example:
    baseurl         => "http://your.server.fqdn/yum/foo",
    enabled         => 1,
    enablegroups    => 0,
    gpgcheck        => 0,
    keepalive       => 0,
    metadata_expire => 3600
  }


Session auditing
----------------

By default, a SIMP system uses :term:`Sudosh` to enable logging of sudo
sessions to ``Rsyslog``.

To open a ``sudo`` session from a regular user to ``root``, you should type
``sudo sudosh``.

``sudosh`` logs are stored in ``/var/log/sudosh.log``. Sessions can be replayed
by typing ``sudosh-syslog-replay``.

.. NOTE::

   The SIMP system does not allow the ``root`` user to execute ``sudo`` by
   default per common configuration guidance.

.. NOTE::

   If you built your system from an ISO, you will probably have a local
   ``simp`` user that has the ability to run ``sudo su - root`` directly and
   bypass ``sudosh``.

   This is meant as an emergency 'break glass' user and should be removed or
   disabled once your environment is configured to your satisfaction.


User Accounts
-------------

The SIMP team tests both local and :term:`LDAP` account access to systems.
Other modes of access may function but are not tested by the SIMP test suite at
this time.

We recommend that LDAP be used for adding all human users so that there is no
conflict with multiple system updates and synchronization.  For more
information on managing LDAP users, refer to the :ref:`User_Management`
chapter.

If you need to create local system accounts, you can use the ``user`` and
``group`` Native Types.

.. _Certificate Management:

Certificate Management
----------------------

This section describes the two different types of certificates used in a SIMP
system and how to manage them. For information on initial certificate setup,
refer to the :ref:`Certificates` section of :ref:`Client_Management`.


Infrastructure Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Server certificates are the standard :term:`PKI` certificates assigned either
by an official :term:`CA` (preferred) or generated using the FakeCA utility
offered by SIMP. Generated certificates are placed in the ``/etc/pki/simp``
directory of all managed systems.  These certificates are set to expire
annually. To change this, edit the following files with the number of days for
the desired lifespan of the certificates:

.. NOTE::

    This assumes that the user has generated Certificates with the FakeCA
    provided by SIMP. If official certificates are being used, these settings
    **must be changed within the official CA, not on the SIMP system**.

-  ``/var/simp/environments/simp/FakeCA/CA``

-  ``/var/simp/environments/simp/FakeCA/ca.cnf``

-  ``/var/simp/environments/simp/FakeCA/default\_altnames.cnf``

-  ``/var/simp/environments/simp/FakeCA/default.cnf``

-  ``/var/simp/environments/simp/FakeCA/user.cnf``

In addition, any certificates that have already been created and signed will
have a config file containing all of its details in
``/var/simp/environments/simp/FakeCA/output/conf/``.

.. IMPORTANT::

    Editing any entries in the above mentioned config files will **not** affect
    existing certificates. Existing certificates must be regenerated if you need
    to make changes.

The following is an example of how to change the expiration time from one year
(the default) to five years for any newly created certificate.

.. code-block:: bash

   for file in $(grep -rl 365 /var/simp/environments/simp/FakeCA/)
   do
      sed -i 's/365/1825/' $file
   done


Puppet Certificates
^^^^^^^^^^^^^^^^^^^

Puppet certificates are issued and maintained strictly within Puppet.  They are
different from the server certificates and should be managed with the
``puppet cert`` utility.

For documentation on the ``puppet cert`` tool, visit the `Puppet Inc. cert manual`_.

You can find the location for the Puppet certificates on your system by running
``puppet config print ssldir``.

.. NOTE::

   By default, Puppet certificates expire every five (5) years.


The SIMP Utility
----------------

The SIMP server provides a command line utility called ``simp`` that is an
interface into SIMP-specific settings and subsystems.

You can get information on the ``simp`` utility by running ``simp help`` on
your SIMP server.

.. _simp passgen:

simp passgen
^^^^^^^^^^^^

Throughout the SIMP codebase, you may find references to the ``passgen()``
function. This function auto-generates passwords and stores them in
``/opt/puppetlabs/server/data/puppetserver/simp/environments/<environment>/simp_autofiles/gen_passwd``
on the Puppet server.

For more information, see the `passgen()`_ documentation.

GUI
---

SIMP was designed as a minimized system, but you may occasionally need a GUI.
Refer to the :ref:`Graphical Desktop Setup` documentation for information on
setting up GUIs for the systems.

.. _Puppet Environments: https://docs.puppet.com/puppet/latest/environments.html
.. _Puppet Inc. cert manual: https://docs.puppet.com/puppet/latest/man/cert.html
.. _Puppet Type Reference: https://docs.puppet.com/puppet/latest/type.html
.. _Red Hat local repository Documentation: https://access.redhat.com/solutions/9892
.. _passgen(): https://github.com/simp/pupmod-simp-simplib/blob/master/lib/puppet/parser/functions/passgen.rb

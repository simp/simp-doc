.. _ug-sa-ga-nightly-updates:

Nightly Updates
---------------

All SIMP systems are configured, by default, to do a YUM update of the entire
system on a nightly basis. When the update task runs, it will pull **ALL**
updates that the system is aware of.

.. NOTE::

    Refer to :ref:`howto-exclude-yum-repos` for additional configuration
    information.

.. NOTE::

   See :ref:`howto-disable-nightly-update-schedule` for information on
   changing the nightly update schedule or disabling the nightly updates
   altogether.

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
Puppet master, is ``/var/www/yum/<OSTYPE>/<MAJORRELEASE>/x86_64``.

An ``Updates`` repository has been configured in this space. All OS updates
should be placed within this directory.

You should run the following in the ``Updates`` directory after **ANY** package
addition or removal within that directory.

.. code-block:: bash

   # createrepo .
   # chown -R root.apache ./*
   # find . -type f -exec chmod 640 {} \;
   # find . -type d -exec chmod 750 {} \;


Adding a Custom Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^

For this section, we will assume that you have a repository named ``foo`` that
you would like to expose to your systems. To do this, perform the following:

   .. code-block:: bash

       # cd /var/www/yum
       # mkdir foo
       # cd foo
       # -- copy all RPMs into the folder
       # createrepo .
       # chown -R root.apache ./*
       # find . -type f -exec chmod 640 {} \;
       # find . -type d -exec chmod 750 {} \;

.. NOTE::

   For more information on managing YUM repos, please see the
   `Red Hat local repository Documentation`_.


Configuring the Clients
^^^^^^^^^^^^^^^^^^^^^^^

Now that you have added this repository, you are going to want to add it to your
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

.. _Puppet Type Reference: https://docs.puppet.com/puppet/latest/type.html
.. _Red Hat local repository Documentation: https://access.redhat.com/solutions/9892

.. _sssd_local_user_management:

Managing SSSD LOCAL Domain and Users
====================================

Though the SIMP team **highly recommends** using :ref:`LDAP <Managing LDAP Users>`
to centrally manage your users, you may wish to create users within the SSSD
LOCAL provider domain.  Note that you can run LOCAL and LDAP domains
concurrently!

This section walks you through doing this in a way that is compatible with
SIMP.

The following examples assume that you are using the ``site`` module to set up
your users. The examples may easily be extrapolated into defined types if you
wish but are presented as classes for simplicity.

SSSD LOCAL Domain
-----------------

Set up a LOCAL domain in SSSD. If one already exists in /etc/sssd/sssd.conf,
you can optionally skip this step.  If the LOCAL domain is not managed with
SIMP, you may experience difficulties.

.. code-block:: ruby

  class site::sssd_local {

    sssd::provider::local { 'LOCAL': }

    sssd::domain { 'LOCAL':
      description   => 'Default Local Domain',
      id_provider   => 'local',
      auth_provider => 'local'
    }
  }

In ``default.yaml``:

.. code-block:: yaml

  classes:
    - 'site::sssd_local'

In :term:`Hiera`, you will need to add the LOCAL sssd domain to
``sssd::domains`` if it does not already exist.  If you wish to include the
LOCAL domain in all of ``$simp_options::trusted_nets``, simply add ``sssd::domains`` variable
to ``default.yaml``, copy existing domains from ``simp_config_settings.yaml``
and add ``local`` to the list of domain ``id_providers``.

In ``default.yaml``:

.. code-block:: yaml

  sssd::domains:
    - 'LOCAL'
    - <existing domains, ex. LDAP>

Run ``puppet``. A LOCAL domain should be created and referenced in
``/etc/sssd/sssd.conf``.  The sssd service should be running.

Adding an SSSD Local User
-------------------------

Create a local user, using ``sss_useradd``.  See the ``sss_useradd`` man page
for more options.

.. code-block:: shell

  sss_useradd <user> -h </path/to/home/dir> -u <uid> -m -k /etc/skel


.. NOTE:
  There is a bug in :term:`EL` 6 which does not allow sssd to modify
  ``/etc/passwd``.

To update an EL6 system, perform the following step

.. code-block:: shell

  vipw
  <user>:x:<uid>:<gid>::</path/to/home/dir>:/bin/bash

Next, set the user's password.  As root, run:

.. code-block:: shell

  passwd <user>

Giving the User Access
----------------------

.. code-block:: ruby

  pam::access::rule { '<user> access':
    permission => '+',
    users      => ['<user>'],
    origins    => ['ALL'],
    order      => 1000
  }

  sudo::user_specification { '<user> privs':
    user_list => ["<user>"],
    host_list => [$::fqdn],
    runas     => 'root',
    cmnd      => ['/bin/cat /var/log/app.log'],
    passwd    => false
  }

You are done! You should be able to ``id <user>``, ``su - <user>``, and run
commands allowed by sudo rules.

Test authentication by ssh-ing as the ``user`` onto the host machine, with the
password specified after user creation.  If you want to set up a ssh key,
you may want to follow the relevant `GitHub documentation`_.

.. _GitHub documentation: https://help.github.com/articles/generating-ssh-keys

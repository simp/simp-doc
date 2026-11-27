.. _sssd_local_user_management:

Managing local users and groups with SSSD
=========================================

Though the SIMP team **highly recommends** using :ref:`LDAP <Managing LDAP Users>`
to centrally manage your users, you may also wish to manage users via the local
system.

.. NOTE::

   Prior to SSSD 1.16, there was a LOCAL provider. It is deprecated on
   :term:`EL` 8 and should no longer be used. The SIMP ``sssd`` module no
   longer manages it.


This section walks you through setting up local user and group support using the
SIMP ``sssd`` module.

The following examples assume that you are using the ``site`` module to set up
your users. The examples are easily extrapolated into defined types but are
presented as classes for simplicity.

The Simple Method
-----------------

If you just want SSSD to pull from :file:`/etc/passwd` and :file:`/etc/group` then you
just need to set the following in :term:`Hiera`:

.. code-block:: yaml

   ---
   sssd::enable_files_domain: true

SSSD 2.10, on EL10, removed the implicit files domain and this option. On
EL10, declare a ``LOCAL`` proxy domain for the local files instead, and list
it in ``sssd::domains_entries``:

.. code-block:: yaml

   ---
   sssd::config::manage_base_domain: true
   sssd::domains_entries:
     LOCAL: {}

Using Alternate Files
---------------------

If you want to use your own files, as documented in the :program:`sssd-files(5)` man
page, then you will need to set up an explicit domain with the correct settings.

To do this, use the following puppet code.

.. WARNING::

   This example uses the ``files`` ID provider, which relies on the files
   domain that SSSD 2.10 removed. It has not been tested on EL10.

.. IMPORTANT::

   The module will **not** manage the target files for you. You must ensure that
   the files have the correct content and exist prior to restarting SSSD.


.. code-block:: ruby

   class site::sssd_local {

     sssd::provider::files { 'local':
       passwd_files => ['/usr/local/etc/passwd'],
       group_files  => ['/usr/local/etc/group']
     }

     sssd::domain { 'local':
       description   => 'Default Local Domain',
       id_provider   => 'files',
     }
   }

In ``default.yaml``:

.. code-block:: yaml

   simp::classes:
     - 'site::sssd_local'

In :term:`Hiera`, you will need to add the :code:`local` :code:`sssd` domain to
:code:`sssd::domains` if it does not already exist.

If you wish to include the domain in all of :code:`$simp_options::trusted_nets`, add
:code:`sssd::domains` variable to :file:`default.yaml`, copy existing domains from
:file:`simp_config_settings.yaml` and add :code:`local` to the list of domain :code:`id_providers`.

In :file:`default.yaml`:

.. code-block:: yaml

   sssd::domains:
     - 'local'
     - <existing domains, ex. LDAP>

Run :program:`puppet`.

A :code:`local` domain should be created and referenced in :file:`/etc/sssd/sssd.conf` and the
:program:`sssd` service should be running.

Additional Resources
====================

If you have any issues logging in, you may want to see the
:ref:`Troubleshooting` section of the documentation.

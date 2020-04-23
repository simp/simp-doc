.. _sssd_local_user_management:

Managing local users and groups with SSSD
=========================================

Though the SIMP team **highly recommends** using :ref:`LDAP <Managing LDAP Users>`
to centrally manage your users, you may also wish to manage users via the local
system.

.. NOTE::

   Prior to SSSD 1.16 (EL7+), there was a LOCAL provider. This can still be used
   on EL6 systems but has been fully deprecated as of EL8 and is not recommended
   for use by the vendor.

   If you still need to use this capability, please see the documentation for
   sssd::provider::local directly.


This section walks you through setting up local user and group support using the
SIMP ``sssd`` module.

The following examples assume that you are using the ``site`` module to set up
your users. The examples are easily extrapolated into defined types but are
presented as classes for simplicity.

The Simple Method
-----------------

If you just want SSSD to pull from ``/etc/passwd`` and ``/etc/group`` then you
just need to set the following in :term:`Hiera`:

.. code-block:: yaml

   ---
   sssd::enable_files_domain: true

Using Alternate Files
---------------------

If you want to use your own files, as documented in the `sssd-files(5)` man
page, then you will need to set up an explicit domain with the correct settings.

To do this, use the following puppet code.

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

In :term:`Hiera`, you will need to add the ``local`` ``sssd`` domain to
``sssd::domains`` if it does not already exist.  If you wish to include the
domain in all of ``$simp_options::trusted_nets``, simply add
``sssd::domains`` variable to ``default.yaml``, copy existing domains from
``simp_config_settings.yaml`` and add ``local`` to the list of domain
``id_providers``.

In ``default.yaml``:

.. code-block:: yaml

   sssd::domains:
     - 'local'
     - <existing domains, ex. LDAP>

Run ``puppet``. A ``local`` domain should be created and referenced in
``/etc/sssd/sssd.conf`` and the ``sssd`` service should be running.

Additional Resources
====================

If you have any issues logging in, you may want to see the
:ref:`Troubleshooting` section of the documentation.


.. _grub_user_management:

Managing GRUB Users
===================

In addition to being configured at initialization through the SIMP CLI or
kickstarting via PXE boot, GRUB users can be managed by the
``simp-simp_grub`` module on both GRUB 2 and legacy GRUB systems.

.. NOTE::

   ``simp-simp_grub`` is not a core module and its corresponding package,
   ``pupmod-simp-simp_grub``, may need to be installed prior to following
   this guide.

In :term:`Hiera`, you will need to include the ``simp_grub`` class and set
the appropriate parameters. For both GRUB 2 and legacy GRUB, this must
include:

.. code-block:: yaml

  simp::classes:
    - 'simp_grub'

  simp_grub::password: '<password hash or password>'

For GRUB 2, you will also have to set ``simp_grub::admin`` and can
optionally specify whether unmanaged GRUB user accounts should be
reported and/or purged and the number of rounds to use for hashing
the password.

.. code-block:: yaml

  # required for GRUB 2
  simp_grub::admin: '<username>'

  # optional for GRUB 2
  simp_grub::purge_unmanaged_users: <true or false>
  simp_grub::report_unmanaged_users: <true or false>
  simp_grub::hash_rounds: <integer>

  
After configuring Hiera, run ``puppet``. The accounts and hashed passwords
should be included in the ``/etc/grub2.cfg`` or ``/etc/grub2-efi.cfg`` files
for GRUB 2 systems and in ``/etc/grub.conf`` on legacy GRUB systems.

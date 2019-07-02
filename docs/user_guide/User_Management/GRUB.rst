.. _grub_user_management:

Managing GRUB Users
===================

In addition to being configured at initialization through the SIMP CLI or
kickstarting via PXE boot, GRUB users can be managed by the
``simp-simp_grub`` module on both GRUB 2 and legacy GRUB systems.

In :term:`Hiera`, you will need to include the ``simp_grub`` class and set
the appropriate parameters. For both GRUB 2 and legacy GRUB, this must
include:

.. code-block:: yaml

  classes:
    - 'simp_grub'

  simp_grub::password: '<password hash or password>'

Legacy GRUB can accept password hashes, beginning with $1$, $5$ or $6$, or
a plain password which will be converted. GRUB 2 only accepts plain passwords,
and also requires that an administrative username be included in the Hiera.
Additionally, GRUB 2 can accept optional parameters specifying whether
unmanaged GRUB user accounts should be reported and/or purged, and the number
of rounds to use hashing the password.

.. code-block:: yaml

  simp_grub::admin: '<username>'
  simp_grub::purge_unmanaged_users: <true or false>
  simp_grub::report_unmanaged_users: <true or false>
  simp_grub::hash_rounds: <integer>
  
After configuring Hiera, run ``puppet``. The accounts and hashed passwords
should be included in the ``/etc/grub2.cfg`` or ``/etc/grub2-efi.cfg`` files
for GRUB 2 systems and in ``/etc/grub.conf`` on legacy GRUB systems.

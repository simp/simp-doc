HOWTO Enable Core Dumps
=======================

The ``simp::sysctl`` class can restrict system :term:`core dump` capabilities
for improved system security.

As of ``simp-simp`` 11.0.0, a bare ``include simp::sysctl`` does not manage
core dumps. Core dumps are restricted only when
``simp::sysctl::core_dumps`` is ``false`` and ``simp_options::pam`` is
``true``. ``core_dumps`` is set to ``false`` either directly in :term:`Hiera`
or by enforcing the ``simp:defaults`` compliance profile (see
:ref:`SIMP Compliance Engine`):

.. code-block:: yaml

   ---
   # Use compliance_markup::enforcement instead if your Hiera configuration
   # uses the older compliance_markup backend
   compliance_engine::enforcement:
     - simp:defaults

In that case, ``simp::sysctl`` adds a ``prevent_core`` :term:`PAM` limits rule
(``* hard core 0``).

At times, users may need to re-enable core dumps for system troubleshooting.

Enabling Core Dumps at the System Level
---------------------------------------

Overall system core dumps can be enabled by setting the following in
:term:`Hiera`:

.. code-block:: yaml

   ---
   # Enable system core dumps
   simp::sysctl::core_dumps: true

   # Set the core dump output directory
   simp::sysctl::core_dump_dir: /fully/qualified/path

   # Write core dumps into that directory
   simp::sysctl::kernel__core_pattern: '/fully/qualified/path/%u_%g_%p_%t_%h_%e.core'

``simp::sysctl::core_dump_dir`` only creates the directory (mode ``0640``). The
kernel writes core dumps to the location set by ``kernel.core_pattern``, so
``simp::sysctl::kernel__core_pattern`` must point inside ``core_dump_dir``.

This will also disable enforcement of core dump restrictions in :term:`PAM`:
with ``core_dumps: true``, no ``prevent_core`` rule is declared. On nodes where
``pam::limits`` manages ``/etc/security/limits.conf`` (for example, the
``simp``, ``simp_lite`` and ``one_shot`` scenarios), that file is rebuilt from
the rules in the catalog, so an existing ``prevent_core`` line is removed.

Preventing Core Dumps via PAM
-----------------------------

If you decide to enable core dumps, you may want to still restrict them for
users on your system.

To do this, you will need to add the following type of :term:`puppet` code.

.. code-block:: puppet

   pam::limits::rule { 'prevent_core_dumps_all':
     # Add to all PAM domains
     domains => ['*'],
     # Set both hard and soft limits
     type    => 'hard',
     # Affect core dumps
     item    => 'core',
     # Set to '0'
     value   => 0,
     # Set at 99 in the order list (first match wins)
     order   => 99
   }

Now, if you want to enable core dumps for the `root` user, you will want to add
the following as well:

.. code-block:: puppet

   pam::limits::rule { 'allow_root_core_dump':
     domains => ['root'],
     type    => 'hard',
     item    => 'core',
     value   => 1,
     order   => 10
   }

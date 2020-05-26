HOWTO Enable Core Dumps
=======================

SIMP disables system :term:`core dump` capabilities by default for improved
system security.

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

This will also disable enforcement of core dump restrictions in :term:`PAM`.

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

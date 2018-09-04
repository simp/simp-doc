Baseline Configuration
----------------------

SIMP uses ``crond`` to schedule a number of jobs that help keep systems in a
consistent and known baseline.  The ``simp`` module ensures that the ``cron``
daemon is installed and running on all systems.

Specifically, the :term:`puppet` agent is run via ``cron`` as is ``aide`` and a
small number of maintenance tasks.

References: :ref:`CM-2 (1)`

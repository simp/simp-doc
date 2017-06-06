Access Enforcement
------------------

SIMP uses a combination of discretionary and mandatory access control
configurations to protect the operating system and the applications installed.
Both forms of access control are built upon a model where a subject's (user or
process) access to an object is controlled by the underlying operating system.

System Access
^^^^^^^^^^^^^

The ``simp::admin`` class provides a default set of accesses to SIMP systems.

By default, the ``administrators`` group may access the system and may gain
access to the ``root`` account via passwordless ``sudo``. Passwordless ``sudo``
was chosen since many systems run without passwords and only key access. This
may be changed by setting the appropriate ``Boolean`` in the class.

These users may access the system from any location via ``ssh``.

Users in the ``security`` group may access all systems from the
``simp_options::trusted_nets`` setting and are restricted to privileged use of
a specific set of auditing-related commands which have been selected to
disallow escalation of privileges.

Mountpoint Control
^^^^^^^^^^^^^^^^^^

The ``simp::mountpoints`` class puts some specific access control
configurations in place.  The ``/tmp``, ``/var/tmp``, and ``/dev/shm``
directories have ``nodev``, ``noexec``, and ``nosuid`` set to prevent users
from misusing the systems global read/write directories.

Additionally, ``/sys`` is mounted with ``nodev`` and ``noexec``.

References: :ref:`AC-3`

Access Enforcement
------------------

SIMP uses a combination of discretionary and mandatory access control
configurations to protect the operating system and the applications installed.
Both forms of access control are built upon a model where a subject's (user or
process) access to an object is controlled by the underlying operating system.

SIMPLib puts some specific access control configurations in place.  The ``/tmp`` and
``/var/tmp`` directories have nodev, noexec, and nosuid set to prevent users from
misusing the systems global read/write directories.

References: :ref:`AC-3`

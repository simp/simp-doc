Discretionary Access Control
----------------------------

SIMP uses the implementation of Discretionary Access Control (DAC) that is
native to Linux. Specific file permissions have been assigned based on published
security guidance for Red Hat, CentOS, and UNIX.

To ensure default permissions are as restrictive as possible, the user's umask
is set to 0077 while the daemon umask is set to 0027.

References: :ref:`AC-3 (4)`

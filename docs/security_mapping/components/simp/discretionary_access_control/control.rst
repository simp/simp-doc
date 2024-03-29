Discretionary Access Control
----------------------------

SIMP uses the implementation of Discretionary Access Control (DAC) that is
native to Linux. Specific file permissions have been assigned based on
published security guidance for the supported operating in :ref:`changelog-latest`.

To ensure default permissions are as restrictive as possible, the user's
:term:`umask` is set to 0077 while the daemon umask is set to 0027.

References: :ref:`AC-3 (4)`

Least Functionality
-------------------

The SIMP stunnel service is configured to run within a chroot jail.  This ensures
that the service cannot see or access files outside of stunnel directory. Should
the stunnel service become remotely compromised, the attack cannot be escalated to
other parts of the file system.

References: :ref:`CM-7`

Privileged Accounts
--------------------

Linux historically uses the wheel group to as an administrators group.  SIMP
makes use of the sudoers file with more granular group permissions.  The PAM
module enforces that only the root user is in the wheel group.

References: :ref:`AC-6 (5)`

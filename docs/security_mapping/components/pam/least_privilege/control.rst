Least Privilege
---------------

SIMP uses the access conf file to identify which accounts can login to a system.
After all other identification and authentication checks have passed, the pam
access.conf file is checked to ensure the user is allowed to login.  SIMP
allows ``root`` and the ``adminstrators`` group to login to all systems and the
``simp`` user to login to the puppet master.  All other users must be explicitly
added to the access.conf file using the SIMP pam module.

References: :ref:`AC-6`

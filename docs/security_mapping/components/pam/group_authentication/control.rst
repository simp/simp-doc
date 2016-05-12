Group Authentication
----------------------

SIMP does not use group accounts for authenticators.  Instead, users are added
to a group.  In the case of the ``administrators`` group, a user first
authenticates to their account, and then escalates to ``root`` using sudo.

References: :ref:`IA-2 (5)`

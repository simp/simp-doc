Account Management
-------------------

SIMP makes several account management decisions that are part of the overall
account management strategy.  One of those cases is the use of passwordless sudo
for any user in the ``administrators`` or ``auditors`` groups. This is on by
default due to the expected use of SSH keys and lack of local passwords.

References: :ref:`AC-2`, :ref:`AC-6 (1)`

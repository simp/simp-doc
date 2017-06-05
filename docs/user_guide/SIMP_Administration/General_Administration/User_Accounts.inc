User Accounts
-------------

The SIMP team tests both local and :term:`LDAP` account access to systems.
Other modes of access may function but are not tested by the SIMP test suite at
this time.

We recommend that LDAP be used for adding all human users so that there is no
conflict with multiple system updates and synchronization.  For more
information on managing LDAP users, refer to the :ref:`User_Management`
chapter.

If you need to create local system accounts, you can use the ``user`` and
``group`` Native Types.

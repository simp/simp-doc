Session Audit
-------------

The ``sudosh`` tool is installed on each SIMP node.  It` logs the terminal
output of user's terminal session, which is written to the log file
``/var/log/sudosh/log``.  Another utility, ``sudosh-replay`` can be used to
replay the session.

The PAM module ``pam_tty_audit`` is used to record keystrokes during a ``root``
user's session.  Additional accounts can be audited by adding them to the
parameter ``pam::tty_audit_users``,

.. NOTE::
   As a safeguard against recording sensitive credentials (such as passwords),
   both ``sudosh`` and ``pam_tty_audit`` do NOT record when ``echo`` is turned off.

.. WARNING::
   The audit logs **WILL RECORD SENSITIVE DETAILS** (such as passwords) for any
   scripts or applications that:

   * Do _not_ protect terminal output while entering or echoing sensitive data
   * AND are run by an audited user (e.g., ``root``)

   It is therefore HIGHLY RECOMMENDED to update any such scripts or
   applications to turn of echo during these sensitive operations.


References: :ref:`AU-14`

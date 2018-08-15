Session Audit
-------------

The :term:`Tlog` application is installed on each SIMP node. It is set, by
default, to log interactive shell sessions to privileged user accounts via a
login shell hook.

The ``tlog-rec-session`` application may optionally be set as the user's
default shell to log all sessions without the optional hook.

A ``tlog-play`` application is also provided to replay captured sessions.

In addition to :term:`Tlog`, the :term:`PAM` module ``pam_tty_audit`` is used
to record keystrokes during a ``root`` user's session.  Additional accounts can
be audited by adding them to the parameter ``pam::tty_audit_users``.

.. NOTE::
   As a safeguard against recording sensitive credentials (such as passwords),
   both ``tlog`` and ``pam_tty_audit`` do NOT record when ``echo`` is turned off.

.. WARNING::
   The audit logs **WILL RECORD SENSITIVE DETAILS** (such as passwords) for any
   scripts or applications that:

     * Do _not_ protect terminal output while entering or echoing sensitive data
     * AND are run by an audited user (e.g., ``root``)

   It is therefore HIGHLY RECOMMENDED to update any such scripts or
   applications to turn of echo during these sensitive operations.

References: :ref:`AU-14`

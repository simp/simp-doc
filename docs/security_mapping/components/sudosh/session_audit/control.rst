Session Audit
-------------

The sudosh tool is installed on each SIMP node.  Sudosh is shell that logs the
user's keystrokes.  The keystrokes are written to a log file
``/var/log/sudosh/log``.  Another utility, ``sudosh-replay`` is used to replay
the keystrokes of a session.

References: :ref:`AU-14`

Session Lock
-------------

Sessions do not "lock".  Instead, when there is a shell open and idle for 15
minutes, the session will timeout.  This applies only when the shell is not
running a command/process.  Once the session is terminated, the user must
reestablish the shell via console or SSH.

References: :ref:`AC-11a.`,  :ref:`AC-11b.`

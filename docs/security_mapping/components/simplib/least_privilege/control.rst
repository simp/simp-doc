Least Privilege
---------------

SIMP utilizes the cron daemon's access control by implementing the cron.allow
feature.  Only users in the cron.allow file are allowed to schedule cron jobs.
Only the ``root`` user is in that file.  The cron.deny file is forced to be absent,
therefore all other users are denied the ability to schedule jobs.

The AT and incron services have the same access control configuration setup.
Only the ``root`` user can schedule jobs and all other users are denied.

References: :ref:`AC-6`

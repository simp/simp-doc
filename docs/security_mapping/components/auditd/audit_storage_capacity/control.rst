Audit Storage Capacity
----------------------

To help manage the amount of local storage occupied by audit logs, the following
rules are applied:

- A maximum of 5 log files are retained.  The oldest is removed when the logs are rotated.
- Files can reach a maximum of 24 Mb before being rotated.

References: :ref:`AU-4`

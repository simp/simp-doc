Audit Storage Capacity
----------------------

When a SIMP client serves as syslog server, logrotate is used
to help manage storage capacity.  The following log rotate rules are
applied:

- Logs are rotated weekly
- A maximum of 12 rotated logs are stored

References: :ref:`AU-4`

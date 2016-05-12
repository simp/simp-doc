Response To Audit Processing Failures - Audit Storage Capacity
--------------------------------------------------------------

Auditd has been configured to handle audit failures or potential failures due to
storage capacity.  Those settings include:

- Send a warning to syslog when there is less than 75Mb of space on the audit partition (space_left).
- Suspend the audit daemon when there is less than 50Mb of space left on the audit partition (admin_space_left).

References: :ref:`AU-5 (1)`

Audit Generation
-----------------

SIMP enables auditd on all systems.
Auditd is the userspace component to the Linux Auditing System. It's responsible
for writing audit records to the disk. Viewing the logs is done with the
ausearch or aureport utilities. Configuring the audit rules is done with the
auditctl utility. During startup, the rules in /etc/audit/audit.rules are read
by auditctl.

The audit daemon is configured to initiate auditing at boot time.

References: :ref:`AU-12`, :ref:`AU-12a.`, :ref:`AU-12c.`

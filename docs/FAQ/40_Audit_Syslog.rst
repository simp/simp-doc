.. _faq-audit-syslog:

Why aren't audit logs being forwarded to syslog?
================================================

Audit logs can be sent to syslog in addition to being persisted locally in :file:`/var/log/audit`.
By default, SIMP disables forwarding of audit logs due to the excessive size of the collected logs.

When audit logs are sent to remote syslog servers, the logs can quickly overwhelm the underlying network.

If forwarding of audit logs via syslog is appropriate for your site, you can enable that forwarding
by setting the following in :term:`hiera`:

.. code-block:: yaml

   auditd::config::audisp::syslog::drop_audit_logs: false

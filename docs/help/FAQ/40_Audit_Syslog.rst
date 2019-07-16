.. _faq-audit-syslog:

Why aren't audit logs being forwarded to syslog?
================================================

Audit logs can be sent to syslog in addition to being persisted
locally in ``/var/log/audit``.  However, SIMP disables forwarding
of audit logs to syslog, by default, because the logs are voluminous.
When these logs are sent to one or more remote syslog servers, the
logs can easily overwhelm the underlying network.

If forwarding of audit logs via syslog is appropriate for your site,
you can enable that forwarding by setting the following in :term:`hiera`:

.. code-block:: yaml

   auditd::config::audisp::syslog::drop_audit_logs: false

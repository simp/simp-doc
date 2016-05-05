Response to Audit Processing Failures
-------------------------------------

The auditing dispatcher is system that allows external applications to
access and make use of the auditd daemon in real time. When the internal queue
of the audit dispatcher is full, a message is sent to syslog.

References: :ref:`AU-5`

Centralized Management of Planned Audit Record Content
------------------------------------------------------

SIMP centrally controls what audit events are recorded on the clients.  The
SIMP module controls which of the those events are sent to local :term:`syslog`
daemon so that they may be optionally forwarded to a central syslog server. The
following orthogonal list contains the conditions to be met for the SIMP logs
to be sent to syslog.

- $programname == 'tlog-rec-session'
- $programname == 'tlog'
- $programname =='yum'
- $syslogfacility-text == 'cron'
- $syslogfacility-text == 'authpriv'
- $syslogfacility-text == 'local5'
- $syslogfacility-text == 'local6
- $syslogfacility-text == 'local7'
- $syslogpriority-text == 'emerg'
- $syslogfacility-text == 'kern' and $msg startswith 'IPT:'

SIMP also has a stock :term:`rsyslog` module which is able to configure an
``rsyslog`` server for centralized collection. The stock ``rsyslog`` server
configures the ``rsyslog`` daemon to accept logs from SIMP clients and places
them in ``/var/log/hosts/``. The following files are created for each host in
that directory:

- tlog.log
- httpd.log
- dhcpd.log
- puppet-agent-err.log
- puppet-agent.log
- puppet-master.log
- audit.log
- slapd.log
- iptables.log
- secure.log
- messages.log
- maillog.log
- cron.log
- spooler.log
- boot.log

References: :ref:`AU-3 (2)`, :ref:`AU-13 (2)`, :ref:`AU-6 (4)`

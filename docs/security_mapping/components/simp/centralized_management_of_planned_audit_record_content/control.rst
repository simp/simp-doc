Centralized Management of Planned Audit Record Content
------------------------------------------------------

SIMP centrally controls what audit events are recorded on the clients.  The SIMP
module controls which of the those events are sent to local syslog daemon so that they may be
forwarded to a central syslog server. The following list contains the conditions
to be met for the SIMP logs to be sent to syslog.

- $programname == 'sudosh'
- $programname =='yum'
- $syslogfacility-text == 'cron'
- $syslogfacility-text == 'authpriv'
- $syslogfacility-text == 'local5'
- $syslogfacility-text == 'local6
- $syslogfacility-text == 'local7'
- $syslogpriority-text == 'emerg'
- $syslogfacility-text == 'kern' and $msg startswith 'IPT:'

SIMP also has a stock ryslog module that exists within the SIMP module. The
stock rsylsog server configures the rsylog daemon to accept logs from SIMP
clients and places them in ``/var/log/hosts/``. The following files are
recreated in that directory:

- sudosh.log
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

Audit Events
------------

The following puppet files are added to the audit rules so that modifications to
them are audited by auditd.

- ``-a always,exit -F dir=${confdir} -F uid!=puppet -p wa -k Puppet_Config``
- ``-a always,exit -F dir=${logdir} -F uid!=puppet -p wa -k Puppet_Log``
- ``-a always,exit -F dir=${rundir} -F uid!=puppet -p wa -k Puppet_Run``
- ``-a always,exit -F dir=${ssldir} -F uid!=puppet -p wa -k Puppet_SSL``

References: :ref:`AU-2`

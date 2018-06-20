The file permissions, ownership, and group membership of system files and commands must match the vendor values
---------------------------------------------------------------------------------------------------------------

The following files have permissions that differ from the vendor supplied values:

* /etc/ntp.conf (from '0644' to '0600')
* /etc/sysconfig/ntpd (from '0644' to '0640')
* /etc/sysconfig/ntpdate (from '0644' to '0640')
* /var/lib/ntp (from '0755' to '0750')

The following files have group memberships that differ from the vendor supplied
values:

* /etc/ntp.conf (from 'root' to 'ntp')


The permissions put into place by SIMP are stronger than the default. The ntp
service runs as the ntp user, further strengthening the permissions.


References: `V-71849 <http://rhel7stig.readthedocs.io/en/latest/high.html#v-71849-the-file-permissions-ownership-and-group-membership-of-system-files-and-commands-must-match-the-vendor-values-rhel-07-010010>`_

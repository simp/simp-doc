The file permissions, ownership, and group membership of system files and commands must match the vendor values
---------------------------------------------------------------------------------------------------------------

The following files have permissions that differ from the vendor supplied values:

* /etc/default/nss (from '0644' to '0600')
* /etc/default/useradd (from '0644' to '0600')
* /etc/login.defs (from '0640' to '0640')
* /etc/securetty (from '0600' to '0400')

The permissions put into place by SIMP are stronger than the default.

References: `V-71849 <http://rhel7stig.readthedocs.io/en/latest/high.html#v-71849-the-file-permissions-ownership-and-group-membership-of-system-files-and-commands-must-match-the-vendor-values-rhel-07-010010>`_

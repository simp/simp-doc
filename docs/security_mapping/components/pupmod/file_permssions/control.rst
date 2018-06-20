The file permissions, ownership, and group membership of system files and commands must match the vendor values
---------------------------------------------------------------------------------------------------------------

The following files have permissions that differ from the vendor supplied values:

* /etc/sysconfig/puppetserver (from '0644' to '0640')
* /etc/puppetlabs/code (from '0755' to '0750')
* /etc/puppetlabs/code/environments (from '0755' to '0770')
* /etc/puppetlabs/code/environments/production (from '0755' to '0770')
* /etc/puppetlabs/puppet (from '0755' to '0750')
* /etc/puppetlabs/puppet/puppet.conf (from '0644' to '0640')
* /etc/puppetlabs/puppetserver/conf.d (from '0755' to '0750')
* /etc/puppetlabs/puppetserver/conf.d/puppetserver.conf (from '0644' to '0640')
* /etc/puppetlabs/puppetserver/conf.d/web-routes.conf (from '0644' to '0640')
* /etc/puppetlabs/puppetserver/conf.d/webserver.conf (from '0644' to '0640')
* /etc/puppetlabs/puppetserver/logback.xml (from '0644' to '0640')
* /etc/puppetlabs/puppetserver/services.d/ca.cfg (from '0644' to '0640')

The following files have group memberships that differ from the vendor supplied
values:

* /etc/sysconfig/puppetserver (from 'root' to 'puppet')
* /etc/puppetlabs/code (from 'root' to 'puppet')
* /etc/puppetlabs/code/environments (from 'root' to 'puppet')
* /etc/puppetlabs/code/environments/production (from 'root' to 'puppet')
* /etc/puppetlabs/puppet (from 'root' to 'puppet')
* /etc/puppetlabs/puppet/puppet.conf (from 'root' to 'puppet')
* /etc/puppetlabs/puppetserver/conf.d (from 'root' to 'puppet')
* /etc/puppetlabs/puppetserver/conf.d/puppetserver.conf (from 'root' to 'puppet')
* /etc/puppetlabs/puppetserver/conf.d/web-routes.conf (from 'root' to 'puppet')
* /etc/puppetlabs/puppetserver/conf.d/webserver.conf (from 'root' to 'puppet')
* /etc/puppetlabs/puppetserver/logback.xml (from 'root' to 'puppet')
* /etc/puppetlabs/puppetserver/services.d/ca.cfg (from 'root' to 'puppet')


The permissions put into place by SIMP are stronger than the default. The
puppetserver service runs as the puppet user, further strengthening the
permissions. Puppet agents still run as root.


References: `V-71849 <http://rhel7stig.readthedocs.io/en/latest/high.html#v-71849-the-file-permissions-ownership-and-group-membership-of-system-files-and-commands-must-match-the-vendor-values-rhel-07-010010>`_

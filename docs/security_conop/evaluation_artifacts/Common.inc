Verify and Correct File Permissions with RPM
============================================

* Rule ID: xccdf_org.ssgproject.content_rule_rpm_verify_permissions
* Type: Mixed - Mostly False Positives
* Recommend SSG Feedback: Permissions that are obviously more restrictive should not be flagged
* Identifier: `V-71849`_

Most files have more restrictive permissions than provided by the RPMs. Some
services, like ``openldap``, run as a service specific system user.


The following exceptions are a combination of running the above command on an EL 6
and EL 7 SIMP system:

.. csv-table::
   :header-rows: 1
   :widths: 10,45,45

   ,File,Puppet log
   ``SM5....T.c``,/etc/audit/auditd.conf,mode changed '0640' to '0600'
   ``.M.......c``,/etc/default/nss,mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/default/useradd,mode changed '0644' to '0600'
   ``SM5....T.c``,/etc/hosts.allow,mode changed '0644' to '0444'
   ``SM5....T.``,/etc/init/control-alt-delete.conf,mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/login.defs,mode changed '0644' to '0640'
   ``.M....G..c``,/etc/ntp.conf,group changed 'root' to 'ntp' ; mode changed '0644' to '0600'
   ``.M....G..c``,/etc/openldap/schema/dyngroup.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/dyngroup.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/inetorgperson.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/inetorgperson.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/java.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/java.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/misc.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/misc.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/nis.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/nis.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/openldap.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/openldap.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/pmi.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/pmi.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/ppolicy.ldif,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..c``,/etc/openldap/schema/ppolicy.schema,group changed 'root' to 'ldap' ; mode changed '0444' to '0644'
   ``.M....G..``,/etc/puppetlabs/code,group changed 'root' to 'puppet' ; mode changed '0755' to '0750'
   ``.M....G..``,/etc/puppetlabs/code/environments,"group changed 'root' to 'puppet' ; mode changed '0755' to '0770' (u=rwx,g=rwx,o-rwx)"
   ``.M....G..``,/etc/puppetlabs/code/environments/production,"group changed 'root' to 'puppet' ; mode changed '0755' to '0770' (u=rwx,g=rwx,o-rwx)"
   ``.M....G..``,/etc/puppetlabs/puppet,group changed 'root' to 'puppet' ; mode changed '0755' to '0750'
   ``SM5...GT.c``,/etc/puppetlabs/puppet/puppet.conf,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``.M....G..n``,/etc/puppetlabs/puppetserver/conf.d,group changed 'root' to 'puppet' ; mode changed '0755' to '0750'
   ``SM5...GT.c``,/etc/puppetlabs/puppetserver/conf.d/puppetserver.conf,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``SM5...GT.c``,/etc/puppetlabs/puppetserver/conf.d/web-routes.conf,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``SM5...GT.c``,/etc/puppetlabs/puppetserver/conf.d/webserver.conf,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``SM5...GT.c``,/etc/puppetlabs/puppetserver/logback.xml,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``SM5...GT.c``,/etc/puppetlabs/puppetserver/services.d/ca.cfg,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/rsyncd.conf,mode changed '0644' to '0400'
   ``SM5....T.c``,/etc/rsyslog.conf,mode changed '0644' to '0600'
   ``SM5....T.c``,/etc/securetty,mode changed '0600' to '0400'
   ``SM5....T.c``,/etc/security/limits.conf,mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/sysconfig/ktune,mode changed '0777' to '0640'
   ``SM5....T.c``,/etc/sysconfig/ntpd,mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/sysconfig/ntpdate,mode changed '0644' to '0640'
   ``SM5...GT.c``,/etc/sysconfig/puppetserver,group changed 'root' to 'puppet' ; mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/sysconfig/rsyslog,mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/sysconfig/slapd,mode changed '0644' to '0640'
   ``SM5....T.c``,/etc/tuned.conf,mode changed '0777' to '0640'
   ``.M.......``,/var/lib/ntp,mode changed '0755' to '0750'


.. _V-71849: https://rhel7stig.readthedocs.io/en/latest/high.html#v-71849-the-file-permissions-ownership-and-group-membership-of-system-files-and-commands-must-match-the-vendor-values-rhel-07-010010

The file permissions, ownership, and group membership of system files and commands must match the vendor values
---------------------------------------------------------------------------------------------------------------

The following files have permissions that differ from the vendor supplied values:

* /etc/sysconfig/slapd (from '0644' to '0640')
* /etc/openldap/schema/dyngroup.ldif (from '0444' to '0644')
* /etc/openldap/schema/dyngroup.schema (from '0444' to '0644')
* /etc/openldap/schema/inetorgperson.ldif (from '0444' to '0644')
* /etc/openldap/schema/inetorgperson.schema (from '0444' to '0644')
* /etc/openldap/schema/java.ldif (from '0444' to '0644')
* /etc/openldap/schema/java.schema (from '0444' to '0644')
* /etc/openldap/schema/misc.ldif (from '0444' to '0644')
* /etc/openldap/schema/misc.schema (from '0444' to '0644')
* /etc/openldap/schema/nis.ldif (from '0444' to '0644')
* /etc/openldap/schema/nis.schema (from '0444' to '0644')
* /etc/openldap/schema/openldap.ldif (from '0444' to '0644')
* /etc/openldap/schema/openldap.schema (from '0444' to '0644')
* /etc/openldap/schema/pmi.ldif (from '0444' to '0644')
* /etc/openldap/schema/pmi.schema (from '0444' to '0644')
* /etc/openldap/schema/ppolicy.ldif (from '0444' to '0644')
* /etc/openldap/schema/ppolicy.schema (from '0444' to '0644')

The following files have group memberships that differ from the vendor supplied
values:

* /etc/openldap/schema/dyngroup.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/dyngroup.schema (from 'root' to 'ldap')
* /etc/openldap/schema/inetorgperson.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/inetorgperson.schema (from 'root' to 'ldap')
* /etc/openldap/schema/java.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/java.schema (from 'root' to 'ldap')
* /etc/openldap/schema/misc.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/misc.schema (from 'root' to 'ldap')
* /etc/openldap/schema/nis.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/nis.schema (from 'root' to 'ldap')
* /etc/openldap/schema/openldap.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/openldap.schema (from 'root' to 'ldap')
* /etc/openldap/schema/pmi.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/pmi.schema (from 'root' to 'ldap')
* /etc/openldap/schema/ppolicy.ldif (from 'root' to 'ldap')
* /etc/openldap/schema/ppolicy.schema (from 'root' to 'ldap')


The permissions put into place by SIMP are stronger than the default. The
openldap service runs as the ldap user, further strengthening the permissions.


References: `V-71849 <http://rhel7stig.readthedocs.io/en/latest/high.html#v-71849-the-file-permissions-ownership-and-group-membership-of-system-files-and-commands-must-match-the-vendor-values-rhel-07-010010>`_

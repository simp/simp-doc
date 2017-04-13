SSG SCAP Scan Against SIMP Server - RHEL 7 Draft STIG
======================================================


The `SCAP Security Guide`_ (SSG) is an open source project that authors
security guidance for RedHat and CentOS.  SSG uses the Security Automation
Content Protocol (`SCAP`_) to automate configuration checks against
profiles.  SSG includes several compliance profiles with their content,
including a DISA STIG profile.  The DISA STIG profile is the ideal target
for most SIMP users.  Therefore, the SIMP team regularly runs that profile
against SIMP versions.

SIMP makes every attempt to adhere to the STIG profile.  The SSG checks
are very strict in nature and therefore SIMP will always fail some of the
tests.  That doesn't mean that SIMP has a reduced security posture.  To
help users understand why we fail the tests, we are including
justifications in our documentation.  Those justifications are included
in this section.

-  RPM: scap-security-guide-0.1.30-1.el7.noarch
-  Data Stream: ``/usr/share/xml/scap/ssg/content/ssg-rhel7-ds.xml``
-  Run against a SIMP Server

--------------

Ensure gpgcheck Enabled for Local Packages
------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_ensure\_gpgcheck\_never\_disabled
-  Type: Finding

Justification
~~~~~~~~~~~~~

| This is not yet managed by SIMP and was not a finding until the RHEL 7
| baseline. Given that the baseline is in Draft form, it has not been
  marked as a
| priority.

--------------

Ensure gpgcheck Enabled for Repository Metadata
-----------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_ensure\_gpgcheck\_repo\_metadata
-  Type: Exception
-  **Requested feedback from the SSG team**

   -  This should not be a High finding if using TLS
   -  This opens potential vulnerabilities to the system

Justification
~~~~~~~~~~~~~

| Unfortunately, the way that YUM works means that all GPG keys become
  *trusted*
| by the entire system. Enabling repository metadata signatures means
  that RPMs
| will be trusted that come from any of these GPG systems and may allow
| inappropriate software to be installed on systems.

--------------

Build and Test AIDE Database
----------------------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_aide\_build\_database
-  Type: False Positive

Notes
~~~~~

.. code:: shell

    ls /var/lib/aide/
    aide.db.gz

--------------

Configure Periodic Execution of AIDE
------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_aide\_periodic\_cron\_checking
-  Type: Alternate Implementation
-  **Requested feedback from the SSG team**

   -  Enabling the aide scan via regular ``cron`` should be valid

Notes
~~~~~

| This is **not** enabled in SIMP by default since it can be an extreme
  burden on
| your system depending on your partitioning.

If you wish to enable this, you can use the following Hiera data:

.. code:: yaml

    ---
    aide::enable: true

| This is implemented using the native ``cron`` Puppet resource and,
  therefore,
| is placed into the root crontab directly.

.. code:: shell

    22 4 * * 0 /bin/nice -n 19 /usr/sbin/aide -C

--------------

Verify and Correct File Permissions with RPM
--------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_rpm\_verify\_permissions
-  Type: Mixed - Mostly False Positives
-  **Requested feedback from the SSG team**

   -  Permissions that are obviously more restrictive should not be flagged

Notes
~~~~~

| Most files have *more restrictive* permissions than the permissions
  provided by
| the RPMs.

Exceptions are noted in the output below.

.. code:: shell

    for f in `rpm -Va | grep '^.M' | rev | cut -f1 -d' ' | rev`; do echo -n "RPM: "; rpm -qvlf $f | grep -e "[[:space:]]${f}$"; echo -n "Local: "; ls -ld $f; echo; done

    RPM: -rw-r--r--    1 root    root                     9438 Jul 12 09:00 /etc/httpd/conf.d/ssl.conf
    Local: -rw-r-----. 1 apache apache 1055 Dec 15 19:02 /etc/httpd/conf.d/ssl.conf

    RPM: -rw-r--r--    1 root    root                      473 Jul 27 09:08 /etc/rc.d/rc.local
    Local: -rw-------. 1 root root 49 Dec 15 17:30 /etc/rc.d/rc.local

    RPM: -rw-r--r--    1 root    root                    20876 Jan 26  2014 /etc/postfix/access
    Local: -rw-r-----. 1 root root 20876 Jan 26  2014 /etc/postfix/access

    RPM: -rw-r--r--    1 root    root                    11681 Jan 26  2014 /etc/postfix/canonical
    Local: -rw-r-----. 1 root root 11681 Jan 26  2014 /etc/postfix/canonical

    RPM: -rw-r--r--    1 root    root                     9904 Jan 26  2014 /etc/postfix/generic
    Local: -rw-r-----. 1 root root 9904 Jan 26  2014 /etc/postfix/generic

    RPM: -rw-r--r--    1 root    root                    21545 Jan 26  2014 /etc/postfix/header_checks
    Local: -rw-r-----. 1 root root 21545 Jan 26  2014 /etc/postfix/header_checks

    RPM: -rw-r--r--    1 root    root                     6105 Jan 26  2014 /etc/postfix/master.cf
    Local: -rw-r-----. 1 root root 6105 Jan 26  2014 /etc/postfix/master.cf

    RPM: -rw-r--r--    1 root    root                     6816 Jan 26  2014 /etc/postfix/relocated
    Local: -rw-r-----. 1 root root 6816 Jan 26  2014 /etc/postfix/relocated

    RPM: -rw-r--r--    1 root    root                    12549 Jan 26  2014 /etc/postfix/transport
    Local: -rw-r-----. 1 root root 12549 Jan 26  2014 /etc/postfix/transport

    RPM: -rw-r--r--    1 root    root                    12494 Jan 26  2014 /etc/postfix/virtual
    Local: -rw-r-----. 1 root root 12494 Jan 26  2014 /etc/postfix/virtual

    # There were issues when this was not executable
    RPM: -rw-r--r--    1 root    root                    26990 Jan 26  2014 /usr/libexec/postfix/main.cf
    Local: -rwxr-xr-x. 1 root root 26990 Jan 26  2014 /usr/libexec/postfix/main.cf

    # There were issues when this was not executable
    RPM: -rw-r--r--    1 root    root                     6105 Jan 26  2014 /usr/libexec/postfix/master.cf
    Local: -rwxr-xr-x. 1 root root 6105 Jan 26  2014 /usr/libexec/postfix/master.cf

    # There were issues when this was not executable
    RPM: -rw-r--r--    1 root    root                    19366 Jan 26  2014 /usr/libexec/postfix/postfix-files
    Local: -rwxr-xr-x. 1 root root 19366 Jan 26  2014 /usr/libexec/postfix/postfix-files

    RPM: -rw-r--r--    1 root    root                      253 Nov 22 21:37 /etc/puppetlabs/orchestration-services/conf.d/authorization.conf
    Local: -rw-r-----. 1 pe-orchestration-services pe-orchestration-services 2263 Dec 14 20:42 /etc/puppetlabs/orchestration-services/conf.d/authorization.conf

    RPM: -rw-r--r--    1 root    root                      388 Nov 22 21:37 /etc/puppetlabs/orchestration-services/conf.d/orchestrator.conf
    Local: -rw-r-----. 1 pe-orchestration-services pe-orchestration-services 1344 Dec 14 20:40 /etc/puppetlabs/orchestration-services/conf.d/orchestrator.conf

    RPM: -rw-r--r--    1 root    root                      327 Nov 22 21:37 /etc/puppetlabs/orchestration-services/conf.d/pcp-broker.conf
    Local: -rw-r-----. 1 pe-orchestration-services pe-orchestration-services 379 Dec 22 21:07 /etc/puppetlabs/orchestration-services/conf.d/pcp-broker.conf

    RPM: -rw-r--r--    1 root    root                     1149 Nov 22 21:37 /etc/puppetlabs/orchestration-services/conf.d/webserver.conf
    Local: -rw-r-----. 1 pe-orchestration-services pe-orchestration-services 916 Dec 14 20:40 /etc/puppetlabs/orchestration-services/conf.d/webserver.conf

    RPM: drwxrwx---    2 pe-orchepe-orche                    0 Nov 22 21:37 /opt/puppetlabs/server/data/orchestration-services
    Local: drwxr-xr-x. 2 pe-orchestration-services pe-orchestration-services 27 Dec 14 20:42 /opt/puppetlabs/server/data/orchestration-services

    RPM: -rw-------    1 root    root                      221 May 24  2015 /etc/securetty
    Local: -r--------. 1 root root 49 Dec 15 17:30 /etc/securetty

    RPM: drwxr-xr-x    2 root    root                        0 Jan 27  2014 /etc/stunnel
    Local: drwxr-x---. 2 root stunnel 25 Dec 15 19:02 /etc/stunnel

    RPM: -rw-r--r--    1 root    root                     2422 Aug  4  2015 /etc/security/limits.conf
    Local: -rw-r-----. 1 root root 34 Dec 15 17:38 /etc/security/limits.conf

    RPM: drwxr-x---    2 root    puppet                      0 Nov 27 01:34 /usr/share/simp/environments/simp
    Local: drwxrws---. 7 root root 4096 Dec 14 21:18 /usr/share/simp/environments/simp

    # This needs to be writable by the 'clam' group for all components to function properly
    RPM: -rw-r--r--    1 clamupdaclamupda                76781 Jun 13  2016 /var/lib/clamav/bytecode.cvd
    Local: -rw-rw-r--. 1 clam clam 96528 Dec 15 19:02 /var/lib/clamav/bytecode.cvd

    # This needs to be writable by the 'clam' group for all components to function properly
    RPM: -rw-r--r--    1 clamupdaclamupda            109143933 Jun 13  2016 /var/lib/clamav/main.cvd
    Local: -rw-rw-r--. 1 clam clam 109143933 Jun 13  2016 /var/lib/clamav/main.cvd

    RPM: -rw-r--r--    1 root    root                      119 Nov 25  2014 /etc/default/useradd
    Local: -rw-------. 1 root root 110 Dec 15 17:30 /etc/default/useradd

    RPM: -rw-r--r--    1 root    root                     2028 Nov 25  2014 /etc/login.defs
    Local: -rw-r-----. 1 root root 644 Dec 15 17:30 /etc/login.defs

    RPM: -rw-r--r--    1 root    root                   242153 Mar 16  2016 /etc/ssh/moduli
    Local: -rw-------. 1 root root 242153 Mar 16  2016 /etc/ssh/moduli

    RPM: drwxr-xr-x    2 clamupdaclamupda                    0 Jun 13  2016 /var/lib/clamav
    Local: drwxrwxr-x. 2 clam clam 56 Dec 15 19:02 /var/lib/clamav

    RPM: -rw-r--r--    1 root    root                      190 Nov 23 23:10 /etc/puppetlabs/puppetserver/conf.d/global.conf
    Local: -rw-r-----. 1 pe-puppet pe-puppet 476 Dec 14 20:37 /etc/puppetlabs/puppetserver/conf.d/global.conf

    RPM: -rw-r--r--    1 root    root                     1030 Nov 23 23:10 /etc/puppetlabs/puppetserver/conf.d/metrics.conf
    Local: -rw-r-----. 1 pe-puppet pe-puppet 1215 Dec 14 20:40 /etc/puppetlabs/puppetserver/conf.d/metrics.conf

    RPM: -rw-r--r--    1 root    root                     1766 Nov 23 23:10 /etc/puppetlabs/puppetserver/conf.d/pe-puppet-server.conf
    Local: -rw-r-----. 1 pe-puppet pe-puppet 1960 Dec 14 20:37 /etc/puppetlabs/puppetserver/conf.d/pe-puppet-server.conf

    RPM: -rw-r--r--    1 root    root                     1666 Nov 23 23:10 /etc/puppetlabs/puppetserver/conf.d/web-routes.conf
    Local: -rw-r-----. 1 pe-puppet pe-puppet 1772 Dec 14 20:37 /etc/puppetlabs/puppetserver/conf.d/web-routes.conf

    RPM: -rw-r--r--    1 root    root                      478 Nov 23 23:10 /etc/puppetlabs/puppetserver/conf.d/webserver.conf
    Local: -rw-r-----. 1 pe-puppet pe-puppet 766 Dec 14 20:37 /etc/puppetlabs/puppetserver/conf.d/webserver.conf

    RPM: drwxrwx---    2 pe-puppepe-puppe                    0 Nov 23 23:10 /opt/puppetlabs/server/data/puppetserver
    Local: drwxr-xr-x. 10 pe-puppet pe-puppet 4096 Dec 20 18:04 /opt/puppetlabs/server/data/puppetserver

    RPM: drwx------    2 pe-puppepe-puppe                    0 Nov 23 23:10 /var/log/puppetlabs/puppetserver
    Local: drwxr-x---. 2 pe-puppet pe-puppet 4096 Dec 29 00:06 /var/log/puppetlabs/puppetserver

    RPM: -rw-r--r--    1 root    root                      621 Nov 29 20:56 /etc/puppetlabs/puppetdb/conf.d/config.ini
    Local: -rw-r-----. 1 pe-puppetdb pe-puppetdb 655 Dec 22 21:07 /etc/puppetlabs/puppetdb/conf.d/config.ini

    RPM: -rw-r--r--    1 root    root                      550 Nov 29 20:56 /etc/puppetlabs/puppetdb/conf.d/database.ini
    Local: -rw-r-----. 1 pe-puppetdb pe-puppetdb 966 Dec 14 20:41 /etc/puppetlabs/puppetdb/conf.d/database.ini

    RPM: -rw-r--r--    1 root    root                     1081 Nov 29 20:56 /etc/puppetlabs/puppetdb/conf.d/jetty.ini
    Local: -rw-r-----. 1 pe-puppetdb pe-puppetdb 1460 Dec 14 20:40 /etc/puppetlabs/puppetdb/conf.d/jetty.ini

    RPM: -rw-r--r--    1 root    root                      358 Nov 29 20:56 /etc/puppetlabs/puppetdb/conf.d/rbac_consumer.conf
    Local: -rw-r-----. 1 pe-puppetdb pe-puppetdb 651 Dec 14 20:40 /etc/puppetlabs/puppetdb/conf.d/rbac_consumer.conf

    # Not changed by SIMP - File bug report with Puppet, Inc.
    RPM: drwxrwx---    2 pe-puppepe-puppe                    0 Nov 29 20:56 /opt/puppetlabs/server/data/puppetdb
    Local: drwxr-xr-x. 3 pe-puppetdb pe-puppetdb 36 Dec 14 20:41 /opt/puppetlabs/server/data/puppetdb

    # Not changed by SIMP - File bug report with Puppet, Inc.
    RPM: drwx------    2 pe-puppepe-puppe                    0 Nov 29 20:56 /var/log/puppetlabs/puppetdb
    Local: drwxr-x---. 2 pe-puppetdb pe-puppetdb 4096 Dec 29 00:06 /var/log/puppetlabs/puppetdb

    RPM: -rw-r--r--    1 root    root                     1756 Jun 17  2016 /etc/default/nss
    Local: -rw-r-----. 1 root root 78 Dec 15 17:30 /etc/default/nss

    # Needs to be fixed in SIMP to match the defaults
    RPM: drwx--x--x    2 root    root                        0 Mar 16  2016 /var/empty/sshd
    Local: drwxr-xr-x. 3 root root 16 Dec 15 19:01 /var/empty/sshd

    RPM: drwxr-xr-x    2 root    root                        0 Dec 27  2013 /etc/cron.daily
    drwxr-xr-x    2 root    root                        0 Dec  3  2015 /etc/cron.daily
    Local: dr-x------. 2 root root 111 Dec 27 21:37 /etc/cron.daily

    RPM: drwxr-xr-x    2 root    root                        0 Dec 27  2013 /etc/cron.hourly
    drwxr-xr-x    2 root    root                        0 Dec  3  2015 /etc/cron.hourly
    Local: dr-x------. 2 root root 44 Dec 22 21:02 /etc/cron.hourly

    RPM: drwxr-xr-x    2 root    root                        0 Dec 27  2013 /etc/cron.monthly
    Local: dr-x------. 2 root root 6 Dec 27  2013 /etc/cron.monthly

    RPM: drwxr-xr-x    2 root    root                        0 Dec 27  2013 /etc/cron.weekly
    Local: dr-x------. 2 root root 6 Dec 27  2013 /etc/cron.weekly

    RPM: -rw-r--r--    1 root    root                      458 Jun 24  2015 /etc/rsyncd.conf
    Local: -r--------. 1 root root 6047 Dec 27 21:37 /etc/rsyncd.conf

    RPM: drwxr-xr-x    2 root    root                        0 Jul 12 09:03 /etc/httpd/conf
    Local: drwxr-x---. 3 root apache 45 Dec 15 19:02 /etc/httpd/conf

    RPM: drwxr-xr-x    2 root    root                        0 Jul 12 09:03 /etc/httpd/conf.d
    Local: drwxr-x---. 2 root apache 50 Dec 15 19:02 /etc/httpd/conf.d

    RPM: -rw-r--r--    1 root    root                    11753 Jul 12 09:00 /etc/httpd/conf/httpd.conf
    Local: -rw-r-----. 1 root apache 7972 Dec 15 19:02 /etc/httpd/conf/httpd.conf

    RPM: -rw-r--r--    1 root    root                    13077 Jul 12 09:03 /etc/httpd/conf/magic
    Local: -rw-r-----. 1 root apache 12958 Dec 15 19:02 /etc/httpd/conf/magic

    RPM: drwxr-xr-x    2 root    root                        0 Jul 12 09:03 /var/www
    Local: drwxr-x---. 8 root apache 74 Dec 15 19:02 /var/www

    RPM: drwxr-xr-x    2 root    root                        0 Jul 12 09:03 /var/www/cgi-bin
    Local: drwxr-x---. 2 root apache 6 Jul 12 09:03 /var/www/cgi-bin

    RPM: drwxr-xr-x    2 root    root                        0 Jul 12 09:03 /var/www/html
    Local: drwxr-x---. 2 root apache 6 Jul 12 09:03 /var/www/html

    RPM: -rw-r--r--    1 root    root                     3232 Sep  7  2015 /etc/rsyslog.conf
    Local: -rw-------. 1 root root 42 Dec 20 18:08 /etc/rsyslog.conf

    RPM: -rw-r--r--    1 root    root                      196 Sep  7  2015 /etc/sysconfig/rsyslog
    Local: -rw-r-----. 1 root root 19 Dec 15 17:30 /etc/sysconfig/rsyslog

    RPM: -rw-r-----    1 root    root                      701 Jan 14  2015 /etc/audit/auditd.conf
    Local: -rw-------. 1 root root 454 Dec 15 17:30 /etc/audit/auditd.conf

    RPM: -rwxr-xr-x    1 root    root                     6776 Dec  6 01:12 /etc/puppetlabs/activemq/activemq.xml
    Local: -rw-r-----. 1 root pe-activemq 3982 Dec 14 20:40 /etc/puppetlabs/activemq/activemq.xml

    RPM: -rwxr-xr-x    1 root    root                     7764 Dec  6 01:12 /etc/puppetlabs/activemq/jetty.xml
    Local: -rw-r-----. 1 root pe-activemq 7764 Dec  6 01:12 /etc/puppetlabs/activemq/jetty.xml

    RPM: -rwxr-xr-x    1 root    root                     2980 Dec  6 01:12 /etc/puppetlabs/activemq/log4j.properties
    Local: -rw-r-----. 1 root pe-activemq 2980 Dec  6 01:12 /etc/puppetlabs/activemq/log4j.properties

    RPM: drwxrwxr-x    2 pe-activpe-activ                    0 Dec  6 01:12 /var/run/puppetlabs/activemq
    Local: drwxr-xr-x. 2 pe-activemq pe-activemq 60 Dec 22 20:52 /var/run/puppetlabs/activemq

    RPM: -rw-r--r--    1 root    root                     1992 May  3  2016 /etc/ntp.conf
    Local: -rw-------. 1 root ntp 319 Dec 22 15:14 /etc/ntp.conf

    RPM: -rw-r--r--    1 root    root                       45 May  3  2016 /etc/sysconfig/ntpd
    Local: -rw-r-----. 1 root root 62 Dec 15 17:30 /etc/sysconfig/ntpd

    RPM: drwxr-xr-x    2 ntp     ntp                         0 May  3  2016 /var/lib/ntp
    Local: drwxr-x---. 2 ntp ntp 18 Dec 29 17:52 /var/lib/ntp

    RPM: -rw-r--r--    1 root    root                      775 Nov 23 00:58 /etc/puppetlabs/console-services/bootstrap.cfg
    Local: -rw-r-----. 1 pe-console-services pe-console-services 933 Dec 14 20:43 /etc/puppetlabs/console-services/bootstrap.cfg

    RPM: -rw-r--r--    1 root    root                        0 Nov 23 00:58 /etc/puppetlabs/console-services/conf.d/classifier.conf
    Local: -rw-r-----. 1 pe-console-services pe-console-services 403 Dec 14 20:41 /etc/puppetlabs/console-services/conf.d/classifier.conf

    RPM: -rw-r--r--    1 root    root                        0 Nov 23 00:58 /etc/puppetlabs/console-services/conf.d/console.conf
    Local: -rw-r-----. 1 pe-console-services pe-console-services 2154 Dec 15 17:40 /etc/puppetlabs/console-services/conf.d/console.conf

    RPM: -rw-r--r--    1 root    root                        0 Nov 23 00:58 /etc/puppetlabs/console-services/conf.d/global.conf
    Local: -rw-r-----. 1 pe-console-services pe-console-services 189 Dec 14 20:40 /etc/puppetlabs/console-services/conf.d/global.conf

    RPM: -rw-r--r--    1 root    root                        0 Nov 23 00:58 /etc/puppetlabs/console-services/conf.d/rbac.conf
    Local: -rw-r-----. 1 pe-console-services pe-console-services 360 Dec 14 20:41 /etc/puppetlabs/console-services/conf.d/rbac.conf

    RPM: -rw-r--r--    1 root    root                        0 Nov 23 00:58 /etc/puppetlabs/console-services/conf.d/webserver.conf
    Local: -rw-r-----. 1 pe-console-services pe-console-services 1880 Dec 14 20:40 /etc/puppetlabs/console-services/conf.d/webserver.conf

    RPM: drwxrwx---    2 pe-consope-conso                    0 Nov 23 00:58 /opt/puppetlabs/server/data/console-services
    Local: drwxr-xr-x. 3 pe-console-services pe-console-services 39 Dec 14 20:43 /opt/puppetlabs/server/data/console-services

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/apache
    Local: drwx------. 3 root root 16 Dec 14 21:13 /var/simp/rsync/RedHat/7/apache

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/bind_dns
    Local: drwx------. 3 root root 20 Dec 14 21:13 /var/simp/rsync/RedHat/7/bind_dns

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/bind_dns/default
    Local: drwx------. 3 root root 18 Dec 14 21:13 /var/simp/rsync/RedHat/7/bind_dns/default

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/bind_dns/default/named/etc
    Local: drwxr-xr-x. 3 root root 50 Dec 14 21:13 /var/simp/rsync/RedHat/7/bind_dns/default/named/etc

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/bind_dns/default/named/var
    Local: drwxr-xr-x. 4 root root 28 Dec 14 21:13 /var/simp/rsync/RedHat/7/bind_dns/default/named/var

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/default
    Local: drwx------. 3 root root 23 Dec 14 21:13 /var/simp/rsync/RedHat/7/default

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc
    Local: drwxr-xr-x. 6 root root 90 Dec 14 21:13 /var/simp/rsync/RedHat/7/default/global_etc

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.daily
    Local: dr-x------. 2 root root 6 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.daily

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.hourly
    Local: dr-x------. 2 root root 6 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.hourly

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.monthly
    Local: dr-x------. 2 root root 6 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.monthly

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.weekly
    Local: dr-x------. 2 root root 6 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/cron.weekly

    RPM: -rw-r-----    1 root    root                     1298 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/issue
    Local: -rw-r--r--. 1 root root 1298 Nov 24 19:00 /var/simp/rsync/RedHat/7/default/global_etc/issue

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/dhcpd
    Local: drwx------. 2 root root 23 Dec 14 21:13 /var/simp/rsync/RedHat/7/dhcpd

    RPM: drwxr-x---    2 root    root                        0 Nov 24 19:00 /var/simp/rsync/RedHat/7/mcafee
    Local: drwxr-xr-x. 2 root root 6 Nov 24 19:00 /var/simp/rsync/RedHat/7/mcafee

    RPM: -rw-r--r--    1 root    root                      293 Feb 23  2016 /etc/pam.d/crond
    Local: -rw-r-----. 1 root root 293 Feb 23  2016 /etc/pam.d/crond

    RPM: dr-xr-x---    2 root    root                        0 May 25  2015 /root
    Local: drwx------. 12 root root 4096 Dec 29 18:18 /root

    RPM: drwxrwxr-x    2 root    mail                        0 May 25  2015 /var/spool/mail
    Local: drwxr-xr-x. 2 root mail 67 Dec 29 00:12 /var/spool/mail

    RPM: -rw-r--r--    1 root    root                      272 Jun 22  2015 /etc/pam.d/atd
    Local: -rw-r-----. 1 root root 272 Jun 22  2015 /etc/pam.d/atd

    RPM: drwxr-xr-x    2 root    root                        0 Dec 27  2013 /etc/cron.daily
    drwxr-xr-x    2 root    root                        0 Dec  3  2015 /etc/cron.daily
    Local: dr-x------. 2 root root 111 Dec 27 21:37 /etc/cron.daily

    RPM: drwxr-xr-x    2 root    root                        0 Dec 27  2013 /etc/cron.hourly
    drwxr-xr-x    2 root    root                        0 Dec  3  2015 /etc/cron.hourly
    Local: dr-x------. 2 root root 44 Dec 22 21:02 /etc/cron.hourly

    RPM: drwxr-xr-x    2 root    root                        0 Dec  6 00:32 /etc/puppetlabs/code/environments/production
    Local: lrwxrwxrwx. 1 root root 4 Dec 14 21:23 /etc/puppetlabs/code/environments/production -> simp

    RPM: -rw-r--r--    1 root    root                      879 Dec  6 00:17 /etc/puppetlabs/code/environments/production/environment.conf
    Local: -rw-r-----. 1 root pe-puppet 678 Nov 27 01:34 /etc/puppetlabs/code/environments/production/environment.conf

    RPM: drwxr-xr-x    2 root    root                        0 Dec  6 00:18 /etc/puppetlabs/code/environments/production/hieradata
    Local: drwxr-x---. 6 root pe-puppet 4096 Dec 29 16:58 /etc/puppetlabs/code/environments/production/hieradata

    RPM: drwxr-xr-x    2 root    root                        0 Dec  6 00:18 /etc/puppetlabs/code/environments/production/manifests
    Local: drwxr-x---. 2 root pe-puppet 33 Dec 15 21:53 /etc/puppetlabs/code/environments/production/manifests

    RPM: drwxr-xr-x    2 root    root                        0 Dec  6 00:18 /etc/puppetlabs/code/environments/production/modules
    Local: drwxr-x---. 71 root pe-puppet 4096 Dec 22 17:43 /etc/puppetlabs/code/environments/production/modules

    RPM: -rw-r--r--    1 root    root                      634 Dec  6 00:17 /etc/puppetlabs/mcollective/server.cfg
    Local: -rw-rw----. 1 root root 2620 Dec 14 20:38 /etc/puppetlabs/mcollective/server.cfg

    RPM: -r--r--r--    1 root    root                     2036 Feb 23  2016 /etc/openldap/schema/collective.ldif
    Local: -rw-r--r--. 1 root ldap 2036 Feb 23  2016 /etc/openldap/schema/collective.ldif

    RPM: -r--r--r--    1 root    root                     6190 Feb 23  2016 /etc/openldap/schema/collective.schema
    Local: -rw-r--r--. 1 root ldap 6190 Feb 23  2016 /etc/openldap/schema/collective.schema

    RPM: -r--r--r--    1 root    root                     1845 Feb 23  2016 /etc/openldap/schema/corba.ldif
    Local: -rw-r--r--. 1 root ldap 1845 Feb 23  2016 /etc/openldap/schema/corba.ldif

    RPM: -r--r--r--    1 root    root                     8063 Feb 23  2016 /etc/openldap/schema/corba.schema
    Local: -rw-r--r--. 1 root ldap 8063 Feb 23  2016 /etc/openldap/schema/corba.schema

    RPM: -r--r--r--    1 root    root                    20612 Feb 23  2016 /etc/openldap/schema/core.ldif
    Local: -rw-r--r--. 1 root ldap 20612 Feb 23  2016 /etc/openldap/schema/core.ldif

    RPM: -r--r--r--    1 root    root                    20499 Feb 23  2016 /etc/openldap/schema/core.schema
    Local: -rw-r--r--. 1 root ldap 20499 Feb 23  2016 /etc/openldap/schema/core.schema

    RPM: -r--r--r--    1 root    root                    12006 Feb 23  2016 /etc/openldap/schema/cosine.ldif
    Local: -rw-r--r--. 1 root ldap 12006 Feb 23  2016 /etc/openldap/schema/cosine.ldif

    RPM: -r--r--r--    1 root    root                    73994 Feb 23  2016 /etc/openldap/schema/cosine.schema
    Local: -rw-r--r--. 1 root ldap 73994 Feb 23  2016 /etc/openldap/schema/cosine.schema

    RPM: -r--r--r--    1 root    root                     4842 Feb 23  2016 /etc/openldap/schema/duaconf.ldif
    Local: -rw-r--r--. 1 root ldap 4842 Feb 23  2016 /etc/openldap/schema/duaconf.ldif

    RPM: -r--r--r--    1 root    root                    10388 Feb 23  2016 /etc/openldap/schema/duaconf.schema
    Local: -rw-r--r--. 1 root ldap 10388 Feb 23  2016 /etc/openldap/schema/duaconf.schema

    RPM: -r--r--r--    1 root    root                     3330 Feb 23  2016 /etc/openldap/schema/dyngroup.ldif
    Local: -rw-r--r--. 1 root ldap 3330 Feb 23  2016 /etc/openldap/schema/dyngroup.ldif

    RPM: -r--r--r--    1 root    root                     3289 Feb 23  2016 /etc/openldap/schema/dyngroup.schema
    Local: -rw-r--r--. 1 root ldap 3289 Feb 23  2016 /etc/openldap/schema/dyngroup.schema

    RPM: -r--r--r--    1 root    root                     3481 Feb 23  2016 /etc/openldap/schema/inetorgperson.ldif
    Local: -rw-r--r--. 1 root ldap 3481 Feb 23  2016 /etc/openldap/schema/inetorgperson.ldif

    RPM: -r--r--r--    1 root    root                     6267 Feb 23  2016 /etc/openldap/schema/inetorgperson.schema
    Local: -rw-r--r--. 1 root ldap 6267 Feb 23  2016 /etc/openldap/schema/inetorgperson.schema

    RPM: -r--r--r--    1 root    root                     2979 Feb 23  2016 /etc/openldap/schema/java.ldif
    Local: -rw-r--r--. 1 root ldap 2979 Feb 23  2016 /etc/openldap/schema/java.ldif

    RPM: -r--r--r--    1 root    root                    13901 Feb 23  2016 /etc/openldap/schema/java.schema
    Local: -rw-r--r--. 1 root ldap 13901 Feb 23  2016 /etc/openldap/schema/java.schema

    RPM: -r--r--r--    1 root    root                     2082 Feb 23  2016 /etc/openldap/schema/misc.ldif
    Local: -rw-r--r--. 1 root ldap 2082 Feb 23  2016 /etc/openldap/schema/misc.ldif

    RPM: -r--r--r--    1 root    root                     2387 Feb 23  2016 /etc/openldap/schema/misc.schema
    Local: -rw-r--r--. 1 root ldap 2387 Feb 23  2016 /etc/openldap/schema/misc.schema

    RPM: -r--r--r--    1 root    root                     6809 Feb 23  2016 /etc/openldap/schema/nis.ldif
    Local: -rw-r--r--. 1 root ldap 6809 Feb 23  2016 /etc/openldap/schema/nis.ldif

    RPM: -r--r--r--    1 root    root                     7640 Feb 23  2016 /etc/openldap/schema/nis.schema
    Local: -rw-r--r--. 1 root ldap 7640 Feb 23  2016 /etc/openldap/schema/nis.schema

    RPM: -r--r--r--    1 root    root                     3308 Feb 23  2016 /etc/openldap/schema/openldap.ldif
    Local: -rw-r--r--. 1 root ldap 3308 Feb 23  2016 /etc/openldap/schema/openldap.ldif

    RPM: -r--r--r--    1 root    root                     1514 Feb 23  2016 /etc/openldap/schema/openldap.schema
    Local: -rw-r--r--. 1 root ldap 1514 Feb 23  2016 /etc/openldap/schema/openldap.schema

    RPM: -r--r--r--    1 root    root                     6904 Feb 23  2016 /etc/openldap/schema/pmi.ldif
    Local: -rw-r--r--. 1 root ldap 6904 Feb 23  2016 /etc/openldap/schema/pmi.ldif

    RPM: -r--r--r--    1 root    root                    20467 Feb 23  2016 /etc/openldap/schema/pmi.schema
    Local: -rw-r--r--. 1 root ldap 20467 Feb 23  2016 /etc/openldap/schema/pmi.schema

    RPM: -r--r--r--    1 root    root                     4356 Feb 23  2016 /etc/openldap/schema/ppolicy.ldif
    Local: -rw-r--r--. 1 root ldap 4356 Feb 23  2016 /etc/openldap/schema/ppolicy.ldif

    RPM: -r--r--r--    1 root    root                    19963 Feb 23  2016 /etc/openldap/schema/ppolicy.schema
    Local: -rw-r--r--. 1 root ldap 19963 Feb 23  2016 /etc/openldap/schema/ppolicy.schema

    RPM: -rw-r--r--    1 root    root                      527 Feb 23  2016 /etc/sysconfig/slapd
    Local: -rw-r-----. 1 root root 42 Dec 15 17:29 /etc/sysconfig/slapd

    # Group access does not weaker permissions
    RPM: drwx------    2 ldap    ldap                        0 Feb 23  2016 /var/lib/ldap
    Local: drwxrwx---. 4 ldap ldap 99 Dec 27 15:55 /var/lib/ldap

    # Required for user-based virus scanning
    RPM: drwxr-x---    2 root    root                        0 Nov 27 01:33 /var/simp/rsync/RedHat/7/clamav
    Local: drwxrwxr-x. 2 clam clam 56 Dec 14 21:16 /var/simp/rsync/RedHat/7/clamav

    # Required for user-based virus scanning
    RPM: -rw-r-----    1 root    root                    96528 Nov 24 22:20 /var/simp/rsync/RedHat/7/clamav/bytecode.cvd
    Local: -rw-rw-r--. 1 clam clam 96528 Nov 24 22:20 /var/simp/rsync/RedHat/7/clamav/bytecode.cvd

    # Required for user-based virus scanning
    RPM: -rw-r-----    1 root    root                 63135232 Nov 27 01:33 /var/simp/rsync/RedHat/7/clamav/daily.cld
    Local: -rw-rw-r--. 1 clam clam 63135232 Nov 27 01:33 /var/simp/rsync/RedHat/7/clamav/daily.cld

    # Required for user-based virus scanning
    RPM: -rw-r-----    1 root    root                109143933 Nov 24 22:19 /var/simp/rsync/RedHat/7/clamav/main.cvd
    Local: -rw-rw-r--. 1 clam clam 109143933 Nov 24 22:19 /var/simp/rsync/RedHat/7/clamav/main.cvd

    RPM: drwx--x--x    2 sssd    sssd                        0 Jul 14 10:33 /etc/sssd
    Local: drwxr-x---. 3 root root 52 Dec 15 17:38 /etc/sssd

    # SIMP should restrict global access
    RPM: drwx------    2 pe-postgpe-postg                    0 Dec  6 01:33 /opt/puppetlabs/server/data/postgresql
    Local: drwxr-xr-x. 8 pe-postgres pe-postgres 4096 Dec 14 20:39 /opt/puppetlabs/server/data/postgresql

    # SIMP should restrict global access
    RPM: drwx------    2 pe-postgpe-postg                    0 Dec  6 01:33 /opt/puppetlabs/server/data/postgresql/9.4
    Local: drwxr-xr-x. 4 pe-postgres pe-postgres 31 Dec 14 20:38 /opt/puppetlabs/server/data/postgresql/9.4

    RPM: drwxrwxr-x    2 pe-postgpe-postg                    0 Dec  6 01:33 /var/run/puppetlabs/postgresql
    Local: drwxr-xr-x. 2 pe-postgres pe-postgres 80 Dec 22 20:52 /var/run/puppetlabs/postgresql

--------------

Install Virus Scanning Software
-------------------------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_install\_antivirus
-  Type: False Positive

Notes
~~~~~

.. code:: shell

    rpm -q clamav
    clamav-0.99.2-1.el7.x86_64

--------------

Ensure Users Re-Authenticate for Privilege Escalation - sudo NOPASSWD
---------------------------------------------------------------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_sudo\_remove\_nopasswd
-  Type: Exception
-  **Requested feedback from the SSG team**

   -  Need rules based around SSH-only systems
   -  Passwords are known to be less secure than keys (as long as the keys
      are properly protected)

Justification
~~~~~~~~~~~~~

| It is generally recommended that SIMP systems do not use passwords on
  systems
| and only allow authentication via SSH keys. This necessarily precludes
  the use
| of passwords to authenticate via ``sudo``.

| This may be configured differently and, by default, is restricted to
  only the
| ``administrators`` and ``security`` groups.

.. code:: shell

     cat /etc/sudoers | grep NOP
    %administrators    ALL=(root) NOPASSWD:EXEC:SETENV: /bin/rm -rf /etc/puppetlabs/puppet/ssl
    %administrators    ALL=(ALL) NOPASSWD:EXEC:SETENV: /usr/bin/sudosh
    %administrators    ALL=(root) NOPASSWD:EXEC:SETENV: /usr/sbin/puppetca
    %administrators    ALL=(root) NOPASSWD:EXEC:SETENV: /usr/sbin/puppetd
    %security    ALL=(root) NOPASSWD:EXEC:SETENV: AUDIT

--------------

Disable Kernel Support for USB via Bootloader Configuration
-----------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_bootloader\_nousb\_argument
-  Type: Exception - Dangerous
-  **Working on a SIMP fix**

Notes
~~~~~

| Disabling global USB is *extremely* dangerous and will, most likely,
  cripple
| the ability to update systems and troubleshoot systems at all given
  that most
| modern systems no longer make USB keyboards and mice available.

SIMP attempts to be sensible and disable block device connections
instead.

| An enhancement request could be filed against SIMP to allow setting
  this kernel
| parameter but it should *not* be set by default unless no USB devices
  are
| detected on the system.

.. code:: shell

    cat /etc/modprobe.d/00_simp_blacklist.conf
    # This file managed by Puppet.
    install ieee1394 /bin/true
    install usb-storage /bin/true

--------------

Ensure All Files Are Owned by a User
------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_no\_files\_unowned\_by\_user
-  Type: Exception

Justification
~~~~~~~~~~~~~

| The SIMP server serves files over encrypted ``rsync`` which require
  proper
| **numeric** ownership after transfer. The server, not requiring the
  ``rsync``
| specified users will show the files as unowned. This is **correct**
  and must
| not be modified if the client systems are to maintain proper
  functionality.

--------------

Ensure All Files Are Owned by a Group
-------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_file\_permissions\_ungroupowned
-  Type: Exception

Justification
~~~~~~~~~~~~~

| The SIMP server serves files over encrypted ``rsync`` which requires
  proper
| **numeric** ownership after transfer. The server, not requiring the
  ``rsync``
| specified users will show the files as unowned. This is **correct**
  and must
| not be modified if the client systems are to maintain proper
  functionality.

--------------

Set Daemon Umask
----------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_umask\_for\_daemons
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The check should be fixed

Notes
~~~~~

The policy allows for ``022`` or ``027`` but the check only checks for
``022``.

| Using a default umask of ``022`` caused too many daemons to fail and
  caused a
| **very** high instance of troubleshooting overhead.

.. code:: shell

    grep umask /etc/init.d/functions
    # Make sure umask is sane
    umask 0027

--------------

Ensure No Daemons are Unconfined by SELinux
-------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_selinux\_confinement\_of\_daemons
-  Type: Exception
-  **Recommend RedHat Feedback**

   -  An SELinux policy should be shipped for running rsync in daemon mode

Notes
~~~~~

| Rsync does not presently have a vendor supplied policy for running in
  daemon
| mode at start time but running in daemon mode is supported via
| ``/etc/rsyncd.conf``. The vendor should supply documentation and/or a
  policy
| for running ``rsync`` in daemon mode and restricting content access
  when
| running from the ``init`` system.

| Since SIMP systems need to transfer contexts to client systems, it is
  likely
| that the ``rsync_full_access`` SELinux boolean will need to be set so
  that
| ``rsync`` can properly access the files within the rsync share.

--------------

Ensure No Device Files are Unlabeled by SELinux
-----------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_selinux\_all\_devicefiles\_labeled
-  Type: False Positive
-  **Requested feedback from the SSG team**

Notes
~~~~~

This check simply appears to be broken

--------------

Direct root Logins Not Allowed
------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_no\_direct\_root\_logins
-  Type: Exception

Notes
~~~~~

| Removing all ability for Root to login from the console prevents "last
  effort"
| recovery of systems. This is not something that SIMP will enable by
  default.

You can make this compliant by setting the following in Hiera:

.. code:: yaml

    ---
    simplib::securetty : []

--------------

Restrict Serial Port Root Logins
--------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_restrict\_serial\_port\_logins
-  Type: Exception

Justification
~~~~~~~~~~~~~

| Removing all ability for Root to login from serial ports prevents
  "last effort"
| recovery of remote systems. This is not something that SIMP will
  enable by
| default.

You can make this compliant by setting the following in Hiera:

.. code:: yaml

    ---
    simplib::securetty :
      - 'console'
      - 'tty1'
      - 'tty2'
      - 'tty3'
      - 'tty4'
      - 'tty5'
      - 'tty6'

--------------

Set Password Maximum Age
------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_maximum\_age\_login\_defs
-  Type: Exception

Justification
~~~~~~~~~~~~~

SIMP sets ``PASS_MAX_DAYS`` to ``180`` by default per most common
guidance.

| The scan checks for ``60`` days but this tends to be too short for the
  enforced
| password complexity requirements.

If you need a shorter duration set the following in Hiera:

.. code:: yaml

    ---
    simplib::login_defs::pass_max_days: '60'

--------------

Set Account Expiration Following Inactivity
-------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_account\_disable\_post\_pw\_expiration
-  Type: False Positive

-  **Requested feedback from the SSG team**

   -  Simply a badly formed check

Notes
~~~~~

The check is incorrect.

--------------

Set Password Retry Prompts Permitted Per-Session
------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_retry
-  Type: Alternate Implementation

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -o retry=3 /etc/pam.d/system-auth
    retry=3

--------------

Set Password Maximum Consecutive Repeating Characters
-----------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_maxrepeat
-  Type: Alternate Implementation

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -o maxrepeat /etc/pam.d/system-auth
    maxrepeat

--------------

Set Password to Maximum of Consecutive Repeating Characters from Same Character Class
-------------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_maxclassrepeat
-  Type: Alternate Implementation - Finding

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -o maxclassrepeat /etc/pam.d/system-auth
    maxclassrepeat=0

| Maxclassrepeat is set to ``0`` (not enforced) by default because we
  found that
| it was too difficult for users to come up with passwords that could
  meet all
| requirements when enabled.

To enable this, with a value of ``4``, use the following in Hiera:

.. code:: yaml

    ---
    pam::cracklib_maxclassrepeat: '4'

--------------

Set Password Strength Minimum Digit Characters
----------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_dcredit
-  Type: Alternate Implementation

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "dcredit=.*? "  /etc/pam.d/system-auth
    dcredit=-1

--------------

Set Password Minimum Length
---------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_minlen
-  Type: Alternate Implementation - Finding

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "minlen=.*? "  /etc/pam.d/system-auth
    minlen=14

| The ``minlen`` requirements vary **vastly** between policy documents.
  The
| previous requirement was ``14`` and is has been changed to ``15``.

| This can be made compliant using the following Hieradata:

.. code:: yaml

    ---
    pam::cracklib_minlen: '15'

--------------

Set Password Strength Minimum Uppercase Characters
--------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_ucredit
-  Type: Alternate Implementation

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "ucredit=.*? "  /etc/pam.d/system-auth
    ucredit=-1

--------------

Set Password Strength Minimum Lowercase Characters
--------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_lcredit
-  Type: Alternate Implementation

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "lcredit=.*? "  /etc/pam.d/system-auth
    lcredit=-1

--------------

Set Password Strength Minimum Different Characters
--------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_password\_pam\_difok
-  Type: Alternate Implementation - Finding

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "difok=.*? "  /etc/pam.d/system-auth
    difok=4

| The ``difok`` requirements vary **vastly** between policy documents.
  The
| previous requirement was ``3`` and is has been changed to ``4``.

This can be made compliant using the following Hieradata:

.. code:: yaml

    ---
    pam::cracklib_difok: '4'

--------------

Set Password Strength Minimum Different Categories
--------------------------------------------------

-  Rule ID:
-  Type: Alternate Implementation - False Positive
-  **Requested feedback from the SSG team**

   -  This should be combined with/overridden by the ``*credit`` checks

Notes
~~~~~

| The policy indicates that ``pam_cracklib`` may be used in lieu of
| ``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "minclass=.*? "  /etc/pam.d/system-auth
    minclass=3

| Though ``minclass`` is set to ``3``, setting the ``*credit`` items to
  ``-1``
| ensures that they must be used in the password which renders this
  setting
| useless.

Nevertheless, it should be changed in SIMP to match the scan.

--------------

Set Deny For Failed Password Attempts
-------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_passwords\_pam\_faillock\_deny
-  Type: Exception

Justification
~~~~~~~~~~~~~

.. code:: shell

    grep -P "deny=.*? "  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900

| Setting ``deny`` to less than ``5`` was causing premature lockouts
  when
| presented with alternate authentication systems and also, at times,
  when using
| ``sudo`` and attempting to ``^C`` out of the session. This may be
  fixed in the
| latest releases of RHEL, but has not been verified.

--------------

Set Lockout Time For Failed Password Attempts
---------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_passwords\_pam\_faillock\_unlock\_time
-  Type: Exception
-  **Requested feedback from the SSG team**

   -  The defaults are unreasonable for production systems and should be
      changed

Justification
~~~~~~~~~~~~~

| Waiting for more than ``15`` minutes is not conducive to effective
  security and
| causes a heavy burden on helpdesk systems relating to password resets
  where the
| user remembers their password but simply typed it incorrectly multiple
  times.

| Even the most rudmentary log auditing system should be able to
  identify
| repeated failed logins over multi-15 minute boundaries.

.. code:: shell

    grep -P "unlock_time=.*? "  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900

This can be made compliant using the following Hieradata:

.. code:: yaml

    ---
    pam::unlock_time: 604800

--------------

Configure the root Account for Failed Password Attempts
-------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_passwords\_pam\_faillock\_deny\_root
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  False Positive

Notes
~~~~~

.. code:: shell

    grep -P "unlock_time=.*? "  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900

--------------

Set Interval For Counting Failed Password Attempts
--------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_accounts\_passwords\_pam\_faillock\_interval
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The position before, or after, ``pam_unix.so`` is irrelevant if
      ``pam_unix.so`` is set to ``required`` and not ``sufficient``
-  **Pending SIMP Fix**

    -  SIMP should go ahead and fix this so that the scans do not fail

Notes
~~~~~

False Positive

.. code:: shell

    grep -P "faillock"  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900
    account     required      pam_faillock.so

--------------

Set Boot Loader Password
------------------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_bootloader\_password
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  False Positive

Notes
~~~~~

| The script should check the **built** ``/etc/grub2.cfg``. Checking the
| configuration files is not useful if they have not been applied.

.. code:: shell

    grep pbkdf /etc/grub2.cfg
    password_pbkdf2 root grub.pbkdf2.sha512.10000.83E1E6452551

--------------

Disable Ctrl-Alt-Del Reboot Activation
--------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_disable\_ctrlaltdel\_reboot
-  Type: Finding

Notes
~~~~~

| This needs to be files with SIMP and fixed. The last implementation
  was in
| ``upstart`` for EL6 and was not ported to ``systemd`` for EL7.

This can be mitigated with the following Puppet code:

.. code:: ruby

    file { '/etc/systemd/system/ctrl-alt-del.target':
      type   => symlink,
      force  => true,
      target => '/dev/null'
    }

--------------

Modify the System Login Banner
------------------------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_banner\_etc\_issue
-  Type: False Positive

Notes
~~~~~

There is a login banner, but it is not the DoD default.

--------------

Disable Kernel Parameter for IP Forwarding
------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sysctl\_net\_ipv4\_ip\_forward
-  Type: Exception
-  **Recommend SSG Discussion**

   -  Almost all systems run containers, namespaces, or VMs these days

Justification
~~~~~~~~~~~~~

| This is an antequated rule given that almost all environments run
  subsystems
| that require some sort of internal routing. To support these
  subsystems, SIMP
| needs to manage IP forwarding rules elsewhere and the system
  **defaults** are
| correct.

--------------

Configure Kernel Parameter for Accepting Source-Routed Packets for All Interfaces
---------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sysctl\_net\_ipv6\_conf\_all\_accept\_source\_route
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  Per the Description, the check is incorrect

Notes
~~~~~

.. code:: shell

    sysctl -a | grep source_route
    net.ipv4.conf.all.accept_source_route = 0
    net.ipv4.conf.default.accept_source_route = 0
    net.ipv4.conf.ens192.accept_source_route = 0
    net.ipv4.conf.lo.accept_source_route = 1
    net.ipv6.conf.all.accept_source_route = 0
    net.ipv6.conf.default.accept_source_route = 0
    net.ipv6.conf.ens192.accept_source_route = 0
    net.ipv6.conf.lo.accept_source_route = 0

--------------

Verify firewalld Enabled
------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_service\_firewalld\_enabled
-  Type: Alternate Implementation
-  **Requested feedback from the SSG team**

   -  The scan should allow for either ``firewalld`` or ``iptables`` since
      the
      policy does

Notes
~~~~~

| To use the same code to manage both EL6 and EL7 systems, SIMP manages
| ``iptables`` directly. Additionally, for server systems, most admins
  that we
| have encountered find it easier to deal with direct IPTables rules
  when
| debugging firewall issues.

| Finally, ``firewalld`` hooks into ``dbus`` which opens the possibility
  of
| software that can independently manage firewall settings at run time
  without
| explicit authorization.

| When EL6 is no longer supported SIMP may move to having ``firewalld``
  support,
| but not before then.

.. code:: shell

     systemctl status iptables
    ● iptables.service - LSB: start and stop iptables firewall
       Loaded: loaded (/etc/rc.d/init.d/iptables)
       Active: active (exited) since Thu 2016-12-22 20:52:06 GMT; 1 weeks 0 days ago
         Docs: man:systemd-sysv-generator(8

--------------

Set Default firewalld Zone for Incoming Packets
-----------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_set\_firewalld\_default\_zone
-  Type: Alternate Implementation
-  **Requested feedback from the SSG team**

   -  The scan should allow for either ``firewalld`` or ``iptables`` since
      the
      policy does

Notes
~~~~~

SIMP provides full IPTables management by default with a "default drop"
policy.

.. code:: shell

    iptables-save
    *filter
    :INPUT ACCEPT [0:0]
    :FORWARD ACCEPT [0:0]
    :OUTPUT ACCEPT [0:0]
    :LOCAL-INPUT - [0:0]
    -A INPUT -j LOCAL-INPUT
    -A FORWARD -j LOCAL-INPUT
    -A LOCAL-INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
    -A LOCAL-INPUT -i lo -j ACCEPT
    -A LOCAL-INPUT -p tcp -m state --state NEW -m tcp -m multiport --dports 22 -j ACCEPT
    -A LOCAL-INPUT -p icmp -m icmp --icmp-type 8 -j ACCEPT
    -A LOCAL-INPUT -m pkttype --pkt-type broadcast -j DROP
    -A LOCAL-INPUT -m addrtype --src-type MULTICAST -j DROP
    -A LOCAL-INPUT -m state --state NEW -j LOG --log-prefix "IPT:"
    -A LOCAL-INPUT -j DROP
    COMMIT

--------------

Ensure Logs Sent To Remote Host
-------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_rsyslog\_remote\_loghost
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not take into account the new Rainerscript format and
      does
      not process the full configuration

Notes
~~~~~

.. code:: shell

     cat /etc/rsyslog.simp.d/10_simp_remote/simp_stock_remote.conf
    ruleset(
      name="simp_stock_remote_ruleset"
    ) {
      action(
        type="omfwd"
        protocol="tcp"
        target="1.2.3.4"
        port="6514"
        TCP_Framing="traditional"
        ZipLevel="0"
        StreamDriverMode="1"
        StreamDriverAuthMode="x509/name"
        StreamDriverPermittedPeers="*.my.domain"
        ResendLastMSGOnReconnect="on"
      )
    }

    if $programname == 'sudosh' or $programname == 'yum' or $syslogfacility-text == 'cron' or $syslogfacility-text == 'authpriv' or $syslogfacility-text == 'local5' or $syslogfacility-text == 'local6' or $syslogfacility-text == 'local7' or $syslogpriority-text == 'emerg' or ( $syslogfacility-text == 'kern' and $msg startswith 'IPT:' ) then
    call simp_stock_remote_ruleset

--------------

Configure auditd space\_left Action on Low Disk Space
-----------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_auditd\_data\_retention\_space\_left\_action
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not match the ``Description``

--------------

Configure auditd admin\_space\_left Action on Low Disk Space
------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_auditd\_data\_retention\_admin\_space\_left\_action
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not match the ``Description``

--------------

Configure auditd flush priority
-------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_auditd\_data\_retention\_flush
-  Type: Exception

Justification
~~~~~~~~~~~~~

| During use, the SIMP team found that setting the ``auditd`` ``flush``
  parameter
| to ``data`` caused kernel-level locking far too often to be reasonable
  under
| heavy workloads.

If you wish to make this compliant, you can use the following Hiera
settings:

.. code:: yaml

    ---
    auditd::flush: 'DATA'

--------------

Record attempts to alter time through adjtimex
----------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_time\_adjtimex
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

     grep adjtimex /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday -k audit_time_rules
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b64 -S adjtimex -S clock_settime -S settimeofday -k audit_time_rules

--------------

Record Attempts to Alter Time Through stime
-------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_time\_stime
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep stime /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday -k audit_time_rules

--------------

Record Attempts to Alter Time Through clock\_settime
----------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_time\_clock\_settime
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

     grep clock_settime /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday -k audit_time_rules
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b64 -S adjtimex -S clock_settime -S settimeofday -k audit_time_rules

--------------

Record Events that Modify the System's Discretionary Access Controls - chmod
----------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_chmod
-  Type: Finding

Notes
~~~~~

This should be filed as a SIMP bug.

| Note: Logging all ``chmod`` calls would likely result in a system
  denial of
| service if done for all users.

--------------

Record Events that Modify the System's Discretionary Access Controls - chown
----------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_chown
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep chown /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - fchmod
-----------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_fchmod

-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep fchmod /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - fchmodat
-------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_fchmodat
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep fchmodat /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - fchown
-----------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_fchown
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep fchown /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - fchownat
-------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_fchownat
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep fchownat /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - fremovexattr
-----------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_fremovexattr
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep fremovexattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - fsetxattr
--------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_fsetxattr
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep fsetxattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - lchown
-----------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_lchown
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep lchown /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - lremovexattr
-----------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_lremovexattr
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep lremovexattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - lsetxattr
--------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_lsetxattr
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep lsetxattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - removexattr
----------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_removexattr
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep removexattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Events that Modify the System's Discretionary Access Controls - setxattr
-------------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_dac\_modification\_setxattr
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan does not properly handle optimized rules which are
      recommended by
      the prose guide

Notes
~~~~~

.. code:: shell

    grep setxattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

Record Attempts to Alter Logon and Logout Events
------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_login\_events
-  Type: Finding - Partial
-  **Requested feedback from the SSG team**

   -  While valid, this watch creates a lot of unnecessary noise since it
      is
      triggered on every login regardless of attempted edits to files
   -  This should be a new rule, the name is misleading
-  **Pending SIMP Fix**

   -  SIMP should be enhanced to watch the missing entries

Notes
~~~~~

The audit daemon **does** track all login and logout events by default.

SIMP contains the rule for ``lastlog`` but it needs the rules for
``tallylog`` and ``faillock``.

--------------

Ensure auditd Collects Unauthorized Access Attempts to Files (unsuccessful)
---------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_unsuccessful\_file\_modification
-  Type: Finding
-  **Requested feedback from the SSG team**

   -  Once fixed in SIMP, this will still trigger since we have additional
      optimizations
-  **Pending SIMP Fix**

   -  The following system calls need to be added to the ``-k access``
      list:

   -  ``open_by_handle_at``

Notes
~~~~~

The remainder of the checks (plus additional ones) are already covered

--------------

Ensure auditd Collects Information on the Use of Privileged Commands
--------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_privileged\_commands
-  Type: Alternate Implementation
-  **Requested feedback from the SSG team**

   -  The rule that is dictated by the SSG relies on generating file lists
      and is
      untenable over time as well as being file system intensive when it is
      run.
      It also misses suid/sgid binaries that are run on remote partitions.

Justification
~~~~~~~~~~~~~

| The SIMP audit rules check for binary execution where the ``auid`` is
  not ``0`` and
| the ``uid`` is ``0``. This should capture the execution of any
  ``suid`` binary
| regardless of location.

.. code:: shell

    grep su-root-activity /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -F auid!=0 -F uid=0 -S capset -S mknod -S pivot_root -S quotactl -S setsid -S settimeofday -S setuid -S swapoff -S swapon -k su-root-activity
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -F auid!=0 -F uid=0 -S capset -S mknod -S pivot_root -S quotactl -S setsid -S settimeofday -S setuid -S swapoff -S swapon -k su-root-activity

--------------

Ensure auditd Collects Information on Exporting to Media (successful)
---------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_media\_export
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan should be checking for the sysetm calls and not match any
      tags or extra information

Notes
~~~~~

.. code:: shell

    grep mount /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S mount -S umount -S umount2 -k mount
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S mount -S umount2 -k mount

--------------

Ensure auditd Collects File Deletion Events by User
---------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_file\_deletion\_events
-  Type: False Positive

Notes
~~~~~

| These were optimized and added to the other rules that fail against
  ``EACCES``
| and ``EPERM`` to help reduce load on the system.

.. code:: shell

     grep unlinkat /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EACCES -k access
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EPERM -k access
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EACCES -k access
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EPERM -k access

--------------

Ensure auditd Collects Information on Kernel Module Loading and Unloading
-------------------------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_audit\_rules\_kernel\_module\_loading
-  Type: Finding and Bug in SSG
-  **Requested feedback from the SSG team**

   -  EL6 systems only have ``/sbin/insmod``. EL7 systems have
      ``/sbin/insmod`` and
      ``/usr/sbin/insmod``. All of these are symlinks that point back to
      ``/bin/kmod``. All should be watched.
-  **Pending SIMP Fix**

   -  SIMP should add the additional paths as watches

--------------

Make the auditd Configuration Immutable
---------------------------------------

-  Rule ID: xccdf\_org.ssgproject.content\_rule\_audit\_rules\_immutable
-  Type: Will not do

Justification
~~~~~~~~~~~~~

SIMP uses Puppet to automate the management of the audit rules and these
rules may change over time based on the addition of different management
capabilities.

Modifying the audit rules requires a system reboot if they are made
immutable which means that adding system capabilities may require
routine system reboots as the purpose of the system is expanded.

If you wish to make the rules immutable, you can set the following:

.. code:: yaml

    ---
    auditd::immutable: true

--------------

Remove telnet Clients
---------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_package\_telnet\_removed
-  Type: Finding
-  **Requested feedback from the SSG team**

   -  ``telnet`` is a valid systems troubleshooting tool. Given that no
      system on
      the network should *allow* ``telnet`` login connections, the presence
      of
      ``telnet`` on the system should not be a finding.

Notes
~~~~~

| The SIMP team is already planning to remove ``telnet`` as a default
  package in
| future updates. However, the presence of a *client* application that
  is
| commonly used for troubleshooting connectivity issues should not be a
  finding.

--------------

Allow Only SSH Protocol 2
-------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_allow\_only\_protocol2
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  If the system default passes then it should pass

Notes
~~~~~

This is the system default

--------------

Disable Kerberos Authentication
-------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_disable\_kerb\_auth
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  If the system default passes then it should pass

Notes
~~~~~

This is the system default

--------------

Enable Use of Strict Mode Checking
----------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_enable\_strictmodes
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  If the system default passes then it should pass

Notes
~~~~~

This is the system default

--------------

Set SSH Idle Timeout Interval
-----------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_set\_idle\_timeout
-  Type: Will Not Do
-  **Requested feedback from the SSG team**

   -  While this is laudable, all of our shell connections have the
      ``TMOUT``
      parameter set. Additionally, it was found that enabling this in the
      field
      caused extreme disruption in workflow. For instance, sessions would
      timeout
      when working across multiple windows on complex issues and while
      reading
      ``man`` pages or logs during troubleshooting. Request that SSG team
      live with
      this setting on non-GUI systems before attempting to enforce it.

--------------

Do Not Allow SSH Environment Options
------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_do\_not\_permit\_user\_env
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  If the system default passes then it should pass

Notes
~~~~~

This is the system default

--------------

Use Only Approved Ciphers
-------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_use\_approved\_ciphers
-  Type: Finding - Partial
-  **Pending SIMP Fix**

   -  The system presently falls back to ``CBC`` ciphers for cross-system
      compatibility reasons. These should be changed to ``CTR`` to meet the
      guide.
      Some network devices may not be able to login to the system.
-  **Requested feedback from the SSG team**

   -  The scan should only check for FIPS ciphers if the system is
      operating in
      FIPS mode (kernel ``fips=1``). If the system is not running in FIPS
      mode,
      stronger MACs should be allowed.

.. code:: shell

    fipscheck
    usage: fipscheck [-s <hmac-suffix>] <paths-to-files>
    fips mode is off

    grep Ciphers /etc/ssh/sshd_config
    # Ciphers and keying
    Ciphers aes256-gcm@openssh.com,aes128-gcm@openssh.com,aes256-cbc,aes192-cbc,aes128-cbc

--------------

Use Only FIPS Approved MACs
---------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_sshd\_use\_approved\_macs
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan should only check for FIPS ciphers if the system is
      operating in
      FIPS mode (kernel ``fips=1``). If the system is not running in FIPS
      mode,
      stronger MACs should be allowed.

.. code:: shell

    fipscheck
    usage: fipscheck [-s <hmac-suffix>] <paths-to-files>
    fips mode is off

    grep MAC /etc/ssh/sshd_config
    MACs hmac-sha2-512-etm@openssh.com,hmac-sha2-256-etm@openssh.com,hmac-sha2-512,hmac-sha2-256

--------------

Verify Permissions on SSH Server Private \*\_key Key Files
----------------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_file\_permissions\_sshd\_private\_key
-  Type: False Positive
-  **Requested feedback from the SSG team**
  
   -  The system generated keys have a group of ``ssh_keys``, this should
      probably remain.
   -  Also, mode ``640 root:root`` is no less secure than ``400 root:root``
      as long
      as root group membership is limited (which it should be)

Notes
~~~~~

.. code:: shell

     ll /etc/ssh/*_key
    -rw-r-----. 1 root ssh_keys  227 Dec  6 16:55 /etc/ssh/ssh_host_ecdsa_key
    -rw-r-----. 1 root ssh_keys  387 Dec  6 16:55 /etc/ssh/ssh_host_ed25519_key
    -rw-------. 1 root root     6552 Dec 19 12:58 /etc/ssh/ssh_host_rsa_key

--------------

Configure LDAP Client to Use TLS For All Transactions
-----------------------------------------------------

-  Rule ID:
   xccdf\_org.ssgproject.content\_rule\_ldap\_client\_start\_tls
-  Type: False Positive
-  **Requested feedback from the SSG team**

   -  The scan should not assume that ``authconfig`` is being used and
      should
      simply check the system
   -  This may also be affected by the use of ``sssd`` which would
      completely
      preclude the use of the ``pam_ldap.conf`` settings

Notes
~~~~~

.. code:: shell

    grep -i ssl /etc/pam_ldap.conf
    ssl start_tls
    tls_ciphers HIGH:-SSLv2

    grep -i ssl /etc/sssd/sssd.conf
    ldap_tls_cipher_suite = HIGH:-SSLv2

.. _SCAP Security Guide: https://www.open-scap.org/security-policies/scap-security-guide/
.. _SCAP: https://scap.nist.gov

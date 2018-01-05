SSG Scan - CentOS 7 STIG
========================

* Scan Date: 12/29/2017
* SIMP Version: ``6.1.0-0``
* SSG Version: ``0.1.36-5``
* Data Stream: ``ssg-rhel7-ds.xml``
* SIMP Enforcement Profile: ``disa_stig``

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_ensure_gpgcheck_never_disabled``
-  Type: Finding
-  Remediation:

   This is not yet managed by SIMP and was not a finding until the RHEL 7
   baseline. Given that the baseline is in Draft form, it has not been
   marked as a priority.

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_ensure_gpgcheck_repo_metadata``
-  Type: Exception
-  Notes:

  -  Recommend SSG Feedback

    -  This should not be a High finding if using TLS
    -  This opens potential vulnerabilities to the system

-  Remediation:

   Unfortunately, the way that YUM works means that all GPG keys become
   *trusted* by the entire system. Enabling repository metadata signatures
   means that RPMs will be trusted that come from any of these GPG systems
   and may allow inappropriate software to be installed on systems.

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_aide_build_database``
-  Type: False Positive
-  Remediation:

.. code:: shell

    ls /var/lib/aide/
    aide.db.gz

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_aide_periodic_cron_checking``
-  Type: Alternate Implementation - Dangerous
-  Notes:
  -  Recommend SSG Feedback
    -  Enabling the aide scan via regular ``cron`` should be valid
-  Remediation:

This is **not** enabled in SIMP by default since it can be an extreme
burden on your system depending on your partitioning.

If you wish to enable this, you can use the following Hiera data:

.. code:: yaml

    ---
    aide::enable: true

This is implemented using the native ``cron`` Puppet resource and,
therefore, is placed into the root crontab directly.

.. code:: shell

    22 4 * * 0 /bin/nice -n 19 /usr/sbin/aide -C

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_aide_scan_notification``
-  Type: Exception - Dangerous
-  Notes:
  -  Recommend SIMP Feedback
    -  We should expose ``aide::set_schedule`` command/user so users can easily
       tweak Hiera data, and add compliant values in compliance data
       ``(05 4 * * * root /usr/sbin/aide --check | /bin/mail -s \"$(hostname)- AIDE Integrity Check\" root@localhost)``
-  Remediation:

This is not enabled in SIMP by default since it can be an extreme burden on your system depending on your partitioning.

If you wish to enable this, you can add the following Hiera data:

.. code:: yaml

    ---
    aide::enable: false

Add the following to a manifest:

.. code:: ruby

    cron { 'aide_schedule':
      command  => '/bin/nice -n 19 /usr/sbin/aide -C | /bin/mail -s "$(hostname) - AIDE Integrity Check" root@localhost'
      user     => 'root',
      minute   => $minute,
      hour     => $hour,
      monthday => $monthday,
      month    => $month,
      weekday  => $weekday
    }

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_aide_verify_ext_attributes``
-  Type: Finding
-  Notes:
  -  Recommend SIMP Feedback
    -  We should create a stig-compliant profile for aide::aliases
-  Remediation:

Modify aide::aliases per ssg feedback

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_adie_use_fips_hashes``
-  Type: Finding
-  Notes:
  -  Recommend SIMP Feedback
    -  We should create a stig-compliant profile for aide::aliases
-  Remediation:

Modify aide::aliases per ssg feedback

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_rpm_verify_permissions``
-  Type: Mixed - Mostly False Positives
-  Notes:
-  Recommend SSG Feedback
  -  Permissions that are obviously more restrictive should not be flagged
-  Remediation:

Most files have *more restrictive* permissions than the permissions
provided by the RPMs.

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

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_rpm_verify_hashes``
-  Type: Finding
-  Remediation:

**TODO**

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_install_mcafee_antivirus``
-  Type: Altertate Implementation
-  Remediation:

We use ClamAV in place of Mcafee, and it is enabled by default.

If ClamAV is *not* enabled, set the following in Hiera data:

.. code:: yaml

    ---
    classes:
      - clamav

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_grub2_enable_fips_mode``
-  Type: Finding
-  Remediation:

**TODO**

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_instaltled_OS_is_certified``
-  Type: False Positive
-  Remediation:

It is the job of the vendor to ensure the OS is maintained and certified

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_install_antivirus``
-  Type: False Positive
-  Remediation:

.. code:: shell

    rpm -q clamav
    clamav-0.99.2-1.el7.x86_64

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_sudo_remove_nopasswd``
-  Type: Exception
-  Notes:
  -  Recommend SSG Feedback
    -  Need rules based around SSH-only systems
    -  Passwords are known to be less secure than keys (as long as the keys
       are properly protected)
-  Remediation:

It is generally recommended that SIMP systems do not use passwords on
systems and only allow authentication via SSH keys. This necessarily
precludes the use of passwords to authenticate via ``sudo``.

This may be configured differently and, by default, is restricted to
only the ``administrators`` and ``security`` groups.

.. code:: shell

     cat /etc/sudoers | grep NOP
     %administrators    ALL=(root) NOPASSWD:EXEC:SETENV: /bin/rm -rf /etc/puppetlabs/puppet/ssl
     %administrators    ALL=(ALL) NOPASSWD:EXEC:SETENV: /usr/bin/sudosh
     %administrators    ALL=(root) NOPASSWD:EXEC:SETENV: /usr/sbin/puppetca
     %administrators    ALL=(root) NOPASSWD:EXEC:SETENV: /usr/sbin/puppetd
     %security    ALL=(root) NOPASSWD:EXEC:SETENV: AUDIT

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_bootloader_nousb_argument``
-  Type: Exception - Dangerous
-  Notes:
-  Recommend SIMP Enhancement Request
-  Remediation:

Disabling global USB is *extremely* dangerous and will, most likely,
cripple the ability to update systems and troubleshoot systems at all
given that most modern systems no longer make USB keyboards and mice
available.

SIMP attempts to be sensible and disable block device connections
instead.

An enhancement request could be filed against SIMP to allow setting this
kernel parameter but it should *not* be set by default unless no USB
devices are detected on the system.

.. code:: shell

    cat /etc/modprobe.d/00_simp_blacklist.conf
    # This file managed by Puppet.
    install ieee1394 /bin/true
    install usb-storage /bin/true

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_no_files_unowned_by_user``
-  Type: Exception
-  Remediation:

The SIMP server serves files over encrypted ``rsync`` which require
proper **numeric** ownership after transfer. The server, not requiring
the ``rsync`` specified users will show the files as unknowned. This is
**correct** and must not be modified if the client systems are to
maintain proper functionality.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_file_permissions_ungroupowned``
-  Type: Exception
-  Remediation:

The SIMP server serves files over encrypted ``rsync`` which require
proper **numeric** ownership after transfer. The server, not requiring
the ``rsync`` specified users will show the files as unknowned. This is
**correct** and must not be modified if the client systems are to
maintain proper functionality.

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_dir_perms_world_writable_system_owned``
-  Type: Finding
-  Remediation:

**TODO**

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_umask_for_daemons``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
  -  The check should be fixed
-  Remediation:

The policy allows for ``022`` or ``027`` but the check only checks for
``022``.

Using a default umask of ``022`` caused too many daemons to fail and
caused a **very** high instance of troubleshooting overhead.

.. code:: shell

    grep umask /etc/init.d/functions
    # Make sure umask is sane
    umask 0027

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_selinux_confinement_of_daemons``
-  Type: Exception
-  Notes:
-  Recommend RedHat Feedback
  -  An SELinux policy should be shipped for running rsync in daemon mode
-  Remediation:

Rsync does not presently have a vendor supplied policy for running in
daemon mode at start time but running in daemon mode is supported via
``/etc/rsyncd.conf``. The vendor should supply documentation and/or a
policy for running ``rsync`` in daemon mode and restricting content
access when running from the ``init`` system.

Since SIMP systems need to transfer contexts to client systems, it is
likely that the ``rsync_full_access`` SELinux boolean will need to be
set so that ``rsync`` can properly access the files within the rsync
share.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_selinux_all_devicefiles_labeled``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  Remediation:

This check simply appears to be broken

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_no_direct_root_logins``
-  Type: Exception
-  Remediation:

Removing all ability for Root to login from the console prevents "last
effort" recovery of systems. This is not something that SIMP will enable
by default.

You can make this compliant by setting the following in Hiera:

.. code:: yaml

    ---
    simplib::securetty : []

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_restrict_serial_port_logins``
-  Type: Exception
-  Remediation:

Removing all ability for Root to login from serial ports prevents "last
effort" recovery of remote systems. This is not something that SIMP will
enable by default.

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

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_maximum_age_login_defs``
-  Type: Exception
-  Remediation:

SIMP sets ``PASS_MAX_DAYS`` to ``180`` by default per most common
guidance.

The scan checks for ``60`` days but this tends to be too short for the
enforced password complexity requirements.

If you need a shorter duration set the following in Hiera:

.. code:: yaml

    ---
    simplib::login_defs::pass_max_days: '60'

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_account_disable_post_pw_expiration``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  Simply a badly formed check
-  Remediation:

The check is incorrect.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_retry``
-  Type: Alternate Implementation
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -o retry=3 /etc/pam.d/system-auth
    retry=3

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_maxrepeat``
-  Type: Alternate Implementation
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

     grep -o maxrepeat /etc/pam.d/system-auth
    maxrepeat

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_maxclassrepeat``
-  Type: Alternate Implementation - Finding
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

     grep -o maxclassrepeat /etc/pam.d/system-auth
    maxclassrepeat=0

Maxclassrepeat is set to ``0`` (not enforced) by default because we
found that it was too difficult for users to come up with passwords that
could meet all requirements when enabled.

To enable this, with a value of ``4``, use the following in Hiera:

.. code:: yaml

    ---
    pam::cracklib_maxclassrepeat: '4'

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_dcredit``
-  Type: Alternate Implementation
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "dcredit=.*? "  /etc/pam.d/system-auth
    dcredit=-1

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_minlen``
-  Type: Alternate Implementation - Finding
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

     grep -Po "minlen=.*? "  /etc/pam.d/system-auth
    minlen=14

The ``minlen`` requirements vary **vastly** between policy documents.
The previous requirement was ``14`` and is has been changed to ``15``.

This can be made compliant using the following Hieradata:

.. code:: yaml

    ---
    pam::cracklib_minlen: '15'

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_ucredit``
-  Type: Alternate Implementation
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "ucredit=.*? "  /etc/pam.d/system-auth
    ucredit=-1

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_lcredit``
-  Type: Alternate Implementation
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "lcredit=.*? "  /etc/pam.d/system-auth
    lcredit=-1

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_password_pam_difok``
-  Type: Alternate Implementation - Finding
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

     grep -Po "difok=.*? "  /etc/pam.d/system-auth
     difok=8

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_accounts_password_pam_minclass``
-  Type: Alternate Implementation - False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  This should be combined with/overridden by the ``*credit`` checks
-  Remediation:

The policy indicates that ``pam_cracklib`` may be used in lieu of
``pam_pwquality``. SIMP has not yet changed to use ``pam_pwquality``.

.. code:: shell

    grep -Po "minclass=.*? "  /etc/pam.d/system-auth
    minclass=4

Though ``minclass`` is set to ``4``, setting the ``*credit`` items to
``-1`` ensures that they must be used in the password which renders this
setting useless.

Nevertheless, it should be changed in SIMP to match the scan.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_passwords_pam_faillock_deny``
-  Type: Exception
-  Remediation:

.. code:: shell

    grep -P "deny=.*? "  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900

Setting ``deny`` to less than ``5`` was causing premature lockouts when
presented with alternate authentication systems and also, at times, when
using ``sudo`` and attempting to ``^C`` out of the session. This may be
fixed in the latest releases of RHEL, but has not been verified.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_passwords_pam_faillock_unlock_time``
-  Type: Exception
-  Notes:
  -  Recommend SSG Feedback
    -  The defaults are unreasonable for production systems and should be
       changed
-  Remediation:

Waiting for more than ``15`` minutes is not conducive to effective
security and causes a heavy burden on helpdesk systems relating to
password resets where the user remembers their password but simply typed
it incorrectly multiple times.

Even the most rudmentary log auditing system should be able to identify
repeated failed logins over multi-15 minute boundaries.

.. code:: shell

    grep -P "unlock_time=.*? "  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900

This can be made compliant using the following Hieradata:

.. code:: yaml

    ---
    pam::unlock_time: 604800

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_passwords_pam_faillock_deny_root``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  False Positive
-  Remediation:

System value:

.. code:: shell

    grep -P "unlock_time=.*? "  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_accounts_passwords_pam_faillock_interval``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The position before, or after, ``pam_unix.so`` is irrelevant if
       ``pam_unix.so`` is set to ``required`` and not ``sufficient``
  -  Recommend SIMP Feedback
    -  SIMP should go ahead and fix this so that the scans do not fail
-  Remediation:

System value:

.. code:: shell

    grep -P "faillock"  /etc/pam.d/system-auth
    auth     required      pam_faillock.so preauth silent deny=5 even_deny_root audit unlock_time=900 root_unlock_time=60 fail_interval=900
    account     required      pam_faillock.so

--------------

-  Rule ID:  ``xccdf_org.ssgproject.content_rule_accounts_umask_etc_login_defs``
-  Type:  Finding
-  Remediation:

We default the UMASK to 007 because 077 is too difficult to work with everywhere.
Recommend changing locally, as needed.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_accounts_have_homedir_login_defs``
-  Type:  False Positive
-  Remediation:

System value:

.. code:: bash

    grep CREATE_HOME /etc/login.defs 
    CREATE_HOME yes

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_accounts_tmout``
-  Type: Finding
-  Remediation:

SIMP manages TMOUT in ``/etc/profile.d/simp.*``. SIMP defaults to a timeout
of 15, but it can be changed to 10 by setting the following in Hiera data:

.. code:: yaml

    ---
    useradd::etc_profile::session_timeout: 10

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_bootloader_password``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  False Positive
-  Remediation:

The script should check the **built** ``/etc/grub2.cfg``. Checking the
configuration files is not useful if they have not been applied.

.. code:: shell

    grep pbkdf /etc/grub2.cfg
        password_pbkdf2 root grub.pbkdf2.sha512.10000.83E1E6452551

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_package_screen_installed``
-  Type: Finding
-  Notes:
  - Recommend SIMP Feedback
    - We should manage the screen package
-  Remediation:

SIMP does not manage the screen package by default. ``yum install screen``

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_smartcard_auth``
-  Type: Finding
-  Remediation:

SIMP does not currently support smart card (CAC) authentication, but
development is in progress.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_disable_ctrlaltdel_reboot``
-  Type: Finding
-  Remediation:

By default, SIMP disables ctrl-alt-del reboot and creates a logged entry,
if pressed.  To disable per the STIG recommendations, set the following in
Hiera data:

.. code:: yaml

    ---
    simp::ctrl_alt_del::enable: false
    simp::ctrl_alt_del::log: false

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_banner_etc_issue``
-  Type: False Positive
-  Notes:
  -  Recommend SIMP Feedback
    - We should add a us_dod_stig profile
-  Remediation:

There is a login banner, but it is not the DoD default.

Set the following in Hiera Data:

.. code:: yaml

    ---
    issue::profile: us_dod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sysctl_net_ipv4_ip_forward``
-  Type: Exception
-  Notes:
  -  Recommend SSG Discussion
    -  Almost all systems run containers, namespaces, or VMs these days
  -  Recommend SIMP Feedback
    -  We should add the option to toggle ipv4 forwarding to simp::sysctl
-  Remediation:

This is an antequated rule given that almost all environments run
subsystems that require some sort of internal routing. To support these
subsystems, SIMP needs to manage IP forwarding rules elsewhere and the
system **defaults** are correct.

To disable ipv4 forwarding, include the following in a manifest:

.. code:: ruby

    sysctl { "net.ipv4.ip_forward":
      ensure => present,
      value  => "0",
    }

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sysctl_net_ipv6_conf_all_accept_source_route``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  Per the Description, the check is incorrect
  -  Recommend SIMP Feedback
    - We should add a setting to explicitly set
      ``net.ipv6.conf.all.accept_source_route=0`` to simp::sysctl
-  Remediation:

System value:

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

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_service_firewalld_enabled``
-  Type: Alternate Implementation
-  Notes:
  -  Recommend SSG Feedback
    -  The scan should allow for either ``firewalld`` or ``iptables`` since
       the policy does
-  Remediation:

To use the same code to manage both EL6 and EL7 systems, SIMP manages
``iptables`` directly. Additionally, for server systems, most admins
that we have encountered find it easier to deal with direct IPTables
rules when debugging firewall issues.

Finally, ``firewalld`` hooks into ``dbus`` which opens the possibility
of software that can independently manage firewall settings at run time
without explicit authorization.

When EL6 is no longer supported SIMP may move to having ``firewalld``
support, but not before then.

.. code:: shell

     systemctl status iptables
     ● iptables.service - LSB: start and stop iptables firewall
       Loaded: loaded (/etc/rc.d/init.d/iptables)
       Active: active (exited) since Thu 2016-12-22 20:52:06 GMT; 1 weeks 0 days ago
         Docs: man:systemd-sysv-generator(8

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_set_firewalld_default_zone``
-  Type: Alternate Implementation
-  Notes:
  -  Recommend SSG Feedback
    -  The scan should allow for either ``firewalld`` or ``iptables`` since
       the policy does
-  Remediation:

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

-  Rule ID: ``xccdf_org:ssgproject:content_rule_network_configure_name_resolution``
-  Type: Finding
-  Remediation:

SIMP cannot pre determine an environment's DNS servers.  To specify them, set the following in Hiera data:

.. code:: yaml

    ---
    simp_options::dns::servers: ['1.2.3.4','5.6.7.8']

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_rsyslog_cron_logging``
-  Type: False Positive
-  Remediation:

By default, cron is logged, per simp_rsyslog::default_logs.

System value:

.. code:: bash

    grep cron /etc/rsyslog.simp.d/99_simp_local/ZZ_default.conf 
    *.info;mail.none;authpriv.none;cron.none;local6.none;local5.none action(type="omfile" file="/var/log/messages")
    cron.*  action(type="omfile" file="/var/log/cron")

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_rsyslog_remote_loghost``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The scan does not take into account the new Rainerscript format and
       does not process the full configuration
-  Remediation:

To set up a remote log server, follow the SIMP documentation https://simp.readthedocs.io/en/master/user_guide/HOWTO/Central_Log_Collection/Rsyslog.html. Once set up,
the scan may still fail, since it does not take into account the new Rainerscript
format and does not process the full configuration.

System value:

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

-  Rule ID: ``xccdf_org:ssgproject:content_rule_service_kdump_disabled``
-  Type: Finding
-  Remediation:

SIMP does not disable kdump by default.  To stop the service and disable it,
add the following to a manifest:

.. code:: ruby

    service { \'kdump\':
      ensure => \'stopped\',
      enable => false
    }

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_auditd_data_retention_space_left_action``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not match the ``Description``

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_auditd_data_retention_admin_space_left_action``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not match the ``Description``

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_auditd_data_retention_flush``
-  Type: Exception
-  Remediation:

During use, the SIMP team found that setting the ``auditd`` ``flush``
parameter to ``data`` caused kernel-level locking far too often to be
reasonable under heavy workloads.

If you wish to make this compliant, you can use the following Hiera
settings:

.. code:: yaml

    ---
    auditd::flush: 'DATA'

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_time_adjtimex``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

     grep adjtimex /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday -k audit_time_rules
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b64 -S adjtimex -S clock_settime -S settimeofday -k audit_time_rules

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_time_stime``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep stime /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday -k audit_time_rules

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_time_clock_settime``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

     grep clock_settime /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday -k audit_time_rules
    /etc/audit/rules.d/50_base.rules:-a exit,always -F arch=b64 -S adjtimex -S clock_settime -S settimeofday -k audit_time_rules

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_chmod``
-  Type: Finding
-  Remediation:

This should be filed as a SIMP bug.

Note: Logging all ``chmod`` calls would likely result in a system denial
of service if done for all users.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_chown``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep chown /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_fchmod``

-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep fchmod /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_fchmodat``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep fchmodat /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_fchown``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep fchown /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_fchownat``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep fchownat /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_fremovexattr``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep fremovexattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_fsetxattr``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep fsetxattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_lchown``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep lchown /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_lremovexattr``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep lremovexattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_lsetxattr``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep lsetxattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_removexattr``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep removexattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_dac_modification_setxattr``
-  Type: False Positive
-  Notes:
-  Recommend SSG Feedback
-  The scan does not properly handle optimized rules which are
   recommended by the prose guide
-  Remediation:

.. code:: shell

    grep setxattr /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr -S fremovexattr -k perm_mod

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_login_events``
-  Type: Finding - Partial
-  Notes:
-  Recommend SSG Feedback
-  While valid, this watch creates a lot of unnecessary noise since it
   is triggered on every login regardless of attempted edits to files
-  This should be a new rule, the name is misleading
-  Notes:
-  Recommend SIMP Feedback
-  SIMP should be enhanced to watch the missing entries
-  Remediation:

The audit daemon **does** track all login and logout events by default.

SIMP contains the rule for ``lastlog`` but it needs the rules for
``tallylog`` and ``faillock``.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_unsuccessful_file_modification``
-  Type: Finding
-  Notes:
-  Recommend SSG Feedback
-  Once fixed in SIMP, this will still trigger since we have additional
   optimizations
-  Notes:
-  Recommend SIMP Feedback
-  The following system calls need to be added to the ``-k access``
   list:

   -  ``open_by_handle_at``
-  Remediation:

The remainder of the checks (plus additional ones) are already covered

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_privileged_commands``
-  Type: Alternate Implementation
-  Notes:
  -  Recommend SSG Feedback
    -  The rule that is dictated by the SSG relies on generating file lists
       and is untenable over time as well as being file system intensive
       when it is run. It also misses suid/sgid binaries that are run on
       remote partitions.
-  Remediation:

The SIMP audit rules check for binary execution where the ``auid`` is
not ``0`` and the ``uid`` is ``0``. This should capture the execution of
any ``suid`` binary regardless of location.

.. code:: shell

    grep su-root-activity /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -F auid!=0 -F uid=0 -S capset -S mknod -S pivot_root -S quotactl -S setsid -S settimeofday -S setuid -S swapoff -S swapon -k su-root-activity
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -F auid!=0 -F uid=0 -S capset -S mknod -S pivot_root -S quotactl -S setsid -S settimeofday -S setuid -S swapoff -S swapon -k su-root-activity

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_media_export``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The scan should be checking for the sysetm calls and not match any
     tags or extra information
-  Remediation:

System value:

.. code:: shell

    grep mount /etc/audit/rules.d/*
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S mount -S umount -S umount2 -k mount
    /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S mount -S umount2 -k mount

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_file_deletion_events``
-  Type: False Positive
-  Remediation:

These were optimized and added to the other rules that fail against
``EACCES`` and ``EPERM`` to help reduce load on the system.

.. code:: shell

     grep unlinkat /etc/audit/rules.d/*
     /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EACCES -k access
     /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b64 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EPERM -k access
     /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EACCES -k access
     /etc/audit/rules.d/50_base.rules:-a always,exit -F arch=b32 -S creat -S mkdir -S mknod -S link -S symlink -S mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename -S renameat -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EPERM -k access

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_audit_rules_kernel_module_loading``
-  Type: Finding and Bug in SSG
-  Notes:
-  Recommend SSG Feedback
-  EL6 systems only have ``/sbin/insmod``. EL7 systems have
   ``/sbin/insmod`` and ``/usr/sbin/insmod``. All of these are symlinks
   that point back to ``/bin/kmod``. All should be watched.
-  Notes:
-  Recommend SIMP Feedback
-  SIMP should add the additional paths as watches

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_audit_rules_immutable``
-  Type: Will not do
-  Remediation:

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

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_package_telnet_removed``
-  Type: Finding
-  Notes:
-  Recommend SSG Feedback
-  ``telnet`` is a valid systems troubleshooting tool. Given that no
   system on the network should *allow* ``telnet`` login connections,
   the presence of ``telnet`` on the system should not be a finding.
-  Remediation:

The SIMP team is already planning to remove ``telnet`` as a default
package in future updates. However, the presence of a *client*
application that is commonly used for troubleshooting connectivity
issues should not be a finding.

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_allow_only_protocol2``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes then it should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set Protocol, set the following in a manifest:

.. code:: ruby

    sshd_config { 'Protocol': value => '2' }

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_disable_kerb_auth``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes then it should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set
KerberosAuthentication, set the following in a manifest:

.. code:: ruby

    sshd_config { 'KerberosAuthentication': value => 'no' }

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_enable_strictmodes``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes then it should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set
StrictModes, set the following in a manifest:

.. code:: ruby

    sshd_config { 'StrictModes': value => 'yes' }

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_set_idle_timeout``
-  Type: Will Not Do
-  Notes:
  -  Recommend SSG Feedback
    -  While this is laudable, all of our shell connections have the
       ``TMOUT`` parameter set. Additionally, it was found that enabling
       this in the field caused extreme disruption in workflow. For
       instance, sessions would timeout when working across multiple windows
       on complex issues and while reading ``man`` pages or logs during
       troubleshooting. Request that SSG team live with this setting on
       non-GUI systems before attempting to enforce it.
-  Remediation:

To explicitly set ClientAliveInterval, set the following in a manifest:

.. code:: ruby

  sshd_config { 'ClientAliveInterval': value => '600' }

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_sshd_set_keepalive``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes, the scan should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set ClientAliveCountMax, set the following in a manifest:

.. code:: ruby

    sshd_config { 'ClientAliveCountMax': value => '0' }

--------------

-  Rule ID: ``xccdf_org.ssgproject.content_rule_sshd_disable_user_known_hosts``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes, the scan should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set
IgnoreUserKnownHosts, set the following in a manifest:

.. code:: ruby

    sshd_config { 'IgnoreUserKnownHosts': value => 'yes' }

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_sshd_disable_rhosts_rsa``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes, the scan should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set
RhostsRSAAuthentication, set the following in a manifest:

.. code:: ruby

    sshd_config { 'RhostsRSAAuthentication': value => 'no' }

--------------


-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_do_not_permit_user_env``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  If the system default passes then it should pass
  -  Recommend SIMP Feedback
    -  Add this to ssh::server::conf
-  Remediation:

If the system default passes, the scan should pass.  To explicitly set
PermitUserEnvironment, set the following in a manifest:

.. code:: ruby

    sshd_config { 'PermitUserEnvironment': value => 'no' }

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_use_approved_ciphers``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The scan should only check for FIPS ciphers if the system is
       operating in FIPS mode (kernel ``fips=1``). If the system is not
       running in FIPS mode, stronger MACs should be allowed.
-  Remediation:

By default in FIPS mode, fallback ciphers will be used, which are all FIPS approved.

.. code:: bash

    grep Ciphers /etc/ssh/sshd_config 
    # Ciphers and keying
    Ciphers aes256-ctr,aes192-ctr,aes128-ctr

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_sshd_use_approved_macs``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The scan should only check for FIPS ciphers if the system is
       operating in FIPS mode (kernel ``fips=1``). If the system is not
       running in FIPS mode, stronger MACs should be allowed.
-  Remediation:

By default in FIPS mode, fips macs will be used, which are all FIPS approved.

.. code:: bash

    grep MAC /etc/ssh/sshd_config 
    MACs hmac-sha2-256,hmac-sha1

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_file_permissions_sshd_private_key``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The system generated keys have a group of ``ssh_keys``, this should
       probably remain.
    -  Also, mode ``640 root:root`` is no less secure than ``400 root:root``
       as long as root group membership is limited (which it should be)
-  Remediation:

.. code:: shell

     ll /etc/ssh/*_key
     -rw-r-----. 1 root ssh_keys  227 Dec  6 16:55 /etc/ssh/ssh_host_ecdsa_key
     -rw-r-----. 1 root ssh_keys  387 Dec  6 16:55 /etc/ssh/ssh_host_ed25519_key
     -rw-------. 1 root root     6552 Dec 19 12:58 /etc/ssh/ssh_host_rsa_key

--------------

-  Rule ID:   ``xccdf_org.ssgproject.content_rule_ldap_client_start_tls``
-  Type: False Positive
-  Notes:
  -  Recommend SSG Feedback
    -  The scan should not assume that ``authconfig`` is being used and
       should simply check the system
    -  This may also be affected by the use of ``sssd`` which would
       completely preclude the use of the ``pam_ldap.conf`` settings
-  Remediation:

.. code:: shell

    grep -i tls /etc/sssd/sssd.conf 
    ldap_id_use_start_tls = true
    
    grep -i ssl /etc/sssd/sssd.conf
    ldap_tls_cipher_suite = HIGH:-SSLv2

--------------

-  Rule ID:  ``xccdf_org:ssgproject:content_rule_snmpd_not_default_password``
-  Type: Finding
-  Remediation:

**TODO**

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_chmod``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_chown``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_fchmodat``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_fchown``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_fchownat``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_fremovexattr``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_fsetxattr``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_lchown``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_lremovexattr``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_lsetxattr``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_removexattr``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_setxattr``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_dac_modification_fchmod``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_login_events_faillock``
-  Type: False Positive
-  Remediation:

System value:

.. code:: bash

    grep faillock /etc/audit/rules.d/50_base.rules
    -w /var/run/faillock -p wa -k logins

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_unsuccessful_file_modification_create``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_unsuccessful_file_modification_open``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_unsuccessful_file_modification_openat``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_unsuccessful_file_modification_open_by_handle_at``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_unsuccessful_file_modification_truncate``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_unsuccessful_file_modification_ftruncate``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_execution_semanage``
-  Type: Finding
-  Notes:
  -  Recommend SIMP Feedback
    - SIMP should support this audit
-  Remediation:

SIMP does not currently support this audit

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_execution_setsebool``
-  Type: Finding
-  Notes:
  -  Recommend SIMP Feedback
    - SIMP should support this audit
-  Remediation:

SIMP does not currently support this audit

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_execution_chcon``
-  Type: Finding
-  Notes:
  -  Recommend SIMP Feedback
    - SIMP should support this audit
-  Remediation:

SIMP does not currently support this audit

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_execution_restorecon``
-  Type: Finding
-  Notes:
  -  Recommend SIMP Feedback
    - SIMP should support this audit
-  Remediation:

SIMP does not currently support this audit

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_file_deletion_events_rmdir``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_file_deletion_events_unlink``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_file_deletion_events_unlinkat``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_file_deletion_events_rename``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_file_deletion_events_renameat``
-  Type: False Positive
-  Remediation:

The scan does not properly handle optimized rules which are recommended by the
prose guide.  See /etc/audit/rules.d/50_base.rules.

--------------

-  Rule ID: ``xccdf_org:ssgproject:content_rule_audit_rules_system_shutdown``
-  Type: Finding
-  Remediation

**TODO**

--------------

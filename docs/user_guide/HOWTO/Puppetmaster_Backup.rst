HOWTO Back up the Puppet Master
===============================

This section details all of the steps required for backing up the Puppet
Master.

.. NOTE::

   SIMP, by default, provides two ways to back up data. They are BackupPC and
   Git. If there is a different preferred method, the user may install it and
   configure it first.

.. WARNING::

   BackupPC may, or may not, work properly for you on RHEL7+ systems. The SIMP
   team is currently evaluating other options for an inbuilt backup system.

1. Backup ``/etc/puppetlabs/puppet/ssl``
2. Backup ``/etc/puppetlabs/puppet``
3. Backup ``/srv/rsync``
4. Backup ``/var/simp``
5. Backup ``\`puppet config --section master print vardir\`/simp``
6. **Optional:** Backup ``/var/www``


**Simple Full Backup Command**

```bash
tar --selinux --xattrs -czpvf simp_backup-$(date +%Y-%m-%d).tar.gz /etc/puppetlabs /var/simp `puppet config --section master print vardir`/simp /var/www /var/simp
```

**Simple Full Restore Command**

```bash
# WARNING: This will overwrite your current system files!
tar --selinux --xattrs -C / -xzpvf simp_backup-<date>.tar.gz
```

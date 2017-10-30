.. _ug-howto-back-up-the-puppet-master:

HOWTO Back up the Puppet Master
===============================

This section details the steps required to back up the Puppet Master.

.. NOTE::

   A default SIMP installation can use Git as a rudimentary method to back up
   the Puppet master. If a different method is preferred, the user must install
   and configure it first.

.. WARNING::

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

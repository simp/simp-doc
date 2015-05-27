Backing up the Puppet Master
============================

This section details all of the steps required for backing up the Puppet
Master.

    **Note**

    SIMP, by default, provides two ways to back up data. They are
    BackupPC and Git. If there is a different preferred method the user
    may install it and configure it first.

    **Warning**

    BackupPC may, or may not, work properly for you on RHEL7+ systems.
    The SIMP team is currently evaluating other options for an inbuilt
    backup system.

+--------+------------------------------------------------+
| Step   | Process/Action                                 |
+========+================================================+
| 1.     | Backup */var/lib/puppet/ssl*                   |
+--------+------------------------------------------------+
| 1.     | Backup */etc/puppet*                           |
+--------+------------------------------------------------+
| 1.     | Backup */srv/rsync* and/or */var/simp/rsync*   |
+--------+------------------------------------------------+
| 1.     | **Optional:** Backup /var/www                  |
+--------+------------------------------------------------+

Table: SIMP Upgrade Process

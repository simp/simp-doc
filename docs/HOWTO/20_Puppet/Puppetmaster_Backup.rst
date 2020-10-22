.. _ug-howto-back-up-the-puppet-master:

Backing up the Puppet Server
============================

This section details the steps required to back up the Puppet master.

.. NOTE::

   A default SIMP installation can use Git as a rudimentary method to back up
   the Puppet server. If a different method is preferred, the user must install
   and configure it first.

#. Backup :file:`/etc/puppetlabs/puppet/ssl`
#. Backup :file:`/etc/puppetlabs/puppet`
#. Backup :file:`/var/simp`
#. Backup :file:`\`puppet config --section master print vardir\`/simp`
#. *Optional:* Backup :file:`/var/www`


**Simple Full Backup Command**

.. code-block:: bash

   # tar --selinux --xattrs -czpvf simp_backup-$(date +%Y-%m-%d).tar.gz /etc/puppetlabs /var/simp `puppet config --section master print vardir`/simp /var/www /var/simp

**Simple Full Restore Command**

.. code-block:: bash

   # WARNING: This will overwrite your current system files!
   tar --selinux --xattrs -C / -xzpvf simp_backup-<date>.tar.gz

.. NOTE::

   This only backs up data that is managed/owned by Puppet. It will not backup any
   application-centric data (such as the contents of LDAP).

.. _ug-howto-back-up-the-puppet-master:

HOWTO Back up the Puppet Server
===============================

This section details the steps required to back up the :term:`Puppet Server`.

.. NOTE::

   A default SIMP installation can use :term:`Git` as a rudimentary method to
   back up the Puppet Server. If a different method is preferred, the user must
   install and configure it first.

#. Backup :file:`/etc/puppetlabs/puppet/ssl`
#. Backup :file:`/etc/puppetlabs/puppet`
#. Backup :file:`/var/simp`
#. Backup :file:`\`puppet config --section server print vardir\`/simp`
#. *Optional:* Backup :file:`/var/www`


**Simple Full Backup Command**

.. code-block:: bash

   # tar --selinux --xattrs -czpvf simp_backup-$(date +%Y-%m-%d).tar.gz /etc/puppetlabs /var/simp `puppet config --section server print vardir`/simp /var/www /var/simp

**Simple Full Restore Command**

.. code-block:: bash

   # WARNING: This will overwrite your current system files!
   tar --selinux --xattrs -C / -xzpvf simp_backup-<date>.tar.gz

.. NOTE::

   This only backs up data that is managed/owned by Puppet. It will not backup any
   application-centric data (such as the contents of :term:`LDAP`).

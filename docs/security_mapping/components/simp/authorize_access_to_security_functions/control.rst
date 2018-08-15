Authorize Access to Security Functions
--------------------------------------

One of the main mechanisms to control access to security functions is the use of
sudo.  SIMP installs the following :term:`sudo` rules:

.. NOTE:
   The lack of a required password is due to the presumption that users will be
   using SSH keys, and not passwords, to access their systems.

.. list-table::
  :header-rows: 1

  * - Account
    - Sudo Commands
    - Run As Account
    - Password Required
  * - administrators
    - /bin/su - root -l
    - root
    - no
  * - administrators
    - /usr/sbin/puppetd
    - root
    - no
  * - administrators
    - /usr/sbin/puppetca
    - root
    - no
  * - administrators
    - /bin/rm -rf /var/lib/puppet/ssl
    - root
    - no
  * - auditors
    - /bin/cat,
      /bin/ls,
      /usr/bin/lsattr,
      /sbin/aureport,
      /sbin/ausearch,
      /sbin/lspci,
      /sbin/lsusb,
      /sbin/lsmod,
      /usr/sbin/lsof,
      /bin/netstat,
      /sbin/ifconfig -a,
      /sbin/route,
      /sbin/route -[venC],
      /usr/bin/getent,
      /usr/bin/tail
    - root
    - no

References: :ref:`AC-6 (1)`

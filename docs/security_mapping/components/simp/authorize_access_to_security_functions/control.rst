Authorize Access to Security Functions
--------------------------------------

One of the main mechanisms to control access to security functions is the use of
sudo.  SIMP installs the following sudo rules

.. list-table::
  :header-rows: 1

  * - Account
    - Sudo Commands
    - Run As Account
    - Password Required
  * - administrators
    - /usr/bin/sudosh
    - root
    - no
  * - administrators
    - /usr/sbin/puppetd
    - root
    - no
  * - administrators
    - /usr/sbin/puppeca
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

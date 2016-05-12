Audit Events
------------
SIMP audit rules were built by using industry best practices gathered over the
years. The heaviest reliance has been on the SCAP-Security Guide (SSG). SIMP
aims for a balance between performance and operational needs so the settings are
rarely an exact match from these guides.

The following audit rules are applied to SIMP systems:

::

  ## For audit 1.6.5 and higher
  ##

  # Ignore errors
  # This may sound counterintuitive, but we'd rather skip bad rules and load the
  # rest than miss half the file.  Warnings are still logged in the daemon
  # restart output.
  -i

  ## Remove any existing rules
  -D

  ## Continue loading rules on failure.
  # Particularly with the automatically generated nature of these rules in
  # Puppet, it is possible that one or more may fail to load. We want to continue
  # in that case so that we audit as much as possible.
  -c

  ## Increase buffer size to handle the increased number of messages.
  ## Feel free to increase this if the machine panic's
  # Default: 8192
  -b 32768

  ## Set failure mode to panic
  # Default: 2
  -f 1

  ## Rate limit messages
  # Default: 0
  # If you set this to non-zero, you almost definitely want to set -f to 1 above.
  -r 0

  ## Get rid of all anonymous and daemon junk.  It clogs up the logs and doesn't
  # do anyone # any good.
  -a exit,never -F auid=-1

  # Ignore system services. In most guides this is tagged onto every rule but
  # that just makes for more processing time.
  -a exit,never -F auid!=0 -F auid<500

  ## unsuccessful file operations
  -a always,exit -F arch=b64 -S creat -S mkdir -S mknod -S link -S symlink -S
  mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename
  -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EPERM -k access
  -a always,exit -F arch=b32 -S creat -S mkdir -S mknod -S link -S symlink -S
  mkdirat -S mknodat -S linkat -S symlinkat -S openat -S open -S close -S rename
  -S truncate -S ftruncate -S rmdir -S unlink -S unlinkat -F exit=-EPERM -k access

  -a always,exit -F perm=a -F exit=-EPERM -k access

  # Permissions auditing
  -a always,exit -F arch=b64 -S chown -S fchmod -S fchmodat -S fchown -S fchownat
  -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr
  -S fremovexattr -k perm_mod
  -a always,exit -F arch=b32 -S chown -S fchmod -S fchmodat -S fchown -S fchownat
  -S lchown -S setxattr -S lsetxattr -S fsetxattr -S removexattr -S lremovexattr
  -S fremovexattr -k perm_mod

  # Audit useful items that someone does when su'ing to root.
  # Had to add an entry at the top for getting rid of anonymous records.  They
  # are only moderately useful and contain *way* too much noise since this covers
  # things like cron as well.

  -a always,exit -F arch=b64 -F auid!=0 -F uid=0 -S capset -S mknod -S pivot_root
  -S quotactl -S setsid -S settimeofday -S setuid -S swapoff -S swapon -k
  su-root-activity
  -a always,exit -F arch=b32 -F auid!=0 -F uid=0 -S capset -S mknod -S pivot_root
  -S quotactl -S setsid -S settimeofday -S setuid -S swapoff -S swapon -k
  su-root-activity

  # Audit the execution of suid and sgid binaries.
  -a always,exit -F arch=b64 -F euid=0 -F uid!=0 -S execve -k suid-root-exec
  -a always,exit -F arch=b32 -F euid=0 -F uid!=0 -S execve -k suid-root-exec

  ## Audit the loading and unloading of kernel modules.
  -w /sbin/insmod -p x -k modules
  -w /sbin/rmmod -p x -k modules
  -w /sbin/modprobe -p x -k modules
  -a always,exit -F arch=b64 -S init_module -S delete_module -k modules
  -a always,exit -F arch=b32 -S init_module -S delete_module -k modules

  ## Things that could affect time
  -a exit,always -F arch=b32 -S adjtimex -S stime -S clock_settime -S settimeofday
  -k audit_time_rules
  -a exit,always -F arch=b64 -S adjtimex -S clock_settime -S settimeofday -k
  audit_time_rules

  -w /etc/localtime -p wa -k audit_time_rules

  ## Things that could affect system locale
  -a always,exit -F arch=b32 -S sethostname -S setdomainname -k
  audit_network_modifications
  -a always,exit -F arch=b64 -S sethostname -S setdomainname -k
  audit_network_modifications
  -w /etc/issue -p wa -k audit_network_modifications
  -w /etc/issue.net -p wa -k audit_network_modifications
  -w /etc/hosts -p wa -k audit_network_modifications
  -w /etc/sysconfig/network -p wa -k audit_network_modifications

  # Mount options.
  -a always,exit -F arch=b32 -S mount -S umount -S umount2 -k mount
  -a always,exit -F arch=b64 -S mount -S umount2 -k mount

  # audit umask changes.
  # This is uselessly noisy.
  # -a exit,always -S umask -k umask

  -w /etc/group -p wa -k audit_account_changes
  -w /etc/group- -p wa -k audit_account_changes
  -w /etc/passwd -p wa -k audit_account_changes
  -w /etc/passwd- -p wa -k audit_account_changes
  -w /etc/gshadow -p wa -k audit_account_changes
  -w /etc/shadow -p wa -k audit_account_changes
  -w /etc/shadow- -p wa -k audit_account_changes
  -w /etc/security/opasswd -p wa -k audit_account_changes

  -w /etc/selinux/ -p wa -k MAC-policy

  -w /var/log/faillog -p wa -k logins
  -w /var/log/lastlog -p wa -k logins

  -w /var/run/utmp -p wa -k session
  -w /var/run/btmp -p wa -k session
  -w /var/run/wtmp -p wa -k session

  -w /etc/sudoers -p wa -k CFG_sys

  # Generally good things to audit.
  -w /boot/grub/grub.conf -p wa -k CFG_grub
  -w /etc/aliases -p wa -k CFG_sys
  -w /etc/anacrontab -p wa -k CFG_cron
  -w /etc/at.deny -p wa -k CFG_sys
  -w /etc/bashrc -p wa -k CFG_shell
  -w /etc/cron.d -p wa -k CFG_cron
  -w /etc/cron.daily -p wa -k CFG_cron
  -w /etc/cron.deny -p wa -k CFG_cron
  -w /etc/cron.hourly -p wa -k CFG_cron
  -w /etc/cron.monthly -p wa -k CFG_cron
  -w /etc/cron.weekly -p wa -k CFG_cron
  -w /etc/crontab -p wa -k CFG_cron
  -w /etc/csh.cshrc -p wa -k CFG_shell
  -w /etc/csh.login -p wa -k CFG_shell
  -w /etc/default -p wa -k CFG_sys
  -w /etc/exports -p wa -k CFG_sys
  -w /etc/fstab -p wa -k CFG_sys
  -w /etc/host.conf -p wa -k CFG_sys
  -w /etc/hosts.allow -p wa -k CFG_sys
  -w /etc/hosts.deny -p wa -k CFG_sys
  -w /etc/initlog.conf -p wa -k CFG_sys
  -w /etc/inittab -p wa -k CFG_sys
  -w /etc/issue -p wa -k CFG_sys
  -w /etc/issue.net -p wa -k CFG_sys
  -w /etc/krb5.conf -p wa -k CFG_sys
  -w /etc/ld.so.conf -p wa -k CFG_sys
  -w /etc/ld.so.conf.d -p wa -k CFG_sys
  -w /etc/login.defs -p wa -k CFG_sys
  -w /etc/modprobe.conf.d -p wa -k CFG_sys
  -w /etc/modprobe.d/00_simp_blacklist.conf -p wa -k CFG_sys
  -w /etc/nsswitch.conf -p wa -k CFG_sys
  -w /etc/pam.d -p wa -k CFG_pam
  -w /etc/pam_smb.conf -p wa -k CFG_pam
  -w /etc/profile -p wa -k CFG_shell
  -w /etc/rc.d/init.d -p wa -k CFG_sys
  -w /etc/rc.local -p wa -k CFG_sys
  -w /etc/rc.sysinit -p wa -k CFG_sys
  -w /etc/resolv.conf -p wa -k CFG_sys
  -w /etc/securetty -p wa -k CFG_sys
  -w /etc/security -p wa -k CFG_security
  -w /etc/services -p wa -k CFG_services
  -w /etc/shells -p wa -k CFG_shell
  -w /etc/snmp/snmpd.conf -p wa -k CFG_sys
  -w /etc/ssh/sshd_config -p wa -k CFG_sys
  -w /etc/sysconfig -p wa -k CFG_sys
  -w /etc/sysctl.conf -p wa -k CFG_sys
  -w /etc/xinetd.conf -p wa -k CFG_xinetd
  -w /etc/xinetd.d -p wa -k CFG_sys
  -w /etc/yum.conf -p wa -k yum-config
  -w /etc/yum.repos.d -p wa -k yum-config
  -w /lib/firmware/microcode.dat -p wa -k CFG_sys
  -w /var/spool/at -p wa -k CFG_sys
  -a exit,always -F arch=b32 -S ptrace -k paranoid
  -a exit,always -F arch=b64 -S ptrace -k paranoid
  -a always,exit -F arch=b32 -S personality -k paranoid
  -a always,exit -F arch=b64 -S personality -k paranoid
  -w /etc/aide.conf -p wa -k CFG_aide
  -w /etc/aide.conf.d/default.aide -p wa -k CFG_aide
  -w /etc/rc.d/init.d/auditd -p wa -k auditd
  -w /var/log/audit.log -p wa -k audit-logs
  -w /etc/pam_ldap.conf -p a -k CFG_etc_ldap
  -w /etc/pki/private -p wa -k PKI
  -w /etc/pki/public -p wa -k PKI
  -w /etc/pki/cacerts -p wa -k PKI
  -w /etc/pki/private/blade01.tasty.bacon.pem -p wa -k PKI
  -w /etc/pki/public/blade01.tasty.bacon.pub -p wa -k PKI
  -a always,exit -F dir=/etc/puppet -F uid!=puppet -p wa -k Puppet_Config
  -a always,exit -F dir=/var/log/puppet -F uid!=puppet -p wa -k Puppet_Log
  -a always,exit -F dir=/var/run/puppet -F uid!=puppet -p wa -k Puppet_Run
  -a always,exit -F dir=$vardir/ssl -F uid!=puppet -p wa -k Puppet_SSL
  -w /var/log/audit.log.1 -p rwa -k audit-logs
  -w /var/log/audit.log.2 -p rwa -k audit-logs
  -w /var/log/audit.log.3 -p rwa -k audit-logs
  -w /var/log/audit.log.4 -p rwa -k audit-logs
  -w /var/log/audit.log.5 -p rwa -k audit-logs
  -w /etc/init/ -p wa -k CFG_upstart

References: :ref:`AU-2`

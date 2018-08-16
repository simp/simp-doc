.. _upgrade-6.1.0-to-6.2.0:

Upgrading from SIMP-6.1.0 to SIMP-6.2.0
---------------------------------------

.. IMPORTANT::

   It is *highly recommended* that you read the information in this section
   in its entirety.

Update kickstart files
^^^^^^^^^^^^^^^^^^^^^^

The kickstart example file, ``pupclient_x86_64.cfg``,  was updated.  Changes in this
file should be used to create any new kickstart files and backported to any existing
kickstart files.  The new, OS-specific versions of this file are included in the
SIMP-6.2.0 ISOs, but may also be found in the `simp-core repository`_ under
``build/distributions/<OS>/<OS Major Version>/<Architecture>/DVD/ks``.

Two major changes were made:

#. To solve two timeout problems on particularly loaded systems, both of which
   caused client Puppet bootstrapping to fail and require manual intervention,
   the ``runpuppet`` script was completely rewritten.  Although ``runpuppet`` is
   still available, it is deprecated and has been replaced with a pair of scripts:

     - A ``systemd`` unit file for CentOS 7 (``simp_client_bootstrap.service``)
       or a ``systemv`` init script for CentOS 6 (``simp_client_bootstrap``).
     - A common bootstrap script (``bootstrap_simp_client``) used by both.

   The use of an actual ``systemd`` unit file for CentOS 7 solved the problem in
   which ``systemd`` on CentOS 7 was killing ``runpuppet``, when it ran longer
   than 5 minutes.  By default, setting the static hostname of the client on
   CentOS 7 at the beginning of the client Puppet bootstrap process solved
   the problem whereby the DHCP lease may expire in the middle of bootstrapping,
   resulting in ``localhost`` being erroneously used for the client hostname
   within generated Puppet configuration.

   The sample kickstart files for CentOS 6 and CentOS 7 were updated to download
   the appropriate pair of files, and then enable the service at boot.

#. The sample client kickstart file, ``pupclient_86_64.cfg``, was updated to
   include instructions for changing the file to accomodate UEFI boot.  Also,
   an error in that configuration file that prevented UEFI systems from booting
   was fixed.

Update dhcpd.conf
^^^^^^^^^^^^^^^^^

Changes were added to the ``dhcpd.conf`` file  that enable the DHCP server to
determine what mode, BIOS or UEFI, a system is kickstarting in and then to set
the appropriate boot loader file on the TFTP server.

On a SIMP server, the example ``dhcpd.conf`` file is installed in
``/var/simp/environments/simp/RedHat/Global/dhcpd/dhcpd.conf`` via the
``simp-rsync`` package. This file may also be found in the
`simp-rsync-skeleton repository`_ under
``environments/simp/rsync/RedHat/Global/dhcpd``.

Update the TFTP root directory
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The default TFTP root directory was changed to ``/var/lib/tftpboot`` to
conform to DISA STIG standards.  To continue using ``/tftpboot`` set
``tftpboot::tftpboot_root_dir`` in hiera to ``/tftpboot``.  Alternately, to use
the new directory, copy any files not managed by the ``rsync`` module (i.e., not
stored in ``/var/simp/environments/<environment>/rsync/<OS>/Global/tftpboot``)
to the new directory.  Make sure the permissions, including selinux context, are
correct. TFTP boot will fail to find boot files that have the incorrect selinux
context.

Optionally remove OBE logrotate configuration files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP-managed ``logrotate`` rules are now in ``/etc/logrotate.simp.d`` instead of
``/etc/logrotate.d``.  The rules in ``/etc/logrotate.d`` are still applied, but
``logrotate`` is configured to read the rules in ``/etc/logrotate.simp.d``,
first.

This change was made to ensure SIMP-managed rules take precedence over
vendor-supplied rules, because, when there are multiple rules specified for the
same file, only the first rule is applied.  Any subsequent rules are discarded.
In fact, for some versions of ``logrotate``, a rule with a duplicate log file
is discarded *in its entirety*, even if only one of the managed log files is a
duplicate.  This means the remaining log files specified in that discarded rule
will *not* be rotated!

Because of the location of the SIMP-managed ``logrotate`` rules has changed,
existing, but now OBE, SIMP rules will still reside in ``/etc/logrotate.d``.
Although these rules cause no issues with ``logrotate``, they may be confusing
to system administrators.  So, you may wish to manually remove these rules.

You can easily identify OBE SIMP rules in ``/etc/logrotate.d`` by their
``This file managed by puppet.`` comment lines.

.. _simp-core repository: https://github.com/simp/simp-core
.. _simp-rsync-skeleton repository: https://github.com/simp/simp-rsync-skeleton

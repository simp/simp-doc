.. _upgrade-6.1.0-to-6.2.0:

Upgrading from SIMP-6.1.0 to SIMP-6.2.0
---------------------------------------

.. IMPORTANT::

   It is *highly recommended* that you read the information in this section
   in its entirety.

Update Kickstart Files
^^^^^^^^^^^^^^^^^^^^^^

The example kickstart file (``pupclient_86_64.cfg``—see
:ref:`Setting_Up_Kickstart`) was updated.  Existing kickstart files that are
modeled on older versions of this file should backport these changes.  New,
OS-specific versions of this file are included in the SIMP-6.2.0 ISOs, but may
also be found in the `simp-core repository`_ under
``build/distributions/<OS>/<OS Major Version>/<Architecture>/DVD/ks/``.

Two major changes were made:

#. ``pupclient_86_64.cfg`` was updated to include instructions regarding what
   to change in order to accommodate UEFI boot.  The associated configuration
   lines are commented out by default.

#. ``pupclient_86_64.cfg`` was updated to download and use new bootstrap service
   files.  These files, managed by ``simp::server::kickstart``, are:

     - A ``systemd`` unit file for CentOS 7 (``simp_client_bootstrap.service``)
       or a ``systemv`` init script for CentOS 6 (``simp_client_bootstrap``).
     - A common bootstrap script (``bootstrap_simp_client``) used by both.

   This pair of files replaces the deprecated ``runpuppet`` script. They
   are required in order to solve two timeout problems on particularly loaded
   systems, both of which can cause client Puppet bootstrapping to fail and
   require subsequent manual intervention in order to fix:

   - On CentOS 7, ``systemd`` was killing ``runpuppet``, when it ran longer
     than 5 minutes.  This was solved by using an actual ``systemd`` unit file
     (``simp_client_bootstrap.service``) with a default timeout of 30 minutes.
   - On CentOS 7, if the DHCP lease expired in the middle of bootstrapping,
     the generated Puppet configuration would erroneously use ``localhost`` for
     the client hostname.  This problem was solved by setting the static
     hostname of the client at the beginning of the client Puppet bootstrap
     process.

   In addition to solving these specific problems, the new bootstrap scripts
   use a configurable backoff algorithm in order prevent flooding a
   heavily-loaded Puppet master with requests. See the
   ``simp::server::kickstart::simp_client_bootstrap`` class for details.

.. IMPORTANT::

   Careful examination of ``pupclient_86_64.cfg`` is recommended, if you have
   customized it for your site beyond token replacement. The latest version
   contains a few subtle bug fixes, such as fixing an incorrect path that would
   prevent UEFI systems from booting.

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

.. NOTE::

   When the ``simp-rsync`` RPM is upgraded, a message may be displayed that
   indicates

   .. code-block:: bash

      warning: /var/simp/environments/simp/rsync/RedHat/Global/dhcpd/dhcpd.conf\
      created as /var/simp/environments/simp/rsync/RedHat/Global/dhcpd/\
      dhcpd.conf.rpmnew

   Because the contents of the ``/var/simp/environments/simp/rsync/`` directory
   are pushed to remote systems, files ending with ``.rpmnew`` are deleted from
   the directory as part of the RPM deployment.
   
   The commands below can be used to extract the new config file from the RPM,
   which can then be merged with the existing ``dhcpd.conf`` file.
   
   #. Run ``$ rpm2cpio simp-rsync-6.2.1-0.el7.noarch.rpm |cpio -ir
      "*dhcpd.conf"``

       *  rpm2cpio converts the RPM file to cpio archive.  In this example, the
          command is being run from the directory containing the RPM file; if
          the file is not in the current working directory, the full path to the
          file must be provided.
       *  cpio extracts files from the cpio archive.  In this case, the cpio
          options *-i*, *-r*, and *"\*dhcpd.conf"* direct cpio to extract one or
          more files from the archive, interactively rename the file, and the
          path and filename of files to be extracted should end with dhcpd.conf.

   #. When prompted ``rename ./var/simp/environments/simp/rsync/RedHat/Global/
      dhcpd/dhcpd.conf ->``, enter the desired path and name of the file to be
      extracted, such as ``/tmp/dhcpd.conf.rpmnew``.  Paths can be either
      absolute or relative, depending whether they start with a ``/`` or ``./``.


Update the TFTP Root Directory
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The default TFTP root directory was changed to ``/var/lib/tftpboot`` to
conform to DISA STIG standards.  To continue using ``/tftpboot`` set
``tftpboot::tftpboot_root_dir`` in Hiera to ``/tftpboot``.  Alternately, to use
the new directory, copy any files not managed by the ``rsync`` module (i.e., not
stored in ``/var/simp/environments/<environment>/rsync/<OS>/Global/tftpboot``)
to the new directory.  Make sure the permissions, including selinux context, are
correct. TFTP boot will fail to find boot files that have the incorrect selinux
context.

Optionally Remove OBE Logrotate Configuration Files
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

Because the location of the SIMP-managed ``logrotate`` rules has changed,
existing, but now OBE, SIMP rules will still reside in ``/etc/logrotate.d``.
Although these rules cause no issues with ``logrotate``, they may be confusing
to system administrators.  So, you may wish to manually remove these rules.

You can easily identify OBE SIMP rules in ``/etc/logrotate.d`` by their
``This file managed by puppet.`` comment lines.

.. _simp-core repository: https://github.com/simp/simp-core
.. _simp-rsync-skeleton repository: https://github.com/simp/simp-rsync-skeleton

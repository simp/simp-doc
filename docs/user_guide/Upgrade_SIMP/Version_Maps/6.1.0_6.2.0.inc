.. _upgrade-6.1.0-to-6.2.0:

Upgrading from SIMP-6.1.0 to SIMP-6.2.0
---------------------------------------

.. IMPORTANT::

   It is *highly recommended* that you read the information in this section
   in its entirety before upgrading.

Exclude `puppet-agent` from the `yum update`
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A bug (`SIMP-5021`_) was identified in SIMP 6.1.0 that will prevent the
``puppetserver`` service from starting after the ``puppet-agent`` package is
upgraded. The issue is fixed in 6.2.0, but special handling is necessary during
the upgrade.

.. NOTE::

   * This problem affects ISO/RPM-based installations of SIMP when
     upgrading from 6.1.0, unless precautions are taken (detailed below).

   * SIMP installations that are upgraded using :term:`r10k` and :term:`Code Manager`
     are unlikely to be affected, unless the SIMP master's
     ``puppet-agent`` package is updated independently.

   * These instructions only apply to the SIMP master—no changes are required on
     clients.

#. Before upgrading anything, add the following line to ``/etc/yum.conf``:

   .. code-block:: ini

      exclude=puppet-agent

   Assuming you aren't already using ``/etc/yum.conf`` to exclude other
   packages, you can add this line automatically by running:

   .. code-block:: sh

      puppet resource file_line workaround path=/etc/yum.conf line='exclude=puppet-agent'


#. Proceed with the upgrade as outlined in :ref:`ug-incremental-upgrades`,
   up through the ``yum update``:

   .. code-block:: sh

      # (ISO Installations only:) Unpack the new ISO's RPMs into yum repositories
      unpack_dvd </path/to/ISO>

      # Make sure yum picks up the new RPMs
      yum clean all; yum makecache

      # Apply updates to the local master
      yum update -y

#. After running ``yum update``, remove the ``exclude=puppet-agent`` line from `/etc/yum.conf`.

   If you added the line using ``puppet resource``, you can remove it the same way:

   .. code-block:: sh

      puppet resource file_line workaround path=/etc/yum.conf line='exclude=puppet-agent' ensure=absent


#. Run ``puppet agent -t`` to upgrade the **puppet-agent** package

#. Run ``puppet agent -t`` again to ensure that everything runs cleanly.


.. admonition:: Recovering if the **puppeserver** fails to restart

   If, during the upgrade, the ``puppetserver`` service fails to start with the
   error:

   .. code-block:: ini

      Puppet::Error: Cannot determine basic system flavour

   You should be able to recover by running the command:

   .. code-block:: sh

      puppet resource file /opt/puppetlabs/puppet/cache owner=puppet group=puppet

      # The service should now restart
      puppet resource service puppetserver ensure=running

   After you have recovered in this manner, it is safe to proceed re-running
   the step you had been attempting when the puppetserver failed.


Update Kickstart Files
^^^^^^^^^^^^^^^^^^^^^^

The example :term:`kickstart` file
(``pupclient_86_64.cfg``—see :ref:`Setting_Up_Kickstart`)
was updated.  Existing kickstart files that are modeled on older versions of
this file should backport these changes.  New, OS-specific versions of this
file are included in the SIMP-6.2.0 ISOs, but may also be found in the
`simp-core repository`_ under
``build/distributions/<OS>/<OS Major Version>/<Architecture>/DVD/ks/``.

.. IMPORTANT::

   Careful examination of ``pupclient_86_64.cfg`` is recommended, if you have
   customized it for your site beyond token replacement. The latest version
   contains a few subtle bug fixes, such as fixing an incorrect path that would
   prevent UEFI systems from booting.

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

   #. Run ``rpm2cpio simp-rsync-6.2.1-0.el7.noarch.rpm |cpio -ir
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

The default :term:`TFTP` root directory was changed to ``/var/lib/tftpboot`` to
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

Known Issues
------------

Issues With Copying Repo Contents
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A bug has been reported in some cases where the `simp-adapter` package does not
appear to be properly updating existing repository spaces in all cases.

If you are getting unexpected errors, then you should run the following command
and evaluate any differences. The materials in `/usr/share/simp/modules` are
authoritative for the latest build but will not be copied over if the target
files are checked into a version control system.

.. code-block:: sh

   diff -qr /usr/share/simp/modules /etc/puppetlabs/code/environments/simp/modules

Once you have corrected any issues, you should run
`/opt/puppetlabs/bin/puppetserver reload` and the system should function
properly again.

.. _simp-core repository: https://github.com/simp/simp-core
.. _simp-rsync-skeleton repository: https://github.com/simp/simp-rsync-skeleton
.. _SIMP-5021: https://simp-project.atlassian.net/browse/SIMP-5021

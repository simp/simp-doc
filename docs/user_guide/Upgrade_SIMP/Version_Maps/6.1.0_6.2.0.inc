.. _upgrade-6.1.0-to-6.2.0:

Upgrading from SIMP-6.1.0 to SIMP-6.2.0
---------------------------------------

.. IMPORTANT::

   It is *highly recommended* that you read the information in this section
   in its entirety.

Update kickstart files
^^^^^^^^^^^^^^^^^^^^^^

The kickstart example file, pupclient_x86_64.cfg,  was updated.  Changes in this
file should be used to create any new kickstart files and backported to any existing
kickstart files.
The new templatescan be found in the simp core module under
build/distributions/<OS>/<OS Major Vesrion>/<Architecture>/DVD/ks.

Two changes were made:

#. It was discovered that, on particularly loaded systems, the init script may
   be killed by systemd on EL7 for running for more than 5 minutes.  The core runpuppet
   capability was split out and an init script is added to EL6 and a systemd unit file
   is added to EL7.  The kickstart file was updated to download both of these files, the
   runpuppet file and the appropriate service file.  Code was also added to configure
   enable the service.  You  need seperate kickstart files
   for CentOS 6 and CentOS 7 systems.

#. UEFI PXE booting was configured and tested in SIMP 6.2.  The pupclient template
   was updated to include instructions for changing the file to accomodate UEFI
   boot.  Also, an error in the config file that prevented UEFI systems from
   booting correctly was fixed.

Update dhcpd.conf
^^^^^^^^^^^^^^^^^

Changes were added to the ``dhcpd.conf`` file  that enable the DHCP server to determine
what mode, Legacy or UEFI, a system is kickstarting in and send the appropiate tftp
directory to boot from.

In a default puppet system the dhcpd.conf file is copied from rsync. The  new file
can be found in the ``simp-rsync-skeleton`` module under
``environments/simp/rsync/RedHat/Global/dhcpd``.  Look at this file for the changes
and update the ``dhcpd.conf file in the appropriate rsync directory.  The rsync
root is /var/simp/environments/<environment name>/rsync.

Update the TFTP root directory
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The default TFTP root directory was changed to ``/var/lib/tftpboot``.  To continue using
``/tftpboot`` set ``tftpboot::tftpboot_root_dir``  in hiera to ``/tftpboot``.
Alternately, to use the new directory, copy any files not maintained in rsync
to the new directory.  Make sure the permissions, including selinux context, are correct.

Unless ``/tftproot`` is a seperately mounted partition, the tftp files existed in
the root partion.  The new directory, besides being the default used by RedHat, also
does not reside in the root partition if the system is partitioned according to STIG
standards.

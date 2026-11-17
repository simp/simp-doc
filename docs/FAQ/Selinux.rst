.. _faq-selinux:

Recovering from SELINUX policy failures
=======================================

If you experience a failed boot after running :command:`simp bootstrap` with an error
that says something along the lines of ``Failed to load SELINUX policy, freezing``, follow these
instructions:

#. Reboot into a rescue shell (instructions on `EL8`_, `EL9`_ and `EL10`_). You may need your
   GRUB password that was set during :command:`simp config` or set using the
   :pupmod:`simp/simp_grub` module.

#. Reinstall the selinux policy: :command:`dnf reinstall -y selinux-policy-targeted`

#. Tell the kernel to relabel all files during next boot: :command:`touch /.autorelabel`

#. Reboot

.. _EL10: https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/10/install-using_rescue_mode#booting-into-rescue-mode
.. _EL8: https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/8/html/interactively_installing_rhel_from_installation_media/troubleshooting-after-installation_rhel-installer#booting-into-rescue-mode_using-rescue-mode
.. _EL9: https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/9/html/interactively_installing_rhel_from_installation_media/troubleshooting-after-installation_rhel-installer#booting-into-rescue-mode_using-rescue-mode

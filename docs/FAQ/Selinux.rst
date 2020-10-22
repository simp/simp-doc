.. _faq-selinux:

Recovering from SELINUX policy failures
=======================================

If you experience a failed boot after running :command:`simp bootstrap` with an error
that says something along the lines of ``Failed to load SELINUX policy, freezing``, follow these
instructions:

#. Reboot into single user mode or a rescue shell (instructions on `EL6`_ and
   `EL7`_). You may need your GRUB password that was set during :command:`simp config` or set using
   the :pupmod:`simp/simp_grub` module.

#. Reinstall the selinux policy: :command:`yum reinstall -y selinux-policy-targeted`

#. Tell the kernel to relabel all files during next boot: :command:`touch /.autorelabel`

#. Reboot

.. _EL6: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/deployment_guide/sec-single-user_mode
.. _EL7: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html-single/system_administrators_guide/index#sec-Terminal_Menu_Editing_During_Boot

.. _faq-selinux:

How to recover from SELINUX policy failure
==========================================

If you experience a failed boot after running ``simp bootstrap`` with an error
that says something along the lines of ``Failed to load SELINUX policy,
freezing``, follow these instructions:

#. Reboot into single user mode or a rescue shell (instructions on `EL6`_ and
   `EL7`_). You may need your GRUB password that was set during ``simp
   config`` or set using the ``simp-simp_grub`` module.

#. Reinstall the selinux policy: ``yum reinstall -y selinux-policy-targeted``

#. Tell the kernel to relabel all files during next boot: ``touch /.autorelabel``

#. Reboot

.. _EL6: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/deployment_guide/sec-single-user_mode
.. _EL7: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system_administrators_guide/sec-terminal_menu_editing_during_boot

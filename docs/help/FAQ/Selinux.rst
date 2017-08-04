.. _faq-selinux:

How to recover from SELINUX policy failure
==========================================

If you experience a failed boot after running ``simp bootstrap`` with an error
that says something along the lines of ``Failed to load SELINUX policy,
freezing``, follow these instructions:

1. Reboot into single user mode or a rescue shell (instructions on `EL6`_ and
   `EL7`_). You may need your GRUB password that was set during ``simp
   config``.

2. Reinstall the selinux policy: ``yum reinstall -y selinux-policy-targeted``

3. Tell the kernel to relabel all files during next boot: ``touch /.autorelabel``

4. Reboot

.. _EL6: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/6/html/Deployment_Guide/sec-Single-User_Mode.html
.. _EL7: https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/7/html/System_Administrators_Guide/sec-Terminal_Menu_Editing_During_Boot.html

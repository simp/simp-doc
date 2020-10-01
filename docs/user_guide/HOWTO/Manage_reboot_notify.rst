.. _howto-manage-reboot-notify:

Manage reboot_notify
====================

``simplib::reboot_notify`` was added to provide a capability for notifying
users, across puppet runs, that their system needed to be rebooted.

It was originally created due to updates to kernel settings requiring a reboot
but discovering that users would often forget to reboot unless a message
continued to appear in front of them.

Disabling Reboot Notifications
------------------------------

Technically, reboot notifications cannot be excluded from the puppet catalog.
However, you can silence them by setting ``simplib::reboot_notify::log_level``
to the level of your choosing.

For instance, setting the following via :term:`Hiera` will ensure that you only
see log messages when running in ``debug`` mode:

.. code-block:: yaml
   ---
   simplib::reboot_notify::log_level: debug

You may use any values that are valid for the `puppet loglevel metaparameter`_.

.. _puppet loglevel metaparameter: https://puppet.com/docs/puppet/latest/metaparameter.html#loglevel

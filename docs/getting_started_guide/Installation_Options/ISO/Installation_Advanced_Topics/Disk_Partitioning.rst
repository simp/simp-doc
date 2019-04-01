.. _ig-disk-partitioning:

Disk Partitioning
-----------------

The default SIMP installation has a disk partitioning scheme that is
designed to meet multiple compliance and system hardening standards and
requirements. If you choose to implement a custom partitioning scheme
you will need to make sure to use one that meets the standards and
requirements your organization implements.

Default Partition Scheme
^^^^^^^^^^^^^^^^^^^^^^^^

By default SIMP has the following partitions:

* swap
* /
* /boot
* /boot/efi
* /home
* /tmp
* /var
* /var/log
* /var/log/audit

Required Partitions
^^^^^^^^^^^^^^^^^^^

SIMP will fail to install if the following partitions do not exist:
* /
* /boot

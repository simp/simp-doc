Audit Storage and Capacity
--------------------------

The Puppet logs are written to the ``/var/log`` partition.  This puts them on
the same logical volume as the audit logs.  That volume is mounted on a separate
partition so that log space does not interfere with operations.

The puppet master logs reports from client puppet runs in
``/var/lib/puppet/reports``.  The SIMP pupmod puppet module purges reports older
than 7 days.

References: :ref:`AU-4`

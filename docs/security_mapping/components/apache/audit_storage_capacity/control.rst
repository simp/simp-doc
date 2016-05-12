Audit Storage and Capacity
---------------------------

The Apache logs are written to the ``/var/log`` partition.  This puts them on
the same logical volume as the audit logs.  That volume is mounted on a separate
partition so that log space does not interfere with operations.

References: :ref:`AU-4`

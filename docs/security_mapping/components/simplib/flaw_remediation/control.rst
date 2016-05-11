Flaw Remediation
-----------------

The Yellowdog Updater, Modified(YUM) client is configured to point to all SIMP
repositories.  Each night, a cron job runs yum update to install updated
packages on each SIMP client.  Therefore any packages in a repository are
delivered within a 24 hour time period.

References: :ref:`SI-2`

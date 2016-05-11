Automated Central Management / Application / Verification
----------------------------------------------------------

SIMP uses rsync (over stunnel) to keep files in ``/var/www`` synchronized between
all web servers.  Any files that need to be the same on all web servers are
then managed from the puppet master.

References: :ref:`CM-7 (1)`

Malicious Code Protection
---------------------------

SIMP installs and configures ClamAV.  ClamAV is a command line malicious code
detection tool.

ClamAV is scheduled to run once per day and scans ``/tmp``, ``/var/tmp``, and
``/dev/shm``.

References: :ref:`SI-3`, :ref:`SI-3a.`

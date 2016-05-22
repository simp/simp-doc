Transmission Confidentiality and Integrity
------------------------------------------

Rsync is not encrypted.  To mitigate this, SIMP only allows rsync to listen on
the local host.  The server to client communications is then protected using the
SIMP stunnel module.

References: :ref:`SC-8`

Transmission Confidentiality and Integrity
------------------------------------------

The stunnel module is a framework used by other modules to encrypt
communications for applications that might not natively support it.

The cipher negotiation is determined by the OpenSSL ciphers.  In a default
SIMP system, this will be TLSv1.1 or higher.

The certificates used for stunnel are in the ``/etc/pki`` directory.

References: :ref:`SC-8`

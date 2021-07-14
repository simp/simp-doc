Transmission Confidentiality and Integrity
------------------------------------------

The :term:`SIMP Server` has a :term:`TLS` enabled Apache web server running on
port 443.  The protocols are limited to TLSv1.2.  If the web client does not
support those protocols, the connection will be rejected.  The certificates are
in the ``/etc/pki/simp_apps/simp_apache/x509`` directory.

References: :ref:`SC-8`

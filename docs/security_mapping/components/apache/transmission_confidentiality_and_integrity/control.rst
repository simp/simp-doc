Transmission Confidentiality and Integrity
------------------------------------------

The SIMP server/puppet master has an SSL enabled Apache web server running on
port 443.  The protocols are limited to TLSv1, TLSv1.1, and TLSv1.2.  If the
web client does not support those protocols, the connection will be rejected.
The certificates are in the ``/etc/httpd/conf/pki`` directory.

References: :ref:`SC-8`

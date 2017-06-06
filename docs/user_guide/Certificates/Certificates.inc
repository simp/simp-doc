Apply Certificates
==================

All clients in a SIMP system must have :term:`Public Key Infrastructure` (PKI)
keypairs generated for the server.  These keys reside in the
``/var/simp/environments/simp/site_files/pki_files/files/keydist`` directory on the
SIMP server and are served to the clients over the puppet protocol.

.. NOTE::

   These keypairs are **not** the keys that the Puppet server uses for its
   operation. Do not get the two confused.

   See :ref:`Certificate Management` for more information.

This section provides guidance on installing official certificates or, as an
interim measure, generating certificates from the Fake (self-signing)
Certificate Authority provided by SIMP.

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

Installing Official Certificates
--------------------------------

Below are the steps to install official certificates for a SIMP client on
the SIMP server:

1. Copy the certificates received from a proper :term:`CA` to the SIMP server.
2. Add the keys for the node to ``/var/simp/environments/simp/site_files/pki_files/files/keydist``.

  a) Type ``mkdir -p /var/simp/environments/simp/site_files/pki_files/files/keydist/***<Client System FQDN>***``
  b) Type

     .. code-block:: bash

       mv ***<Certificate Directory>***/***<FQDN>***.[pem|pub] \
       /var/simp/environments/simp/site_files/pki_files/files/keydist/***<FQDN>***

  c) Type ``chown -R root.puppet /var/simp/environments/simp/site_files/pki_files/files/keydist``
  d) Type ``chmod -R u=rwX,g=rX,o-rwx /var/simp/environments/simp/site_files/pki_files/files/keydist``

3. Create and populate the ``/var/simp/environments/simp/site_files/pki_files/files/keydist/cacerts``
   directory.

  a) Type ``cd /var/simp/environments/simp/site_files/pki_files/files/keydist``
  b) Type ``mkdir cacerts`` and copy the root CA public certificates into cacerts in Privacy
     Enhanced Mail (PEM) format (one per file).
  c) Type ``cd cacerts``
  d) Type ``for file in *.pem; do ln -s $file `openssl x509 -in $file -hash -noout`.0; done``

Generating Certificates from the Fake CA
----------------------------------------

If server certificates have not or could not be obtained at the time of client
installation, SIMP provides a way to create them for the system, so that it
will work until proper certificates are provided.

.. NOTE::

   This option should not be used for any operational system that can use
   proper enterprise PKI certificates.

Below are the steps to generate the certificates using the SIMP-provided, Fake CA.

1. Type ``cd /var/simp/environments/simp/FakeCA``
2. Type ``vi togen``
3. Remove old entries from the file and add the :term:`Fully Qualified Domain Name`
   (FQDN) of the systems (one per line) for which certificates will be created.

  .. NOTE::

     To use alternate DNS names for the same system, separate the names with
     commas and without spaces.

     For example, ``.name,alt.name1,alt.name2.``

4. Type ``wc cacertkey``

  .. NOTE::

     Ensure that the ``cacertkey`` file is not empty. If it is, enter text into
     the file; then save and close the file.

5. Type ``./gencerts_nopass.sh auto``

  .. NOTE::

     To avoid using the default Fake CA values, remove the ``auto`` statement
     from the ``./gencerts_nopass.sh`` command.

.. WARNING::

   If the ``clean.sh`` command is run after the certificates have been
   generated, you will not be able to generate new host certificates under the
   old CA. To troubleshoot certificate problems, see the
   :ref:`cm-troubleshoot-cert-issues` section.

If issues arise while generating keys, type ``cd /etc/puppetlabs/code/environments/simp/FakeCA``
to navigate to the ``/etc/puppetlabs/code/environments/simp/FakeCA`` directory, then type
``./clean.sh`` to start over.

After running the ``clean.sh`` script, type ``./gencerts_nopass.sh`` to
run the script again using the previous procedure table.

.. _Certificates:

Apply Certificates
==================

This section provides guidance on obtaining official certificates and
generating a Fake CA.

Obtaining Official Certificates
-------------------------------

All SIMP systems must have :term:`Public Key Infrastructure (PKI)` keypairs generated for the server. These
keys reside in the ``/etc/puppet/keydist`` directory and are served to the
clients over the Puppet protocol.

.. note::
  These keypairs are not the keys that the Puppet server uses for its
  operation. Do not get the two confused.

The table below lists the steps to add any keys for the server that were
received from a proper CA to ``/etc/puppet/keydist``.

1. Type ``mkdir /etc/puppet/keydist/***<Client System FQDN>***``
2. Type ``mv ***<Certificate Directory>***/***<FQDN>***.[pem|pub] /etc/puppet/keydist/***<FQDN>***``
3. Type ``chown -R root.puppet /etc/puppet/keydist``
4. Type ``chmod -R u=rwX,g=rX,o-rwx /etc/puppet/keydist``

Table: Official Certificates Procedure

The table below lists the steps to create and populate the
``/etc/puppet/keydist/cacerts`` directory.

1. Type ``cd /etc/puppet/keydist``
2. Type ``mkdir cacerts`` and copy the root CA public certificates into *cacerts* in :term:`Privacy Enhanced Mail (PEM)` format (one per file).
3. Type ``cd cacerts``
4. Type ``for file in *.pem; do ln -s $file `openssl x509 -in $file -hash -noout`.0; done``

Table: */etc/puppet/keydist/cacerts* Directory Creation Procedure

Generating Fake CAs
-------------------

If server certificates have not or could not be obtained at the time of
client installation, the SIMP team provides a way to create them for the
system so that it will work until proper certificates are provided.

.. note::
  This option should not be used for any operational system that can
  use proper enterprise PKI certificates.

The table below lists the steps to generate the Fake CAs.

1. Type ``cd /etc/puppet/Config/FakeCA``

2. Type ``vi togen``

3. Remove old entries from the file and add the Fully Qualified Domain Name (FQDN) of the systems (one per line) for which certificates will be created.

.. note:: To use alternate DNS names for the same system, separate the names with commas and without spaces. For example, .name,alt.name1,alt.name2.

4. Type ``wc cacertkey``

.. note:: Ensure that the cacertkey file is not empty. If it is, enter text into the file; then save and close the file.

5. Type ``./gencerts_nopass.sh auto``

.. note:: To avoid using the default Fake CA values, remove the auto statement from the ./gencerts_nopass.sh command.

Table: Generating Fake CAs Procedure

.. warning::
  If the ``clean.sh`` command is run after the certificates have been
  generated, the running system will break. To troubleshoot
  certificate problems, see the section at the end of this chapter.

If issues arise while generating keys, type ``cd /etc/puppet/Config/FakeCA`` to navigate to the
*/etc/puppet/Config/FakeCA* directory, then type ``./clean.sh`` to start over.

After running the *clean.sh* script, type ``./gencerts_nopass.sh`` to
run the script again using the previous procedure table.

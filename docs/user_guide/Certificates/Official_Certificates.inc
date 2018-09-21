Installing Official Certificates
--------------------------------

This section describes how to install infrastructure certificates from an
official certificate authority on the Puppet master for distribution to client
servers.  You need to have simp_options::pki set to ``simp`` on the client for
this to work.

The key distribution directory on the Puppet master is the ``pki_files/files/keydist``
sub-directory located under the SIMP-specific, alternate module path,
``/var/simp/environments/<environment>/site_files``. Within the ``keydist``
directory, the SIMP system expects there to be:

  * A directory named ``cacerts`` that contains the :term:`CA` public
    certificates.
  * Client-specific directories, each of which contains the public and private
    certificates for an individual client.  The name of each client directory
    must be the ``certname`` of that client, which by default is the client's
    FQDN.

Here is an example key distribution directory for a ``simp`` environment:

.. code-block:: shell

   /var/simp/environments/simp/site_files/pki_files/files/keydist/cacerts/
   /var/simp/environments/simp/site_files/pki_files/files/keydist/cacerts/cacert_a7a23f33.pem
   /var/simp/environments/simp/site_files/pki_files/files/keydist/cacerts/cca9a35.0
   /var/simp/environments/simp/site_files/pki_files/files/keydist/mycomputer.my.domain/
   /var/simp/environments/simp/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pem
   /var/simp/environments/simp/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pub
   /var/simp/environments/simp/site_files/pki_files/files/keydist/yourcomputer.your.domain/
   /var/simp/environments/simp/site_files/pki_files/files/keydist/yourcomputer.your.domain/yourcomputer.your.domain.pem
   /var/simp/environments/simp/site_files/pki_files/files/keydist/yourcomputer.your.domain/yourcomputer.your.domain.pub

To install official certificates on the Puppet master, do the following:

1. Copy the certificates received from a proper :term:`CA` to the SIMP server.
2. Add the certificates for the node to the key distribution directory in ``site_files``.

   a) Make the directory under the key distribution directory for the client's
      certificates using the client's ``certname``.
   b) Copy the official public and private certificates to that directory.

   For example to install certificates for a system named ``mycomputer.my.domain``
   into the ``simp`` environment:

   .. code-block:: shell

     mkdir -p /var/simp/environments/simp/site_files/pki_files/files/keydist/mycomputer.my.domain
     mv /dir/where/the/certs/were/myprivatecert.pem \
        /var/simp/environments/simp/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pem
     mv /dir/where/the/certs/were/mypubliccert.pub \
        /var/simp/environments/simp/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pub

3. Create and populate the :term:`CA` certificates directory.

   a) Make the CA directory, ``cacerts``.
   b) Copy the root CA public certificates into ``cacerts`` in Privacy Enhanced Mail (PEM) format, one per file.

   .. code-block:: shell

     cd /var/simp/environments/simp/site_files/pki_files/files/keydist
     mkdir cacerts
     cd cacerts
     for file in *.pem; do ln -s $file `openssl x509 -in $file -hash -noout`.0; done

4.  Make sure the permissions are correct.

   .. code-block:: shell

     chown -R root.puppet /var/simp/environments/simp/site_files/pki_files/files/keydist
     chmod -R u=rwX,g=rX,o-rwx /var/simp/environments/simp/site_files/pki_files/files/keydist

.. NOTE::

   The SIMP-specific alternate modules path is configured in each environment's
   ``environment.conf`` file.  For example, for the ``simp`` environment,
   ``/etc/puppetlabs/code/environments/simp/environment.conf``, would contain:

   .. code-block:: shell

      modulepath = modules:/var/simp/environments/simp/site_files:$basemodulepath


Generating Infrastructure Certificates from the Fake CA
-------------------------------------------------------

The Fake (self signing) Certificate Authority (Fake CA) is provided by SIMP
as a way to obtain server certificates if official certificates could not be
obtained at the time of client installation or the servers are operating in
testing environments.

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

5. Type ``./gencerts_nopass.sh``

.. WARNING::

   If the ``clean.sh`` command is run after the certificates have been
   generated, you will not be able to generate new host certificates under the
   old CA. To troubleshoot certificate problems, see the
   :ref:`cm-troubleshoot-cert-issues` section.

If issues arise while generating keys, type ``cd /var/simp/environments/simp/FakeCA``
to navigate to the ``/var/simp/environments/simp/FakeCA`` directory, then type
``./clean.sh`` to start over.

After running the ``clean.sh`` script, type ``./gencerts_nopass.sh`` to
run the script again using the previous procedure table.

The certificates generated by the FakeCA in SIMP are set to expire annually. To change
this, edit the following files with the number of days for the desired lifespan of the
certificates:


-  ``/var/simp/environments/simp/FakeCA/CA``

-  ``/var/simp/environments/simp/FakeCA/ca.cnf``

-  ``/var/simp/environments/simp/FakeCA/default\_altnames.cnf``

-  ``/var/simp/environments/simp/FakeCA/default.cnf``

-  ``/var/simp/environments/simp/FakeCA/user.cnf``

In addition, any certificates that have already been created and signed will
have a config file containing all of its details in
``/var/simp/environments/simp/FakeCA/output/conf/``.

.. IMPORTANT::

    Editing any entries in the above mentioned config files will **not** affect
    existing certificates. Existing certificates must be regenerated if you need
    to make changes.

The following is an example of how to change the expiration time from one year
(the default) to five years for any newly created certificate.

.. code-block:: bash

   for file in $(grep -rl 365 /var/simp/environments/simp/FakeCA/)
   do
      sed -i 's/365/1825/' $file
   done


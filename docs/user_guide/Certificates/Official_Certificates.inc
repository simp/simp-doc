Installing Official Certificates
--------------------------------

This section describes how to install infrastructure certificates from an
official certificate authority on the Puppet master for distribution to client
servers.  You need to have :code:`simp_options::pki` set to ``simp`` on the
client for this to work.

The key distribution directory on the Puppet master is the
:file:`site_files/pki_files/files/keydist` sub-directory located under the
:term:`SIMP Secondary Environment`,
:file:`/var/simp/environments/{environment}`.
Within the :file:`keydist/` directory, the SIMP system expects there to be:

* A directory named :file:`cacerts/` that contains the :term:`CA` public
  certificates.
* Client-specific directories, each of which contains the public and private
  certificates for an individual client.  The name of each client directory
  must be the :code:`certname` of that client, which by default is the client's
  FQDN.

Here is an example key distribution directory for a ``production``
:term:`SIMP Omni-Environment`:

.. code-block:: shell

   /var/simp/environments/production/site_files/pki_files/files/keydist/cacerts/
   /var/simp/environments/production/site_files/pki_files/files/keydist/cacerts/cacert_a7a23f33.pem
   /var/simp/environments/production/site_files/pki_files/files/keydist/cacerts/cca9a35.0
   /var/simp/environments/production/site_files/pki_files/files/keydist/mycomputer.my.domain/
   /var/simp/environments/production/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pem
   /var/simp/environments/production/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pub
   /var/simp/environments/production/site_files/pki_files/files/keydist/yourcomputer.your.domain/
   /var/simp/environments/production/site_files/pki_files/files/keydist/yourcomputer.your.domain/yourcomputer.your.domain.pem
   /var/simp/environments/production/site_files/pki_files/files/keydist/yourcomputer.your.domain/yourcomputer.your.domain.pub

To install official certificates on the Puppet master, do the following:

#. Copy the certificates received from a proper :term:`CA` to the SIMP server.
#. Add the certificates for the node to the key distribution directory in
   :file:`site_files/`.

   a) Make the directory under the key distribution directory for the client's
      certificates using the client's :code:`certname`.
   b) Copy the official public and private certificates to that directory.

   For example, to install certificates for a system named
   ``mycomputer.my.domain`` into the ``production`` environment:

   .. code-block:: shell

      mkdir -p /var/simp/environments/production/site_files/pki_files/files/keydist/mycomputer.my.domain
      mv /dir/where/the/certs/were/myprivatecert.pem \
         /var/simp/environments/production/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pem
      mv /dir/where/the/certs/were/mypubliccert.pub \
         /var/simp/environments/production/site_files/pki_files/files/keydist/mycomputer.my.domain/mycomputer.my.domain.pub

#. Create and populate the :term:`CA` certificates directory.

   a) Make the CA directory, :file:`cacerts/`.
   b) Copy the root CA public certificates into :file:`cacerts/` in :term:`PEM`
      format, one per file.

   .. code-block:: shell

      cd /var/simp/environments/production/site_files/pki_files/files/keydist
      mkdir cacerts
      cd cacerts
      for file in *.pem; do ln -s $file `openssl x509 -in $file -hash -noout`.0; done

#.  Make sure the permissions are correct.

    .. code-block:: shell

       chown -R root.puppet /var/simp/environments/production/site_files/pki_files/files/keydist
       chmod -R u=rwX,g=rX,o-rwx /var/simp/environments/production/site_files/pki_files/files/keydist

.. NOTE::

   The :file:`site_files/` sub-directory of the SIMP Secondary Environment is
   configured as another module path in each
   :term:`Puppet Environment's<Puppet Environment>` :file:`environment.conf`.
   For example, for the ``production`` environment,
   :file:`/etc/puppetlabs/code/environments/production/environment.conf` would
   contain:

   .. code-block:: ini

      modulepath = modules:/var/simp/environments/production/site_files:$basemodulepath


Generating Infrastructure Certificates from the Fake CA
-------------------------------------------------------

The "Fake" (self-signing) Certificate Authority (Fake CA) is provided by SIMP
as a way to manage server certificates if official certificates could not be
obtained at the time of client installation or the servers are operating in
testing environments.

.. NOTE::

   This option should not be used for any operational system that can use
   proper enterprise PKI certificates.

Below are the steps to generate the certificates using the SIMP-provided, Fake CA.
These steps assume the ``production`` environment.

#. cd to :file:`/var/simp/environments/production/FakeCA/`
#. Run :command:`vi togen`

   a) Remove old entries from the file and add the :term:`Fully Qualified
      Domain Name` (FQDN) of the systems (one per line) for which certificates
      will be created.

      .. NOTE::

         To use alternate DNS names for the same system, separate the names with
         commas and omit any spaces.

         For example, :code:`.name,alt.name1,alt.name2.`

#. Run :command:`wc cacertkey`

   a) Verify that the :file:`cacertkey` file is not empty.
   b) If it is empty: enter text into the file, then save and close the file.

#. Run :command:`./gencerts_nopass.sh`

.. WARNING::

   If the :program:`clean.sh` command is run after the certificates have been
   generated, you will not be able to generate new host certificates under
   the old CA. To troubleshoot certificate problems, see the
   :ref:`cm-troubleshoot-cert-issues` section.

If issues arise while generating keys, navigate to the
:file:`/var/simp/environments/production/FakeCA/` directory, then type
:command:`./clean.sh` to start over.

After running the :program:`clean.sh` script, type
:command:`./gencerts_nopass.sh` to run the script again using the previous
procedure table.

The certificates generated by the FakeCA in SIMP are set to expire annually. To change
this, edit the following files with the number of days for the desired lifespan of the
certificates:

- :file:`/var/simp/environments/production/FakeCA/CA`
- :file:`/var/simp/environments/production/FakeCA/ca.cnf`
- :file:`/var/simp/environments/production/FakeCA/default_altnames.cnf`
- :file:`/var/simp/environments/production/FakeCA/default.cnf`
- :file:`/var/simp/environments/production/FakeCA/user.cnf`

In addition, any certificates that have already been created and signed will
have a config file containing all of its details in
:file:`/var/simp/environments/production/FakeCA/output/conf/`.

.. IMPORTANT::

   Editing any entries in the above mentioned config files will **not** affect
   existing certificates. Existing certificates must be regenerated if you need
   to make changes.

The following is an example of how to change the expiration time from one year
(the default) to five years for any newly created certificate:

.. code-block:: bash

   for file in $(grep -rl 365 /var/simp/environments/production/FakeCA/)
   do
      sed -i 's/365/1825/' $file
   done

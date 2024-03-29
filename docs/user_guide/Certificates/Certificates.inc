Apply Certificates
==================

All clients in a SIMP system should have :term:`Public Key Infrastructure` (PKI)
keypairs generated for the server.  These are the referred to as the
infrastructure or server keys.  These certificates are used to encrypt
communication and identify clients and are used by common applications such as
LDAP and Apache.

.. NOTE::

   These keypairs are **not** the keys that the Puppet server uses for its
   operation. Do not get the two confused.

   See :ref:`Certificate Management` for more information.

SIMP uses the :pupmod:`simp/pki` module to help distribute infrastructure
keypairs. The global variable, :code:`simp_options::pki` determines what parts
of the module are included.  It can be overridden in hiera data at several
levels if different hosts or applications need to handle certificates
differently.

:code:`simp_options::pki` can have one of three settings:

#. ``simp`` -  Keypairs are distributed from a central location on the Puppet
   server to the :file:`/etc/pki/simp/x509` directory on the client. Any
   applications using them will then make a copy in
   :file:`/etc/pki/simp_apps/<app name>/x509` with the correct permissions for
   an application to use.
#. ``true`` -  Applications on the clients will copy the keypairs from a local
   directory on the client to :file:`/etc/pki/simp_apps/<app name>/x509`. The
   default local directory to copy from is :file:`/etc/pki/simp/x509` but this can
   be overridden by setting the :code:`simp_options::pki::source` variable.
#. ``false`` -  The user will have to manage keypairs themselves. You will need
   to look at each module that uses PKI on a client to determine what variables
   need to be set.

   .. NOTE::

      A setting of ``false`` does **not** disable the use of PKI in a module.

The following sections describe how to populate the central key distribution
directory that :pupmod:'simp/pki` uses, when :code:`simp_options::pki`
is set to ``simp``.

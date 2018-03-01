Apply Certificates
==================

All clients in a SIMP system should have :term:`Public Key Infrastructure` (PKI)
keypairs generated for the server.  These are the refered to as the infrastructure
or server keys.  These certificates are used to encrypt communication
and identify clients and are used by common applications such as LDAP and Apache.

.. NOTE::

   These keypairs are **not** the keys that the Puppet server uses for its
   operation. Do not get the two confused.

   See :ref:`Certificate Management` for more information.

SIMP uses the ``pupmod-simp-pki`` module to help distribute keypairs.
The global variable, ``simp_options::pki`` determines what parts of the module are
included.  It can be overridden at several levels if different hosts or
applications need to hande certificates differently.

``simp_options::pki`` can have one of three settings:

1) ``simp`` -  Keypairs are distributed from a central location on the puppetserver to
the ``/etc/pki/simp/x509`` directory on the client. Any applications using them will
then make a copy in ``/etc/pki/simp_apps/<app name>/x509`` with the correct permissions
for an application to use.

2) ``true`` -  Applicatons on the clients will copy the keypairs from a local directory
on the client to ``/etc/pki/simp_apps/<app name>/x509``. The default local directory to
copy from is ``/etc/pki/simp/x509`` but this can be overidden by setting the ``simp_options::pki::source``
variable.

3) ``false`` -  The user will have to manage keypairs themselves. You will need to look at
each module that uses pki on a client to determine what variables need to be set.
NOTE: This does not disable the use of pki in a module.

The following sections describe how to manage certificate when using the ``simp`` option.

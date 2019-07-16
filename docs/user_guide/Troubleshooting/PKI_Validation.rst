.. _pki_validation:

Checking Your SIMP PKI Communication
====================================

SIMP comes with a fully functional :term:`Public Key Infrastructure` in the
guise of an aptly named Fake CA.

The Fake CA can be very useful for getting your environment running prior to
obtaining proper certificates from an official CA.

.. WARNING::

   The Fake CA is **not** hardware backed by default and should not be used for
   sensitive cryptographic operations unless there is no other alternative

Each Puppet environment that is part of a :term:`SIMP Omni-Environment` contains
its own Fake CA.  That Fake CA is located within the corresponding
:term:`SIMP Secondary Environment`.

Basic Server Setup Check
^^^^^^^^^^^^^^^^^^^^^^^^

Just as with Puppet certificates, the time on your system must be correct and
your DNS must be fully functional. Check that these are correct before
proceeding.

Fake CA Setup Check
^^^^^^^^^^^^^^^^^^^

For the remainder of this section, we will assume the following:

* The active Puppet environment returned by ``puppet config print environment``
  is part of a :term:`SIMP Omni-Environment`.
* The :term:`FQDN` of the system with issues is ``system.my.domain``.
* The LDAP server to which ``system.my.domain`` is attempting to connect is
  ``ldap.my.domain``.


#. Change directories to the *keydist* directory for the active Puppet
   environment's Fake CA.

   .. code-block:: bash

       # cd /var/simp/environments/`puppet config print environment`/site_files/pki_files/files/keydist

#. Validate the client system.  When validating certificates, you want to make
   sure that there are no errors regarding your certificate or :term:`CA`. Ideally,
   the command will simply return the string 'OK'.

   .. code-block:: bash

      # openssl verify -CApath cacerts system.my.domain/system.my.domain.pub

#. Validate the LDAP system.

   .. code-block:: bash

      # openssl verify -CApath cacerts ldap.my.domain/ldap.my.domain.pub

If there are any issues, you may need to follow the steps in :ref:`Certificates`
to generate new certificates for one or more of your hosts.

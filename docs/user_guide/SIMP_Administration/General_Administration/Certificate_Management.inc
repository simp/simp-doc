.. _Certificate Management:

Certificate Management
----------------------

This section describes the two different types of certificates used in a SIMP
environment and provides links to further information.

.. _ug-sa-ga-puppet-certificates:

Puppet Certificates
^^^^^^^^^^^^^^^^^^^

Puppet certificates are issued and maintained strictly within Puppet.  Communication
between the server and agents is granted and secured with HTTPS, which
requires valid identifying SSL certificates. The :term:`Puppet Server` acts as the certificate
authority for managing these certificates.

The client will automatically send a certificate request to the server if it cannot find
a valid certificate. The Puppet Server will automatically sign a certificate request if
the client's name is in the  ``autosign.conf`` file, otherwise an administrator must sign
the request using the ``puppetserver ca`` tool.

.. NOTE::

   By default, Puppet certificates expire every five (5) years.

.. _ug-sa-ga-infrastructure-certificates:

Infrastructure Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Infrastructure certificates, commonly referred to as server certificates,
are the standard :term:`PKI` certificates assigned by an official :term:`CA`.
These are used by all other applications that require certificates.

SIMP offers capabilities to help manage these certificates including a FakeCA
utility that will provide self-signed certificates for use  on testing systems
or until official certificates can be obtained.

The modules in SIMP have been designed so that infrastructure certificates
can be managed from a central location and SIMP will distribute them to the applications
that need them.

For more information on how to manage infrastructure certificates and how to use the
FakeCA utility refer to the :ref:`Certificates` section of :ref:`Client_Management`.


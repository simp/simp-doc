.. _Certificate Management:

Certificate Management
----------------------

This section describes the two different types of certificates used in a SIMP
and provides links to further information.


Puppet Certificates
^^^^^^^^^^^^^^^^^^^

Puppet certificates are issued and maintained strictly within Puppet.  Communication
between the master and agents is granted and secured with HTTPS, which
requires valid identifying SSL certificates. The Puppet master acts as the certificate
authority for managing these certificates.

The client will automatically send a certificate request to the server if it can not find
a valid certifcate. The puppetserver will automatically sign a certificate request if the
the client's name is in the  ``autosign.conf`` file, otherwise an administrator must sign
the request using the ``puppet cert`` tool.

For documentation on the ``puppet cert`` tool, visit the `Puppet Inc. cert manual`_.

You can find the location for the Puppet certificates on your system by running
``puppet config print ssldir``.

For some general information on troubleshooting Puppet certificates see the
section :ref:`ug-puppet-certificate-issues`.

.. NOTE::

   By default, Puppet certificates expire every five (5) years.

.. _Puppet Inc. cert manual: https://docs.puppet.com/puppet/latest/man/cert.html

Infrastructure Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Infrastructure certificates, commonly refered to as server certificates,
are the standard :term:`PKI` certificates assigned by an official :term:`CA`.
These are used by all other applications that require certifcates.

SIMP offers capabilities to help manage these certificates including a FakeCA
utility that will provide self-signed certificates for use  on testing systems
or until official certifcates can be obtained.

The modules in SIMP have been designed so that infrastructure certificates
can be managed from a central location and SIMP will distribute them to the applications
that need them.

For more information on how to manage infrastructure certificates and how to use the
FakeCA utility refer to the :ref:`Certificates` section of :ref:`Client_Management`.


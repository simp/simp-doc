Public Key Infrastructure
-------------------------

Puppet has its own public key infrastructure (:term:`PKI`) that is used exclusively for
the puppet application. The PKI is used to provide access control and protect
communications between the :term:`Puppet Server` and the clients.

Additional information on Puppet and PKI can be found at `<https://docs.puppet.com/background/ssl/certificates_pki.html>`_.

SIMP installs a scheduled job that will download a copy of the certificate
revocation list (:term:`CRL`) two times per day.  If there is a client
certificate that needs to be revoked, they can be added to the CRL and will no
longer be able to connect to the Puppet Server.

References: :ref:`SC-17`

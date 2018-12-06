Public Key Infrastructure
-------------------------

Puppet has its own public key infrastructure (PKI) that is used exclusively for
the puppet application. The PKI is used to provide access control and protect
communications between the puppet master and the clients.

Additional information on Puppet and PKI can be found at `<https://docs.puppet.com/background/ssl/certificates_pki.html>`_.

SIMP installs a cron job that will download a copy of the certificate revocation
list(CRL) two times per day.  If there is a client certificate that needs to be
revoked, they can be added to the CRL and will no longer be able to connect to
the puppet master.

References: :ref:`SC-17`

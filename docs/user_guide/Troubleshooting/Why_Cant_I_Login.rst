Why Can't I Login?!
===================

If you've reached this page, you're having issues logging into your system with
a newly created account.

In almost all cases, this is because either your user has not been placed in a
group allowed to access the system, your :term:`DNS` is setup incorrectly, or
your :term:`PKI` certificates are invalid.

PAM Access Restrictions
-----------------------

By default, SIMP uses the `pam_access.so` :term:`PAM` module to restrict access
on each individual host. While this may not seem as flexible as some methods,
it is the most failsafe method for ensuring that you don't accidentally
interrupt services due to network issues connecting to your :term:`LDAP`
server.

To allow a user to access a particular system, you need to use the
`pam::access::manage <https://github.com/simp/pupmod-simp-pam/blob/master/manifests/access/manage.pp#L8:L44>`_
define as shown below.

.. code-block:: ruby

  pam::access::manage { 'Allow the security group into the system':
    users   => ['(security)'],
    origins => ['ALL'],
    comment => 'The core security team'
  }

  pam::access::manage { 'Allow bob into the system from the proxy only':
    users   => ['bob'],
    origins => ["proxy.${::domain}"],
    comment => 'Bob the proxied'
  }

Troubleshooting DNS
-------------------

If :term:`PAM` is not the issue, you may be having :term:`DNS` issues. This can
evidence itself in two ways.

First, per the 'Bob' example above, you may be using an :term:`FQDN` to
identify a host on your network. If DNS is not properly configured, then there
is no way for the host to understand that you should have access from this
remote system.

Second, the default :term:`PKI` settings in SIMP ensure that all connections
are validated against the :term:`FQDN` of the client system. In the case of an
:term:`LDAP` connection, a misconfiguration in DNS may result in an inability to
authenticate against the :term:`LDAP` service.

In the following sections, we will assume that we have a host named
'system.my.domain' with the IP address '1.2.3.4'.

Testing a Forward Lookup
~~~~~~~~~~~~~~~~~~~~~~~~

The following should return the expected IP address for your system.

.. code-block:: bash

  $ nslookup system.my.domain

Testing a Reverse Lookup
~~~~~~~~~~~~~~~~~~~~~~~~

The following should return the expected hostname for your system. This
hostname **must** be either the primary name in the PKI certificate or a valid
alternate name.

.. code-block:: bash

  $ nslookup 1.2.3.4

PKI Issues
----------

If both PAM and DNS appear to be correct, you should next validate that your
:term:`PKI` certificates are both valid and functional.

See :ref:`pki_validation` for additional guidance.

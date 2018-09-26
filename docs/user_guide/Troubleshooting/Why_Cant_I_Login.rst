Why Can't I Login?!
===================

If you have reached this page, you are having issues logging into your system
with a newly created account.

In almost all cases, this is because either your user has not been placed in a
group allowed to access the system, your :term:`DNS` is set up incorrectly, or
your :term:`PKI` certificates are invalid.

SSSD Password Checks
--------------------

:term:`SSSD` has been made the default name service caching service in SIMP.
During this process, we discovered that SSSD will enforce password complexity
restrictions **upon login**. This means that, if your password does not meet
the system password complexity requirements, you will not be able to login
until an administrator changes your password to something stronger.

For the default complexity rules, see the :ref:`faq-password-complexity` FAQ.

.. _PAM Access Restrictions:

PAM Access Restrictions
-----------------------

By default, SIMP uses the ``pam_access.so`` :term:`PAM` module to restrict
access on each individual host. While this may not seem as flexible as some
methods, it is the most failsafe method for ensuring that you do not
accidentally interrupt services due to network issues connecting to your
:term:`LDAP` server.

To allow a user to access a particular system, you need to use the
`pam::access::rule`_ define as shown below.

.. code-block:: ruby

  pam::access::rule { 'Allow the security group into the system':
    users   => ['(security)'],
    origins => ['ALL'],
    comment => 'The core security team'
  }

  pam::access::rule { 'Allow bob into the system from the proxy only':
    users   => ['bob'],
    origins => ["proxy.${facts['domain']}"],
    comment => 'Bob the proxied'
  }

Faillock
--------

If a user fails to authenticate properly in **5** consecutive tries (the
default ``pam::deny``), :term:`PAM` will lock the account.

To see a list of user authentication attempts, run ``faillock``.

If a user is marked as invalid (I) or reaches the max number of attempts, you
will need to reset ``faillock`` before authentication can occur.  To do so, run

.. code-block:: bash

   # faillock --reset --user <user>

LDAP Lockout
------------

If your account is in LDAP, you may have locked yourself out.  Like
:term:`PAM`, :term:`LDAP` has a maximum number of logins, **5** by default.
See ``openldap::server::conf::default_ldif::ppolicy_pwd_max_failure``.

To determine if the account is locked, run the following on the LDAP server:

.. code-block:: bash

  # slapcat -a uid=<user>

If you see ``pwdAccountLockedTime`` then the account is locked, and you will
need to follow the instructions in :ref:`unlock-ldap-label` to unlock it.

Troubleshooting DNS
-------------------

If :term:`PAM` is not the issue, you may be having :term:`DNS` issues. This can
evidence itself in two ways.

First, per the 'Bob' example above, you may be using an :term:`FQDN` to
identify a host on your network. If :term:`DNS` is not properly configured,
then there is no way for the host to understand that you should have access
from this remote system.

Second, the default :term:`PKI` settings in SIMP ensure that all connections
are validated against the :term:`FQDN` of the client system. In the case of an
:term:`LDAP` connection, a misconfiguration in DNS may result in an inability
to authenticate against the :term:`LDAP` service.

In the following sections, we will assume that we have a host named
``system.my.domain`` with the IP address ``1.2.3.4``.

Testing a Forward Lookup
~~~~~~~~~~~~~~~~~~~~~~~~

The following should return the expected IP address for your system.

.. code-block:: bash

  $ dig +short system.my.domain

Testing a Reverse Lookup
~~~~~~~~~~~~~~~~~~~~~~~~

The following should return the expected hostname for your system. This
hostname **must** be either the primary name in the :term:`PKI` certificate or
a valid alternate name.

.. code-block:: bash

  $ dig +short -x 1.2.3.4

PKI Issues
----------

If both PAM and DNS appear to be correct, you should next validate that your
:term:`PKI` certificates are both valid and functional.

See :ref:`pki_validation` for additional guidance.

.. _pam::access::rule: https://github.com/simp/pupmod-simp-pam/blob/master/manifests/access/rule.pp

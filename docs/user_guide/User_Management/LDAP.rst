.. _Managing LDAP Users:

Managing Accounts in Lightweight Directory Access Protocol (LDAP)
=================================================================

By default, SIMP provides a :term:`LDAP` server for centralized account
management and to assist with meeting common policy requirements for account
lockout and invalidation.

As of EL8, :term:`OpenLDAP` has been removed from support but :term:`389-DS` has
become available and appears to be the path forward for general LDAP support.

As such, all SIMP systems running EL8+ will provide 389-DS as the default LDAP
server. Client system configurations have been tested to support either system
as a LDAP server.

.. toctree::
   :maxdepth: 1

   389-DS <LDAP/389_DS>
   OpenLDAP <LDAP/OpenLDAP>

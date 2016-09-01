.. _faq-password-complexity:

What is the Password Complexity for SIMP?
=========================================

The following is the default password requirements for a standard SIMP system.
This is based off of an amalgam of various password policies and may vary based
on individual policies that are set for your installation.

The default complexity is enforced in both :term:`PAM` and :term:`LDAP`.

Complexity Rules
----------------

  * 14 Characters or greater
  * 1 Upper case letter
  * 1 Lower case letter
  * 1 Number
  * 1 Special character
  * No more than 2 repetitions of the same character
  * No more than 4 characters in a monotonic character sequence
  * Must not be one of the last 24 passwords that you have used

.. NOTE::
  Locked out accounts **will** unlock automatically after 15 minutes for
  non-root users and one minute for the root user.


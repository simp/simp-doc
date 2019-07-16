.. _faq-password-complexity:

What is the Password Complexity for SIMP?
=========================================

The following are the default password requirements for a standard SIMP system.
This is based off of an amalgam of various password policies and may vary based
on individual policies that are set for your installation.

The default complexity is enforced in both :term:`PAM` and :term:`LDAP`, which
are managed by SIMP's ``pam`` and ``simp_openldap`` modules, respectively.

.. WARNING::

   If you are using the :term:`SIMP Compliance Engine` to enforce a specific
   compliance profile, the enforced rules may be different than those shown
   below.  Be sure to examine the profile to understand the rules applicable to
   your system.

Complexity Rules
----------------

  * 15 Characters or greater
  * 1 Upper case letter
  * 1 Lower case letter
  * 1 Number
  * 1 Special character
  * No more than 2 repetitions of the same character

    * OK: ``aab``
    * BAD: ``aaa``

  * No more than 3 repetitions of a character from the same character class

    * OK: ``abcD``
    * BAD: ``abcd``

  * No more than 4 characters in a monotonic character sequence

    * OK: ``1b2c3d``
    * BAD: ``aBcDe``
    * BAD: ``EdCbA``

  * Cannot contain your username in straight or reversed form
  * Cannot contain items from your GECOS field (usually your full name)
  * Must have more than 4 character changes from the old password
  * Must not be one of the last 24 passwords that you have used

You can check whether or not a password will meet the system requirements
as follows:

* On systems that use ``pam_pwquality``, use the ``pwscore`` utility.
* On systems that use ``pam_cracklib``, use the ``cracklib-check`` utility.

.. NOTE::

   Locked out accounts **will** unlock automatically after 15 minutes for
   non-root users and one minute for the root user.

.. IMPORTANT::

   Systems that use ``pam_cracklib`` may differ slightly in behavior from
   systems that use ``pam_pwquality``. If issues are found, please file a bug
   with the :term:`OS` vendor noting the issue.

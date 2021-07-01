.. _ug-user_management-ldap-389_ds-cheat_sheet:

The following provides base information for working with SIMP-managed
:term:`389-DS` systems.

For more information about getting started with 389-DS, see the
`389-DS Quickstart`_

File Locations
==============

* ``/etc/dirsrv``

  * The default location for directory services

* ``/usr/share/puppet_ds389_config``

  * Information used to bootstrap the 389-DS instances. May, or may not, be
    relevant once the system is fully operational.

ds* Commands
============

389-DS uses a collection of commands for managing instances.

Some of the more useful are listed below.

.. NOTE::

   It is important to know that the SIMP-managed 389-DS instances use an
   account-to-DN mapping for the ``root`` user that automatically maps ``root``
   to the administrative DN for the instance if you are using LDAPI.

   This makes the administration process much easier for daily activities and is
   recommended unless you need to manage the system remotely.

dsctl - Directory Server Control
--------------------------------

* ``dsctl -l``

  * List all instances on the system (ignore the ``slapd-`` prefix when
    referencing them in other commands).

* ``dsctl [instance_name] <start|stop|restart|status>``

  * The easiest manner to manage the running state of your instances.

* ``dsctl [instance_name] healthcheck``

  * Check the instance for common issues

dsconf - Directory Server Configuration
---------------------------------------

* ``dsconf [instance_name] config get``

  * Print the main configuration of the specified instance.

* ``dsconf [instance_name] security get``

  * Print the security configuration of the specified instance.

* ``dsconf [instance_name] pwpolicy get``

  * Print the **global** password policy for the instance.

* ``dsconf [instance_name] localpwp list``

  * Print all known local password policies in the instance.

* ``dsconf [instance_name] localpwp get [DN]``

  * Print the details of the local password policy specified by ``[DN]`` (This
    is one of the items output by ``localpwp list``).
  * Note that local password policies are overrides to individual global
    password policy entries.

dsidm - Directory Server Identity Management
--------------------------------------------

The ``dsidm`` command provides account management capabilities and the usage is
covered in detail in the account management sections.

To make using ``dsidm`` easier, you may want to add something like the following
to ``~/.dsrc``:

.. code-block:: ini

   [<instance_name>]
   uri = ldapi://%%2fvar%%2frun%%2fslapd-<instance_name>.socket
   basedn = <base DN>

For a more concrete example, we will use the ``accounts`` instance provided by
the ``simp/simp_ds389`` module.

To find your base DN, you can run the following:

.. code-block:: shell

   dsidm accounts account list | head -1

Assuming that our base DN is ``dc=local,dc=com``, our configuration file would
look like the following:

.. code-block:: ini

   [accounts]
   uri = ldapi://%%2fvar%%2frun%%2fslapd-accounts.socket
   basedn = dc=local,dc=com

.. _389-DS Quickstart: https://directory.fedoraproject.org/docs/389ds/howto/quickstart.html

.. _Services_dying:

My Services Are Dying!
======================

The following section describes how to mitigate issues relating to
destructive reasoning and avoiding destruction of the SIMP system.

Destructive Reasoning with `svckill`
------------------------------------

Most security guides that have been published on the Internet strongly
suggest disabling all services that are not necessary for system
operation. However, to list every possible service that may be
controlled by the ``chkconfig`` or ``systemctl`` on a given system
in a manifest would not be useful and would bloat the memory space of
the running Puppet process.

As an alternative solution, the SIMP Team implemented the svckill
module that runs with every Puppet run.

The svckill module:

-  Collects a list of all services on the system. These are the same
   services that the user sees using ``systemctl list-unit-files --type=service --state=enabled``

-  Ignores certain critical services, including those for Puppet,
   IPtables/firewalld, and the network.

-  Collects a list of all services that are defined in the manifests and
   modules.

-  Ensures that every service that is defined in the manifests and
   modules is excluded from the list of services to kill.

-  Kills and disables everything else.

Avoiding Destruction
--------------------

If certain services should not be killed, you have two options:

#. Add the service names to the ``svckill::ignore`` array in :term:`Hiera`.

   .. code-block::  yaml

      svckill::ignore:
      - keepmealive1
      - keepmealive2

#. Declare the services in the node manifest space:

  .. code-block:: ruby

     # Preventing these services from being killed by svckill
     service { "keepmealive1": }
     service { "keepmealive2": }

  .. NOTE::

     The key to declaring the services in manifests is to use the
     ``service`` resource without setting any other options.

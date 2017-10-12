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
controlled by the chkconfig type on a given system in a manifest would
not be useful and would bloat the memory space of the running Puppet
process.

As an alternative solution, the SIMP Team implemented the svckill
module that runs with every Puppet run.

The svckill module:

-  Collects a list of all services on the system. These are the same
   services that the user sees after typing ``chkconfig --list``

-  Ignores certain critical services, including Puppet, IPtables, and
   the network.

-  Collects a list of all services that are defined in the manifests and
   modules.

-  Ensures that every service that is defined in the manifests and
   modules is excluded from the list of services to kill.

-  Kills and disables everything else.

Avoiding Destruction
--------------------

If certain services should not be killed, declare them in the node
manifest space or in the `svckill::ignore` array in hiera.

.. NOTE::

    The key is to declare the services and not set them to any other
    option. By adding them to the manifest, the *svckill* module will
    ignore them.

The example below demonstrates this in a manifest, assuming that the
*keepmealive* service is added to the *chkconfig*.

.. code-block:: ruby

   #Preventing a service from being killed by svckill
   service { "keepmealive": }

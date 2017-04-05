Integrating Applications
========================

This section describes how to integrate external applications into the SIMP
managed infrastructure.

For most applications, there are only three SIMP control components that must
be addressed for successful product integration.


IPTables
--------

By default, the SIMP system drops all **incoming** connections to the server,
save port 22. Port 22 is allowed from **all** external sources since there is
no safe way to restrict this that will not lock users out of freshly installed
systems in some cases.

The default SIMP :term:`IPTables` start-up sequence has been set to *fail
safe*. This means that if the IPTables rules cannot cleanly apply, the system
will only allow port 22 into the system for SSH troubleshooting and recovery.

There are many examples of how to use the IPTables module in the source code;
the Apache module at ``/etc/puppetlabs/code/environments/simp/modules/simp_apache``
is a particularly good example. You can also reference the Defined Types in the
IPTables Puppet module to understand their purpose and choose the best option.


Service Kill
------------

To ensure that the system does not run unnecessary services, the SIMP team
implemented a ``svckill.rb`` script has been implemented to stop any service
(not process) that is not properly defined in the Puppet catalog.

To prevent services from stopping, refer to the instructions in the
:ref:`Services_Dying` Troubleshooting section.

As of SIMP 6.0.0, the ``svckill`` Puppet Resource will now warn you that it
would kill items by default and you will explicitly need to enable ``svckill``
enforcement.


Local Access Controls
---------------------

Following defense in depth best practice, SIMP does not trust a single system
to determine the access that someone has to a system. All system accesses are,
by default, restricted to users in the ``administrators`` group.

If you have an application that needs to use a login shell for configuration,
or to run the service, you will need to follow the guidance in
:ref:`PAM Access Restrictions` to ensure that your local user accounts have
appropriate system access.

.. NOTE::

   This **does** affect ``sudo`` accounts! If your application is using a
   ``sudo`` account in a startup script, please consider switching to
   ``runuser`` since it is not affected by PAM controls.

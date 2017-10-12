Service Kill
------------

To ensure that the system does not run unnecessary services, the SIMP team
implemented a ``svckill.rb`` script to stop any service
(not process) that is not properly defined in the Puppet catalog.

To prevent services from stopping, refer to the instructions in the
:ref:`Services_Dying` Troubleshooting section.

As of SIMP 6.0.0, the ``svckill`` Puppet Resource will now warn you that it
would kill items by default and you will explicitly need to enable ``svckill``
enforcement.

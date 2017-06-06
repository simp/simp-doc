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

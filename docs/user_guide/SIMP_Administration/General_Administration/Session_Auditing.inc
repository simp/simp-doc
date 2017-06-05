Session Auditing
----------------

By default, a SIMP system uses :term:`Sudosh` to enable logging of sudo
sessions to ``Rsyslog``.

To open a ``sudo`` session from a regular user to ``root``, you should type
``sudo sudosh``.

``sudosh`` logs are stored in ``/var/log/sudosh.log``. Sessions can be replayed
by typing ``sudosh-syslog-replay``.

.. NOTE::

   The SIMP system does not allow the ``root`` user to execute ``sudo`` by
   default per common configuration guidance.

.. NOTE::

   If you built your system from an ISO, you will probably have a local
   ``simp`` user that has the ability to run ``sudo su - root`` directly and
   bypass ``sudosh``.

   This is meant as an emergency 'break glass' user and should be removed or
   disabled once your environment is configured to your satisfaction.

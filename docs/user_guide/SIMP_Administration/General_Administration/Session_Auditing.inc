Session Auditing
----------------

Older versions of SIMP used :term:`Sudosh` to enable logging of privileged user
activities.

This has been replaced by :term:`Tlog` for a more seamless user experience and
all activities should be able to be done without the need for additional
explicit ``sudo`` commands from this point on.

By default, the actions of the ``root`` user at a login shell are audited.
Please see the :term:`Tlog` documentation and the `SIMP Tlog Puppet Module`_
for additional details.

.. NOTE::

   The SIMP system does not allow the ``root`` user to execute ``sudo`` by
   default per common configuration guidance.

.. NOTE::

   If you built your system from an ISO, you will probably have a local
   ``simp`` user that has the ability to run ``sudo su - root`` directly.

   The ``simp`` user is meant as an emergency 'break glass' user and should be
   removed or disabled once your environment is configured to your
   satisfaction.

.. _SIMP Tlog Puppet Module: https://github.com/simp/pupmod-simp-tlog

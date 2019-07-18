HOWTO Disable SSH Management
============================

If the ``ssh`` class (from the ``simp-ssh`` Puppet module) is included in your
SIMP scenario and you wish remove it from the class list and stop managing
SSH configuration, add the following to your :term:`Hiera` configuration:

.. code-block::  yaml

   ---
   # For client nodes
   simp::classes:
     - '--ssh'

   # For the SIMP server itself
   simp::server::classes:
     - '--ssh'



If the ``svckill`` class (from the ``simp-svckill`` Puppet module) is also
included in your SIMP scenario, it will *not* automatically kill ``sshd`` when
you cease management of the SSH configuration. This is because ``sshd`` has
been whitelisted by ``svckill::ignore_default``.  So, if you want ``svckill``
to kill running ``sshd`` services, you must add the following to your Hiera
configuration:

.. code-block::  yaml

   ---
   svckill::ignore:
     - '--sshd'


.. NOTE::

   The ``'--ssh'`` knockout prefix above **can not** prevent ``ssh`` from
   being classified if it is included  directly from Puppet code (e.g.,
   ``include 'ssh'``) or by an :term:`ENC`.  However, you can stop managing the
   SSH client and server configurations with the following Hiera configuration:

   .. code-block::  yaml

      ---
      ssh::enable_client: false
      ssh::enable_server: false




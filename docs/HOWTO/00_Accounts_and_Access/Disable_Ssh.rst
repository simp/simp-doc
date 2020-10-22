.. _disable_ssh_management:

Disable SSH Management
======================

If the :code:`ssh` class from :pupmod:`simp/ssh` is included in your SIMP scenario and you wish
remove it from the class list and stop managing SSH configuration, add to your :term:`Hiera`
configuration as follows:

* To remove from the client nodes only:

.. code-block::  yaml

   ---
   simp::classes:
     - '--ssh'

* To remove from the client nodes and SIMP server:

.. code-block::  yaml

   ---
   simp::classes:
     - '--ssh'

   simp::server::classes:
     - '--ssh'


Removing SIMP's :code:`ssh` class also removes the :code:`iptables` rule that allows
connections to :program:`sshd`.  However, if the :code:`svckill` class (from the
:pupmod:`simp/svckill` Puppet module) is also included in your SIMP scenario, it
will **not** automatically kill :program:`sshd` when you cease management of the SSH
configuration. This is because :program:`sshd` has been whitelisted by
:code:`svckill::ignore_defaults`.  So, if you want :code:`svckill` to kill running
:program:`sshd` services, you must add the following to your Hiera configuration:

.. code-block::  yaml

   ---
   svckill::ignore:
     - '--sshd'

.. NOTE::

   The :code:`'--ssh'` knockout prefix above **cannot** prevent :code:`ssh` from
   being classified if it is included  directly from Puppet code (e.g.,
   :code:`include 'ssh'`) or by an :term:`ENC`.  However, you can stop managing the
   SSH client and server configurations with the following Hiera configuration:

   .. code-block::  yaml

      ---
      ssh::enable_client: false
      ssh::enable_server: false

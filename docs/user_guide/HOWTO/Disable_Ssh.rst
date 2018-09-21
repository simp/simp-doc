HOWTO Disable SSH
=================

If SSH is included in your SIMP scenario and you wish to cherry-pick
it out of the class list and cease to manage its configuration, add
the following Hiera:

.. code-block::  yaml

  ---
  simp::classes:
    - '--ssh'

SVCKill will *not* automatically kill sshd when you cease management
of the module; it is whitelisted in the default ``svckill::ignore_default``
list. If you want svckill to kill running sshd processes, include:

.. code-block::  yaml

  ---
  svckill::ignore:
    - '--sshd'

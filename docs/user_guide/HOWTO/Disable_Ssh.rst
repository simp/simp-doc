HOWTO Disable SSH Management
============================

If ``simp-ssh`` is included in your SIMP scenario and you wish to cherry-pick
it out of the class list and cease to manage its configuration, add the
following to your :term:`Hiera` configuration:

.. code-block::  yaml

   ---
   simp::classes:
     - '--ssh'

``simp-svckill`` will *not* automatically kill ``sshd`` when you cease
management of the module since it has been whitelisted by
``svckill::ignore_default``. If you want ``svckill`` to kill running ``sshd``
services then add the following to your Hiera configuration.

.. code-block::  yaml

   ---
   svckill::ignore:
     - '--sshd'

.. _faq_omni_exists:

OMNI Environment Already Exists Error
=====================================

This FAQ covers what to do when running simp config gives you the 
``"Unable to configure: Invalid SIMP omni-environment for 'production' exists"`` error

If installing SIMP on Puppet Enterprise (or an already configured server), a user may encounter this error when running `simp config`:

.. code-block:: bash

    Unabled to configure: Invalid SIMP omni-environment for 'production' exists:
      >> Puppet environment 'production' exists
      >> Secondary environment 'production' does not exist

or (depending on the version):

.. code-block:: bash

    Unable to configure: Invalid SIMP omni-environment for 'production' exists:
      >> Puppet environment 'production' exists
      >> Secondary environment 'production' does not exist


If your environment has been set up and you would like to start over, then just rename the 
production directory ``(/var/simp/environments/production)`` to ``production.bak`` and try 
``simp config`` and ``simp bootstrap`` with a clean start.

If you are using Puppet Enterprise, we recommend you use a control repo. See the section on 
:ref:`howto-setup-a-simp-control-repository`

 

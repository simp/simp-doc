.. _faq_omni_exists:
OMNI Environment Already Exists Error
=====================================

This FAQ covers what to do when running simp config gives you the "Unable to configure: Invalid SIMP omni-environment for 'production' exists" error

If running SIMP 6.4.0 and you encounter this error:

.. code-block:: bash

    Unabled to configure: Invalid SIMP omni-environment for 'production' exists:
      >> Puppet environment 'production' exists
      >> Secondary environment 'production' does not exist

This is because you have either already run simp config, PE has set up that directory, or you have already set up your environment.

If your environment has been set up and you would like to start over, then just rename the 
production directory `(/var/simp/environments/production)` to `production.bak` and try again with a clean start.

If you are using Puppet Enterprise, we recommend you use a control repo. See the section on 
:ref:`howto-setup-a-simp-control-repository`
 

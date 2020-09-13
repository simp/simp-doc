.. _faq_omni_exists:
OMNI Environment Already Exists Error
=====================================

This FAQ covers what to do when running simp config gives you the "Unable to configure: Invalid SIMP omni-environment for 'production' exists" error

If running SIMP 6.4.0 and you encounter this error:

.. code-block:: bash

    Unabled to configure: Invalid SIMP omni-environment for 'production' exists:
      >> Puppet environment 'production' exists
      >> Secondary environment 'production' does not exist

This is because you have either already sun simp config, or you have already set up your environment.

If your environment has been set up and you would like to start over, then just rename the 
production directory to `production.bak` and try again with a clean start.

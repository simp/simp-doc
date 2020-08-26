.. _howto-use-an-alternate-simp-config-environment:

HOWTO Use an Alternate ``simp config`` Environment
==================================================

Generally, when running the ``simp config`` and ``simp bootstrap`` commands, all
items will be placed into the ``production`` :term:`Puppet Environment` and
:term:`SIMP Secondary Environment`.

If you are installing SIMP onto a preexisting system, you may want to instead
install into an environment other than ``production``.

As of ``simp-cli`` version 6.0.0 (new with SIMP 6.5), you can configure
the ``simp config`` and ``simp bootstrap`` commands to use an
alternate initial environment by setting the shell environment 
variable, ``$SIMP_ENVIRONMENT``.

For example, if you wanted to name your initial environment ``simp``, then you
would do the following:

  .. code-block:: bash

     export SIMP_ENVIRONMENT='simp'
     simp config
     ...
     simp bootstrap

See :ref:`gsg-advanced-configuration` for more information on ``simp config``.

.. _howto-use-an-alternate-simp-config-environment:

Using an Alternate 'simp config' Environment
============================================

Generally, when running the :program:`simp config` and :program:`simp bootstrap` commands, all
items will be placed into the :code:`production` :term:`Puppet Environment` and
:term:`SIMP Secondary Environment`.

If you are installing SIMP onto a preexisting system, you may want to instead
install into an environment other than :code:`production`.

As of ``simp-cli`` version 6.0.0 (new with SIMP 6.5), you can configure
the :program:`simp config` and :program:`simp bootstrap` commands to use an
alternate initial environment by setting the shell environment
variable, :code:`$SIMP_ENVIRONMENT`.

For example, if you wanted to name your initial environment :code:`simp`, then you
would do the following:

  .. code-block:: bash

     export SIMP_ENVIRONMENT='simp'
     simp config
     ...
     simp bootstrap

See :ref:`gsg-advanced-configuration` for more information on :program:`simp config`.

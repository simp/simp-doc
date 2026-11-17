.. _ug-puppet-issues:

Puppet Issues
=============


.. _ug-ts-puppet-depwarnings:

Puppet Deprecation Warnings
---------------------------

:term:`OpenVox` prints a deprecation warning for some settings.  If one of
these settings is present in a host's ``puppet.conf``, you may see warnings at
the beginning of every ``puppet`` run like this:

.. code-block:: none

   Warning: Setting <name> is deprecated.

These messages do not stop the run.  Remove the deprecated setting from
``puppet.conf`` to stop the warning.

SIMP's ``pupmod`` module manages its settings in ``puppet.conf`` without using
deprecated settings.  It turns the Puppet CA on or off in the
:file:`services.d/ca.cfg` file of OpenVox Server, not with the deprecated
``ca`` setting in ``puppet.conf``.


.. _ug-ts-puppet-disabling-depwarnings:

Disabling `all` Puppet deprecation warnings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you are encountering an unavoidable deprecation warning and find it
unacceptable, you can suppress such warnings during your ``puppet`` runs by disabling `all`
deprecation warnings:

.. WARNING::

   **This will disable all deprecation warnings**.  If new settings are deprecated
   in future releases, you will not see warnings about them. This is
   particularly important if you manage additional ``puppet.conf`` settings.
   Use with caution!

.. code-block:: puppet

   pupmod::conf { 'disable_all_deprecation_warnings':
     section => 'main',
     setting => 'disable_warnings',
     value   => 'deprecations',
     confdir => $facts['puppet_settings']['main']['confdir'],
     notify  => Service[puppetserver],
     ensure  => present,
   }

This will ensure the following setting in ``puppet.conf``:

.. code-block:: ini

  [main]
  disable_warnings = deprecations

If you want to enable deprecation warnings again, change ``ensure => present`` to ``ensure =>
absent``.

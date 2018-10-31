.. _ug-puppet-issues:

Puppet Issues
=============


.. _ug-ts-puppet-depwarnings:

Puppet Deprecation Warnings
---------------------------

Puppet 5 has added deprecation warnings to several settings.  If one of these
settings is present in a host's ``puppet.conf``, you may see warnings at the
beginning of every ``puppet`` run like this:
::

    Warning: Setting configprint is deprecated.
    (location: /opt/puppetlabs/puppet/lib/ruby/vendor_ruby/puppet/settings.rb:1169:in 'issue_deprecation_warning')

These messages are innocuous.  The deprecated settings can still be used until
Puppet 6.

SIMP's ``pupmod`` module usually manages its settings in ``puppet.conf``
without using deprecated settings.  However, under some circumstances it has no
other choice, resulting in unavoidable error messages.


``Warning: Setting ca is deprecated.``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``[master] ca`` is marked as deprecated from Puppet 5.5.6 onward.  This results
in a harmless—but unavoidable—deprecation warning whenever ``puppet`` is run
on Puppet masters configured to `not` act as the Puppet CA
(``pupmod::master::enable_ca: false``):
::

    Warning: Setting ca is deprecated.
    (location: /opt/puppetlabs/puppet/lib/ruby/vendor_ruby/puppet/settings.rb:1169:in 'issue_deprecation_warning')



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

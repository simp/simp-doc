.. _upgrade-6.2.0-to-6.3.0:

Upgrading from SIMP-6.2.0 to SIMP-6.3.0
---------------------------------------

Possible warnings when running ``puppet``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.. NOTE::

   After upgrading to ``puppet-agent`` 5.5.6+, Puppet masters that are `not`
   Puppet CAs (``pupmod::master::enable_ca: false``) will display a
   deprecation warning whenever ``puppet`` is run:
   ::
       Warning: Setting ca is deprecated.
       (location: /opt/puppetlabs/puppet/lib/ruby/vendor_ruby/puppet/settings.rb:1169:in `issue_deprecation_warning')

   This message is innocuous, but unavoidable unless you are comfortable
   with :ref:`ug-ts-puppet-disabling-depwarnings`.


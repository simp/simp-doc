.. _faq-puppet:

Puppet-Related Issues
=====================

.. _faq-puppet-debug_mode_crash:

Running Puppet Agent in Debug Mode Crashes
------------------------------------------

The `FACT-1732`_ bug, present in some versions of `Facter 3`_, can cause
`facter` to crash when attempting to print `Bignum`_ level numbers.

.. NOTE::
   On a 64-bit system, a ``Bignum`` value is ``(2**62)`` or higher

This will affect runs of ``puppet agent -t --debug`` as well as ``facter -p``.

It is highly likely that you will have one of these values from the ``shmall``
fact provided by the ``simplib`` module.

.. _Bignum: https://ruby-doc.org/core-2.2.0/Bignum.html
.. _FACT-1732: https://tickets.puppetlabs.com/browse/FACT-1732
.. _Facter 3: https://docs.puppet.com/facter/3.8/

.. _faq-puppet-generate_types:

When Should I Run `puppet generate types`?
------------------------------------------

The `puppet generate types`_ command was added to help solve the `SERVER-94`_
:term:`puppet environment` isolation issue by caching :term:`custom type`
definitions in each environment.

SIMP has a Puppet class called ``pupmod::master::generate_types``, which is
enabled by default, and uses ``incron`` to automatically run ``puppet generate
types`` in the following cases:

  * The ``puppet`` or ``puppetserver`` binaries have been updated.
  * A new environment is added to the system.

You will need to run ``puppet generate types`` manually if the following occurs:

  * A new module is added to your environment that includes custom types.
  * An existing custom type has its code modified.

.. NOTE::
   Versions 7.6.0 through 7.7.1 of SIMP's ``pupmod`` Puppet module tried to
   include all of the cases above to ensure that users did not need to
   manually adjust any aspects of their systems. However, this proved to
   potentially add too much load to the system in certain situations and
   was reduced to the current functionality.

If you are using :term:`r10k` then you can add a `postrun`_ snippet to run
``puppet generate types`` on the modified environments.

.. _SERVER-94: https://tickets.puppetlabs.com/browse/SERVER-94
.. _postrun: https://github.com/puppetlabs/r10k/blob/master/doc/dynamic-environments/configuration.mkd#postrun
.. _puppet generate types: https://puppet.com/docs/puppet/latest/environment_isolation.html

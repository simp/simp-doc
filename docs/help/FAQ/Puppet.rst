Puppet-Related Issues
=====================

Running Puppet Agent in Debug Mode Crashes
------------------------------------------

The `FACT-1732`_ bug, present in some versions of `Facter 3`_, can cause
`facter` to crash when attempting to print `Bignum`_ level numbers.

.. NOTE::
   On a 64-bit system, a ``Bignum`` value is ``(2**62)`` or higher

This will affect runs of ``puppet agent -t --debug`` as well as ``facter -p``.

It is highly likely that you will have one of these values from the ``shmall``
fact provided by the ``simplib`` module.

.. _FACT-1732: https://tickets.puppetlabs.com/browse/FACT-1732
.. _Facter 3: https://docs.puppet.com/facter/3.8/
.. _Bignum: https://ruby-doc.org/core-2.2.0/Bignum.html

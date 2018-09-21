Node Classification in SIMP
---------------------------

From the Puppet, Inc. website:

  Hiera is a key/value lookup tool for configuration data, built to set
  node-specific data without repeating yourself.

SIMP uses :term:`Hiera` to attempt to make configuration of the overall system easier
for our end users by providing a simple, centralized, method for setting class
parameters using `automatic parameter lookup`_ and as a method for
`basic node classification`_.

It is **highly recommended** that you read the `Hiera Documentation`_ prior to
jumping into using a SIMP system.

.. _Hiera Documentation: https://docs.puppet.com/hiera/3.3/complete_example.html
.. _automatic parameter lookup: https://docs.puppet.com/hiera/3.3/puppet.html#automatic-parameter-lookup
.. _basic node classification: https://docs.puppet.com/hiera/3.3/puppet.html#assigning-classes-to-nodes-with-hiera-hierainclude

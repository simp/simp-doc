.. _faq-puppet:

Puppet-Related Issues
=====================

.. contents:: :local:

.. _faq-puppet-debug_certs:

What do I do when puppet gets certificate verification errors?
--------------------------------------------------------------

If you are experiencing an error when running puppet such as ``certificate
verify failed`` then there are a few things that you can try in an attempt to
troubleshoot the issue.

#. Make sure that your system clocks are within one hour of each other.
#. Ensure that the forward and reverse lookup for the FQDN of your systems is
   correct and matches the hostnames listed in the output of
   ``openssl x509 -text -noout -in $(puppet config print hostcert) | less``

   HINT: Look at the `Subject` and `X509v3 Subject Alternative Name` sections.

#. Check that the connection from the client system to the server can
   successfully connect:

   .. code-block:: bash

      openssl s_client -host $(puppet config print server) \
      -port $(puppet config print masterport) \
      -cert $(puppet config print hostcert) \
      -key $(puppet config print hostprivkey) \
      -CAfile $(puppet config print localcacert)

If none of these items provides useful information, you may need to check
permissions on your server and/or dig more closely into the puppetserver or
client logs.

.. _faq-puppet-debug_mode_crash:

Why is my Puppet Agent crashing when run with ``--debug``?
----------------------------------------------------------

The bug `FACT-1732`_ can cause Facter to crash while attempting to print a
`Bignum`_-sized number.  On 64-bit systems, this is any integer greater than
**4611686018427387903** [#]_.

.. NOTE::

   Facts provided by SIMP's modules are **not affected** by FACT-1732.

* This issue only affects facts introduced from *non-SIMP* sources.
* It will cause the commands ``puppet agent -t --debug`` and ``facter -p``
  to fail with errors when they encounter Bignum-sized *numeric* fact values.
* You can fix your own facts to avoid FACT-1732 by returning any potentially
  large numeric value as a String.

.. rubric:: Older versions of SIMP and FACT-1732

SIMP modules' facts haven't been susceptible to FACT-1732 since SIMP
6.1.0-0.  Before that, the ``shmall`` and ``shmax`` facts from
:program:`simp-simplib` would crash on systems with a lot of memory.

.. _Bignum: https://ruby-doc.org/core-2.3.0/Bignum.html
.. _FACT-1732: https://tickets.puppetlabs.com/browse/FACT-1732
.. _Facter 3: https://docs.puppet.com/facter/3.8/
.. [#] 4611686018427387904 == 2 :sup:`62`

.. _faq-puppet-generate_types:

When should I run ``puppet generate types``?
--------------------------------------------

The ``puppet generate types`` command addresses the problem of `Puppet
Environment isolation`_ (`SERVER-94`_) by generating :term:`custom type`
metadata definitions for each environment.  The command must therefore be
re-run in response to changes in Puppet environments and compilers.

By default, SIMP automates some of these cases using :program:`incron`
triggers. However, there are still some situations where you will have to make
sure that ``puppet generate types`` is run.

Situations :program:`incron` handles automatically
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By default, SIMP configures the :program:`incron` daemon to automatically run
``puppet generate types`` under either of the following circumstances:

  * The ``puppet`` or ``puppetserver`` binaries have been updated.
  * A new :term:`Puppet environment` directory is added to the system.

This behavior is managed by the Puppet class ``pupmod::master::generate_types``.

.. rubric:: Differences from Previous versions of SIMP

Earlier versions of :program:`simp-pupmod` (7.6.0 through 7.7.1, shipped with
SIMP 6.2.0-0 through 6.3.1-0) attempted to automatically trigger ``puppet
generate types`` under every relevant circumstance.  However, some of the
triggers could add too much load on the system and were removed from the
:program:`incron`'s watchlist.

These situations must be addressed by other means (see below).


Situations :program:`incron` doesn't handle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:program:`incron` does not handle all cases, so you will need to ensure that
``puppet generate types`` is after the following events:

  * A new *module* that includes custom types is added to an existing environment.
  * An existing custom type's internal code is updated.


Generating types manually
^^^^^^^^^^^^^^^^^^^^^^^^^

You can run the ``puppet generate types`` command as **root** on the Puppet
Server.  However, in order to ensure that the Puppet Server process can read
the generated files, you must also ensure they have the correct ownership and
permissions.  One way to do this is by running the following command:

.. code-block:: bash

   (umask 0027 && sg puppet -c 'puppet generate types --environment ENVIRONMENT')

This creates all files with the correct group ownership.


Automatically generating types after ``r10k deploy environment``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you are using :term:`r10k` to deploy :term:`Control Repository` branches
using ``r10k deploy environment``, you can set the `"generate_types" option`_
in the :file:`r10k.yaml` file to automatically run :command:`puppet generate
types` for each environment after it is deployed:

.. code-block:: yaml
   :emphasize-lines: 3
   :caption: Inside :file:`r10k.yaml`:

   # Important: this option *must* be defined under a top-level `deploy:`
   deploy:
     generate_types: true

If you use :program:`r10k` to deploy modules as **root** on the Puppet Server,
you must ensure that the generated files have the correct ownership and
permissions for the Puppet Server process to read them.  One way to do this is
by running the following command:

.. code-block:: bash

   ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k deploy environment production' )

This will deploy the environment with the correct permissions and group
ownership.  If ``deploy/generate_types`` is set to ``true``, it will also
generate environment-safe type metadata files  with the same permissions and
ownership.

.. _SERVER-94: https://tickets.puppetlabs.com/browse/SERVER-94
.. _postrun: https://github.com/puppetlabs/r10k/blob/master/doc/dynamic-environments/configuration.mkd#postrun
.. _generate_types: https://github.com/puppetlabs/r10k/blob/master/doc/dynamic-environments/configuration.mkd#generate_types
.. _"generate_types" option: https://github.com/puppetlabs/r10k/blob/master/doc/dynamic-environments/configuration.mkd#generate_types
.. _Puppet Environment isolation: https://puppet.com/docs/puppet/5.5/environment_isolation.html

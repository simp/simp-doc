The SIMP Utility
----------------

The SIMP server provides a command line utility called ``simp`` that is an
interface into SIMP-specific settings and subsystems.

You can get information on the ``simp`` utility by running ``simp help`` on
your SIMP server.

.. _simp passgen:

simp passgen
^^^^^^^^^^^^

Throughout the SIMP codebase, you may find references to the ``simplib::passgen()``
function. This function auto-generates passwords and stores them in
the Puppet master in a sub-directory in the :term:`SIMP Writable Environment`:

``/opt/puppetlabs/server/data/puppetserver/simp/environments/<environment>/simp_autofiles/gen_passwd``


For more information, see the `simplib::passgen()`_ documentation.

.. _simplib::passgen(): https://github.com/simp/pupmod-simp-simplib/blob/master/lib/puppet/functions/simplib/passgen.rb

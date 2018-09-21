The SIMP Environment
--------------------

SIMP fully supports `Puppet Environments`_ and, by default, installs into an
environment named ``simp``. This environment is symlinked to the ``production``
environment by ``simp config`` but that symlink will **not** be overwritten on
update so you may freely change or replace the symlink to meet your needs.

There are a couple of paths on the system that are environment related.

/var/simp
^^^^^^^^^

This space holds all static, non-Puppet created files. It is generally used for
large binary items that will be centrally delivered via rsync and for files
that are too dangerous to add to a version control system. These include things
like the SIMP rsync materials and the Infrastructure keys.

This space is environment aware and you will note that there is an
``environments`` directory under ``/var/simp`` with, by default, the ``simp``
environment represented. If you add new environments, you will need to
replicate the appropriate structure from the ``simp`` environment into your
custom environment.

This space also holds FakeCA. See `Infrastructure Certificates`_.

.. NOTE::

   For more information on the SIMP rsync structure, please see
   :ref:`HOWTO Work with the SIMP Rsync Shares`

/opt/puppetlabs/server/data/puppetserver/simp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This space holds all non-static, Puppet **Server** created files. This is used
by both `passgen()`_ and the ``krb5`` Puppet module for storing dynamically
generated server-side content.

Like ``/var/simp`` this space is also environment aware but you should never
need to manually adjust anything in this directory space.

.. _Puppet Environments: https://docs.puppet.com/puppet/latest/environments.html
.. _passgen(): https://github.com/simp/pupmod-simp-simplib/blob/master/lib/puppet/parser/functions/passgen.rb

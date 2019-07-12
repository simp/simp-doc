The SIMP Environment
--------------------

SIMP fully supports `Puppet Environments`_ and, by default, ``simp config``
initially installs into the ``production`` environment.

However, a functioning SIMP system contains more than the Puppet environment.
It contains two additional environment directories, which, when combined with
the Puppet environment, comprise the :term:`SIMP Omni-Environment`.  ``simp
config`` actually creates a ``production`` SIMP  Omni-Environment when it is
executed on a fresh SIMP install.

.. TIP::

   You can create a new SIMP Omni-Environment or just subsets of the
   environment at any time using ``simp environment new``.

/var/simp/environments
^^^^^^^^^^^^^^^^^^^^^^

This is the parent directory of the
:term:`SIMP Secondary Environments<SIMP Secondary Environment>`. Each
secondary environment (sub-directory) holds all the static, non-Puppet
created files for the environment. A SIMP secondary environment is generally
used for large binary items that will be centrally delivered via ``rsync`` and
for files that are too dangerous to add to a version control system. These
include things like the SIMP rsync materials and the Infrastructure PKI keys. A
secondary environment also holds the ``FakeCA``. See `Infrastructure
Certificates`_.

If you wish to add a new Puppet environment, you must create a new SIMP
Omni-environment that includes a sub-directory at this location.  The
sub-directory needs to be created from the environment skeletons delivered by
the ``simp-environment-skeleton`` and ``simp-rsync-skeleton`` packages and have
the ``simp`` SELinux policy delivered by the ``simp-selinux-policy`` applied to
its contents.

The easiest way to create a new SIMP omni-environment is to use ``simp
environment new``. That will not only copy the files from the skeletons
(installed into ``/usr/share/simp/environment-skeleton``), but also ensure all
the permissions, SELinux context, and :term:`FACLs<FACL>` for the new
sub-directory tree are correct.

.. NOTE::

   For more information on the SIMP rsync structure, please see
   :ref:`HOWTO Work with the SIMP Rsync Shares`

/opt/puppetlabs/server/data/puppetserver/simp/environments
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is the parent directory of the
:term:`SIMP Writable Environments<SIMP Writable Environment>`. Each writable
environment (sub-directory) holds all the static Puppet **Server** created
files.  This is used by both `simplib::passgen()`_ and the ``simp-krb5`` Puppet
module for storing dynamically generated server-side content.

You should never need to manually adjust anything in this directory space and
it will be created automatically for you by the ``puppetserver`` when the SIMP
functions that use this environment are executed.

.. _Puppet Environments: https://docs.puppet.com/puppet/latest/environments.html
.. _simplib::passgen(): https://github.com/simp/pupmod-simp-simplib/blob/master/lib/puppet/functions/simplib/passgen.rb

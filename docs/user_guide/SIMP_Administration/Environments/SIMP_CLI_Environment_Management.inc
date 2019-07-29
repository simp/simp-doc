SIMP CLI Commands for Environment Management
--------------------------------------------

The SIMP CLI provides commands to aid you in many facets of Puppet
environment management.  This section will explain the basics of each command,
and then demonstrate how to use these commands in a typical environment
management scenario.

.. NOTE::

   All SIMP CLI environment management commands need to be run as ``root``.

simp environment new
^^^^^^^^^^^^^^^^^^^^

``simp environment new`` creates a new :term:`SIMP Omni-Environment skeleton`.
This allows sysadmins to create new, SIMP environments easily, when they
need to create a new Puppet environment.

By default, ``simp environment new`` creates a SIMP Omni-Environment skeleton,
which includes a ``Puppetfile`` that can be used to deploy Puppet modules from
local, SIMP-managed Git repositories.  Its behavior, however, is quite
flexible.  You can control which parts of the environment get created,
(including the ``Puppetfile``) with its command options. You can even have it
deploy the Puppet modules using ``r10k puppetfile install``, if you want it to
create the full environment in one step.

``simp environment new`` options are documented in its built-in SIMP CLI
documentation. To see the options available, execute the following:

.. code-block:: bash

   $ simp environment new -h

Default Behavior
~~~~~~~~~~~~~~~~

When no options are specified, ``simp environment new <env name>`` , takes the
following actions:

  * Creates the Puppet environment ``/etc/puppetlabs/code/environments/<env name>``
    from ``/usr/share/simp/environment-skeleton/puppet``.

    - This includes creating the appropriate ``environment.conf`` file from the
      skeleton's ``environment.conf.TEMPLATE``.

  * Generates ``Puppetfile`` and ``Puppetfile.simp`` in
    ``/etc/puppetlabs/code/environments/<env name>``

      * Equivalent to executing:

        .. code-block:: bash

           $ cd /etc/puppetlabs/code/environments/<env name>
           $ simp puppetfile generate --skeleton > Puppetfile
           $ simp puppetfile generate > Puppetfile.simp

      * *Puppetfile*:
          An empty parent Puppetfile that includes ``Puppetfile.simp`` and is
          where you would put site-specific module entries. Is used to deploy
          *all* the Puppet modules in both Puppetfiles.

      * *Puppetfile.simp*:
          A SIMP-only Puppetfile that could be used to deploy the latest versions
          of Puppet modules from local SIMP-managed Git repositories. These local
          repositories are created/updated when SIMP-packaged Puppet module RPMs
          are installed and can be easily updated with each SIMP release by
          executing ``simp puppetfile generate > Puppetfile.simp``.

      * See `simp puppetfile generate`_ for an explanation about the benefits
        of having these two Puppetfiles.

  * Creates the secondary environment ``/var/simp/environments/<env name>`` from
    ``/usr/share/simp/environment-skeleton/secondary``.
  * Copies files from ``/usr/share/simp/environment-skeleton/rsync`` into
    ``/var/simp/environments/<env name>/rsync``.
  * If any ``tftpboot`` :term:`PXE` image files can be found in directories
    matching ``/var/www/yum/CentOS/**/x86_64/images/pxeboot``, copies those files
    into the appropriate sub-directory in
    ``/var/simp/environments/<env name>/rsync/RedHat/Global/tftpboot/linux-install``.
  * Creates a new FakeCA cacertkey file at
    ``/var/simp/environments/<env name>/FakeCA/cacertkey``. The file is populated with
    a random string.

  * Re-applies the :term:`FACLs <FACL>`, :term:`SELinux` contexts, and permissions
    to the newly-created directory trees.

    - Equivalent to executing: ``simp puppetfile fix <env name>``

  * Does not create the Writable environment, because in this scenario, that
    environment will be automatically generated, as needed, when ``puppet``
    is run

simp environment fix
^^^^^^^^^^^^^^^^^^^^

``simp environment fix`` re-applies the :term:`FACLs <FACL>`, :term:`SELinux`
contexts, and permissions to a SIMP Omni-Environment. This can be especially
useful, when you have added files to the environment and are unsure which
settings are appropriate for them.

By default, ``simp environment fix`` operates on all directories in the
SIMP Omni-Environment.  However, its behavior is quite flexible. You can
control which parts of the environment get fixed with its command options.

``simp environment fix`` options are documented in its built-in SIMP CLI
documentation. To see the options available, execute the following:

.. code-block:: bash

   $ simp environment fix -h

Default Behavior
~~~~~~~~~~~~~~~~

When no options are specified, ``simp environment fix <env name>`` , takes the
following actions:

* Applies Puppet permissions (group) to ``/var/simp/environments/<env name>``.
* Applies Puppet permissions (user + group) recursively under
  ``/etc/puppetlabs/code/environments/<env name>``.
* Applies Puppet permissions (group) recursively
  under ``/var/simp/environments/<env name>/site_files``.
* Restores SELinux contexts under ``/var/simp/environments/<env name>``.
* Applies FACL rules to ``/var/simp/environments/<env name>/rsync``


simp puppetfile generate
^^^^^^^^^^^^^^^^^^^^^^^^

``simp puppetfile generate`` creates one of two different types of
:term:`Puppetfiles <Puppetfile>`:

* *SIMP-only Puppetfile*:
    A Puppetfile that could be used to deploy the latest versions of Puppet
    modules from local SIMP-managed Git repositories. These local repositories
    are created/updated when SIMP-packaged Puppet module RPMs are installed.

* *Puppetfile that includes the SIMP-only Puppetfile*:
    An empty parent Puppetfile that includes the SIMP-only Puppetfile
    (``Puppetfile.simp`` in the same directory), provides a place for you
    to put site-specific module entries, and can be used to deploy *all* the
    modules in both Puppetfiles.

SIMP intentionally designed the ``simp puppetfile generate`` with these
different generation capabilities, because the separation of SIMP-only modules
and site-specific modules into different Puppetfiles allows easy update of
SIMP-only Puppet modules, while still allowing all the modules to be deployed
from a single file (the parent Puppetfile).

If you want to update SIMP-only Puppetfile entries after installing/upgrading
SIMP module RPMs, all you have to do is to execute:

.. code-block:: bash

   $ simp puppetfile generate > <your SIMP-only Puppetfile path>

Your list of site-specific Puppet modules in the parent Puppetfile will be
unaffected.

For example, to update the SIMP-only Puppetfile created by ``simp config`` in
a standard SIMP server install from SIMP ISO or SIMP RPMs, execute:

.. code-block:: bash

   $ cd /etc/puppetlabs/code/environments/production
   $ simp puppetfile generate > Puppetfile.simp

Default Behavior: Generating Latest SIMP-only Puppetfile Content
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When no options are specified, ``simp puppetfile generate``, prints to the
console a Puppetfile that could be used to deploy the current set of SIMP Puppet
modules installed via RPM in ``/usr/share/simp/modules`` from the corresponding
local, SIMP-managed Git repositories in ``/usr/share/simp/git/puppet_modules``.

Each Puppetfile entry will look like the following:

.. code-block:: ruby

   mod 'simp-simplib',
     :git => 'file:///usr/share/simp/git/puppet_modules/simp-simplib.git',
     :tag => '3.15.2'

The module name will be the name of the Puppet module in its ``metadata.json``
file (``<Puppet Forge org>-<module name>``), the Git URL will be its local,
SIMP-managed Git repository, and the Git version will be the latest tag in
that local repository.

SIMP chose the naming convention to match that of :term:`PuppetForge`.

.. TIP::

   The corresponding SIMP-packaged Puppet module RPM name is

     ``pupmod-<Puppet Forge org>-<module name>``


Generating a Parent Puppetfile Skeleton
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When the ``--skeleton`` option is specified,
``simp puppetfile generate --skeleton`` prints to the console a parent
Puppetfile that includes ``Puppetfile.simp``, the assumed name of the
Puppetfile containing only SIMP modules.  This parent Puppetfile indicates
where you would add site-specific modules, that would be deployed along with
those in the included ``Puppetfile.simp``.  The ``Puppetfile.simp`` file is
assumed to be in the same directory as the parent Puppetfile.

.. TIP::

  You can optionally have local modules in a specified environment
  automatically added to this parent Puppetfile using the ``--local-modules
  SRC_ENV`` option.  Local modules are modules whose directories are not under
  Git source control and for which there is no local Git repository.

Other Options
~~~~~~~~~~~~~

The options ``simp puppetfile generate`` provides are documented in its built-in
SIMP CLI documentation. To see the options available, execute the following:

.. code-block:: bash

   $ simp puppetfile generate -h


Example:  Create a test environment for assessing a new SIMP version
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*Under Construction*

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
           $ simp puppetfile generate > Puppetfile.simp
           $ simp puppetfile generate -s > Puppetfile

      * *Puppetfile.simp*:
          SIMP-only Puppetfile that contains entries for local, SIMP-managed Git
          repositories.  Can be easily updated with each SIMP release by
          executing ``simp puppetfile generate > Puppetfile.simp``.

      * *Puppetfile*:
          Empty empty Puppetfile that includes ``Puppetfile.simp``.  This is
          where you would put site-specific module entries.

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

The specific actions ``simp environment fix <env name>`` takes are as follows:

* Applies Puppet permissions (group) to ``/var/simp/environments/<env name>``.
* Applies Puppet permissions (user + group) recursively under
  ``/etc/puppetlabs/code/environments/<env name>``.
* Applies Puppet permissions (group) recursively
  under ``/var/simp/environments/<env name>/site_files``.
* Restores SELinux contexts under ``/var/simp/environments/<env name>``.
* Applies FACL rules to ``/var/simp/environments/<env name>/rsync``


simp puppetfile generate
^^^^^^^^^^^^^^^^^^^^^^^^

``simp puppetfile generate`` creates Puppetfiles that simplify SIMP upgrades.
It automatically detects the latest version of SIMP modules installed by RPM,
and creates Puppetfile content with the latest versions.  So, if you want to
update a file containing only SIMP modules, execute:

.. code-block:: bash

   $ simp puppetfile generate > <your SIMP-only Puppetfile path>

FILL-ME-IN


Example:  Create a test environment for assessing a new SIMP version
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*Under Construction*

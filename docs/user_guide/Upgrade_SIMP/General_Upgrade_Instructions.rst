.. _ug-general-upgrade-instructions:

General Upgrade Instructions
----------------------------

SIMP uses the Puppet modules' parameters as the system "API" (in terms of
compatibility) and attempts to limit any API breaking changes to a minimum
during a major release.

API breaking changes will have at least one minor release with deprecation
warnings unless the change was to fix an actual bug in functionality.

A SIMP release version (e.g., "|simp_version|") can be separated into three
major numbers, in the format `X.Y.Z`:

* ``X`` is the MAJOR release number, and indicates severe API-breaking changes.

* ``Y`` is the MINOR release number, and indicates the addition of features or
  minor API-breaking changes either due to functionality bugs or after at least
  one MINOR release announcing the deprecation.

  * All API-breaking changes are kept to an absolute minimum and well
    documented in the release CHANGELOG.

* ``Z`` is the PATCH release number, and indicates full backwards-compatibility
  changes, such as bug fixes and improvements.

This section describes both the general, recommended upgrade procedures for
``X``, ``Y``, or ``Z`` releases.

.. _ug-incremental-upgrades:

Incremental Upgrades
~~~~~~~~~~~~~~~~~~~~

For ``Y`` and ``Z`` SIMP changes, you should feel comfortable dropping the
changes directly into your **test** systems. The promotion cycle from test to
production should be short and painless if you reference the version upgrade
documentation.

.. IMPORTANT::

   Review any :ref:`ug-version-specific-upgrade-instructions` prior to
   executing an Incremental Upgrade. There may be specific instructions
   regarding the upgrade process that you should follow.

.. _ug-incremental-upgrades-w-iso:

Incrementally upgrading systems using local repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Update the Repositories

   * Update the repositories  for an ISO Installation:

     If you built your SIMP server by :ref:`gsg-installing_simp_from_an_iso`,
     update the repositories by unpacking the ISO:

     #. Copy the new SIMP ISO file to the SIMP master
     #. From the SIMP master (as ``root``):

        .. code-block:: sh

           # Unpack the new SIMP ISO's RPMs into yum repositories
           unpack_dvd </path/to/ISO>

   * For RPM-based installation, follow your site's procedures to update your
     repositories.

#. Install the RPMs

   Update the system-local ``git`` repositories by installing the new RPMs

   .. code-block:: sh

      # Make sure yum picks up the new RPMs
      yum clean all; yum makecache

      # Apply updates to the local master
      yum update -y

#. If you are upgrading from a version before SIMP 6.4 you can skip to the last
   step, ``Apply the changes by running puppet``.

#. Generate the new ``Puppetfile.simp``

   **Only do this step you are upgrading from version SIMP 6.4 or later.**

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/<environment to update>

      simp puppetfile generate > Puppetfile.simp

#. Verify the environment's Puppetfile

   **Only do this step you are upgrading from version SIMP 6.4 or later.**

   .. Warning::

      Any module not listed in the ``Puppetfile`` will be deleted from the
      target environment's (``production`` by default) ``modules`` directory.

   Make sure the ``Puppetfile`` you will be deploying from includes the following:

   * A line that includes the ``Puppetfile.simp`` which should look like:

     .. code-block:: ruby

        instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   * A line for each of your own modules.

     To generate a list of non-simp modules in an environment do the following:
     (This example uses the production environment):

     .. code-block:: sh

        simp puppetfile generate -s -l production > /tmp/Puppetfile

     This will generate ``/tmp/Puppetfile`` which has a directive to include
     the file ``Puppetfile.simp`` and  a local entry for each module that
     presently exists in the ``production`` environment's ``modules`` directory
     that is not also in the  SIMP repository directory,
     ``/usr/share/simp/git/puppet_modules``.

     These entries will look like the following:

     .. code-block:: yaml

        mod 'module name', :local => true

     Verify that all modules with a local entry in ``/tmp/Puppetfile`` are  in
     your environment's ``Puppetfile`` in one of the following forms:

      .. code-block:: yaml

          # a module that is not a Git repository and resides in the ``modules`` directory
          mod 'site',
            :local => true

          # a Git repository that resides in a directory on the Puppet server
          mod 'mymodule'
            :git => 'file:///usr/share/mymodules/mymodule',
            :tag => '1.1.1'

          #  a Git repository on a remote server
          mod 'mysrvmod'
            :git => 'https://gitserver.my.domain/mygitproject/mysrvmod.git'
            :tag => '1.0.1'

    .. Note::

       If there are any modules on the local system that are not also in a
       ``git`` repository (the ones that use the ``:local => true`` directive),
       you should seriously consider creating a ``git`` repository for it to
       make sure it does not get removed by ``r10k``.

#. Deploy the modules from the local ``git`` repositories into the Environment

   **Only do this step you are upgrading from version SIMP 6.4 or later.**

   Use ``r10k`` to deploy the modules making sure the ``umask`` and ``group``
   are set correctly so that the ``puppetserver`` has access to the files.

   .. code-block:: sh

      # Set the umask and Run r10k as the puppet group to make sure the modules
      # to make sure the permissions and ownership are correct on the modules
      ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
      --puppetfile /etc/puppetlabs/code/environments/production/Puppetfile \
      --moduledir /etc/puppetlabs/code/environments/production/modules' )


#. Apply the changes by running puppet

   .. code-block:: sh

      puppet agent -t


Incrementally upgrading systems using r10k or Code Manager
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you manage your SIMP server using :term:`r10k` or :term:`Code Manager` you
will need to work with the upstream ``git`` repositories as appropriate for
your workflow.  This is the same for all versions of SIMP.


Breaking Changes
~~~~~~~~~~~~~~~~

If the ``X`` version number has changed then you should expect **major**
breaking changes to the way SIMP works. Please carefully read the Changelog and
the :ref:`_simp-user-guide` and do **not** deploy these changes directly on top
of your ``production`` environment.

If the ``Y`` version number has changed then there may either be deprecation
notices or **minor** breaking changes to the way SIMP works. Please carefully
read the CHANGELOG and the User's Guide and do **not** deploy these changes
directly on top of your production environment.

.. IMPORTANT::

   Upgrading SIMP does **not** require re-kicking your clients, even if some
   core services move to the new Puppet node.  All software configurations can
   be updated in Puppet, as needed.

With the release of 6.4, SIMP RPM upgrades now have a "hands-off" approach to
upgrades that allow users to easily preserve different combinations of module
sets as required by their environment. That being said, the SIMP team does not
test all combinations of modules and may have difficulty providing support for
untested combinations.

New Server Creation and Client Migration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The recommended method for upgrading **major** breaking changes (``X`` bump) is
to create a new Puppet Server and migrate your data and clients to it. This
process follows the path of least destruction; we will guide you through how to
back up the existing Puppet server, create a new server, and transfer your
clients.

#. Set up a new Puppet server that will house your new SIMP environment.

   .. NOTE::

      You must ensure that this node can be reached by any client that is to be
      migrated. The new system will not interfere with your existing Puppet
      system unless you specifically configure it to do so.

   .. IMPORTANT::

      Do **NOT** destroy your old Puppet server until everything has been
      successfully migrated and is in production under the new server.

#. Consider vital services other than Puppet that are housed on your current
   Puppet server node (eg. DNS, DHCP, LDAP, custom kickstart, YUM, NFS, etc.).
   You may choose to keep many of these services running on your old Puppet
   server node. Anything not preserved must be migrated to a new system.

Back Up the Existing Puppet Server
""""""""""""""""""""""""""""""""""

Prior to any modifications to your infrastructure, we **highly** recommend
following :ref:`ug-howto-back-up-the-puppet-master`.

Create a New Server
"""""""""""""""""""

Obtain an `official SIMP ISO <https://download.simp-project.com/simp/ISO/>`_ or point your
server at the latest `YUM Repositories <https://packagecloud.io/simp-project>`_
and follow the :ref:`gsg_iso_installation_options` or
:ref:`gsg-installing_simp_from_a_repository` as appropriate.

Follow the :ref:`Client_Management` guide, and set up services as needed.
Remember, you can opt-out of any core services (DNS, DHCP, etc.)  you want your
clients or old Puppet server to run! If you want the new Puppet server to run
services the existing Puppet server ran, you may be able to use the backup of
the ``rsync`` directories from the old system.

.. WARNING::

   Do not blindly drop ``rsync`` (or other) materials from the old Puppet
   server onto the new one. The required structures for these components may
   have changed.

When you :ref:`ug-apply-certificates` you may wish to transfer client certs to
the new server.  If you are using the FakeCA and still wish to preserve the
certificates, follow the :ref:`ug-apply-certificates-official-certificates`
guidance, and treat the existing Puppet server as your 'proper CA'.

Promote the New Puppet Server and Transfer Your Clients
"""""""""""""""""""""""""""""""""""""""""""""""""""""""

Follow the :ref:`ug-howto-change-puppet-masters` guide to begin integration
of your new Puppet server into the existing environment.

.. NOTE::

   You should *always* start migration with a small number of
   **least critical** clients!

Retire the Old Puppet Server
""""""""""""""""""""""""""""

Once you have transferred the management of all your clients over to
the new Puppet server, you may safely retire the old Puppet server.

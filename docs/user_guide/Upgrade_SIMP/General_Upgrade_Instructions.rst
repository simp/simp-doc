.. _ug-general-upgrade-instructions:

General Upgrade Instructions
----------------------------

SIMP follows Semantic Versioning 2.0.0, using the Puppet modules' parameters as
the "API" (in terms of compatibility).

A SIMP release version (e.g., "|simp_version|") can be separated into three
major numbers, in the format `X.Y.Z`:

* ``X`` is the MAJOR release number, and indicates API-breaking changes
* ``Y`` is the MINOR release number, and indicates the addition of features.
* ``Z`` is the PATCH release number, and indicates backwards-compatible
  changes, such as bug fixes and improvements.

This section describes both the general, recommended upgrade procedures
for ``X``, ``Y``, or ``Z`` releases.

.. _ug-incremental-upgrades:

Incremental Upgrades
~~~~~~~~~~~~~~~~~~~~

For ``Y`` and ``Z`` SIMP changes, you should feel comfortable dropping the changes
directly into your test systems. The promotion cycle from test to production
should be short and painless.

.. IMPORTANT::

   Review any :ref:`ug-version-specific-upgrade-instructions` prior to
   executing an Incremental Upgrade. There may be specific instructions
   regarding the upgrade process that you should follow.

.. _ug-incremental-upgrades-w-yum

Incrementally upgrading an ISO installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you built your SIMP server by :ref:`gsg-installing_simp_from_an_iso`,
updating your entire local SIMP distribution should be as simple as:

#. Copy the new SIMP's ISO file to the SIMP master
#. From the SIMP master (as ``root``):

   .. code-block:: sh

      # Unpack the new SIMP ISO's RPMs into yum repositories
      unpack_dvd </path/to/ISO>

      # Make sure yum picks up the new RPMs
      yum clean all; yum makecache

      # Apply updates to the local master
      yum update -y

      # Apply updated Puppet modules to the local master
      puppet agent -t


.. _ug-incremental-upgrades-w-yum

Incrementally upgrading a yum/RPM-based installation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you built your SIMP server by :ref:`gsg-installing_simp_from_a_repository`,

#. Update your site's ``yum`` repositories with packages for the new version of
   SIMP.
#. From the SIMP master (as ``root``):

   .. code-block:: sh

      # Make sure yum picks up the new RPMs
      yum clean all; yum makecache

      # Apply updates to the local master
      yum update -y

      # Apply updated Puppet modules to the local master
      puppet agent -t


Incrementally upgrading systems using r10k or Code Manager
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you built/manage your SIMP server after
:ref:`gsg-installing_simp_using_r10k_or_code_manager`, you will need to work
with the upstream Git repositories as appropriate for your workflow.



Breaking Changes
~~~~~~~~~~~~~~~~

If the ``X`` version number has changed then you should expect **major**
breaking changes to the way SIMP works. Please carefully read the Changelog and
the new User's Guide and do **not** deploy these changes directly on top of
your production environment.

.. IMPORTANT::

   Upgrading SIMP does **not** require re-kicking your clients, even if some
   core services move to the new Puppet node.  All software configurations can
   be updated in Puppet, as needed.

New Server Creation and Client Migration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The recommended method for upgrading breaking changes is to create a new Puppet
Server and migrate your data and clients to it. This process follows the path
of least destruction; we will guide you through how to back up the existing
Puppet server, create a new server, and transfer your clients.

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

Follow the :ref:`ug-howto-change-puppet-servers` guide to begin integration
of your new Puppet server into the existing environment.

.. NOTE::

   You should *always* start migration with a small number of
   **least critical** clients!

Retire the Old Puppet Server
""""""""""""""""""""""""""""

Once you have transferred the management of all your clients over to
the new Puppet server, you may safely retire the old Puppet server.

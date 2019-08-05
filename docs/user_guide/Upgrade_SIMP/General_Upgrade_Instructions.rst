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

.. contents::  Contents
   :depth: 3
   :local:

.. _ug-incremental-upgrades:

Incremental Upgrades
~~~~~~~~~~~~~~~~~~~~

For ``Y`` and ``Z`` SIMP changes, you should feel comfortable dropping the
changes directly into your **test** systems. The promotion cycle from test to
production should be short and painless if you reference the version upgrade
documentation.

Beginning with SIMP 6.4.0, simply installing SIMP-packaged Puppet module RPMs
will no longer apply the module updates to the ``simp`` :term:`Puppet environment`.
You must deploy the Puppet modules to the desired Puppet environment(s) using
the mechanism appropriate for your :ref:`deployment scenario <ug-deployment_scenarios>`.

.. IMPORTANT::

   Review any :ref:`ug-version-specific-upgrade-instructions` prior to
   executing an Incremental Upgrade. There may be specific instructions
   regarding the upgrade process that you should follow.

.. _ug-incremental-upgrades-w-iso:

Local deployment scenario incremental upgrade
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following instructions assume the Puppet environment you are updating is
named ``test`` and are specific to a
:ref:`Local deployment scenario<ug-local_module_deployment_scenario>`.

Execute these steps as ``root``.

#. Update the YUM Repositories

   * Update the repositories using a SIMP ISO:

     If you have the latest SIMP ISO available to you and have installed the
     ``simp-utils`` package, update the YUM repositories by unpacking the ISO
     using ``unpack_dvd`` from that package:

     #. Copy the new SIMP ISO file to the SIMP master
     #. From the SIMP master (as ``root``):

        .. code-block:: sh

           # Unpack the new SIMP ISO's RPMs into yum repositories
           $ unpack_dvd </path/to/ISO>

   * For RPM-based installation, follow your site's procedures to update your
     repositories.

#. Install the RPMs

   .. code-block:: sh

      # Make sure yum picks up the new RPMs
      $ yum clean all; yum makecache

      # Apply updates to the local master
      $ yum update -y

   For SIMP 6.4 and later, this will also update the system-local, SIMP-managed
   Puppet module :term:`Git` repositories.

#. If you are upgrading from a version before SIMP 6.4 you can skip to the last
   step, *Apply the changes by running puppet*.

   ** **The following steps only apply for upgrades from version 6.4 or later**

   .. include:: ../common/Update_and_Deploy_Local_Environment.inc

   ** **This ends the steps that are only for 6.4 or later.**  The next steps apply
   to all systems.


#. Apply the changes by running ``puppet``

   .. code-block:: sh

      $ puppet agent -t

Other deployment scenario incremental upgrade
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you manage your SIMP server using :term:`r10k` or :term:`Code Manager` and
are not using the server-local, SIMP-managed Git module repositories, you
will need to work with the upstream Git repositories as appropriate for your
workflow.  This is the same for all versions of SIMP.

For SIMP 6.4 and later, the instructions in
:ref:`howto-setup-a-simp-control-repository` may be helpful.

Breaking Changes
~~~~~~~~~~~~~~~~

If the ``X`` version number has changed then you should expect **major**
breaking changes to the way SIMP works. Please carefully read the Changelog and
the :ref:`simp-user-guide` and do **not** deploy these changes directly on top
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

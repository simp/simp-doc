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

  * Updates to packages in the `simp-extras` RPM do not constitute a severe
    API-breaking change.

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
production should be short and painless if you reference the :ref:`version
upgrade documentation <ug-version-specific-upgrade-instructions>`.


Beginning with SIMP 6.4.0, SIMP-packaged Puppet module RPMs
no longer install updates directly into the ``simp/`` :term:`Puppet
environment` directory. You must upgrade your Puppet modules using the
mechanism appropriate for your :ref:`environment deployment
scenario<ug-sa-env-deployment-scenarios>`:

.. IMPORTANT::

   Review any :ref:`ug-version-specific-upgrade-instructions` prior to
   executing an Incremental Upgrade. There may be specific instructions
   regarding the upgrade process that you should follow.


.. _ug-incremental-upgrades-w-iso:

Upgrading systems using the local deployment scenario
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following instructions are specific to the :ref:`Local deployment
scenario<ug-sa-env-deployment-scenarios--local>`.  They assume the Puppet
environment you are updating is named ``test``, and that you execute these
steps as ``root``:

#. Update the YUM Repositories

   * Update the repositories using a SIMP ISO:

     If you have the latest SIMP ISO available to you and have installed the
     :pupmod:`simp-utils` package, update the YUM repositories by unpacking the ISO
     using :program:`unpack_dvd` from that package. 

     :code:`unpack_dvd` uses the  :file:`.treeinfo` to determine the version of the OS included
     on the DVD.  This file sometimes only supplies the major version of the OS so you should supply
     the a more detailed version number for the OS.  In later versions of :code:`unpack_dvd` you will
     get an error if it finds only the major version and you have not supplied a specific version
     using the ``-v`` option.

     #. Copy the new SIMP ISO file to the SIMP master
     #. From the SIMP master (as ``root``):

        .. code-block:: sh

           # Unpack the new SIMP ISO's RPMs into yum repositories
           unpack_dvd -v <OS version number> </path/to/ISO>

   * For RPM-based installation, follow your site's procedures to update your
     repositories.

#. Install the RPMs

   .. code-block:: sh
   
      # Make sure the puppet agent cron job does not run and pick up any
      # interim changes, including Puppet application RPM updates, until you
      # are ready for these changes.
      puppet agent --disable

      # Make sure yum picks up the new RPMs
      yum clean all; yum makecache

      # Apply updates to the local master
      yum update -y

   For SIMP 6.4 and later, this will also update the system-local, SIMP-managed
   Puppet module :term:`Git` repositories.

#. If you are upgrading from a version prior to SIMP 6.4 you can skip to the last
   step, *Apply the changes by running puppet*.

   ** **The following steps only apply for upgrades from version 6.4 or later**

   .. include:: ../common/Update_and_Deploy_Local_Environment.inc

   ** **This ends the steps that are only for 6.4 or later.**  The next steps apply
   to all systems.

#. Update the generated types for the environment

   .. code-block:: sh

     /usr/local/sbin/simp_generate_types -p /etc/puppetlabs/code/environments/test


#. Re-enable Puppet and apply the changes


   .. code-block:: sh

      puppet agent --enable
      puppet agent -t

Upgrading systems that use control repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you manage your SIMP server using :term:`r10k` or :term:`Code Manager` and
are not using the server-local, SIMP-managed Git module repositories, you
will need to work with the upstream Git repositories as appropriate for your
workflow.  This is the same for all versions of SIMP.

For SIMP 6.4 and later, the instructions in
:ref:`howto-setup-a-simp-control-repository` may be helpful.

Breaking Changes
~~~~~~~~~~~~~~~~

If the ``X`` version number has changed then you should expect **major**
breaking changes to the way SIMP works. Please carefully read the
:ref:`CHANGELOG<changelog-latest>` and the :ref:`simp-user-guide` and do **not**
deploy these changes directly on top of your ``production`` environment.

If the ``Y`` version number has changed then there may either be deprecation
notices or **minor** breaking changes to the way SIMP works. Please carefully
read the :ref:`CHANGELOG<changelog-latest>` and the associated
:ref:`ug-version-specific-upgrade-instructions`.

.. IMPORTANT::

   Upgrading SIMP does **not** require re-kicking your clients, even if some
   core services move to the new Puppet node.  All software configurations can
   be updated in Puppet, as needed.

With the release of 6.4, SIMP RPM upgrades now have a "hands-off" approach to
upgrades that allow users to easily preserve different combinations of module
sets as required by their environment. That being said, the SIMP team does not
test all combinations of modules and may have difficulty providing support for
untested combinations.

For releases moving from version of SIMP earlier than 6.3 to versions 6.4+, see
:ref:`howto-migrate-to-new-puppet-server` for the simplest migration path. Also
be sure to read the :ref:`ug-version-specific-upgrade-instructions` for all of
the intermediate versions.

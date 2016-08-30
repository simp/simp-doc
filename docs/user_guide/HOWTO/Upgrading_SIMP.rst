.. _ug-howto-upgrade-simp:

HOWTO Upgrade SIMP
==================

.. WARNING::
  Please refer to the `Migrating To Environments`_ section if you are upgrading
  from a version of SIMP before we started using Puppet Environments. If you
  have an ``/etc/puppet/environments/simp`` directory and no record of
  ``/etc/puppet/extdata`` on your system, then you do not need to migrate your
  code to environments.

This section provides information on how to upgrade a running instance to the
latest codebase.

.. contents::
  :local:
  :depth: 1

Pre-Upgrade Recommendations
---------------------------

The following process should be followed prior to upgrading:

#. Run ``puppet agent --disable`` to disable puppet.

   .. NOTE::
     If you think you will need more than 4 hours to complete this task, also
     disable puppet in root's crontab by running ``crontab -e root``.

#. You may wish to block all communications with agents while updating the
   server. This is not required but could spare you some headaches if something
   doesn't work properly.

   The simplest way to do this is to set the catalog retrieval capability to
   ``127.0.0.1`` in ``/etc/puppet/auth.conf`` as shown below.

   .. code-block:: bash

     path ~ ^/catalog/([^/]+)$
     method find
     # Uncomment this when complete and delete the other entries
     #allow $1
     allow 127.0.0.1

Using the syntax above, you can add fully qualified domain names, one at a
time, to the 'allow' list and only those hosts will be able to retrieve their
catalog from the running server. 127.0.0.1 serves as a placeholder so that no
host can actually retrieve their catalog.

Upgrading SIMP from an ISO
--------------------------

#. Copy the release ISO to the simp server that is being upgraded

#. Backup the existing environment:

   .. code-block:: bash

     cd /etc/puppet/environments
     cp -r simp simp.old

#. Unpack the new ISO using the ``unpack_dvd`` utility:

   .. code-block:: bash

     /usr/local/bin/unpack_dvd -d /var/www/yum <path-to-new-ISO>

   This should update the existing YUM repos on the system to include updated
   system packages and updated SIMP packages.

   .. NOTE::
     If there is a operating system release in between SIMP releases (for
     example, CentOS 6.7 to 6.8), the ``/etc/yum.repos.d/filesystem.repo`` file
     needs to be modified to look for the new version of the OS.

#. Update the system!

   .. code-block:: bash

    yum update

   .. NOTE::
     If there are issues with gpg keys, try running ``yum reinstall
     simp-gpgkeys`` as root.

#. Read the Changelog **carefully** and see what you may need to change in your
   infrastructure. For example, there have been some changes to our default
   SIMP server :term:`Hiera` file. You will need to compare the new
   ``puppet.your.domain.yaml`` to the existing SIMP host Hiera file.

#. Run ``puppet`` on the SIMP server:

   .. code-block:: bash

     puppet agent -t

#. That's it! Updates should propagate automatically throughout all clients as
   puppet and yum runs.

Migrating To Environments
-------------------------

SIMP 4.1 and 5.0 used the deprecated, Rack-based, Puppet Master.  Starting
with 4.2 and 5.1, SIMP now uses the Clojure-based Puppet Server.

Unfortunately, there are some conflicts with directly upgrading from the legacy
Puppet Master to the new Puppet Server since some of the RPM package
prerequisites conflict.

The new Puppet Server can properly utilize Puppet Environments. To provide our
users with this capability, and to facilitate more dynamic workflows in the
future, the SIMP team has migrated **all** existing material to a native ``simp``
environment.  To help facilitate your migration, the SIMP team has created two
migration scripts that both upgrade your Puppet Server and migrate your
existing data into the new ``simp`` environment.

.. WARNING::
    You must have at least **2.4G** of **free memory** to run the new Puppet
    Server.

Migration Script Features
^^^^^^^^^^^^^^^^^^^^^^^^^

The migration script will perform the following actions on your system:

*  Remove the ``puppet-server`` package from your system
*  Install the ``puppetserver`` package onto your system
*  Update all packages from your repositories
*  Create a backup folder at ``/etc/puppet/environments/pre_migration.simp``
*  Create a Git repository in the backup folder under a timestamped directory
*  Commit all current materials from ``/etc/puppet`` into the backup Git
   repository
*  Checkout the backup Git repository under the timestamped directory as
   ``backup_data`` for ease of use
*  Migrate all existing data into the new ``simp`` environment under
   ``/etc/puppet/environments/simp``

.. NOTE::
    All future upgrades will only affect the new ``simp`` environment.  You may
    create new environments and/or modify the contents of
    ``/etc/puppet/modules`` without fear of the SIMP packages overwriting your
    work.

Migration Script Execution
^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Copy the new SIMP ISO onto your system. For the purposes of these
   instructions, we will refer to this as ``SIMP_Update.iso``. Please ensure
   that you are in the directory with the ISO prior to proceeding. Extract the
   new simp-utils package using the following command:

   .. code-block:: bash

     isoinfo -i SIMP_Update.iso -R -x `isoinfo -i SIMP_Update.iso -Rf | \
       grep noarch/simp-utils` > simp-utils-update.rpm

#. Install the new simp-utils RPM:

   .. code-block:: bash

      yum -y localupdate simp-utils*.rpm

#. Unpack the DVD onto the system:

   .. code-block:: bash

      /usr/local/bin/unpack_dvd SIMP_Update.iso

#. Run the migration script (this may take some time, do NOT hit CTRL-C!):

   .. code-block:: bash

      /usr/share/simp/upgrade_script/migrate_to_environments

#. Run the puppet agent:

   .. code-block:: bash

      puppet agent -t

#. Stop the new puppetserver service (it may not be running):

   .. code-block:: bash

     service puppetserver stop

#. Remove any left over PID files:

   .. code-block:: bash

      rm /var/run/puppetserver/puppetserver

#. Kill any running puppet master processes:

   .. code-block:: bash

      pkill -f 'puppet master'

#. Wait for 10 seconds to let things finalize if necessary:

   .. code-block:: bash

      sleep 10

#. Start the new Puppet Server:

   .. code-block:: bash

      service puppetserver start

Your new Puppet Server should now be running and a run of ``puppet agent -t``
should complete as usual.

Converting from Extdata to Hiera
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP now uses :term:`Hiera` natively instead of Extdata. Tools have been put
into place by Puppet, Inc. and SIMP to make the conversion as easy as possible.
Two scripts have been provided to automatically convert generic csv files and
``simp_def.csv`` to yaml. The first example shows how to convert an Extdata csv
file called ``foo.csv`` into a Hiera yaml file called ``bar.yaml``:

.. code-block:: ruby

  extdata2hiera -i foo.csv -o bar.yaml

The second example shows how to convert an Extdata csv simp_def file called
``simp_def.csv`` into a Hiera yaml file called ``simp_def.yaml``.

.. code-block:: ruby

  simpdef2hiera --in simp_def.csv --out simp_def.yaml


Puppet will automatically retrieve class parameters from Hiera, using lookup
keys like ``myclass::parameter_one``. Puppet classes can optionally include
parameters in their definition. This lets the class ask for data to be passed
in at the time that it’s declared, and it can use that data as normal variables
throughout its definition.

There are two main ways to reference Hiera data in puppet manifests. The first,
and preferred way, is to use the automatic class variable lookup capability.
For each class that you create, the variables will be automatically discovered
in hiera should they exist. This is quite powerful in that you no longer need
to provide class parameters in your manifests and can finally properly separate
your data from your code.

.. NOTE::
    For more information on the lookup functions, see `the official Hiera documentation`_

.. code-block:: ruby

  # Some class file in scope...
  class foo (
    $param1 = 'default1'
    $param2 = 'default2'
  ) { .... }

  # /etc/puppet/hieradata/default.yaml
  ---
  foo::param1: 'custom1'


The second is similar to the old Extdata way, and looks like the following:

.. code-block:: ruby

  $var = hiera("some_hiera_variable", "default_value")


The following is from the Puppet, Inc. documentation, and explains the reason
for switching to Hiera.

Automatic parameter lookup is good for writing reusable code because it is
regular and predictable. Anyone downloading your module can look at the first
line of each manifest and easily see which keys they need to set in their own
Hiera data. If you use the Hiera functions in the body of a class instead, you
will need to clearly document which keys the user needs to set.

.. NOTE::
    For more information on hiera and puppet in general, see
    http://docs.puppetlabs.com/hiera/1/complete_example.html.

Scope Functions
^^^^^^^^^^^^^^^

All scope functions must take arguments in array form. For example in
``/etc/puppet/modules/apache/templates/ssl.conf.erb``:

.. code-block:: erb

  <%=scope.function_bracketize(l) %>
  becomes
  <%=scope.function_bracketize([l]) %>


Commands
^^^^^^^^

Deprecated commands mentioned in Puppet 2.7 upgrade are now completely removed.

Lock File
^^^^^^^^^

Puppet agent now uses the two lock files instead of one. These are the
run-in-progress lockfile (``agent_catalog_run_lockfile``) and the disabled
lockfile (``agent_disabled_lockfile``). The ``puppetagent_cron file`` (made by
the pupmod module) must be edited to suit this change.

.. _the official Hiera documentation: https://docs.puppet.com/hiera/3.2/puppet.html#hiera-lookup-functions

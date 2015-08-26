Upgrading SIMP
==============

This chapter provides information on how to upgrade a running instance
to the latest codebase.

Pre-Upgrade Recommendations
---------------------------

The following process should be followed before upgrade.

1. Run ``puppet agent --disable`` to disable puppet.

  .. note:: If you think you will need more than 4 hours to complete this task, also disable puppet in root's crontab.

2. You may wish to block all communications with agents while updating the server. This is not required but could spare you some headaches if something doesn't work properly.

  The simplest way to do this is to set the catalog retrieval capability to 127.0.0.1 in ``/etc/puppet/auth.conf`` as shown below.

  .. code-block:: bash

    path ~ ^/catalog/([^/]+)$
    method find
    # Uncomment this when complete and delete the other entries
    #allow $1
    allow 127.0.0.1

Using the syntax above, you can add fully qualified domain names, one at a time, to the 'allow' list and only those hosts will be able to retrieve their catalog from the running server. 127.0.0.1 serves as a placeholder so that no host can actually retrieve their catalog.


Migrating To Environments
-------------------------

SIMP 4.1 and 5.0 used the traditional, Rack-based, Puppet Master.
Starting with 4.2 and 5.1, SIMP now uses the Clojure-based Puppet
Server. Unfortunately, there are some conflicts with directly upgrading
from the Puppet Master to the Puppet Server since some of the RPM
package prerequisites conflict. This new Puppet Server can properly
utilize Puppet Environments. To provide our users with this capability,
and to facilitate more dynamic workflows in the future, the SIMP team
has migrated **all** existing material to a native *simp* environment.
To help facilitate your migration, the SIMP team has created two
migration scripts that both upgrade your Puppet Server and migrate your
existing data into the new *simp* environment.

.. warning::

    You must have at least **2.2G** of **free memory** to run the new
    Puppet Server.

Migration Script Features
-------------------------

The migration script will perform the following actions on your system:

-  Remove the ``puppet-server`` package from your system

-  Install the ``puppetserver`` package onto your system

-  Update all packages from your repositories

-  Create a backup folder at
   ``/etc/puppet/environments/pre_migration.simp``

-  Create a Git repository in the backup folder under a timestamped
   directory

-  Commit all current materials from ``/etc/puppet`` into the backup Git
   repository

-  Checkout the backup Git repository under the timestamped directory as
   ``backup_data`` for ease of use

-  Migrate all existing data into the new ``simp`` environment under
   ``/etc/puppet/environments/simp``

.. note::

    All future upgrades will only affect the new ``simp`` environment.
    You may create new environments and/or modify the contents of
    ``/etc/puppet/modules`` without fear of the SIMP packages overwriting
    your work.

Migration Script Execution
--------------------------

1. Copy the new SIMP ISO onto your system. For the purposes of these instructions, we will refer to this is SIMP_Update.iso. Please ensure that you are in the directory with the ISO prior to proceeding. Extract the new simp-utils package using the following command:

  .. code-block:: bash

    isoinfo -i SIMP_Update.iso -R -x `isoinfo -i SIMP_Update.iso -Rf | grep noarch/simp-utils` > simp-utils-update.rpm

2. Install the new simp-utils RPM:

  .. code-block:: bash

    yum -y localupdate simp-utils*.rpm

3. Unpack the DVD onto the system:

  .. code-block:: bash

    /usr/local/bin/unpack_dvd SIMP_Update.iso

4. Run the migration script (this may take some time, do NOT hit CTRL-C!):

  .. code-block:: bash

    /usr/share/simp/upgrade_script/migrate_to_environments

5. Run the puppet agent:

  .. code-block:: bash

    puppet agent -t

6. Stop the new puppetserver service (it may not be running):

  .. code-block:: bash

    service puppetserver stop

7. Remove any left over PID files:

  .. code-block:: bash

    rm /var/run/puppetserver/puppetserver

8. Kill any running puppet master processes:

  .. code-block:: bash

    pkill -f 'puppet master'

9. Wait for 10 seconds to let things finalize if necessary:

  .. code-block:: bash

    sleep 10

10. Start the new Puppet Server:

  .. code-block:: bash

    service puppetserver start


Table: Executing the Migration Script

Your new Puppet Server should now be running and a run of ``puppet agent -t`` should complete as usual.

Converting from Extdata to Hiera
--------------------------------

SIMP now uses Hiera natively instead of Extdata. Tools have been put
into place by Puppet Labs and SIMP to make the conversion as easy as
possible. Two scripts have been provided to automatically convert
generic csv files and ``simp_def.csv`` to yaml. The first example shows how
to convert an Extdata csv file called foo.csv into a Hiera yaml file
called ``bar.yaml``:

.. code-block:: ruby

  extdata2hiera -i foo.csv -o bar.yaml


The second example shows how to convert an Extdata csv simp_def file
called simp\_def.csv into a Hiera yaml file called ``simp_def.yaml``.

.. code-block:: ruby

  simpdef2hiera --in simp_def.csv --out simp_def.yaml


Puppet will automatically retrieve class parameters from Hiera, using
lookup keys like ``myclass::parameter_one``. Puppet classes can optionally
include parameters in their definition. This lets the class ask for data
to be passed in at the time that it’s declared, and it can use that data
as normal variables throughout its definition.

There are two main ways to reference Hiera data in puppet manifests. The
first, and preferred way, is to use the automatic class variable lookup
capability. For each class that you create, the variables will be
automatically discovered in hiera should they exist. This is quite
powerful in that you no longer need to provide class parameters in your
manifests and can finally properly separate your data from your code.

.. note::

    For more information on the lookup functions, see
    `Link the puppet documentation on Hiera <http://docs.puppetlabs.com/hiera/1/puppet.html#hiera-lookup-functions>`_.

.. code-block:: ruby

  # Some class file in scope...
  class foo (
    $param1 = 'default1'
    $param2 = 'default2'
  ) { .... }

  # /etc/puppet/hieradata/default.yaml
  ---
  foo::param1: 'custom1'


The second is similar to the old Extdata way, and looks like the
following:

.. code-block:: ruby

  $var = hiera("some_hiera_variable", "default_value")


The following is from the Puppet Labs documentation, and explains the
reason for switching to Hiera.

Automatic parameter lookup is good for writing reusable code because it
is regular and predictable. Anyone downloading your module can look at
the first line of each manifest and easily see which keys they need to
set in their own Hiera data. If you use the Hiera functions in the body
of a class instead, you will need to clearly document which keys the
user needs to set.

.. note::

    For more information on hiera and puppet in general, see
    http://docs.puppetlabs.com/hiera/1/complete_example.html.

Scope Functions
---------------

All scope functions must take arguments in array form. For example in
``/etc/puppet/modules/apache/templates/ssl.conf.erb``:

.. code-block:: erb

  <%=scope.function_bracketize(l) %>
  becomes
  <%=scope.function_bracketize([l]) %>


Commands
--------

Deprecated commands mentioned in Puppet 2.7 upgrade are now completely
removed.

Lock File
---------

Puppet agent now uses the two lock files instead of one. These are the
run-in-progress lockfile (``agent_catalog_run_lockfile``) and the
disabled lockfile (``agent_disabled_lockfile``). The ``puppetagent_cron
file`` (made by the pupmod module) must be edited to suit this change.

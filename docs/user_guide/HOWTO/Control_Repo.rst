.. _howto-setup-a-simp-control-repository:

HOWTO Set up a SIMP Environment in a Control Repository
=======================================================

A :term:`Control Repository` contains the modules, hieradata, and roles/profiles
required in a Puppet infrastructure.  Managing the control repository with
:term:`Git` allows sysadmins to utilize a workflow when updating and developing
their infrastructure.

This HOWTO will describe how to create a :term:`SIMP Omni-Environment skeleton`,
whose :term:`Puppet environment` can be checked into a branch on an existing
control repository.  Before proceding with this section, you should

* Have a basic understanding of control repositories.
* Have a basic understanding of how to use ``git``.
* Have a Git repository that you will be using as your control repository.
* Have read :ref:`Deploying SIMP Environments`.  It contains detailed
  descriptions of key topics:

  - the :term:`SIMP Omni-Environment`
  - local, SIMP-managed Git repositories maintained by SIMP Puppet module RPMs
  - SIMP :term:`CLI` commands to assist with SIMP Omni-Environment management.

.. TIP::

   You may find it helpful to read the section that explains how a control
   repository works in `Puppet, Inc.'s control repository documentation`_ .

.. contents:: Contents
   :depth: 3
   :local:

SIMP Omni-Environment Requirement
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Although (with a detailed understanding of SIMP internals) you can have a
a :term:`SIMP server` that will function with just a Puppet environment, by
default, a functioning SIMP server **requires** its entire SIMP Omni-Environment.
So, the instructions in this HOWTO will create a SIMP Omni-Environment skeleton
whose Puppet environment you can check into a branch in your control repository.

The procedure in this HOWTO assumes the new SIMP Omni-Environment will be
using its own secondary and writable environments.  For more complex SIMP
Omni-Environment configurations (e.g., linking the secondary and writable
directories to another environment in order to share rsync data and secrets),
see the built-in SIMP CLI documentation for creating SIMP Omni-Environments:

.. code-block:: bash

   $ simp environment new -h

Procedures
^^^^^^^^^^

STEP 1: Install Prerequisite Packages
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This HOWTO requires the following RPMs to be installed on your SIMP
server:

* ``rubygem-simp-cli``
* ``simp-environment-skeleton``
* ``simp-rsync-skeleton``
* ``simp-selinux-policy``
* SIMP module RPMs -  **Only if** you will be using the local, SIMP-managed
  Git repositories in your environment's :term:`Puppetfile`.

All of these packages will already be installed if you have installed the SIMP
server from ISO or from RPM.  Otherwise, do the following:

#. Configure SIMP repositories as described in
   :ref:`gsg-installing_simp_from_a_repository`.

#. Install the packages as ``root``:

   * Install the SIMP CLI, environment skeleton, and :term:`SELinux` policy
     packages:

     .. code-block:: bash

        $ yum install rubygem-simp-cli simp-environment-skeleton simp-rsync-skeleton simp-selinux-policy

     Installation of ``rubygem-simp-cli`` will pull in a few SIMP Puppet module
     RPMs as dependencies, but these modules will **NOT** be installed in any
     Puppet environment.

   * Install all core SIMP Puppet module RPMs (if using local Git
     repositories):

     .. code-block:: bash

        $ yum install simp

   * Install desired extra SIMP Puppet module RPMs (if using local Git
     repositories):

     .. code-block:: bash

        # Install all extra SIMP modules
        $ yum install simp-extras

        # **OR**

        # Install only a subset of extra SIMP Puppet modules needed
        $ yum install pupmod-simp-gdm pupmod-simp-gnome

     .. TIP::

        SIMP-provided Puppet module RPMs are named:

          ``pupmod-<Puppet Forge org>-<module name>``

        The last two parts of the RPM name matches the module's name in its
        ``metadata.json`` file and guarantees uniqueness in :term:`PuppetForge`.


STEP 2: Create a New SIMP Omni-Environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section provides instructions for creating the new SIMP Omni-Environment
skeleton for which modules could be deployed by :term:`r10K` or
:term:`Code Manager` using a Puppetfile.  Two typical options are presented:

* :ref:`howto-setup-a-simp-control-repository-using-local-repositories`
* :ref:`howto-setup-a-simp-control-repository-using-internet-repositories`

For illustrative purposes, the new environment will be named ``dev1`` in the
procedures.


.. _howto-setup-a-simp-control-repository-using-local-repositories:

Using Local Module Repositories
'''''''''''''''''''''''''''''''

#. Create the SIMP Omni-Environment skeleton as ``root``:

   .. code-block:: bash

       $ simp environment new dev1

   This will do the following:

   * Create a skeleton Puppet directory at ``/etc/puppetlabs/code/environments/dev1``.
   * Create a skeleton Secondary directory at ``/var/simp/environments/dev1``.
   * Generate new ``Puppetfile`` and ``Puppetfile.simp`` files in
     ``/etc/puppetlabs/code/environments/dev1``.

     - ``Puppetfile`` includes ``Puppetfile.simp``.
     - ``Puppetfile.simp`` contains entries for the latest versions of
       SIMP-packaged Puppet modules for which local Git repositories exist
       (i.e., all SIMP modules installed via RPM).

   It does not have to create the Writable environment, because that will be
   automatically generated, as needed, when ``puppet`` is run.

#. Manually edit the generated ``Puppetfile`` to add entries for any other
   non-SIMP modules your site requires.

#. Add/adjust any of the :term:`Hiera` files in
   ``/etc/puppetlabs/code/environments/dev1/data``

   * If you bootstrapped the SIMP server using the procedures in
     :ref:`ug-initial_server_configuration`, you will likely want to copy
     over the ``simp_config_settings.yaml`` and the SIMP server's
     ``hosts/<SIMP server FQDN>.yaml`` files from the ``production``
     environment.

.. _howto-setup-a-simp-control-repository-using-internet-repositories:

Using Internet Module Repositories
''''''''''''''''''''''''''''''''''

.. IMPORTANT::

   If you intend to bootstrap a SIMP server from the environment created
   in this section, it **must** be named ``production``, instead of ``dev1``.

#. Create the SIMP Omni-Environment skeleton without SIMP local repository
   Puppetfiles as ``root``:

   .. code-block:: bash

      $ simp environment new dev1 --no-puppetfile-gen

   This will do the following:

   * Create a skeleton Puppet directory at ``/etc/puppetlabs/code/environments/dev1``.
   * Create a skeleton Secondary directory at ``/var/simp/environments/dev1``.

   It does not have to create the Writable environment, because that will be
   automatically generated, as needed, when ``puppet`` is run.

#. Download the ``Puppetfile`` used to create a SIMP ISO for a specific release
   from the SIMP `simp-core repository`_. In this example, we are going to use
   the SIMP ``6.4.0-0`` release.

   .. code-block:: bash

      $ cd /etc/puppetlabs/code/environments/dev1
      $ curl -o Puppetfile https://github.com/simp/simp-core/blob/6.4.0-0/Puppetfile.pinned

   .. NOTE::

      This ``simp-core`` ``Puppetfile`` will look a little different from
      Puppetfiles you are used to, because it has entries for SIMP components
      that are not Puppet modules (e.g., ``simp-environment-skeleton``).

      **>> You are going to fix that next! <<**

#. Manually edit the ``Puppetfile`` to remove components that are not Puppet
   modules, by deleting all lines up to and including
   ``moduledir  'src/puppet/modules'``.

#. Manually edit the ``Puppetfile`` to add entries for any other non-SIMP modules
   your site requires.

#. Optionally, manually edit the ``Puppetfile`` to remove any non-core SIMP
   modules that are packaged with the ``simp-extras`` RPM, but you don't need.
   You can discover the list of the SIMP extra modules by examining the RPM
   requirements of the ``simp-extras`` RPM as follows:

   .. code-block:: bash

      $ yum deplist simp-extras

   .. WARNING::

      If you are not sure which modules are core SIMP modules, skip this step!

#. Add/adjust any of the :term:`Hiera` files in
   ``/etc/puppetlabs/code/environments/dev1/data``


STEP 3: Create a New Branch in Your Control Repository
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Create an empty git repository in the newly created Puppet environment:

   .. code-block:: bash

      $ cd /etc/puppetlabs/code/environments/dev1
      $ git init .

#. Add all the files to a branch named for the environment in this repository:

   .. code-block:: bash

      # create the branch
      $ git checkout -b dev1

      # add the directory tree
      $ git add --all

      # verify the directory tree doesn't have any temporary files you created
      $ git status

      $ git commit -m 'Initial dev1 environment'

#. Push the branch to your control repository:

   .. code-block:: bash

      # Add a remote for your control repository
      $ git remote add control_repo <URL to the control repo>

      # Push the branch
      $ git push dev1 control_repo


Advanced Topics
^^^^^^^^^^^^^^^

Bootstrapping A SIMP Server without SIMP Module RPMs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A full set of SIMP module RPMs is not required in order for the SIMP server to
be initially configured. With a slight change to the procedures listed in
:ref:`ug-initial_server_configuration`, a SIMP server can be bootstrapped
with a ``production`` SIMP Omni-Environment skeleton, such as one created
in this HOWTO.

.. TIP::

   You may want to read through :ref:`ug-initial_server_configuration`
   before proceeding.  It provides additional information that will not be
   repeated here.

In these procedures, we assume that you have created a ``production`` SIMP
Omni-Environment skeleton that contains a Puppetfile with URLs to the core
SIMP Puppet modules.  For example, you followed the procedures to create a
control repository for a ``production`` environment using internet module
repositories.

Execute the following steps as ``root``:

#. Deploy the modules in the ``production`` Puppet environment using ``r10K``
   or ``Code Manager``.  Be sure the deployed modules are accessible to the
   ``puppet`` group.

#. Install the ``puppetserver`` package:

   .. code-block:: bash

      $ yum install puppetserver

#. Run ``simp config`` with an option that tells it the SIMP Omni-Environment
   has already been created:

   .. code-block:: bash

      $ simp config --force-config

#. Run ``simp bootstrap``:

   .. code-block:: bash

      $ simp bootstrap

#. After ``simp bootstrap`` completes, add the following generated Hiera files
   in the ``production`` Puppet environment to the ``production`` branch in your
   control repository:

   * ``production/data/simp_config_settings.yaml``
   * ``production/data/hosts/<SIMP server FQDN>.yaml``

To continue configuring the system, move on :ref:`Client_Management` section in
the :ref:`simp-user-guide`.

.. _Puppet, Inc.'s control repository documentation: https://docs.puppet.com/pe/latest/cmgmt_control_repo.html
.. _simp-core repository: https://github.com/simp/simp-core

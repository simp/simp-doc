.. _howto-setup-a-simp-control-repository:

HOWTO Set up a SIMP Environment in a Control Repository
=======================================================

A :term:`Control Repository` contains the modules, hieradata, and roles/profiles
required in a Puppet infrastructure.  Managing the control repository with
:term:`Git` allows sysadmins to utilize a workflow when updating and developing
their infrastructure.

This HOWTO will describe how to create control repostiories from a local installation
and from the internet repos.

Before proceding with this section, you should

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


Create a Control Repository on a SIMP Server
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This section assumes you have installed SIMP using the installation guide either
from ISO or RPM Repository and want to create a R10K control repository
from the installation.  This procedure needs to be done on the SIMP server.


#. Create a GIT Repository

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/production
      git init .

#. Add the files to the git repository:

   .. code-block:: sh

      # Add the files
      git add Puppetfile Puppetfile.simp hiera.yaml environment.conf
      # Add directories.  Do not add modules directory or .resource_types directory
      git add manifests data

#. Commit the changes

   .. code-block:: sh

      git commit -m "Initial production environemnt"

#. Push the branch to your control repository:

   .. code-block:: bash

      # Add a remote for your control repository
      git remote add control_repo <URL to the control repo>

      # Push the branch
      git push dev1 control_repo


Create a Control Repository without SIMP installed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
This section describes how to create a control repository for an environment
called dev1   that uses the SIMP internet puppet modules repositories.

Prerequisites
~~~~~~~~~~~~~
This section requires the following SIMP RPMs to be installed on your server

* ``simp-environment-skeleton``

Configuring the SIMP repositories is described in :ref:`gsg-installing_simp_from_a_repository` if you need it.


Procedure
~~~~~~~~~

.. IMPORTANT::

   If you intend to bootstrap a SIMP server from the environment created
   in this section, it **must** be named ``production``, instead of ``dev1``.

#. Create an empty git repository:

   .. code-block:: bash

      mkdir  $HOME/dev1
      cd $HOME/dev1
      git init .

#. Copy the puppet environment skeleton into your git repository:

   .. code-block:: bash

      # You should still be in $HOME/dev1 directory
      cp -R /usr/share/simp/environment-skeleton/puppet/* .
      sed -e "s/%%SKELETON_ENVIRONMENT%%/dev1/g" ./environment.conf.TEMPLATE > ./environment.conf
      chmod 640 environment.conf
      rm environment.conf.TEMPLATE

#. Generate the Puppetfile.simp file

   - Download the ``Puppetfile`` used to create a SIMP ISO for a specific release
     from the SIMP `simp-core repository`_. In this example, we are going to use
     the SIMP ``6.4.0-0`` release.

     .. code-block:: bash

        cd /etc/puppetlabs/code/environments/dev1
        curl -o Puppetfile.simp https://github.com/simp/simp-core/blob/6.4.0-0/Puppetfile.pinned

   - Manually edit the ``Puppetfile.simp`` to remove components that are not Puppet
     modules, by deleting all lines up to and including
     ``moduledir  'src/puppet/modules'``.

   - Optionally, edit the ``Puppetfile.simp`` to remove any non-core SIMP
     modules that are packaged with the ``simp-extras`` RPM, but you don't need.
     You can discover the list of the SIMP extra modules by examining the RPM
     requirements of the ``simp-extras`` RPM as follows:

     .. code-block:: bash

        yum deplist simp-extras

#. Create the Puppetfile

   Create $HOME/dev1/Puppetfile and include the following line:

   .. code-block:: ruby

      instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   Also add entries for any other non-SIMP modules your site requires.

#. Add/adjust any of the :term:`Hiera` files in the data directory.


#. Add all the files to a branch named for the environment in this repository:

   .. code-block:: bash

      # create the branch
      git checkout -b dev1

      # add the directory tree
      git add --all

      # verify the directory tree doesn't have any temporary files you created
      git status

      git commit -m 'Initial dev1 environment'

#. Push the branch to your control repository:

   .. code-block:: bash

      # Add a remote for your control repository
      git remote add control_repo <URL to the control repo>

      # Push the branch
      git push dev1 control_repo


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

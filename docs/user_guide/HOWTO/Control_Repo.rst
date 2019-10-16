.. _howto-setup-a-simp-control-repository:

HOWTO Set up a SIMP Environment in a Control Repository
=======================================================

This HOWTO describes how to create Puppet :term:`control repositories <Control
Repository>` for use with a :ref:`Control Repository deployment scenario
<ug-sa-env-deployment-scenarios--controlrepo>`.

.. contents:: Contents
   :depth: 3
   :local:


Requirements
^^^^^^^^^^^^

To use any of the procedures in this section, you must:

#. Have access to a remotely-hosted :term:`Git` repository where you will host your
   control repository.
#. Have a basic understanding of:

   * Puppet :term:`Control Repositories <Control Repository>`.
   * How to use the ``git`` command.
   * The topics covered in ":ref:`Deploying SIMP Environments`," particularly:

     - The composition of :ref:`ug-sa-simp-environments`
     - The :ref:`Local Deployment Scenario <ug-sa-env-deployment-scenarios--local>`
     - The ``simp`` commands needed to manage a :term:`SIMP Omni-Environment`

You may find it helpful to read the section that explains how a control
repository works in `Puppet, Inc.'s control repository documentation`_ .


Creating a Control Repository on a SIMP Server
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This procedure creates an r10k control repository from a fresh SIMP ISO/RPM
installation.  It is currently limited to to *just* the control repository—the
RPM-provided Puppet module git repositories will remain on the SIMP server's
local filesystem.

.. IMPORTANT::
   This procedure does NOT describe how to migrate or host SIMP Puppet modules
   in remote git Repositories, or how to update the Puppetfile.simp file to
   deploy them.


.. rubric:: Prerequisites

* You have installed SIMP (per the installation guide) from :ref:`ISO
  <gsg-installing_simp_from_an_iso>` or :ref:`RPM Repository
  <gsg-installing_simp_from_a_repository>`.
* This procedure needs to be done on the SIMP server.

.. rubric:: Procedure

#. Create a git repository inside the ``production`` Puppet environment
   directory:

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/production
      git init .

#. Create a new branch for the ``production`` Puppet environment:

   .. code-block:: sh

      git checkout -b production

#. Add files to the git repository.
   (Do **not** add the ``modules/`` directory or ``.resource_types/`` directory):

   .. code-block:: sh

      # Add the files
      git add Puppetfile Puppetfile.simp hiera.yaml environment.conf

      # Add directories
      git add manifests/ data/


#. Commit the changes

   .. code-block:: sh

      git commit -m "Initial production environemnt"

#. Push the branch to your control repository:

   .. code-block:: bash

      # Add a remote for your control repository
      git remote add control_repo <URL to the control repo>

      # Push the branch
      git push production control_repo


Creating a Control Repository without SIMP installed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This procedure creates a control repository with a branch for an environment
named called ``dev1``.  The Puppet modules will be deployed from the SIMP
project's public git repositories over the internet.

.. IMPORTANT::

   If you intend to use this environment :ref:`to bootstrap a SIMP server
   without RPMs <howto-bootstrapping-a-simpserver-without-rpms>`,
   it **must** be named ``production`` (and not ``dev1``).

.. rubric:: Prerequisites

You will need the SIMP Puppet environment "skeleton" directory, which can be
obtained from one of the following sources:

* ``/usr/share/simp/environment-skeleton/puppet/`` when the RPM package
  **simp-environment-skeleton** is installed [1]_.
* ``environments/puppet/`` under a checkout of the git repository
  https://github.com/simp/simp-environment-skeleton.

.. [1] If your working host doesn't have the **simp-environment-skeleton** RPM
       and you'd like to install it, you can set up the SIMP yum repositories
       (see ":ref:`gsg-installing_simp_from_a_repository`)."

.. rubric:: Procedure

#. Create an empty git repository:

   .. code-block:: bash

      mkdir $HOME/control-repo
      cd $HOME/control-repo
      git init .

#. Copy the puppet environment skeleton into your git repository:

   .. code-block:: bash

      cd $HOME/control-repo
      cp -R /usr/share/simp/environment-skeleton/puppet/* .


#. Substitute your environment's name into ``environment.conf``:

   .. code-block:: bash

      sed -e "s/%%SKELETON_ENVIRONMENT%%/dev1/g" ./environment.conf.TEMPLATE > ./environment.conf
      chmod 640 environment.conf
      rm environment.conf.TEMPLATE

#. Download and edit the ``Puppetfile.simp`` file:

   a.   Download the ``Puppetfile`` used to create a SIMP ISO for a specific release
        from the SIMP `simp-core repository`_ (in this example, it is ``6.4.0-0``):

        .. code-block:: bash

           cd /etc/puppetlabs/code/environments/dev1
           curl -o Puppetfile.simp https://github.com/simp/simp-core/blob/6.4.0-0/Puppetfile.pinned

   b.   Edit ``Puppetfile.simp`` to remove components that are not Puppet modules,
        deleting all lines up to and including ``moduledir 'src/puppet/modules'``.
        You can do this from the command line by running:

        .. code-block:: bash

           sed -i -e "0,/^moduledir 'src\/puppet\/modules'/d" Puppetfile.simp

   c.   (Optionally,) edit ``Puppetfile.simp`` to remove any non-core SIMP modules
        (e.g., the ones packaged with ``simp-extras``) that you don't need. You
        can discover the list of the SIMP extra modules by examining the
        dependencies of the ``simp-extras`` RPM:

        .. code-block:: bash

           yum deplist simp-extras | grep dependency:

#. Create the ``Puppetfile``:

   a.   Create the file ``$HOME/control-repo/Puppetfile``, which should include the
        following line:

        .. code-block:: ruby

           instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   b.   (Optionally,) also add entries for any non-SIMP modules your site requires.

#. Add/adjust any of the :term:`Hiera` files in the ``data/`` directory.

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

.. _howto-bootstrapping-a-simpserver-without-rpms:

Bootstrapping A SIMP Server without SIMP Module RPMs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A full set of SIMP module RPMs is not required in order for the SIMP server to
be initially configured. With a slight change to the procedures listed in
:ref:`ug-initial_server_configuration`, a SIMP server can be bootstrapped
with a ``production`` SIMP Omni-Environment skeleton, such as one created
in this HOWTO.

.. NOTE::

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

      yum install puppetserver

#. Run ``simp config`` with an option that tells it the SIMP Omni-Environment
   has already been created:

   .. code-block:: bash

      simp config --force-config

#. Run ``simp bootstrap``:

   .. code-block:: bash

      simp bootstrap

#. After ``simp bootstrap`` completes, add the following generated Hiera files
   in the ``production`` Puppet environment to the ``production`` branch in your
   control repository:

   * ``production/data/simp_config_settings.yaml``
   * ``production/data/hosts/<SIMP server FQDN>.yaml``

To continue configuring the system, move on :ref:`Client_Management` section in
the :ref:`simp-user-guide`.

.. _Puppet, Inc.'s control repository documentation: https://docs.puppet.com/pe/latest/cmgmt_control_repo.html
.. _simp-core repository: https://github.com/simp/simp-core

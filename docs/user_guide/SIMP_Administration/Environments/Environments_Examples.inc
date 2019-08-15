.. _ug-sa-env-examples-for-deploying:

Examples for Deploying SIMP Environments
----------------------------------------

The example below steps through one way to set up a test environment.
You might also find the following HOWTO documents to be helpful:

* :ref:`How To Generate a  SIMP Puppetfile<howto-generate-a-simp-puppetfile>`

  Gives detailed examples of how to generate a Puppetfile for use with r10k
  from a :ref:`Local deployment scenario<ug-sa-env-deployment-scenarios--local>`
  or using Internet Repos.

* :ref:`How To Create a Control Repository<howto-setup-a-simp-control-repository>`

  Gives examples of how to create Control Repos for use with r10k or Code Manager
  both from a SIMP installation and from the Internet Repos.

* :ref:`How To Set up the SIMP Provided ENC<howto-simp-enc>`

  Explains how to set up the :term:`ENC` provided by the simp-util RPM.  Once this
  is set up, the environment a node is assigned to can be set in using a configuration
  file located on the puppetserver.

.. _example_create_and_deploy_new_env:

Creating and Deploying a Test Environment Using Local Module Repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This example uses the :ref:`Local deployment scenario<ug-sa-env-deployment-scenarios--local>`
It steps through creating a new :term:`SIMP Omni-Environment`, for which the
SIMP :term:`secondary <SIMP Secondary Environment>` and :term:`writable <SIMP Writable Environment>`
environments are linked to the corresponding directories in the existing ``production``
environment.  This will allow the two environments to use common passwords and
PKI certificates.  It is helpful to test out changes in SIMP or other Puppet modules.

It assumes:

* You have upgraded SIMP-packaged Puppet module RPMs via ``yum``.
* You have updated some of your own Puppet modules.
* You want to create a ``test`` Puppet environment with both sets of the changes.
* You want to assign a subset of nodes to this new environment to test the
  changes.



#. **Create the new SIMP Omni-Environment**

   Run the ``simp`` command to create a :term:`SIMP Omni-Environment`
   called  ``test`` for which the SIMP :term:`secondary <SIMP Secondary Environment>` and
   :term:`writable <SIMP Writable Environment>` environments are linked to the
   corresponding directories in the existing ``production`` environment.

   .. code-block:: sh

      simp environment new test --link production

   This will:

   * Create the Puppet environment ``test``
   * Copy over the entire ``production`` Puppet environment over to the ``test``
     environment.
   * Link the ``test`` secondary  environment to the ``production`` secondary
     environment.
   * Link the ``test`` writable  environment to the ``production`` writable
     environment.

#. **Update the Puppetfile Pair**

   Edit /etc/puppetlabs/code/environment/test/Puppetfile and add/update
   any of your own puppet modules that you are updating.

   Then update /etc/puppetlabs/code/environment/test/Puppetfile.simp using the
   following:

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/test
      simp puppetfile generate > Puppetfile.simp


#. **Deploy the Modules**

   Use r10k to update the modules in the test environment:

   .. code-block:: sh

      ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
      --puppetfile /etc/puppetlabs/code/environments/test/Puppetfile \
      --moduledir /etc/puppetlabs/code/environments/test/modules' )

#. **Assign nodes to the new environment**

   Since the secondary and writable environments for the ``production``
   and ``test`` SIMP Omni-Environments are linked in this example, you can move
   nodes between these two environments without generating new certificates.

   You can assign the node to the new environment by changing the ``environment``
   setting in the node's ``/etc/puppetlabs/puppet/puppet.conf`` file, or, if you are
   using :ref:`the ENC supplied by the simp-util module<howto-simp-enc>`, adding a line
   like the following to ``/etc/puppetlabs/puppet/environments.yaml``.

   .. code-block:: yaml

      node.my.domain: test

#. **Apply node configuration**

   To apply the configuration for a node in the ``test`` environment, simply run

   .. code-block:: sh

      puppet agent -t

   You should see a message similar to the following at the beginning of
   the log for the Puppet run:

  ``Info: Using configured environment 'test'``

   .. NOTE::

      Once you have deployed a Puppet environment, you can edit the modules in that
      Puppet environment directly to debug issues you may be having and run puppet
      agent -t to deploy those changes.  However, the next time you use ``r10k``
      to deploy the modules, it will **overwrite** any local changes, unless the module
      is defined as ``:local => true`` in the Puppetfile.  It is recommended that you
      use ``git`` to manage your work flow.



.. _ug-sa-env-examples-setting-up-a-control-repo-using-remote-git-repos:

Setting up a SIMP Environment in a Control Repository using Remote Git repositories
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Although (with a detailed understanding of SIMP internals) you can have a
a :term:`SIMP server` that will function with just a Puppet environment, by
default, a functioning SIMP server **requires** its entire :term:`SIMP
Omni-Environment`.

This example creates a SIMP Omni-Environment skeleton whose Puppet environment
you can check into a branch in your control repository.

This example  assumes the new SIMP Omni-Environment will be
using its own secondary and writable environments.

Before proceding with this section, you should

* Have a basic understanding of control repositories.
* Have a basic understanding of how to use ``git``.
* Have a Git repository that you will be using as your control repository.
* Have read :ref:`Deploying SIMP Environments`.  It contains detailed
  descriptions of key topics:

  - the :term:`SIMP Omni-Environment`
  - local, SIMP-managed Git repositories maintained by SIMP Puppet module RPMs
  - SIMP :term:`CLI` commands to assist with SIMP Omni-Environment management.


* You may find it helpful to read the section that explains how a control
  repository works in `Puppet, Inc.'s control repository documentation`_ .

#. Install Prerequisite Packages

   The following packages are required:

   * ``rubygem-simp-cli``
   * ``simp-environment-skeleton``
   * ``simp-rsync-skeleton``
   * ``simp-selinux-policy``

   All of these packages will already be installed if you have installed the SIMP
   server from ISO or from RPMs.

   These RPMS can be obtained from an ISO or if the system has access to the internet
   the SIMP repositories can configured as described in :ref:`gsg-installing_simp_from_a_repository`.

   .. code-block:: bash

      yum install rubygem-simp-cli simp-environment-skeleton simp-rsync-skeleton simp-selinux-policy


#. Create the SIMP Omni-Environment skeleton without SIMP local repository
   Puppetfiles as ``root``:

   .. code-block:: bash

      simp environment new dev1 --no-puppetfile-gen

   This will create two skeleton directories:

   * a Puppet directory at ``/etc/puppetlabs/code/environments/dev1``.
   * a Secondary directory at ``/var/simp/environments/dev1``.

   A new Writable environment directory will be automatically generated, as
   needed, when ``puppet`` is run.

#. Generate the Puppetfile:

   Download the ``Puppetfile`` used to create a SIMP ISO for a specific release
   from the SIMP `simp-core repository`_. In this example, we are going to use
   the SIMP ``6.4.0-0`` release.

   .. code-block:: bash

      cd /etc/puppetlabs/code/environments/dev1
      simp puppetfile generate --skeleton > Puppetfile
      curl -o Puppetfile.simp  https://github.com/simp/simp-core/blob/6.4.0-0/Puppetfile.pinned

   Manually edit the ``Puppetfile.simp`` to remove components that are not Puppet
   modules, by deleting all lines up to and including
   ``moduledir  'src/puppet/modules'``.

   Manually edit the ``Puppetfile`` to add entries for any other non-SIMP modules
   your site requires.

   For more detailed information on editing these files see :ref:`How to Generate  a Puppetfile<howto-generate-a-simp-puppetfile>`.
   This also explains how to create the Puppetfile if you are using Local Repositories instead of Internet Repositories.

#. Add/adjust any of the :term:`Hiera` files in
   ``/etc/puppetlabs/code/environments/dev1/data``


#. Create a New Branch in Your Control Repository

   Create an empty git repository in the newly created Puppet environment:

   .. code-block:: bash

      cd /etc/puppetlabs/code/environments/dev1
      git init .

   Add all the files to a branch named for the environment in this repository:

   .. code-block:: bash

      # create the branch
      git checkout -b dev1

      # add the directory tree
      git add --all

      # verify the directory tree doesn't have any temporary files you created
      git status

      git commit -m 'Initial dev1 environment'

   Push the branch to your control repository:

   .. code-block:: bash

      # Add a remote for your control repository
      git remote add control_repo <URL to the control repo>

      # Push the branch
      git push dev1 control_repo

#. To continue configuring the environment, move on to the :ref:`Client_Management`
   section in the :ref:`simp-user-guide`.

By default this environment will be using a different Certificate
Authority for  :ref:`Infrastructure Certificates<ug-sa-ga-infrastructure-certificates>`. It
therefore may not be able to communicate to a server in another envirnment when
certificate authentication is required,  for example LDAP.  You will have to create a new LDAP server
or add the new CA to the list of trusted CA's in the other environment.

This environment will also generate a new set of passwords when ``simplib::passgen`` function is
used in a puppet module.  It therefore may not be able to talk to a server in another environment
that requires password authentication.


.. _Puppet, Inc.'s control repository documentation: https://docs.puppet.com/pe/latest/cmgmt_control_repo.html
.. _simp-core repository: https://github.com/simp/simp-core

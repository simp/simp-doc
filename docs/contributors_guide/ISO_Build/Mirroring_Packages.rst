.. _gsg-mirroring_packages:

Mirroring Packages
==================

Getting Started
---------------

Follow the instructions in :ref:`gsg-environment_preparation` if you have not
done that yet.

.. NOTE::

   The following steps must be done on the container hosting system unless you
   have nested container capabilities in your build container.

Install Bolt
------------

See the documentation at :github:`puppetlabs/bolt` to install :program:`bolt` on
your system.

Create the Pulp Cloning System
------------------------------

Using :github:`puppetlabs/bolt` and :github:`simp/bolt-pulp3`, set up the
`pulp`_ container.

.. code-block:: bash

   git clone https://github.com/simp/bolt-pulp3
   cd bolt-pulp3
   rvm use system # You can skip this if not using RVM
   /opt/puppetlabs/bolt/bin/bolt module install --force
   /opt/puppetlabs/bolt/bin/gem install --user -g gem.deps.rb
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container
   # Wait a few minutes for the service to get fully started

Clone the Packages
------------------

This uses a configuration file from the target version of SIMP that you are
going to build.

You will either need to download the file from :github:`simp/simp-core` or clone
the repository and checkout the git tag that you wish to build.

For example, if you are going to build CentOS 8 for SIMP 6.6.0:

.. code-block:: bash

   curl -O https://raw.githubusercontent.com/simp/simp-core/6.6.0-1/build/distributions/CentOS/8/x86_64/bolt_pulp3_config.yaml
   ./slim-pulp-repo-copy.rb -f bolt_pulp3_config.yaml

or:

.. code-block:: bash
   ./slim-pulp-repo-copy.rb -f <path_to_cloned_simp-core_module>/build/distributions/<desired_distribution>/<desired_distribution_version>/x86_64/bolt_pulp3_config.yaml

.. NOTE::

   You can change the ``url`` keys in the packages YAML file if you wish to use
   alternate servers.

Troubleshooting
^^^^^^^^^^^^^^^

Repo Clone Hangs
""""""""""""""""

Occasionally, there are issues with the upstream repos that require the `pulp`_
container to be restarted.

The most common one is when the clone process hangs and continually prints that
it is in status ``Waiting``.

If this occurs:

.. code-block:: bash

   podman stop pulp
   podman start pulp
   # Wait a few minutes for the service to get fully started
   ./slim-pulp-repo-copy.rb -f build/6.6.0/CentOS/8/x86_64/repo_packages.yaml

Updated YAML file
"""""""""""""""""

If you have updated your YAML file, you will need to flush and re-clone the
repositories so that you don't pull stale packages.

To do this:

.. code-block:: bash

   rm -rf output
   rm -rf _download_data # Only if the reposync has been run
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container::destroy
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container
   # Wait a few minutes for the service to get fully started
   ./slim-pulp-repo-copy.rb -f build/6.6.0/CentOS/8/x86_64/repo_packages.yaml

If the slim-pulp-repo-copy.rb Fails
"""""""""""""""""

If the slim-pulp-repo-copy.rb fails for any reason, the pulp container
could be in an unknown state. To ensure that nothing from the failed
run will cause issues with future runs, do the following:

.. code-block:: bash
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container::destroy
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container
   # Wait a few minutes for the service to get fully started

This will ensure that the container is removed and recreated in a fresh
state.

Copy the Repo Contents
----------------------

Once the clone process has completed, you need to copy the cloned packages out
of the repository for use in the rest of the build process.

To do so:

.. code-block:: bash

   cd output
   ./_slim_repos.*reposync.sh

Copy the Packages Into the Build Container
------------------------------------------

You now need to copy the files into the build container for use during the build
process.

.. code-block:: bash

   podman cp _download_path/ simp_build_centos8:/tmp
   podman exec -it simp_build_centos8 chown -R build_user:build_user /tmp/_download_path

(Optional) Cleanup
------------------

When you are done with the `pulp`_ container, you may want to reclaim the space
on your system.

To do this, run the following:

.. code-block:: bash

   cd bolt-pulp3
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container::destroy

.. _`pulp`: https://pulpproject.org

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

Install OpenBolt
----------------

Install :term:`OpenBolt` as described in :ref:`gsg-environment_preparation`.
The commands below use the :program:`bolt`, :program:`bundle` and
:program:`ruby` executables that OpenBolt installs into
:file:`/opt/puppetlabs/bolt/bin`.

Create the Pulp Cloning System
------------------------------

Use OpenBolt and :github:`simp/bolt-pulp3` to set up the `pulp`_ container.
:program:`bundle` installs the Ruby gems that the cloning script needs into
:file:`vendor/bundle` in the :file:`bolt-pulp3` directory, so you don't need
root access to the OpenBolt Ruby.

.. code-block:: bash

   git clone https://github.com/simp/bolt-pulp3
   cd bolt-pulp3
   /opt/puppetlabs/bolt/bin/bolt module install --force
   /opt/puppetlabs/bolt/bin/bundle config set --local path vendor/bundle
   /opt/puppetlabs/bolt/bin/bundle install
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container
   # Wait a few minutes for the service to get fully started

Clone the Packages
------------------

The cloning script reads a configuration file from the version of
:github:`simp/simp-core` that you are going to build.  Download the file, or
clone :github:`simp/simp-core` and check out the tag that you want to build.

.. WARNING::

   Only EL8 has a configuration file, and it still mirrors the Puppet 6 and
   Puppet 7 repositories rather than OpenVox.  There are no EL9 or EL10
   configuration files yet.  See `simp-core#963`_.

Run the cloning script with :program:`bundle exec`, so that it uses the gems
from :file:`vendor/bundle`.  Set ``PULP_PORT`` to the port of the `pulp`_
container, which is ``8080`` unless you changed
``pulp3::server_port``.  Under :program:`bundle exec`, the script can't
look the port up by itself.

.. code-block:: bash

   curl -fsSLO https://raw.githubusercontent.com/simp/simp-core/master/build/distributions/RedHat/8/x86_64/bolt_pulp3_config.yaml
   export PULP_PORT=8080
   /opt/puppetlabs/bolt/bin/bundle exec /opt/puppetlabs/bolt/bin/ruby ./slim-pulp-repo-copy.rb -f bolt_pulp3_config.yaml

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
   /opt/puppetlabs/bolt/bin/bundle exec /opt/puppetlabs/bolt/bin/ruby ./slim-pulp-repo-copy.rb -f bolt_pulp3_config.yaml

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
   /opt/puppetlabs/bolt/bin/bundle exec /opt/puppetlabs/bolt/bin/ruby ./slim-pulp-repo-copy.rb -f bolt_pulp3_config.yaml

If the slim-pulp-repo-copy.rb Fails
"""""""""""""""""""""""""""""""""""

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

   podman cp _download_path/ simp_build_el8:/tmp
   podman exec -it simp_build_el8 chown -R build_user:build_user /tmp/_download_path

(Optional) Cleanup
------------------

When you are done with the `pulp`_ container, you may want to reclaim the space
on your system.

To do this, run the following:

.. code-block:: bash

   cd bolt-pulp3
   /opt/puppetlabs/bolt/bin/bolt plan run pulp3::in_one_container::destroy

.. _`pulp`: https://pulpproject.org
.. _simp-core#963: https://github.com/simp/simp-core/issues/963

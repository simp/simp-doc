Pre-Release Checklist
=====================

The bulk of the work to release a component is to verify that the
component is ready for release.  Below is the list of verifications
that must be executed before proceeding with the release.  If any
of these checks fail, the problem identified must be fixed before
you can proceed with the tag and release steps.

.. contents:: Checklist Steps
   :local:

Verify a release is warranted
-----------------------------

This check verifies a new release is warranted and the version has been
properly update:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone https://github.com/simp/pupmod-simp-iptables.git
      cd pupmod-simp-iptables
      git checkout master # this step isn't needed for master branch

#. Run the ``pkg:compare_latest_tag`` rake task

   .. code-block:: bash

      bundle update
      bundle exec rake pkg:compare_latest_tag

   .. IMPORTANT::

      If this check indicates no new tag is required, there
      is no reason to continue with the release procedures.

Verify the CHANGELOG
--------------------

This check verifies that the CHANGELOG information can be properly
extracted:

#. Run the ``pkg:create_tag_changelog`` rake task

   .. code-block:: bash

      bundle exec rake pkg:create_tag_changelog

#. Manually verify the changelog information is emitted and complete.

   * It should begin with ``Release of x.y.z`` and then be followed by
     one or more comment blocks. For example,

     .. code-block:: none

      Release of 6.0.3

      * Thu Aug 10 2017 Nick Markowski <nmarkowski@keywcorp.com> - 6.0.3-0
        - Updated iptables::listen::tcp_stateful example to pass valid
          Iptables::DestPort types to dports

   * It should be understandable.
   * It should be free from typos.
   * Any parsing error messages emitted should *only* be for changelog
     entries for earlier versions.

.. IMPORTANT::

   The changelog information emitted will be used as the content
   of the `GitHub`_ release notes.

Verify the component's dependencies
-----------------------------------

This check verifies the component's dependencies are correct in the
``metadata.json``:

* Verify that the dependencies in the ``metadata.json`` file
  are complete.  This means that the sources of all external
  functions/classes used within the module are  listed in
  the ``metadata.json``.

* Verify that the version constraints for each dependency are
  correct.

.. IMPORTANT::

   Beginning with ``simp-rake-helpers-4.1.0``, the RPM dependencies
   for a component will determined from its ``metadata.json`` file,
   and if present, the component's entry in the
   ``simp-core/build/rpm/dependencies.yaml``.

Verify RPMs can be created
--------------------------

This check verifies that an RPM can be generated for this module from
``simp-core``:

#. Clone ``simp-core``

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git

#. Update the URL for the component under test ``Puppetfile.tracking``,
   if needed

   .. code-block:: bash

      cd simp-core
      vi Puppetfile.tracking

#.  Build RPM

   .. code-block:: bash

      bundle update
      bundle exec rake deps:checkout
      bundle exec rake pkg:single[iptables]

.. NOTE::

   This command will build the RPM for the OS of the server
   on which it was executed.

Verify unit tests pass
----------------------

This check verifies that the component's unit tests have succeeded
in `TravisCI`_:

* Navigate to the project's TravisCI results page and verify the
  tests for the development branch to be tagged and released have
  passed.  For our project, this page is
  https://travis-ci.org/simp/pupmod-simp-iptables/branches

.. IMPORTANT::

   If the tests in TravisCI fail, you **must** fix them before
   proceeding.  The automated release procedures will only
   succeed, if the unit tests succeed in TravisCI.

Verify acceptance tests pass
----------------------------

This check verifies that the component's acceptance tests have
succeeded:

* Run the ``beaker:suites`` rake task with and without FIPS enabled

  .. code-block:: bash

     BEAKER_fips=yes bundle exec rake beaker:suites
     bundle exec rake beaker:suites

.. NOTE::

   * For older projects that have not been updated to use test
     suites, you may have to run the ``acceptance`` rake task,
     instead.

   * If the GitLab instance for the project is current (it is
     sync'd every 3 hours), you can look at the latest acceptance
     test results run by GitLab.  For our project, the results will
     be at https://gitlab.com/simp/pupmod-simp-iptables/pipelines.

Verify interoperability with last SIMP release
----------------------------------------------

This check verifies that this version of the component interoperates
with the last full SIMP release. For many components, the best
automated way of doing this is by running the ``simp-core`` and
``pupmod-simp-simp`` acceptance tests, as these tests provide
extensive, multi-component, integration tests.

.. NOTE:

   If this component release is not expected to interoperate
   with the last release, substitute the ``simp-core`` and
   and ``pupmod-simp-simp`` versions, below, with the correct
   versions.

#. Checkout the ``simp-core`` project for the last SIMP release.
   For this discussion, we will assume it is ``6.0.0-1``.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git
      cd simp-core
      git fetch -t origin
      git checkout tags/6.0.0-1  # can use a ref spec in lieu of a tag

#. Create a ``Puppetfile.tracking`` file that is a copy of the
   ``Puppetfile.stable`` file for which this component version and any
   newer dependencies this version itself requires have been updated.

#. Run the default ``simp-core`` acceptance tests

   .. code-block:: bash

       bundle update
       bundle exec rake beaker:suites

#. Checkout the version of ``pupmod-simp-simp`` corresponding to the
   last ``simp-core`` release

   .. code-block:: bash

       bundle exec rake deps:checkout
       cd src/puppet/modules/pupmod-simp-simp

#. Create a ``.fixtures.yml`` file that overlays the contents of the
   ``Puppetfile.stable`` file 3 directories above, with this component
   version and any newer dependencies this version itself requires.

   .. NOTE::

      Currently, there are prototype utilities to generate the
      ``.fixtures.yml`` file for you.  When these utilities are
      released,  this documentation will be (thankfully) updated.

#. Run the acceptance tests with and without FIPS mode enabled

   .. code-block:: bash

      bundle update

      BEAKER_fips=yes bundle exec rake beaker:suites
      bundle exec rake beaker:suites

      BEAKER_fips=yes bundle exec rake beaker:suites[base_apps]
      bundle exec rake beaker:suites[base_apps]

      BEAKER_fips=yes bundle exec rake beaker:suites[no_simp_server]
      bundle exec rake beaker:suites[no_simp_server]

      BEAKER_fips=yes bundle exec rake beaker:suites[scenario_one_shot]
      bundle exec rake beaker:suites[scenario_one_shot]

      BEAKER_fips=yes bundle exec rake beaker:suites[scenario_poss]
      bundle exec rake beaker:suites[scenario_poss]

      BEAKER_fips=yes bundle exec rake beaker:suites[scenario_remote_access]
      bundle exec rake beaker:suites[scenario_remote_access]


Verify the component RPM upgrade succeeds
-----------------------------------------

This check verifies that the RPM for this component can be used to
upgrade the last full SIMP release.  For both CentOS 6 and CentOS 7,
do the following:

#. Bring up a CentOS server that was booted from the last SIMP ISO
   release and for which ``simp config`` and ``simp bootstrap`` has
   been run.

   .. NOTE::

      If the VirtualBox for the last SIMP ISO was created by the
      `simp-packer`_ project, you can simply setup the appropriate
      VirtualBox network for that box and then bring up that
      bootstrapped image with ``vagrant up``.

#. Copy the component RPM generated from the above RPM verification
   check to the server and install with yum.  For example,

   .. code-block:: bash

      sudo yum install pupmod-simp-iptables-6.0.3-1.noarch.rpm

   .. NOTE::

      * If the component requires updated dependencies, those RPMs will
        have to be built and installed at the same time.

      * Puppet agent runs will be tested in
        `Verify the component works in an actual SIMP system`_

Verify the component yields valid SIMP ISOs
-------------------------------------------

This check verifies that with this component, valid SIMP ISOs for
for CentoOS 6 and CentOS 7 can be built. An ISO is considered
to be valid when a SIMP server can be booted from it, configured via
``simp config``, and then bootstrapped via ``simp bootstrap``.  For
CentOS 6 and CentOS 7:

#. Login to a machine that has `Docker`_ installed and the ``docker``
   service running.

   .. IMPORTANT::

      In our development environment, the version of Docker
      that is available with CentOS works best.

#. Checkout the ``simp-core`` project for the last SIMP release.
   For this discussion, we will assume it is ``6.0.0-1``.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git
      cd simp-core
      git fetch -t origin
      git checkout tags/6.0.0-1

#. Create a ``Puppetfile.tracking`` file that contains the contents
   of ``Puppetfile.stable`` in which the URLs for the component and
   any of its updated dependencies have been updated to reference
   the versions under test.

#. Populate ``simp-core/ISO`` directory with CentOS6/7 distribution ISOs

   .. code-block:: bash

      mkdir ISO
      cp /net/ISO/Distribution_ISOs/CentOS-6.9-x86_64-bin-DVD*.iso ISO/
      cp /net/ISO/Distribution_ISOs/CentOS-7-x86_64-1708.iso ISO/

#. Build each ISO for CentOS 6 and CentOS 7.  For example,

   .. code-block:: bash

      bundle update
      SIMP_BUILD_docs=no \
      SIMP_BUILD_verbose=yes \
      SIMP_PKG_verbose=yes \
      bundle exec rake beaker:suites[rpm_docker]

   .. IMPORTANT::

      #. By default, the ``default.yml`` for the ``rpm_docker`` suite
         builds an ISO for CentOS 7.  You must manually edit the
         ``default.yml`` file to disable the ``el7-build-server``
         instead of the ``el6-build-server``, in order to create
         a CentOS 6 ISO.

      #. The most reliable way to build each ISO is from a clean checkout
         of ``simp-core``.

#. Use `simp-packer`_ to verify the SIMP ISO can be bootstrapped, when
   booted with the default options.

Verify the component works in an actual SIMP system
---------------------------------------------------

This is the *Eat Our Own Dogfood* soak test. It verifies that
the component operates as expected on a typical SIMP system.  For
this verification, we install the component via R10K in the SIMP
development environment:

#. Create a branch in the control repo for the version under test.
#. Use the module-portion of the ``Puppetfile.tracking`` from the
   ISO-build-verification step as the Puppetfile for the environment.
#. Deploy the environment using r10k.  In this example our environment
   will be ``simp_6_1_0_test``

   .. code-block:: bash

      /opt/puppetlabs/puppet/bin/r10k deploy environment simp_6_1_0_test -p

#. Assign nodes to the test environment using the installed ENC
#. Verify ``puppet agent -t`` successfully runs for each node
   assigned to the test environment.

.. _Docker: https://www.docker.com
.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com
.. _simp-packer: https://github.com/simp/simp-packer
.. _`RPM spec file template`: https://raw.githubusercontent.com/simp/rubygem-simp-rake-helpers/master/lib/simp/rake/helpers/assets/rpm_spec/simpdefault.spec
.. _TravisCI: https://travis-ci.org

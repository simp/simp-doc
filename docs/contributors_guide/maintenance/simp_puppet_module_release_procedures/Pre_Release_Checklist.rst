Pre-Release Checklist
=====================

The bulk of the work to release a component is to verify that the
component is ready for release.  Below is the list of verifications
that must be executed before proceeding with the release.  If any
of these checks fail, the problem identified must be fixed before
you can proceed with the tag and release steps.

* `Verify a release is warranted`_
* `Verify the CHANGELOG`_
* `Verify the component's dependencies`_
* `Verify a Puppet module can be created`_
* `Verify RPMs can be created`_
* `Verify unit tests pass`_
* `Verify acceptance tests pass`_
* `Verify interoperability with last SIMP release`_
* `Verify the component RPM upgrade succeeds`_
* `Verify the component yields valid SIMP ISOs`_
* `Verify the component works in an actual SIMP system`_
  

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

#. Run the ``compare_latest_tag`` rake task

   .. code-block:: bash

      bundle update
      bundle exec rake compare_latest_tag

   .. IMPORTANT::

      If this check fails because no release is required, there
      is no reason to continue with the release procedures.

Verify the CHANGELOG
--------------------

This check verifies that the CHANGELOG information can be properly
extracted:

#. Run the ``changelog_annotation`` rake task

   .. code-block:: bash

      bundle exec rake changelog_annotation

#. Manually verify the changelog information is emitted.

   * It should begin with 'Release of x.y.z' and then be followed by
     one or more comment blocks.
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
``metadata.json`` and ``build/rpm_metadata/requires`` files:

* Verify there are no dependencies in the ``metadata.json`` file
  that do not exist in the ``build/rpm_metadata/requires`` and
  vice versa, except for ``puppetlabs/stdlib`` and ``simp/simplib``.

* Verify that the version constraints for each dependency are
  the same in both files.

.. IMPORTANT::

   The ``puppetlabs/stdlib`` and ``simp/simplib`` dependencies are
   automatically added to the compenent's RPM requires list, when the
   RPM is built.  So, these dependencies **only** have to be listed
   in the ``build/rpm_metadata/requires`` file, **if** the version(s)
   the module requires are newer than than those specified in the
   `RPM spec file template`_ .

Verify a Puppet module can be created
-------------------------------------

This check verifies that a `PuppetForge`_-deployable Puppet module can
be created:

.. code-block:: bash

   bundle exec rake metadata_lint
   puppet module build

Verify RPMs can be created
--------------------------

This check verifies that CentOS 6 and CentOS 7 RPMs can be generated
for this module:

.. code-block:: bash

   bundle exec rake pkg:rpm[epel-6-x86_64]
   bundle exec rake pkg:rpm[epel-7-x86_64]

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
automated way of doing this is by running the ``pupmod-simp-simp``
acceptance tests, as these tests provide extensive, multi-component,
integration tests.

#. Determine the version of ``pupmod-simp-simp`` used in the last SIMP
   release.  This version can be pulled from the ``Puppetfile.stable``
   file of the ``simp-core`` project tag for the last release.

#. Checkout the ``pupmod-simp-simp`` project for the last SIMP release.
   For this discussion, we will assume it is ``4.0.0``.

   .. code-block:: bash

      git clone https://github.com/simp/pupmod-simp-simp.git``
      cd pupmod-simp-simp
      git fetch -t origin
      git checkout tags/4.0.0  # can use a ref spec in lieu of a tag

#. Create a ``.fixtures.yml`` file that overlays the contents of the
   ``Puppetfile.stable`` file  of the ``simp-core`` project tag for
   the last release, with this component version and any newer
   dependencies this version itself requires.

   .. NOTE::

      Currently, there are prototype utilities to generate the
      ``.fixtures.yml`` file for you.  When these utilities are
      released,  this documentation will be (thankfully) updated.

#. Run the acceptance tests with and without FIPS mode enabled

   .. code-block:: bash

      bundle update
      BEAKER_fips-yes bundle exec rake beaker:suites
      bundle exec rake beaker:suites

Verify the component RPM upgrade succeeds
-----------------------------------------

This check verifies that the RPM for this component can be used to
upgrade the last full SIMP release.  For both CentOS 6 and CentOS 7,
do the following:

#. Bring up a CentOS server that was booted from the appropriate SIMP
   ISO and for which ``simp config`` and ``simp bootstrap`` has been
   run.

   .. NOTE::

      The `simp-packer`_ project is the easiest way to create a SIMP
      VM that has been bootstrapped.

#. Copy the component RPM generated from the above RPM verification
   check to the server and install with yum.  For example,

   .. code-block:: bash

      sudo yum install pupmod-simp-iptables-6.0.2-2016.1.noarch.rpm

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

#. Checkout the ``simp-core`` project for the last SIMP release.
   For this discussion, we will assume it is ``6.0.0-0``.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git``
      cd simp-core
      git fetch -t origin
      git checkout tags/6.0.0-0

#. Create a ``Puppetfile.tracking`` file that contains the contents
   of ``Puppetfile.stable`` in which the URLs for the component and
   any of its updated dependencies have been updated to reference
   the versions under test.

#. Build each ISO for CentOS 6 and CentOS 7.  For example

   .. code-block:: bash

      PUPPET_VERSION="~> 4.8.2" \
      SIMP_BUILD_verbose=yes \
      SIMP_PKG_verbose=yes \
      SIMP_BUILD_distro=CentOS/7/x86 _64 \
      bundle exec rake build:auto[/net/ISO/Distribution_ISOs]

   .. IMPORTANT::
      The most reliable way to build each ISO is from a clean checkout
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

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com
.. _simp-packer: https://github.com/simp/simp-packer
.. _`RPM spec file template`: https://raw.githubusercontent.com/simp/rubygem-simp-rake-helpers/master/lib/simp/rake/helpers/assets/rpm_spec/simpdefault.spec
.. _TravisCI: https://travis-ci.org

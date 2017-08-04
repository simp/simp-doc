Pre-Release Checklist
=====================

The bulk of the work to release each component is to verify that the
component is ready for release.  Below is the list of verifications
that must be executed before proceeding with the release.  If any
of these steps fail, the problem identified must be fixed before
you can proceed with the tag and release steps.

* `Verify a release is warranted`_
* `Verify the changelog`_
* `Verify RPMs can be created`_
* `Verify unit tests pass`_
* `Verify acceptance tests pass`_
* `Verify the component RPM upgrade succeeds`_
* `Verify the component yields valid SIMP ISOs`_

Verify a release is warranted
-----------------------------

The check verifies a new release is warranted and the version has been
properly updated.

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone https://github.com/simp/simp-adapter.git
      cd simp-adapter
      git checkout master # this step isn't needed for master branch

#. Manually compare manually the development branch with the last
   release tag. (The existing rake task ``compare_latest_tag`` won't
   necessarily work here.)

   .. code-block:: bash

      git fetch -t origin

      # manually figure out which is last tag

      git diff tags/<last release tag> --name-only

      # manually verify mission-impacting changes have been
      # made (i.e., changes that warrant a release) and the
      # version has been updated in the CHANGELOG, version.rb
      # and/or build/<component>.spec file.

Verify the changelog
--------------------

This check verifies the changelog information is available and can be
extracted:

* Manually inspect the appropriate file (CHANGELOG or %changelog
  section of <component>.spec file).  (The existing rake task
  ``changelog_annotation`` won't necessarily work here.)

* FIXME ``simp-doc`` has its own ``CHANGELOG``, but requires the
  ``Changelog.rst`` from ``simp-core`` to be current as well.
  It may make more sense to move the ``simp-doc`` release into
  the instructions for releasing a SIMP ISO.

Verify RPMs can be created
--------------------------
This check verifies that CentOS 6 and CentOS 7 RPMs can be generated
for this module:

.. code-block:: bash

   bundle update
   bundle exec rake pkg:rpm[epel-6-x86_64]
   bundle exec rake pkg:rpm[epel-7-x86_64]

Verify unit tests pass
----------------------

This check verifies that the component's unit tests have succeeded
in `TravisCI`_:

* Navigate to the project's `TravisCI`_ results page and verify the
  tests for the development branch to be tagged and released have
  passed.  For our project, this page is
  https://travis-ci.org/simp/simp-adapter/branches

.. IMPORTANT::

   If the tests in TravisCI fail, you **must** fix them before
   proceeding.  The automated release procedures will only
   succeed, if the unit tests succeed in TravisCI.

Verify acceptance tests pass
----------------------------

This check verifies that the component's acceptance tests have
succeeded:


* Run the appropriate acceptance test rake task, if it exists.
  For this project, ``rake beaker:suites`` is the appropriate task

  .. code-block:: bash

     bundle exec rake beaker:suites

.. NOTE::

   If the GitLab instance for the project is configured and
   current (it is sync'd every 3 hours), you can look at
   the latest acceptance test results run by GitLab.  For
   our project, the results would be at
   https://gitlab.com/simp/simp-adapter/pipelines.

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
   step to the server and install with yum.  For example,

   .. code-block:: bash

      sudo yum install simp-adapter-0.0.3-0.el7.noarch.rpm

   .. NOTE::

      If the component requires updated dependencies, those RPMs will
      have to be built and installed at the same time.

#. Verify the ``puppet agent`` runs succeed on the Puppet master

   * login as root
   * execute ``puppet agent -t``

#. Execute any other verifications unique to the component

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
      The most reliable way to build each ISO is from a clean
      checkout of ``simp-core``.

#. Use `simp-packer`_ to verify the SIMP ISO can be bootstrapped


.. _GitHub: https://github.com
.. _packagecloud: https://packagecloud.io/simp-project
.. _simp-packer: https://github.com/simp/simp-packer
.. _`RPM spec file template`: https://raw.githubusercontent.com/simp/rubygem-simp-rake-helpers/master/lib/simp/rake/helpers/assets/rpm_spec/simpdefault.spec
.. _TravisCI: https://travis-ci.org

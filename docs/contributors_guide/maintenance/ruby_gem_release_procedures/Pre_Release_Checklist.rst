Pre-Release Checklist
=====================

The bulk of the work to release a component is to verify that the
component is ready for release.  Below is the list of verifications
that must be executed before proceeding with the release.  If any
of these steps fail, the problem identified must be fixed before
you can proceed with the tag and release steps.

* `Verify a release is warranted`_
* `Verify the CHANGELOG`_
* `Verify the component's dependencies`_
* `Verify a Ruby gem can be created`_
* `Verify unit tests pass`_
* `Verify acceptance tests pass`_
* `Verify gem works for SIMP projects`_

Verify a release is warranted
-----------------------------

This check verifies a new release is warranted and the version has been
properly updated:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone https://github.com/simp/rubygem-simp-rake-helpers.git
      cd rubygem-simp-rake-helpers
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
      # version has been updated in the CHANGELOG.md, version.rb
      # and/or TBD file.

Verify the changelog
--------------------

This check verifies that the changelog information is available
and can be extracted

* Manually inspect the appropriate file (e.g., CHANGELOG.md) 
  (The existing rake task ``changelog_annotation`` won't
  necessarily work here.)

Verify the component's dependencies
-----------------------------------

This check verifies that the component's dependencies are correct in
the ``Gemfile`` and ``<component>.gemspec``

* Manually inspect the ``Gemfile`` and ``<component>.gemspec`` to look
  for inconsistencies or missing runtime dependencies.

Verify a Ruby gem can be created
--------------------------------

This check verifies that a Ruby gem can be created for this component:

.. code-block:: bash

   bundle update
   bundle exec rake pkg:gem

Verify unit tests pass
----------------------

This check verifies that the component's unit tests have succeeded
in `TravisCI`_:

* Navigate to the project's `TravisCI`_ results page and verify the
  tests for the development branch to be tagged and released have
  passed.  For our project, this page is
  https://travis-ci.org/simp/rubygem-simp-rake-helpers/branches

  .. IMPORTANT::

     If the tests in TravisCI fail, you **must** fix them before
     proceeding.  The automated release procedures will only
     succeed, if the unit tests succeed in TravisCI.

Verify acceptance tests pass
----------------------------

This check verifies that the component's acceptance tests have
succeeded:

* Run the appropriate acceptance test rake task, if it exists.
  For this project, ``rake -T`` shows that ``rake acceptance``
  is the appropriate task

  .. code-block:: bash

     bundle exec rake acceptance

  .. NOTE::

     If the GitLab instance for the project is configured and
     current (it is sync'd every 3 hours), you can look at
     the latest acceptance test results run by GitLab.  For
     our project, the results would be at
     https://gitlab.com/simp/rubygem-simp-rake-helpers/pipelines.

Verify gem works for SIMP projects
----------------------------------

This check verifies that SIMP components can use this gem for build
and test tasks. 

#. Install the gem you just built, locally.  

   .. code-block:: bash

      rvm all do gem install dist/simp-rake-helpers-4.0.1.gem

#. Download the latest versions of most of the SIMP components using
   the ``simp-core`` project.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git``
      cd simp-core
      bundle update
      bundle exec rake deps:checkout

#. If the major version number for the gem has increased, for the
   following projects, update the Gemfile to permit the newer version

   - All projects in ``src/assets/``
   - All projects in ``src/rsync``
   - All projects in ``src/rubygems/``
   - All SIMP-owned projects in ``src/puppet/modules/``

#. In each project listed above, execute the rake tasks affected
   by the changes.  In this case, we assume the ``spec`` task
   was affected.

   .. code-block:: bash

      bundle update
      bundle exec rake spec

.. _GitHub: https://github.com
.. _RubyGems.org: https://rubygems.org/
.. _TravisCI: https://travis-ci.org

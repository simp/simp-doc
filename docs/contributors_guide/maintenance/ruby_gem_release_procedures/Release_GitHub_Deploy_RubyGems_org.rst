Release To GitHub and Deploy to RubyGems.org
============================================

Each SIMP Ruby gem is configured to automatically create a `GitHub`_
release and push the release to `RubyGems.org`_, when an annotated tag
is created for the `GitHub`_ project **and** the `TravisCI`_ tests for
the annotated tag push succeed.  To create the annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/rubygem-simp-rake-helpers.git
      cd rubygem-simp-rake-helpers
      git checkout master # this step isn't needed for master branch

#. Generate the changelog content

   * Manually extract the changelog content from the ``CHANGELOG.md``,
     file and write into a file.  In this example, the written file
     will be ``foo``.

#. Create the annotated tag.  In this example the content of 'foo' is::

      Release of 4.0.1

      * Reverted the bundler pinning since it was causing too many issues on CI
        systems

   .. code-block:: bash

      git tag -a 4.0.1 -F foo
      git push origin 4.0.1

   .. NOTE::

      For markdown-style changelogs, you will need to specify
      ``--cleanup=whitespace`` so comment headers are not stripped.

#. Verify `TravisCi`_ completes successfully

   .. IMPORTANT::
      If any of the required TravisCI builds for the project fail, for
      example due to intermittent connectivity problems with `GitHub`_,
      you can complete the release process by manually restarting the
      failed build on the Travis page for that build.

#. Verify release exists on `GitHub`_.  This release will have been created by
   ``simp-auto``.

#. Verify release exists on `RubyGems.org`_. 

.. _GitHub: https://github.com
.. _RubyGems.org: https://rubygems.org/
.. _TravisCI: https://travis-ci.org

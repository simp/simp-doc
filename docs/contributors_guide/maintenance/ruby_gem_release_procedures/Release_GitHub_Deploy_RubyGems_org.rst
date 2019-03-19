Release To GitHub and Deploy to RubyGems.org
============================================

At this time, most but not all of the SIMP Ruby build and test gems
are configured to automatically release from an annotated tag.  So,
this section will describe both the automated steps and the manual
steps required to release SIMP Ruby gems to `GitHub`_ and `RubyGems.org`_.

Common Release Steps
--------------------

Most of the SIMP Ruby gems are configured to automatically create a
`GitHub`_ release and push the release to `RubyGems.org`_, when an
annotated tag is created for the `GitHub`_ project **and** the
`TravisCI`_ tests for the annotated tag push succeed.

To create the releases from an annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/rubygem-simp-rake-helpers.git
      cd rubygem-simp-rake-helpers
      git checkout BRANCH # this step isn't needed for master branch

#. Manually generate the changelog content in a file.

   * The first line should be blank.
   * The second line should be 'Release of x.y.z'
   * The third line should be blank
   * The remaining lines should contain the list of changes.


#. Create the annotated tag.  In this example the content of ``foo`` is:

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

Automated Release Steps
-----------------------

This section applies to gems that have a ``deploy`` stage with a ``releases``
provider in their ``.travis.yml`` file.

#. Verify release exists on `GitHub`_.  This release will have been created by
   ``simp-auto``.

#. Verify release exists on `RubyGems.org`_.

Manual Release Steps
--------------------

For any gem that has not been configured to automatically release
from an annotated tag, you must manually release the gem.

To create the releases from an annotated tag:

#. Create a release of the annotated tag on GitHub.

   * Select the ``Draft a new release`` button.
   * Click in the ``Tag version`` box and then select the annotated
     release version from the drop-down menu.
   * Select the ``Publish release`` button.  The changelog information
     for the annotated tag will automatically appear as the release
     notes.

#. Publish to RubyGems.org

   .. NOTE::

      This requires that you have a GPG key in place that allows you to publish
      to `RubyGems.org`_ and is valid for the Gem that you are attempting to
      push.

   * Run ``gem build simp-rake-helpers.gemspec``
   * Run ``gem push simp-rake-helpers-4.0.1.gem``

.. _GitHub: https://github.com
.. _RubyGems.org: https://rubygems.org/
.. _TravisCI: https://travis-ci.org

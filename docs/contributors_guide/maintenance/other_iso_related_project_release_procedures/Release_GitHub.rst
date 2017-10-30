Release to GitHub
=================

At this time only one ISO-related SIMP project (``rubygem-simp-cli``)
is configured to automatically release to GitHub. So, this section will
decribe both the automated steps and the manual steps required to release
the other ISO-related projects to GitHub.

Automated Release Steps
-----------------------

Some SIMP ISO-related project are configured to automatically create a
`GitHub`_ release, when an annotated tag is created for the `GitHub`_
project **and** the `TravisCI`_ tests for the annotated tag push succeed.
Such a project will contain a deploy step for the ``releases`` provider
in its ``.travis.yml`` file.

To create the a release from an annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/rubygem-simp-cli.git
      cd rubygem-simp-cli
      git checkout master # this step isn't needed for master branch

#. Generate the changelog content

   * Manually extract the changelog content from the ``CHANGELOG.md``,
     ``CHANGELOG``, or ``build/<component>.spec`` file and write
     into a file.  In this example, the written file will be ``foo``.

#. Create the annotated tag.  In this example the content of 'foo' is::

      Release of 4.0.4

      * Mon Oct 16 2017 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.0.4
        - Fix intermittent failure in RPM builds due to missing rubygems

   .. code-block:: bash

      git tag -a 4.0.4 -F foo
      git push origin 4.0.4

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

Manual Release Steps
--------------------

Some SIMP ISO-related projects require manual steps to generate a
`GitHub`_ release.  None of these projects will contain a deploy step
in its ``.travis.yml`` file.

To create the release from an annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/simp-adapter.git
      cd simp-adapter
      git checkout master # this step isn't needed for master branch

#. Generate the changelog content

   * Manually extract the changelog content from the ``build/<name>.spec``,
     file and write into a file.  In this example, the written file
     will be ``foo``.

#. Create the annotated tag.  In this example the content of 'foo' is::

      Release of 0.0.5

      * Fri Oct 20 2017 Trevor Vaughan <tvaughan@onyxpoint.com> - 0.0.5-0
        - Fixed the Changelog dates

   .. code-block:: bash

      git tag -a 0.0.5 -F foo
      git push origin 0.0.5

   .. NOTE::

      For markdown-style changelogs, you will need to specify
      ``--cleanup=whitespace`` so comment headers are not stripped.

#. Verify `TravisCi`_ completes successfully

#. Create a release of the annotated tag on GitHub.

   * Select the ``Draft a new release`` button.
   * Click in the ``Tag version`` box and then select the annotated
     release version from the drop-down menu.
   * Select the ``Publish release`` button.  The changelog information
     for the annotated tag will automatically appear as the release
     notes.

.. _GitHub: https://github.com
.. _TravisCI: https://travis-ci.org

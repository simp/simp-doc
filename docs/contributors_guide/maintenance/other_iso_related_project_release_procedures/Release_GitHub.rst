Release to GitHub
=================

At this time all of the ISO-related SIMP projects are configured to
automatically release to GitHub, but as we transition to the new
deploy strategy, there may be issues with old deploy keys.
This section will describe both the automated steps and the manual
corrective steps required to release the other ISO-related projects to
GitHub.

Automated Release Steps
-----------------------

All SIMP ISO-related project are configured to automatically create a
`GitHub`_ release, when an annotated tag is created for the `GitHub`_
project.

To create the a release from an annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/rubygem-simp-cli.git
      cd rubygem-simp-cli
      git checkout master # this step isn't needed for master branch

#. Generate the changelog content

   .. code-block:: bash

      bundle update
      bundle exec rake pkg:create_tag_changelog > foo

#. Create the annotated tag.  In this example the content of 'foo' is::

      Release of 4.0.4

      * Mon Oct 16 2017 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.0.4
        - Fix intermittent failure in RPM builds due to missing rubygems

   .. code-block:: bash

      git tag -a 4.0.4 -F foo
      git push origin 4.0.4

#. Verify release exists on `GitHub`_.  This release will have been created by
   ``simp-auto``.

Fixing a Failed Deploy to GitHub
--------------------------------

If the deploy stage for a project fails to release to GitHub because of
deploy key issues, the following manual steps can be followed to manually
correct the issue:

#. Create a release of the annotated tag on GitHub.

   * Select the ``Draft a new release`` button.
   * Click in the ``Tag version`` box and then select the annotated
     release version from the drop-down menu.
   * Select the ``Publish release`` button.  The changelog information
     for the annotated tag will automatically appear as the release
     notes.

.. _GitHub: https://github.com

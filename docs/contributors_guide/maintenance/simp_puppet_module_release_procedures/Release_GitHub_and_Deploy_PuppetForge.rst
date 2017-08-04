Release to GitHub and Deploy to PuppetForge
===========================================

Each SIMP component is configured to automatically create a `GitHub`_
release and push the release to `PuppetForge`_, when an annotated tag
is created for the `GitHub`_ project **and** the `TravisCI`_ tests for
the annotated tag push succeed.  To create the annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/pupmod-simp-iptables.git
      cd pupmod-simp-iptables
      git checkout master # this step isn't needed for master branch

#. Generate the changelog content

   .. code-block:: bash

      bundle update
      bundle exec rake changelog_annotation > foo

#. Create the annotated tag.  In this example the content of 'foo' is::

      Release of 6.0.2

      * Wed May 24 2017 Brandon Riden <brandon.riden@onyxpoint.com> - 6.0.2-0
        - Added a workaround for Puppet 4.10 type issues
          - There was a bug in Puppet where all lookup() Hash keys were being converted
            into Strings even if they were another data type
          - This is fixed in Puppet > 4.10.2 but this patch will remain for backwards
            compatibility
        - Update puppet dependency in metadata.json
        - Remove OBE pe dependency in metadata.json


   .. code-block:: bash

      git tag -a 6.0.2 -F foo
      git push origin 6.0.2

   .. NOTE::

       For markdown-style changelogs, you will need to specify
       ``--cleanup=whitespace`` so comment headers are not stripped.

#. Verify `TravisCi`_ completes successfully

   .. IMPORTANT::
      If any of the required TravisCI builds for the project fail, for
      example due to intermittent connectivity problems with `GitHub`_,
      you can complete the release process by manually restarting the
      failed build on the Travis page for that build.

#. Verify release exists on `GitHub`_.  This release will have been
   created by ``simp-auto``.

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com
.. _TravisCI: https://travis-ci.org

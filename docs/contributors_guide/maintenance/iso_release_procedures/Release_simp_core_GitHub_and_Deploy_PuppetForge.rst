Release ``simp-core`` to GitHub and PuppetForge
===============================================

``simp-core`` is configured to automatically create a `GitHub`_ release and push
the (meta-module) release to `PuppetForge`_, when an annotated tag is created
for the `GitHub`_ project **and** the GitHub Actions tests for the annotated tag push
succeed.

To create the releases from an annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/simp-core.git
      cd simp-core
      git checkout master # this step isn't needed for master branch

#. Create the annotated tag for the release.  In this example, we
   are assuming the version is ``6.1.0`` and we are using the
   full Changelog.rst content.

   .. code-block:: bash

      git tag -a 6.0.2 -F Changelog.rst --cleanup--whitespace
      git push origin 6.0.2

#. Verify GitHub Actions tests complete successfully

   .. IMPORTANT::

      If any of the required GitHub Actions builds for the project fail, for example
      due to intermittent connectivity problems, you can complete the release
      process by manually restarting the failed build on the GitHub Actions page
      for that build.

#. Verify release exists on `GitHub`_.  This release will have been
   created by ``simp-auto``.

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com

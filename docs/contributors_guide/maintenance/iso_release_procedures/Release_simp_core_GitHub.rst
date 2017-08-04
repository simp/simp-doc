Release ``simp-core`` to GitHub
===============================

``simp-core`` is configured to to automatically create a `GitHub`_ 
release, when an annotated tag is created for the `GitHub`_
project **and** the `TravisCI`_ tests for the annotated tag push succeed.
(Once SIMP-3402 is addressed.) To create the annotated tag:

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

#. Verify `TravisCi`_ completes successfully

   .. IMPORTANT::
      If any of the required TravisCI builds for the project fail, for
      example due to intermittent connectivity problems with `GitHub`_,
      you can complete the release process by manually restarting the
      failed build on the Travis page for that build.

#. Verify release exists on `GitHub`_.  This release will have been
   created by ``simp-auto``.

.. _GitHub: https://github.com
.. _packagecloud: https://packagecloud.io/simp-project
.. _simp-project: http://simp-project.com/ISO/SIMP
.. _simp-packer: https://github.com/simp/simp-packer
.. _TravisCI: https://travis-ci.org

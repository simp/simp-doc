Release to GitHub
=================

FIXME.  Only rubygem-simp-cli is setup to release to GitHub.

Each SIMP ISO-related project is configured to automatically create a
`GitHub`_ release, when an annotated tag is created for the `GitHub`_ 
project **and** the `TravisCI`_ tests for the annotated tag push succeed.
To create the annotated tag:

#. Clone the component repository and checkout the development
   branch to be tagged

   .. code-block:: bash

      git clone git@github.com:simp/simp-adapter.git
      cd simp-adapter
      git checkout master # this step isn't needed for master branch

#. Generate the changelog content

   * Manually extract the changelog content from the ``CHANGELOG.md``,
     ``CHANGELOG``, or ``build/<component>.spec`` file and write
     into a file.  In this example, the written file will be ``foo``.

#. Create the annotated tag.  In this example the content of 'foo' is::

      Release of 0.0.4

      * Removed packaged auth.conf in favor of managing it with Puppet

   .. code-block:: bash

      git tag -a 0.0.4 -F foo
      git push origin 0.0.4

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

.. _GitHub: https://github.com
.. _TravisCI: https://travis-ci.org

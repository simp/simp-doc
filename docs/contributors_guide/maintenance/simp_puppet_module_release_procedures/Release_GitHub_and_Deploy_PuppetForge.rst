Release to GitHub and Deploy to PuppetForge
===========================================

Each SIMP Puppet module releases itself when a version tag is pushed to its
`GitHub`_ project.  The ``Tag: Release to GitHub w/RPMs + Puppet Forge``
workflow (:file:`.github/workflows/tag_deploy.yml`) then:

* checks the version and the CHANGELOG, and builds the module
* creates the GitHub release
* starts the ``RELENG: Build + attach RPMs to GitHub Release`` workflow, which
  attaches signed EL8, EL9 and EL10 RPMs to the release
* deploys the module to `PuppetForge`_, unless the tag is a prerelease

You don't create the tag by hand.  The ``RELENG: Create release tag``
workflow reads the version from :file:`metadata.json`, writes the tag
annotation from the CHANGELOG, and pushes the tag.

To release a module:

#. Merge the pull request that bumps the version in :file:`metadata.json` and
   adds the CHANGELOG entry.

#. Check the tag first with a dry run.  The dry run validates the version and
   prints the tag annotation without pushing anything.  This example releases
   :pupmod:`simp/iptables`:

   .. code-block:: bash

      gh workflow run create_release_tag.yml --repo simp/pupmod-simp-iptables -f dry_run=true

   You can also run the workflow from the **Actions** tab of the project.  Run
   it on the default branch.

#. Create and push the tag:

   .. code-block:: bash

      gh workflow run create_release_tag.yml --repo simp/pupmod-simp-iptables

#. Verify that the ``Tag: Release to GitHub w/RPMs + Puppet Forge`` and
   ``RELENG: Build + attach RPMs to GitHub Release`` workflows complete
   successfully.

   .. IMPORTANT::

      If a job fails, for example due to intermittent connectivity problems,
      re-run the failed job from the GitHub Actions page.

#. Verify that the release exists on `GitHub`_ with the RPMs attached, and
   that the new version is on `PuppetForge`_.

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com

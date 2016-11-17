Contribution Procedure
======================

#. `Fork`_ the project on `GitHub`_

#. Make a new `feature branch`_ for your changes

   * Suggestion: name the branch after the issue (e.g., 'SIMP-999')

#. Make your changes!

   * SIMP contributions should observe the `Puppet Language Style Guide`_
     conventions where feasible
   * Contributions should ideally include relevant spec and/or acceptance tests

#. Save your changes in a **single commit**

   * Use the following commit message conventions:

     .. code-block:: none

        (SIMP-999) Fix the broken thing [50 chars max]

        Discussion about the fix (if needed) [each line: 72 chars max]

        SIMP-998 #comment Comment on a related issue [72 chars max]
        SIMP-999 #close

   * The commit message should be the following format:

     * First line:

       * Start with the Issue name in parentheses [e.g., ``SIMP-999``],
         followed by a summary of the change
       * No longer than **50** characters
       * Followed by a line of white space

     * Subsequent lines:

       * Each line should be no longer than **72** characters

     * Issue references:

       * `JIRA issues can be referenced`_ at the end of the commit message
       * It is recommended to only use the commands ``#comment`` and ``#close``
       * Avoid ``#resolve`` and ``#time`` as it will not update JIRA until
         after the issue is merged

#. Push your changes up to your forked repo on GitHub

#. Create a `pull request`_ ("PR") on GitHub using your new branch

   * The pull request should contain a **single** new commit

     * Use ``git rebase -i`` to squash commits (use ``git rebase -i HEAD~n`` to
       go back ``n`` commits)

#. `Travis-CI`_ will notice the pull request and run CI tests

   * Travis-CI will run tests based on the `.travis.yml`_ file in the
     repository's top-level directory
   * Travis-CI results can be seen in the PR on GitHub and are posted to the
     project's `HipChat`_ channel

#. After passing Travis-CI tests, the GitHub pull request must be picked up in
   `GerritHub`_ for code review

   * **NOTE:** Currently, this is a *manual* process and requires a project
     administrator

#. Pull requests are code reviewed on `GerritHub`_

   * If you need to update an existing pull request, `amend`_ the pull
     request's commit using the following commands:

     * ``git add`` or ``delete`` (this stages the build with the relevant
       changes; ``add --all`` will add all the new changes, otherwise ``add``
       or ``delete`` to tailor your changeset)
     * ``git commit --amend`` (this amends the previous commit)

       * **NOTE:** The final line of the amended commit message must include
         the Gerrit review's Change-ID
         (example: ``Change-Id: Ie536768505a1baff45d6ad3ae4de9e7501ffb53c``)
       * ``git push --force`` (this sends back to the ``master`` branch)

     * If you prefer to amend your change in Gerrithub, you can use the
       `git-review`_ package to make submitting patch sets easier

       * Install the ``git-review`` package
       * Add the remote: ``git remote add gerrit <url>``
       * Run: ``git-review -r gerrit``

#. After the `GerritHub`_ review is approved, the changes will be automatically
   merged into the original `GitHub`_ repository

.. _.travis.yml: http://docs.travis-ci.com/user/build-configuration/
.. _Fork: https://help.github.com/articles/fork-a-repo
.. _GerritHub: https://review.gerrithub.io/#/q/is:open+project:%255Esimp.*
.. _GitHub: https://github.com/simp
.. _HipChat: https://simp-project.hipchat.com/chat
.. _JIRA issues can be referenced: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _Puppet Language Style Guide: https://docs.puppetlabs.com/guides/style_guide.html
.. _Travis-CI: https://travis-ci.org/simp
.. _amend: https://www.atlassian.com/git/tutorials/rewriting-history/git-commit--amend
.. _feature branch: https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow
.. _git-review: https://github.com/openstack-infra/git-review
.. _pull request: https://help.github.com/articles/using-pull-requests

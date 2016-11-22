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

   * Make sure to select the option **Allow edits from maintainers**.  This will allow people in the organization to make edits to the pull request.

#. `Travis-CI`_ will notice the pull request and run CI tests

   * Travis-CI will run tests based on the `.travis.yml`_ file in the
     repository's top-level directory
   * Travis-CI results can be seen in the PR on GitHub and are posted to the
     project's `HipChat`_ channel

#. After passing Travis-CI tests, the GitHub pull request must be reviewed by a
   at least one organization member.

   .. NOTE::

     We recommend using `hub`_ to manage GitHub repositories, and
     all following examples will be using it.

   * If your change gets feedback with status `request changes`, then you can
     update your pull request using the following steps:

     #. In the local repository where the changes were made earlier, the pull
        request can be checked out with
        ``hub checkout https://github.com/simp/<repo>/pull/<number>``.

     #. Checkout the contents of the pull request. Hub will check it out into a
        branch named ``<PR creator username>/<feature branch name>``

     #. ``git add`` or ``delete`` (this stages the build with the relevant
        changes; ``add --all`` will add all the new changes, otherwise ``add``
        or ``delete`` to tailor your changeset)

     #. ``git commit`` and add a new commit.

     #. Add the git remote for the creator of the pull request's repo:
        ``hub remote add <PR creator username>``

     #. Push up your changes:
        ``git push <PR creator username> <feature branch name>``

     #. The pull request will automatically be updated and participants will get
        an email notifying them that there are new commits to review.


#. After the pull request is approved, the all of the commits in the pull
   request will be squashed into one commit and merged into the original
   `GitHub`_ repository

.. _.travis.yml: http://docs.travis-ci.com/user/build-configuration/
.. _Fork: https://help.github.com/articles/fork-a-repo
.. _GitHub: https://github.com/simp
.. _HipChat: https://simp-project.hipchat.com/chat
.. _JIRA issues can be referenced: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _Puppet Language Style Guide: https://docs.puppetlabs.com/guides/style_guide.html
.. _Travis-CI: https://travis-ci.org/simp
.. _amend: https://www.atlassian.com/git/tutorials/rewriting-history/git-commit--amend
.. _feature branch: https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow
.. _hub: https://hub.github.com/
.. _pull request: https://help.github.com/articles/using-pull-requests

.. _gsg-contributors_guide-contribution_procedure:

Contribution Procedure
======================

We use the standard `GitHub workflow`_ for SIMP development with the exception
that we use a `Squash and Merge`_ commit sequence for a linear commit history.

#. Search the `SIMP JIRA`_ for an open ticket that is relevant to the issue or
   open a new one.

#. Use the `GitHub GUI to fork and clone`_ the repository (we'll use
   ``pupmod-simp-iptables`` for the rest of this walkthrough)

#. Clone the repo you want to work on:

   * ``git clone git@github.com:<YOUR_GITHUB_NAME>/pupmod-simp-iptables iptables``

#. Change into the repository's directory:

   * ``cd iptables``

#. Create a `feature branch`_: ``git checkout -b SIMP-XXXX``

#. Do your work! `(Including tests, of course)`

#. Commit your work. We will `squash`_ your `pull request`_ into one commit
   when we merge it, so you can use as many commits as you'd like.

   .. IMPORTANT::
      The **first** commit should use the `Commit Message Conventions`_

#. Push your changes to Github on your feature branch:

   * ``git push origin SIMP-XXXX``

#. Using the GitHub GUI, create a `pull request`_ from your feature branch to the branch of the
   original repo that you want to contribute to.

#. `Travis-CI`_ will run the spec tests for the branch and a member of the SIMP
   team will review your submission. You should receive emails from Github as
   the code reviews progress.

#. If changes are requested, read the section below on amending your pull
   request.

Commit Message Conventions
--------------------------

An example commit message that following the SIMP conventions:

  .. code-block:: none

     (SIMP-999) Fix the broken thing [50 chars max]

     Discussion about the fix (if needed) [each line: 72 chars max]

     SIMP-998 #comment Comment on a related issue [72 chars max]
     SIMP-999 #close

The first commit message should be the following format:

  * First line:

    * Start with the Issue name in parentheses [e.g., ``(SIMP-999)``], followed
      by a summary of the change
    * No longer than **50** characters
    * Followed by a line of white space

  * Subsequent lines:

    * Each line should be no longer than **72** characters

  * Issue references:

    * `JIRA issues can be referenced`_ at the end of the commit message
    * It is recommended to only use `JIRA Smart Commit Tags`_ ``#comment`` and
      ``#close``
    * Avoid ``#resolve`` and ``#time`` as it will not update JIRA until
      after the issue is merged

Contribution Procedure (Long Version)
-------------------------------------

#. Search the `SIMP JIRA`_ for an open ticket that is relevant to the issue or
   open a new one.

#. `Fork`_ the project on `GitHub`_

#. Clone the repository

   * ``git clone git@github.com:<YOUR_GITHUB_USERNAME>/pupmod-simp-iptables``

#. Make a new `feature branch`_ for your changes

   * Suggestion: name the branch after the issue (e.g., ``(SIMP-999)``)

#. Make your changes!

   * SIMP contributions should observe the `Puppet Language Style Guide`_
     conventions where feasible
   * Contributions should ideally include relevant spec and/or acceptance tests

#. Save your changes in a **single commit**. Reference the Commit Message
   Conventions section for details.

#. Push your changes up to your forked repo on GitHub

#. Create a `pull request`_ ("PR") on GitHub using your new branch

   * The pull request should contain a **single** new commit

     * Use ``git rebase -i`` to squash commits (use ``git rebase -i HEAD~n`` to
       go back ``n`` commits)

   * Make sure to select the option `Allow edits from maintainers`.  This
     will allow people in the organization to make edits to the pull request.

     * This is the default for any cloned repository

#. `Travis-CI`_ will notice the pull request and run CI tests

   * Travis-CI will run tests based on the `.travis.yml`_ file in the
     repository's top-level directory
   * Travis-CI results can be seen in the pull request on GitHub and are posted
     to the project's `HipChat`_ channel

#. After passing Travis-CI tests, the GitHub pull request must be reviewed by a
   at least one organization member.

   * If your change gets feedback with status ``request changes``, then you can
     update your pull request using the following steps, or read the
     `Github PR documentation`_:

     #. Add a Git remote reference to the upstream repository:

        * ``git remote add upstream https://github.com/simp/pupmod-simp-iptables``

     #. Rebase off of the upstream pull request

        * ``git fetch upstream pull/<ID>/head:<LOCAL_BRANCHNAME>``
        * ``git rebase <LOCAL_BRANCHNAME>``
        * Resolve any merge conflicts prior to proceeding

     #. Make your changes

     #. ``git add`` or ``delete`` (this stages the build with the relevant
        changes; ``add --all`` will add all the new changes, otherwise ``add``
        or ``delete`` to tailor your changeset)

     #. ``git commit`` and add a new commit.

     #. Push up your changes:
        ``git push origin <feature branch name>``

     #. The pull request will automatically be updated and participants will
        get an email notifying them that there are new commits to review.

#. After the pull request is approved, the all of the commits in the pull
   request will be squashed into one commit and merged into the original
   `GitHub`_ repository

.. _.travis.yml: http://docs.travis-ci.com/user/build-configuration/
.. _Allow edits from maintainers: https://help.github.com/articles/allowing-changes-to-a-pull-request-branch-created-from-a-fork/
.. _Fork: https://help.github.com/articles/fork-a-repo
.. _GitHub GUI to fork and clone: https://help.github.com/articles/fork-a-repo/
.. _GitHub Workflow: https://guides.github.com/introduction/flow/
.. _GitHub: https://github.com/simp
.. _Github PR documentation: https://help.github.com/articles/committing-changes-to-a-pull-request-branch-created-from-a-fork/
.. _HipChat: https://simp-project.hipchat.com/chat
.. _JIRA Smart Commit Tags: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _JIRA issues can be referenced: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _Puppet Language Style Guide: https://docs.puppetlabs.com/guides/style_guide.html
.. _SIMP JIRA: https://simp-project.atlassian.net
.. _Squash and Merge: https://github.com/blog/2141-squash-your-commits
.. _Travis-CI: https://travis-ci.org/simp
.. _feature branch: https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow
.. _pull request: https://help.github.com/articles/using-pull-requests
.. _squash: https://github.com/blog/2141-squash-your-commits

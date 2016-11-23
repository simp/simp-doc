Contribution Procedure (Using hub)
==================================

.. NOTE::

   We recommend using `hub`_ to manage GitHub repositories, and
   all following examples will be using it.


#. Search the `SIMP JIRA`_ for an open ticket that is relevant to the issue or
   open a new one.

#. Clone the repo you want to work on: ``hub clone simp/pupmod-simp-iptables``

#. Change into the repository's directory: ``cd iptables``

#. `Fork`_ the repo into your own Github account: ``hub fork``

#. Create a `feature branch`_: ``git checkout -b SIMP-XXXX``

#. Do your work! `(Including tests, of course)`

#. Commit your work. We will squash your `pull request`_ into one commit when we
   merge it, so you can use commits as you'd like. You should have at least the
   first commit following the guidlines outlined below.

#. Push your changes to Github on your feature branch:
   ``git push YOUR_USER SIMP-XXXX``

#. Create a `pull request`_ from your feature branch to the branch of the original
   repo that you want to contribute to: ``hub pull-request``

#. `Travis-CI`_ will run the spec tests for the branch and a member of the SIMP
   team will review your submission. You should receive an email from Github
   when we code review it.

#. If changes are requested, read the section below on amending your pull
   request.


Commit Message Conventions
==========================

An example commit message that following the SIMP conventions:

  .. code-block:: none

     (SIMP-999) Fix the broken thing [50 chars max]

     Discussion about the fix (if needed) [each line: 72 chars max]

     SIMP-998 #comment Comment on a related issue [72 chars max]
     SIMP-999 #close

The first commit message should be the following format:

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


Amending Changes to Submitted Pull Requests
===========================================

#. Clone the source repo: ``hub clone simp/pupmod-simp-iptables``

#. Pull down the pull request:
   ``hub checkout https://github.com/simp/pupmod-simp-iptables/pull/28``

#. Review the code or make your additional changes.

#. Add a new commit with your changes.

#. Push your new commit to the feature branch of the owner of the pull request.
   In this example, the owner is `jeefberkey`, and the feature branch name is
   `SIMP-1897`: ``git push jeefberkey SIMP-1897``

#. The pull request has been updated, and participants have received an email.


Contribution Procedure (Long Version)
=====================================

#. Search the `SIMP JIRA`_ for an open ticket that is relevant to the issue or
   open a new one.

#. `Fork`_ the project on `GitHub`_

#. Make a new `feature branch`_ for your changes

   * Suggestion: name the branch after the issue (e.g., 'SIMP-999')

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

   * Make sure to select the option **Allow edits from maintainers**.  This will allow people in the organization to make edits to the pull request.

#. `Travis-CI`_ will notice the pull request and run CI tests

   * Travis-CI will run tests based on the `.travis.yml`_ file in the
     repository's top-level directory
   * Travis-CI results can be seen in the PR on GitHub and are posted to the
     project's `HipChat`_ channel

#. After passing Travis-CI tests, the GitHub pull request must be reviewed by a
   at least one organization member.

   * If your change gets feedback with status `request changes`, then you can
     update your pull request using the following steps, or read the `Github documentation`_:

     #. In the local repository where the changes were made earlier, the pull
        request can be checked out with

        .. code-block:: bash

           git fetch pulls/<pull number>/head:<feature branch name>
           git checkout <feature branch name>

     #. Make your changes.

     #. ``git add`` or ``delete`` (this stages the build with the relevant
        changes; ``add --all`` will add all the new changes, otherwise ``add``
        or ``delete`` to tailor your changeset)

     #. ``git commit`` and add a new commit.

     #. Add the git remote for the creator of the pull request's repo:
        ``git remote add <PR creator username> <clone URL from PR owners repo>``

     #. Push up your changes:
        ``git push <PR creator username> <feature branch name>``

     #. The pull request will automatically be updated and participants will get
        an email notifying them that there are new commits to review.


#. After the pull request is approved, the all of the commits in the pull
   request will be squashed into one commit and merged into the original
   `GitHub`_ repository

.. _SIMP JIRA: https://simp-project.atlassian.net
.. _.travis.yml: http://docs.travis-ci.com/user/build-configuration/
.. _Fork: https://help.github.com/articles/fork-a-repo
.. _GitHub: https://github.com/simp
.. _HipChat: https://simp-project.hipchat.com/chat
.. _JIRA issues can be referenced: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _Puppet Language Style Guide: https://docs.puppetlabs.com/guides/style_guide.html
.. _Travis-CI: https://travis-ci.org/simp
.. _feature branch: https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow
.. _hub: https://hub.github.com/
.. _pull request: https://help.github.com/articles/using-pull-requests
.. _Github documentation: https://help.github.com/articles/committing-changes-to-a-pull-request-branch-created-from-a-fork/

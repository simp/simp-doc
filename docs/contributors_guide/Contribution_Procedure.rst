.. _gsg-contributors_guide-contribution_procedure:

Contribution Procedure
======================

We use the standard `GitHub workflow`_ for SIMP development with the exception
that we use a `Squash and Merge`_ merge method for pulling in changes, in order
to maintain a more legible commit history on `master`.

#. Search the `SIMP JIRA`_ for an open ticket that is relevant to the issue or
   open a new one.

#. Use the `GitHub GUI to fork and clone`_ the repository (we'll use
   ``pupmod-simp-iptables`` for the rest of this walkthrough)

#. Clone the repo you want to work on:

   * ``git clone git@github.com:<YOUR_GITHUB_NAME>/pupmod-simp-iptables iptables``

#. Enter the directory and create a `feature branch`_: ``git checkout -b SIMP-XXXX``

#. Do your work! `(Including tests, of course)`

#. Commit your work. We will `squash`_ your `pull request`_ into one commit
   when we merge it, so you can use as many commits as you'd like.

   .. IMPORTANT::
      The **first** commit should use the `Commit Message Conventions`_

#. Push your changes to Github on your feature branch:

   * ``git push origin SIMP-XXXX``

#. Using the GitHub GUI, create a `pull request`_ from your feature branch to
   the branch of the original repo that you want to contribute to. Leave the
   '`Allow edits from maintainers`_' checkbox checked to let a team member add
   add commits to your pull request.

#. `Travis-CI`_ will run the spec tests for the branch and a member of the SIMP
   team will `review`_ your submission. You should receive emails from Github as
   the code reviews progress.

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
    * Describe the previous behavior, why it was changed, and the changes in
      detail

  * Issue references:

    * `JIRA issues can be referenced`_ at the end of the commit message
    * It is recommended to only use `JIRA Smart Commit Tags`_ ``#comment`` and
      ``#close``
    * Avoid ``#resolve`` and ``#time`` as it will not update JIRA until
      after the issue is merged

.. _GitHub Workflow: https://guides.github.com/introduction/flow/
.. _Squash and Merge: https://github.com/blog/2141-squash-your-commits
.. _SIMP JIRA: https://simp-project.atlassian.net
.. _GitHub GUI to fork and clone: https://help.github.com/articles/fork-a-repo/
.. _feature branch: https://www.atlassian.com/git/tutorials/comparing-workflows#feature-branch-workflow
.. _squash: https://github.com/blog/2141-squash-your-commits
.. _pull request: https://help.github.com/articles/using-pull-requests
.. _Allow edits from maintainers: https://help.github.com/articles/allowing-changes-to-a-pull-request-branch-created-from-a-fork/
.. _Travis-CI: http://travis-ci.org/simp
.. _review: https://help.github.com/articles/reviewing-proposed-changes-in-a-pull-request/
.. _JIRA issues can be referenced: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _JIRA Smart Commit Tags: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html

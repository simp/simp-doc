.. _gsg-contributors_guide-contribution_procedure:

Contribution Procedure
======================

We use the standard `GitHub workflow`_ for SIMP development with the exception
that we use a `Squash and Merge`_ merge method for pulling in changes. This is
done to to maintain a more legible commit history on `master`.

#. Search the `SIMP JIRA`_ for an open ticket that is relevant to the issue or
   open a new one.

#. Use the `GitHub GUI to fork and clone`_ the repository (we'll use
   ``pupmod-simp-iptables`` for the rest of this walkthrough)

#. Clone the repo you want to work on:

   * ``git clone git@github.com:<YOUR_GITHUB_NAME>/pupmod-simp-iptables iptables``

#. Enter the directory and create a `feature branch`_: ``git checkout -b SIMP-XXXX``

#. Do your work! `(Including tests, of course)`

   * See `gsg-contributors_guide-contribution_procedure-testing_your_submissions`_
     for detailed guidance on test procedures.

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
   code reviews progress.

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

.. _gsg-contributors_guide-contribution_procedure-testing_your_submissions:

Testing Your Submissions
------------------------

First off, thank you again for your contribution! Things don't get better
without your help!

This section contains two sets of guidelines. First, ones that are recommended
for external contributions from the community. Second, ones that are expected
to be adhered to by the core development team.

External Contributors
^^^^^^^^^^^^^^^^^^^^^

We will happily accept all levels of contributions, small, medium, or large
without any tests.

However, for us to quickly and effectively assess your contribution you should
either add unit (``rspec-puppet``) and/or acceptance (``beaker``) tests.

As the size of the contribution increases, this becomes increasingly important,
because, depending upon the complexity of the changes, it may simply be too
difficult to do a timely assessment of such a contribution without corresponding
tests. In these cases, it would be best if you split your contribution into
smaller pull requests that are easier to assess.

Core Developer Contributions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The core development team is expected to follow these guidelines when adding
code to the project.

In all cases, a cursory ``grep`` through the `simp-doc`_ project should be done
and a ticket should be entered if the overall project documentation may be
affected by your change.

This should also be done by the core development team for any external
contributions, since it is unreasonable to expect external contributors
to take the effort to dig through the `simp-doc`_ project.

Trivial Contributions
"""""""""""""""""""""

Trivial contributions are those that constitute a small documentation update,
code correction, or bug fix consisting of only a few lines. These contributions
must not negatively impact the behavior of the user experience or code.

Trivial contributions do not require an associated ticket and may be covered
under a ``SIMP-MAINT`` branch.

Trivial contributions require **one maintenance team member** review and may
optionally add additional unit or acceptance testing.

Minor Contributions
"""""""""""""""""""

Minor contributions are those that add a feature or fix a larger bug in
components that are more than five or ten lines and/or are not only
documentation updates.

Minor contributions must have unit tests and should have acceptance tests.
Acceptance tests may be deferred but a ticket must be filed with an explanation
and a link in the PR if the acceptance test addition is deferred.

Minor contributions require **one maintenance team member** review. The
reviewing team member may decide that acceptance tests are required based on
the understandability of the contribution.

Major Contributions
"""""""""""""""""""

Major contributions are any changes that affect multiple parts of the system,
any contribution of moderate or higher cyclomatic complexity, or anything that
adds a breaking change to the system.

Major contributions must have unit tests that cover all major code paths and
pay particular attention to edge cases.

Acceptance tests must also be provided that cover the primary usage of the code
that, at a minimum, test the code in a way that end users would use it.

User facing changes should also contain documentation updates that cover the
expected use cases.

Major contributions require **two maintenance team member** reviews.

Emergency Contributions
"""""""""""""""""""""""

On occasion, a fix or patch will need to be made with a very short turn around
time. These may include up to `Minor Contributions`_ and may be added after
**two code reviews** without the addition of tests. However, a ticket must be
added that notes a requirement for tests to be added to the specified
capability.  This ticket should link directly to the PR that added the code for
later reference.

Experimental Contributions
""""""""""""""""""""""""""

Experimental contributions are changes that may not be ready for the end user,
but that need reviews and/or attention.

For items that are not end-user facing, such as the testing components or
frameworks, there may be a need to try out different techniques prior to
releasing a full update. These may be added to the unstable ``master`` branch
without testing but tests should be added if the changes will be released in
the future.

.. _Allow edits from maintainers: https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/allowing-changes-to-a-pull-request-branch-created-from-a-fork
.. _GitHub GUI to fork and clone: https://help.github.com/en/github/getting-started-with-github/fork-a-repo
.. _GitHub Workflow: https://guides.github.com/introduction/flow/
.. _JIRA Smart Commit Tags: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _JIRA issues can be referenced: https://confluence.atlassian.com/bitbucket/processing-jira-software-issues-with-smart-commit-messages-298979931.html
.. _SIMP JIRA: https://simp-project.atlassian.net
.. _Squash and Merge: https://github.blog/2016-04-01-squash-your-commits/
.. _Travis-CI: http://travis-ci.org/simp
.. _feature branch: https://www.atlassian.com/git/tutorials/comparing-workflows#feature-branch-workflow
.. _pull request: https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/about-pull-requests
.. _review: https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/reviewing-proposed-changes-in-a-pull-request
.. _simp-doc: https://github.com/simp/simp-doc
.. _squash: https://github.blog/2016-04-01-squash-your-commits/

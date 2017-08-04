Amending Changes to Submitted Pull Requests
===========================================

.. NOTE::

   It is recommended that all SIMP Maintainers use the `hub`_ Git extensions
   and all examples in this section will expect that `hub`_ is installed and
   ready for use.

#. Clone the source repo:

   * ``git clone https://github.com/simp/simp-doc doc``

     .. IMPORTANT::
        We use 'git clone' instead of 'hub clone' so that we can't accidentally
        push to the main SIMP repositories. While we have protected branches
        for the critical components, one wrong command and and life can get
        unpleasant.

#. Pull down the pull request as found on the GitHub GUI. The local branch should
   match the branch in the PR (for example, branch SIMP-XXXX):

   * ``hub checkout https://github.com/simp/simp-doc/pull/9999 SIMP-XXXX``

#. Review the code or make your additional changes

   * ``HACK HACK HACK``

#. Add a new commit with your changes:

   * ``git commit -a -m "I made the docs better"``

#. Set up the target repo for a push:

   * ``hub remote set-url -p jeefberkey``

#. Push your new commit to the feature branch of the **owner** of the pull
   request.  In this example, the owner is `jeefberkey`, and the feature branch
   name is `SIMP-XXXX`: ``hub push jeefberkey HEAD:SIMP-XXXX``

#. The pull request has been updated, and participants have received an email

.. _hub: https://hub.github.com/

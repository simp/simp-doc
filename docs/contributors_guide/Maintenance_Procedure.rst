Maintenance Procedure
=====================

If you're a SIMP maintainer, you're in the right spot! Otherwise, you'll want
to head over to the :ref:`gsg-contributors_guide-contribution_procedure`.

This section exists to document the correct procedure for SIMP Maintainers to update and work with code.

SIMP Maintainer contributions follow the :ref:`gsg-contributors_guide-contribution_procedure`.

.. NOTE::
   It is recommended that all SIMP Maintainers use the `hub`_ Git extensions
   and all examples in this section will expect that `hub`_ is installed and
   ready for use.

Amending Changes to Submitted Pull Requests
-------------------------------------------

#. Clone the source repo:

   * ``git clone https://github.com/simp/simp-doc doc``

     .. IMPORTANT::
        We use 'git clone' instead of 'hub clone' so that we can't accidentally
        push to the main SIMP repositories. While we have protected branches
        for the critical components, one wrong command and and life can get
        unpleasant.

#. Pull down the pull request as found on the GitHub GUI:

   * ``hub checkout https://github.com/simp/pupmod-simp-doc/pull/28``

#. Review the code or make your additional changes

   * ``HACK HACK HACK``

#. Add a new commit with your changes:

   * ``git commit -a -m "I made the docs better"``

#. Set up the target repo for a push:

   * ``git remote set-url -p jeefberkey``

#. Push your new commit to the feature branch of the **owner** of the pull
   request.  In this example, the owner is `jeefberkey`, and the feature branch
   name is `SIMP-1897`: ``hub push jeefberkey SIMP-1897``

#. The pull request has been updated, and participants have received an email

.. _hub: https://hub.github.com/

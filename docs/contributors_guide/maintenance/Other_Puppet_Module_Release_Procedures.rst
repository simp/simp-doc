Other Puppet Module Release Procedures
======================================

This section will describe the release procedures for Puppet module projects
for which SIMP is not the owner.  In these procedures, the SIMP Team will
release RPMs of these projects, using SIMP forks to which **no SIMP
modifications have been made**.  The purpose of these forks is simply to retain
a backup copy of the official repositories in the case that the upstream
repositories are compromised or taken down unexpectedly.

.. NOTE::

   We **highly** recommend that you keep copies of all external repositories as
   a clone in your internal systems if you are deploying via ``r10k`` or Code
   Manager.

.. IMPORTANT::

   If the owner has made unreleased modifications to the project that are
   essential to SIMP *OR* the SIMP Team has an outstanding pull request for the
   project with essential changes, the SIMP Team must take ownership of this
   version of the Puppet module to release it.  This is the only way for SIMP
   to release the modified version to `PuppetForge`_.

.. NOTE::

   You can identify whether a Puppet module is owned by SIMP, by examining the
   outer-most ``name`` entry in the module's ``metadata.json`` file.  The value
   for the ``name`` key will be of the form *<owner>*-*<module name>*.

Pre-Release Checklist
---------------------

For each project, the verification required is to ensure the version desired
has already been released to `GitHub`_ and `PuppetForge`_ by the project owner
and has been used for testing SIMP components in unit (rspec), acceptance
(beaker), and SIMP ISO validation (packer) tests:

#. Verify the version required has an official `GitHub`_ release.

#. Verify the version required has been released to `PuppetForge`_.

#. Verify the ``.fixtures.yml`` and ``metadata.json`` for SIMP
   components that depend upon the component match the version being
   released.

#. Verify the ``Puppetfile.tracking`` file of the ``simp-core``
   project match the version being released.

.. include:: common/Build_RPM_and_Deploy_packagecloud.inc

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com

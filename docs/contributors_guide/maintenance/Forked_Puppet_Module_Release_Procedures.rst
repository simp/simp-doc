Forked Puppet Module Release Procedures
=======================================

This section will describe the release procedures for projects
for which SIMP is not the owner.

.. Important::

   If SIMP has made modifications to a project that have
   not been accepted by the owner **and** are needed by SIMP,
   the SIMP Team must create a SIMP-owned fork of the project.
   This is the only way for SIMP to release the modified version
   to `PuppetForge`_.

.. Note::

   You can identify whether a Puppet module is owned by SIMP, by
   examining the outer-most ``name`` entry in the module's
   ``metadata.json`` file.  The value for the ``name`` key will be
   of the form *<owner>*-*<module name>*.

* `Pre-Release Checklist`_
* `Release to PuppetForge`_
* `Build Signed RPM and Deploy to packagecloud`_

Pre-Release Checklist
---------------------

The only verification step that needs to be done is to ensure that
the version of each project being released is the version that has
been used for testing SIMP components in unit, acceptance, and
SIMP ISO validation tests:

#. Verify the ``.fixtures.yml``, ``metadata.json``, and
   ``build/rpm_metadata/requires`` files for SIMP components that
   depend upon the component match the version being released.

#. Verify the ``Puppetfile.tracking`` and ``Puppetfile.stable`` files
   of the ``simp-core`` project match the version being released.

Release to PuppetForge
----------------------

* If the owner has not released the version we desire to `PuppetForge`_,
  we must request a release from the owner.  

* If the owner will not release the version we need, our only recourse
  is to create a SIMP-owned fork of the project :

  * Fork the GitHub project
  * Change the owner to 'simp' in the ``metadata.json`` file
  * Create/update the ``.travis.yml`` file to allow automated release
    and deploy from an annotated GitHub tag
  * As time permits, make any adjustments necessary to ensure the
    original owner's tests run.
  * Follow SIMP-owned puppet module release procedures.

Build Signed RPM and Deploy to packagecloud
-------------------------------------------

FILL-ME-IN

#. Obtain/build the RPM

   * If the owner has already released an RPM for the version of the component
     SIMP requires, we will use that RPM.

   * Otherwise, we will

     - Obtain the official key
     - Build a signed RPM from the owner-provided GitHub release tag

#. Publish the RPM to `packagecloud`_

.. _GitHub: https://github.com
.. _packagecloud: https://packagecloud.io/simp-project
.. _PuppetForge: https://forge.puppet.com

Tagging and Releasing Components
================================

This section describes the release procedures for SIMP.  The SIMP
Team releases:

* Individual Puppet modules as tar files to `PuppetForge`_
* Individual Puppet modules as signed RPMs to `packagecloud`_
* Ruby gems for building and testing to `RubyGems.org`_
* Miscellaneous projects required to create a SIMP-system ISO as
  signed RPMs to `packagecloud`_

* SIMP-system ISOs to `simp-project.com`_

Each component release is based off of an official GitHub release
tag for an individual GitHub project. In the case of a SIMP ISO, the
component release tag is for the ``simp-core`` project, which
compiles existing, released component RPMs into an ISO.

.. NOTE::

  The SIMP ISO includes RPMs for Puppet modules that are not maintained
  by SIMP. When an official, signed RPM does not already exist for such
  a module (e.g., ``kmod`` Puppet module maintained by ``camptocamp``),
  SIMP builds a signed RPM for that project, using a SIMP-owned fork
  of that project's official GitHub repository.  The RPM is (usually)
  built using an official release tag imported from the upstream
  project.


.. toctree::
   :maxdepth: 1

   SIMP_Puppet_Module_Release_Procedures
   Forked_Puppet_Module_Release_Procedures
   Ruby_Gem_Release_Procedures
   Other_ISO_Related_Project_Release_Procedures
   ISO_Release_Procedures

.. _PuppetForge: https://forge.puppet.com
.. _packagecloud: https://packagecloud.io/simp-project
.. _RubyGems.org: https://rubygems.org/
.. _simp-project.com: http://simp-project.com/ISO/SIMP

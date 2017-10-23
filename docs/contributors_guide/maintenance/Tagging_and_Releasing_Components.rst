Tagging and Releasing Components
================================

.. WARNING::

   ``!!!! THIS SECTION IS UNDER CONSTRUCTION !!!!``

   The intent of this section is to list the current state of the SIMP
   Team's release processes.  Some details that are only handled by a
   few team members are not yet documented (FILL-ME-INs).  Also, since
   these processes are constantly being improved and automated, you can
   expect this section content to evolve as well.


This section describes the release procedures for SIMP.  The SIMP
Team releases:

* Individual Puppet modules as tar files to `PuppetForge`_
* Individual Puppet modules as signed RPMs to `packagecloud`_
* Ruby gems for building and testing to `RubyGems.org`_
* Miscellaneous projects required to create a SIMP-system ISO as
  signed RPMs to `packagecloud`_

* SIMP-system ISOs to `simp-project.com`_

Each component release listed above is based off of an official
`GitHub`_ release the Simp Team has made to a corresponding GitHub
project. In the case of a SIMP ISO, the component release tag is
for the ``simp-core`` project, which compiles existing, released
component RPMs into an ISO.

.. NOTE::

  The SIMP ISO includes RPMs for Puppet modules that are not maintained
  by SIMP. When a suitable signed RPM does not already exist for such
  a module (e.g., ``kmod`` Puppet module maintained by ``camptocamp``),
  SIMP builds a signed RPM for that project, using one of that project's
  GitHub release tags.


.. toctree::
   :maxdepth: 1

   Component_Versioning
   SIMP_Puppet_Module_Release_Procedures
   Other_Puppet_Module_Release_Procedures
   Ruby_Gem_Release_Procedures
   Other_ISO_Related_Project_Release_Procedures
   ISO_Release_Procedures

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com
.. _packagecloud: https://packagecloud.io/simp-project
.. _RubyGems.org: https://rubygems.org/
.. _simp-project.com: http://simp-project.com/ISO/SIMP

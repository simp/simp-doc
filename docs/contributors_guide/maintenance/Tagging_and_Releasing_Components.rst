Tagging and Releasing Components
================================

.. WARNING::

   The intent of this section is to list the current state of the SIMP Team's
   release processes. Since these processes are constantly being improved and
   automated, you can expect this section content to evolve as well and may be
   best served by reading the version from the ``master`` branch of the
   ``simp-doc`` repository.

This section describes the release procedures for SIMP.  The SIMP
Team releases:

* Individual Puppet modules as tar files to `PuppetForge`_
* Individual Puppet modules as signed RPMs to `packagecloud`_ and the `SIMP Archive`_
* Ruby gems for building and testing to `RubyGems.org`_
* SIMP system dependencies as signed RPMs to `packagecloud`_ and the `SIMP Archive`_
* SIMP-system ISOs to `simp-project.com`_

SIMP component releases listed above are based off of an official
`GitHub`_ release the SIMP Team has made to a corresponding `SIMP GitHub`_
project. In the case of a SIMP ISO, the component release tag is
for the ``simp-core`` project, which compiles existing, released
component RPMs and dependencies into an ISO.

.. NOTE::

   The SIMP ISO includes RPMs for Puppet modules that are not maintained by
   SIMP. When a suitable signed RPM does not already exist for such a module
   (e.g., ``kmod`` Puppet module maintained by ``camptocamp``), SIMP builds a
   signed RPM for that project, using one of that project's GitHub release
   tags.

   All modules provided by the SIMP Project, are directly sourced from
   SIMP-controlled repository forks. We do not pull directly from upstream
   sources.

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
.. _RubyGems.org: https://rubygems.org/
.. _SIMP Archive: https://download.simp-project.com/simp/archive/yum
.. _SIMP GitHub: https://github.com/simp
.. _packagecloud: https://packagecloud.io/simp-project
.. _simp-project.com: https://download.simp-project.com/simp/ISO

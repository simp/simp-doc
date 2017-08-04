SIMP-Owned Puppet Module Tag And Release Procedures
===================================================

This section will describe the partially-automated, release procedures
we use for SIMP-owned Puppet modules.

For demonstration purposes, we will be using the
``pupmod-simp-iptables`` project, which uses the ``master`` branch as
its development branch.

.. NOTE::

  You can identify whether a Puppet module is owned by SIMP, by
  examining the outer-most ``name`` entry in the module's
  ``metadata.json`` file.  The value for the ``name`` key will be
  of the form *<owner>*-*<module name>*.

.. toctree::
   :maxdepth: 1

   simp_puppet_module_release_procedures/Pre_Release_Checklist
   simp_puppet_module_release_procedures/Release_GitHub_and_Deploy_PuppetForge
   simp_puppet_module_release_procedures/Build_RPM_and_Deploy_packagecloud

.. _GitHub: https://github.com
.. _PuppetForge: https://forge.puppet.com
.. _packagecloud: https://packagecloud.io/simp-project

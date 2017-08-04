Other ISO-Related Project Release Procedures
============================================

This section will describe the release procedures we use for the
miscellaneous, non-Puppet-module components required to build a
SIMP ISO.  The relevant components include

* ``rubygem-simp-cli``
* ``simp-adapter``
* ``simp-doc``
* ``simp-environment``
* ``simp-gpgkeys``
* ``simp-rsync``
* ``simp-utils``

For demonstration purposes, we will be using the ``simp-adapter``
project, which uses the ``master`` branch as its development branch.

.. toctree::
   :maxdepth: 1

   other_iso_related_project_release_procedures/Pre_Release_Checklist
   other_iso_related_project_release_procedures/Release_GitHub.rst
   other_iso_related_project_release_procedures/Build_RPM_and_Deploy_packagecloud

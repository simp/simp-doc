Build Final ISO and Deploy to simp-project
==========================================

Building the Final ISO
----------------------

The ``build::auto`` Rake task will pull all upstream published RPMs for any
repositories that are listed as part of the target distribution ``yum_repos``
metadata.

The final ISO should be built from published RPMs by running
``SIMP_BUILD_docs=yes rake build:auto[<path to ISOs>]`` on the **same operating
system version for which you are building**.

The ``rpm_docker`` acceptance test has good working examples of this process.

.. IMPORTANT::

   Validate that no RPMs that were included into the ISO were signed by the
   generated development GPG key. If they were, then there is a disconnect
   between the published RPMs and the local component repository versions.

Publishing to simp-project.com
------------------------------

The final ISO should be provided to personnel with upload access to the
``simp/ISO`` directory of ``https://download.simp-project.com`` for final delivery.

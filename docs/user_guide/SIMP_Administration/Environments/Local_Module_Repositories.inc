Local Module Repositories
-------------------------

.. |simp_module_post_rpm_install| image:: ../../images/diagrams/simp_module_post_rpm_install.png
    :alt: PE Code Manager + Control Repository


|simp_module_post_rpm_install|

Each SIMP-packaged Puppet module RPMs will deliver the Puppet module content to
two locations:

1. Files in a directory under ``/usr/share/simp/modules/<module_name>/``

   - These are reference files, and are recognized by package management
     tools like ``rpm -ql`` and ``rpm -V``.

2. A tagged commit to a local Git repository
   at ``/usr/share/simp/git/puppet_modules/<org>-<module_name>/``

   - These repositories and version tags are created automatically during the
     RPM's post-run (using the ``simp_rpm_helper`` script from the
     ``simp-adapter`` package).
   - The repositories support the ``simp puppetfile`` and
     ``simp environment new`` commands.
   - ``<org>-<module_name>`` is pulled directly from the top-level ``name``
     key in the module's ``metadata.json`` file.  This key is guaranteed to
     be unique for all Puppet modules published to :term:`PuppetForge`.

.. NOTE::

   * When a SIMP-packaged Puppet module RPM is upgraded, a new tag is created
     and all previous tags are preserved.  This supports the use of different
     versions of the module in different Puppet environments.
   * When a SIMP-package Puppet module RPM is removed, its local Git repository
     is not removed, automatically, in case that repository is still being used
     in a Puppet environment.
   * The ``master`` branch of a local Puppet module Git repository will contain
     a local transaction history for the RPM of that module, **NOT** a
     copy of the Git history in the public repository for that project.
     Regardless, you should always use a tagged version from a local module
     repository.


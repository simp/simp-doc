.. _ug-sa-env-local-module-repositories:

Local Module Repositories
-------------------------

.. |simp_module_post_rpm_install| image:: ../../images/diagrams/simp_module_post_rpm_install.png
    :alt: PE Code Manager + Control Repository


|simp_module_post_rpm_install|

Each SIMP-packaged Puppet module RPM delivers it Puppet module content to two
locations:

1. Under the directory ``/usr/share/simp/modules/<module_name>/``

  - These are reference files, and are recognized by package management
    tools like ``rpm -ql`` and ``rpm -V``

2. A tagged commit to a local :term:`Git` repository
   at ``/usr/share/simp/git/puppet_modules/<org>-<module_name>/``

  - This repository supports ``r10k puppetfile install`` and the ``simp``
    commands, ":ref:`puppetfile generate <ug-sa-env-cli-simp-puppetfile-generate>`"
    and ":ref:`environment new <ug-sa-env-cli-simp-environment-new>`"
  - The git repo and tags are created automatically during the RPM's %postrun

When a SIMP-packaged Puppet module RPM is:

  - **Installed**, a new tag is created inside a new repository.
  - **Upgraded**, a new tag is created and all previous tags are preserved.
  - **Removed**, its local Git repository will remain  on the filesystem (in
    case any Puppet environments still reference it).

The RPM's %postinstall git management is handled by the ``simp_rpm_helper``
script (from the simp-adapter package).  It derives the repo's name
(``<org>-<module_name>``) from module's ``metadata.json`` file.  The repo name
is guaranteed to be unique across all Puppet modules published to the
:term:`PuppetForge`.

.. NOTE::

   The ``master`` branch in a SIMP RPM-created Puppet module Git repository
   is a **local transaction history** of RPM-generated module tags.

      - It is **NOT** a copy of the module's Git development history from its
        public repository.
      - It does **NOT** track the highest version availabile from the
        repository.
      - The branch head will be whatever the most recent successful RPM
        %postinstall was—even if that was a *downgrade*.

   Therefore: your Puppetfiles **should reference modules by tag** (and never
   ``master``) when deploying from a SIMP RPM-created Puppet module Git
   repository.


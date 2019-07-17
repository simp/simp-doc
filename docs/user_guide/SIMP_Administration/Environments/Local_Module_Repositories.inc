Local Module Repositories
-------------------------

.. |simp_module_post_rpm_install| image:: ../../images/diagrams/simp_module_post_rpm_install.png
    :alt: PE Code Manager + Control Repository


|simp_module_post_rpm_install|

Each SIMP-packaged Puppet module RPMs will deliver the Puppet module content to
two locations:

1. Files in a directory under ``/usr/share/simp/modules/<org>-<module_name>/``

   - These are reference files, and are recognized by package management
     tools like ``rpm -ql`` and ``rpm -V``

2. A tagged commit to a local git repository
   at ``/usr/share/simp/git/puppet_modules/<org>-<module_name>/``

   - These are created automatically during the RPM's post-run (using
     the ``simp_rpm_helper`` script)
   - They support the ``simp puppetfile`` and ``simp environment new`` commands

After a SIMP-packaged Puppet module RPM is installed, the system's
``simp_rpm_helper`` script ensures that a local bare git repository exists for the
module, and contains a commit of the modules files (tagged with the module'
version).


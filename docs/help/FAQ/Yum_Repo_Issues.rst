YUM Repo Issues
===============

This FAQ covers various issues that relate to YUM repositories and SIMP
systems.

Global repo_gpgcheck=1
----------------------

.. WARNING::

   Disabling ``repo_gpgcheck`` should only be done against repositories that
   you ultimately trust. Doing otherwise could allow untrusted repository
   maintainers to compromise your system.

   More information can be found on this
   `SCAP Security Guide Mailing List Thread`_.

The :term:`DISA STIG` requires that the ``repo_gpgcheck`` setting be set to
``1`` globally on :term:`EL` systems.

When SIMP is set into STIG enforcing mode using the :term:`SIMP Compliance Engine`,
it will automatically flip the global ``repo_gpgcheck`` setting to ``1`` in
accordance with the STIG.

Unfortunately, this will break repositories such as :term:`EPEL` and the
commercial :term:`RHEL` repositories.

To mitigate this, you can modify the global settings by changing the
appropriate value in the ``yum::config_options`` Hash. However, doing this will
show as a finding during STIG compliance scans.

Alternatively, you can update each repository that is having issues and disable
GPG checking for just that repository using the `yumrepo puppet resource`_.

.. _SCAP Security Guide Mailing List Thread: https://lists.fedorahosted.org/archives/list/scap-security-guide@lists.fedorahosted.org/thread/ZDKOEZN3BRXRED6K3ACYEJUXRPDTPJWW/
.. _yumrepo puppet resource: https://puppet.com/docs/puppet/5.5/types/yumrepo.html

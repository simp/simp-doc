.. _changelog-latest:
.. _changelog-6.5.0:

SIMP Community Edition (CE) 6.5.0-Alpha
=======================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak

This release is known to work with:

  * CentOS 6.10 x86_64
  * CentOS 7.0 2003 x86_64
  * CentOS 8.2 2004 x86_64 (client systems only)
  * OEL 6.10 x86_64
  * OEL 7.8 x86_64
  * OEL 8.2 x86_64 (client systems only)
  * RHEL 6.10 x86_64
  * RHEL 7.8 x86_64
  * RHEL 8.2 x86_64 (client systems only)

OS compatibility is subject to the following limitations:

* EL8 support is currently limited to Puppet agents—this release does **not**
  support managing an EL8 SIMP Server or installing SIMP from an EL8 ISO.

  * EL8 management is supported by all Puppet modules provided as core
    dependencies of the ``simp`` RPM.
  * Not all modules provided by the ``simp-extras`` RPM have been updated
    for EL8.
  * EL8 updates to the remaining ``simp-extras`` modules will be phased in over
    future SIMP releases.
  * Support for managing an EL8 SIMP/Puppet server and installing from
    EL8 ISOs) will be provided in a later SIMP release (SIMP 6.6.0).

* Support for managing EL6 system is drawing down.

  * EL6 maintenance support is EOL for both RHEL 6 and CentOS 6, and upstream
    vendor support will end on 30 November 2020.
  * New Puppet modules may not support EL6.
  * Some optional Puppet modules (provided by the ``simp-extras`` RPM package)
    no longer support EL6. In particular, this affects ``simp-autofs``,
    ``simp-nfs``, and ``simp-simp_nfs``.  If you need those capabilities on
    EL6, use earlier versions of these modules in EL6-specific Puppet
    environments.


Breaking Changes
----------------

Deprecated Puppet 3 API Functions Removed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All SIMP-provided Puppet 3 API functions (originally deprecated in SIMP 6.4.0)
have now been removed in order to fully support Puppet 6. The affected
functions and their replacements (when available) are listed in sub-sections
below.

Puppet 3 Functions Removed from simp-compliance_markup
""""""""""""""""""""""""""""""""""""""""""""""""""""""

+-----------------------+---------------------------------------+---------------------------------+
| Puppet 3 API Function | Replacement                           | Replacement Source              |
+=======================+=======================================+=================================+
| ``compliance_map``    | ``compliance_markup::compliance_map`` | simp-compliance_markup >= 3.0.0 |
+-----------------------+---------------------------------------+---------------------------------+

Puppet 3 Functions Removed from simp-simp_apache
""""""""""""""""""""""""""""""""""""""""""""""""

+--------------------------+---------------------------------------+---------------------------+
| Puppet 3 API Function    | Replacement                           | Replacement Source        |
+==========================+=======================================+===========================+
| ``apache_auth``          | ``simp_apache::auth``                 | simp-simp_apache >= 6.0.1 |
+--------------------------+---------------------------------------+---------------------------+
| ``apache_limits``        | ``simp_apache::limits``               | simp-simp_apache >= 6.0.1 |
+--------------------------+---------------------------------------+---------------------------+
| ``munge_httpd_networks`` | ``simp_apache::munge_httpd_networks`` | simp-simp_apache >= 6.0.1 |
+--------------------------+---------------------------------------+---------------------------+

Puppet 3 Functions Removed from simp-simplib
""""""""""""""""""""""""""""""""""""""""""""

.. IMPORTANT::

   Most (but not all) of the Puppet 3 API functions in the table below have
   replacements. If any function that has been removed without a replacement is
   essential to you, let us know by submitting a feature request at
   https://simp-project.atlassian.net.

+------------------------------+--------------------------------------------+-------------------------------+
| Puppet 3 API Function        | Replacement                                | Replacement Source            |
+==============================+============================================+===============================+
| ``array_include``            | Puppet language `in`_ operator *or* Puppet | Puppet >= 5.2.0               |
|                              | built-in functions ``any`` or ``all``      |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``array_size``               | Puppet built-in function ``length``        | Puppet >= 5.5.0               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``array_union``              | Puppet language `+ (concatenation)`_       | Puppet >= 5.0.0               |
|                              | operator, combined with Puppet built-in    |                               |
|                              | function ``unique``                        |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``bracketize``               | ``simplib::bracketize``                    | simp-simplib >= 3.15.0        |
+------------------------------+--------------------------------------------+-------------------------------+
| ``generate_reboot_msg``      | None                                       | N/A                           |
+------------------------------+--------------------------------------------+-------------------------------+
| ``get_ports``                | None                                       | N/A                           |
+------------------------------+--------------------------------------------+-------------------------------+
| ``h2n``                      | None                                       | N/A                           |
+------------------------------+--------------------------------------------+-------------------------------+
| ``host_is_me``               | ``simplib::host_is_me``                    | simp-simplib >= 3.15.0        |
+------------------------------+--------------------------------------------+-------------------------------+
| ``inspect``                  | ``simplib::inspect``                       | simp-simplib >= 3.3.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``ipaddresses``              | ``simplib::ipaddresses``                   | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``ip_is_me``                 | ``simplib::host_is_me`` (checks hostnames  | simp-simplib >= 3.15.0        |
|                              | and IP addresses)                          |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``ip_to_cron``               | ``simplib::ip_to_cron``                    | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``join_mount_opts``          | ``simplib::join_mount_opts``               | simp-simplib >= 3.8.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``localuser``                | None                                       | N/A                           |
+------------------------------+--------------------------------------------+-------------------------------+
| ``mapval``                   | None                                       | N/A                           |
+------------------------------+--------------------------------------------+-------------------------------+
| ``nets2cidr``                | ``simplib::nets2cidr``                     | simp-simplib >= 3.7.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``nets2ddq``                 | ``simplib::nets2ddq``                      | simp-simplib >= 3.8.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``parse_hosts``              | ``simplib::parse_hosts``                   | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``passgen``                  | ``simplib::passgen``                       | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``rand_cron``                | ``simplib::rand_cron``                     | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``simp_version``             | ``simplib::simp_version``                  | simp-simplib >= 3.15.0        |
+------------------------------+--------------------------------------------+-------------------------------+
| ``simplib_deprecation``      | ``simplib::deprecation``                   | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``slice_array``              | Puppet built-in ``slice``                  | Puppet >= 4.0.0               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``strip_ports``              | ``simplib::strip_ports``                   | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``to_integer``               | Puppet built-in ``Integer`` *or*           | ``Integer``: Puppet >= 4.0.0; |
|                              | ``simplib::to_integer``                    | ``simplib::to_integer``:      |
|                              |                                            | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``to_string``                | Puppet built-in ``String``                 | ``String``: Puppet >= 4.0.0;  |
|                              | *or* ``simplib::to_string``                | ``simplib::to_string``:       |
|                              |                                            | simp-simplib >= 3.5.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_array_member``    | ``simplib::validate_array_member``         | simp-simplib >= 3.8.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_array_of_hashes`` | Use a custom Puppet data type              | Puppet >= 4.0.0               |
|                              | such as ``Array[Hash]``                    |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_between``         | Puppet data types ``Integer`` or ``Float`` | simp-simplib >= 3.8.0         |
|                              |  *or* ``simplib::validate_between``        |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_bool_simp``       | Use Puppet ``Boolean`` data type           | Puppet: >= 4.0.0;             |
|                              | *or* ``simplib::validate_bool``            | simp-simplib >= 3.8.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_deep_hash``       | ``simplib::validate_deep_hash``            | simp-simplib >= 3.8.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_float``           | Use Puppet ``Float`` data type             | Puppet: >= 4.0.0;             |
|                              | *or* a check using ``is_float``            | ``is_float``:                 |
|                              | from ``puppetlabs-stdlib``                 | puppetlabs-stdlib >= 2.2.0    |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_macaddress``      | Use ``Simplib::Macaddress`` data           | simp-simplib >= 3.7.0         |
|                              | type                                       |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_net_list``        | Use ``Simplib::Netlist`` data              | simp-simplib >= 3.5.0         |
|                              | type *or*                                  |                               |
|                              | ``simplib::validate_net_list``             |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_port``            | Use ``Simplib::Port`` data type            | simp-simplib >= 3.5.0         |
|                              | *or*                                       |                               |
|                              | ``simplib::validate_net_list``             |                               |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_re_array``        | ``simplib::validate_re_array``             | simp-simplib >= 3.7.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_sysctl_value``    | ``simplib::validate_sysctl_value``         | simp-simplib >= 3.7.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_umask``           | Use ``Simplib::Umask`` data type           | simp-simplib >= 3.7.0         |
+------------------------------+--------------------------------------------+-------------------------------+
| ``validate_uri_list``        | ``simplib::validate_sysctl_value``         | simp-simplib >= 3.7.0         |
+------------------------------+--------------------------------------------+-------------------------------+

.. _in:                https://puppet.com/docs/puppet/6.18/lang_expressions.html#in
.. _+ (concatenation): https://puppet.com/docs/puppet/6.18/lang_expressions.html#+-(concatenation)

simp-ssh Removed Functions
""""""""""""""""""""""""""

+----------------------------+-----------------------------+--------------------+
| Puppet 3 API Function      | Replacement                 | Replacement Source |
+============================+=============================+====================+
| ``ssh_autokey``            | ``ssh::autokey``            | simp-ssh >= 6.2.0  |
+----------------------------+-----------------------------+--------------------+
| ``ssh_global_known_hosts`` | ``ssh::global_known_hosts`` | simp-ssh >= 6.2.0  |
+----------------------------+-----------------------------+--------------------+

Primary API Changed in Optional Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following SIMP modules from the ``simp-extras`` RPM have had breaking API
changes:

* ``simp-autofs``
* ``simp-nfs``
* ``simp-simp_nfs``

The specific changes made are described in detail the New Features section.

.. _changelog-6.5.0-el6-support-dropped-from-some-optional-puppet-modules:

EL6 Support Dropped from Some (Optional) Puppet Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following optional SIMP modules have dropped support for EL6:

* ``simp-autofs``
* ``simp-nfs``
* ``simp-simp_nfs``

If you need EL6 for a client node, place it in an environment with
older versions of the appropriate modules.


Significant Updates
-------------------

EL8 SIMP Client Node Support
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This release provides support for EL8 clients.
This includes all (appropriate) Puppet modules provided by the ``simp`` RPM, and
a subset of the Puppet modules provided by the ``simp-extras`` RPM.

* The remaining changes required for an EL8 SIMP server and ISO will be
  available in the next SIMP minor release.
* EL8 updates to the remaining, optional, Puppet modules will be phased in
  over future SIMP releases. This includes the following SIMP modules:

  * ``simp-gdm``
  * ``simp-gnome``
  * ``simp_hirs_provisioner``
  * ``simp-mate``
  * ``simp-simp_gitlab``
  * ``simp-simp_pki_service``
  * ``simp-simp_snmpd``
  * ``simp-tuned``
  * ``simp-vnc``
  * ``simp-x2go``

Full Puppet 6 Support and Puppet 6 Default Components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All SIMP Puppet modules now work with both Puppet 5 and Puppet 6, and the SIMP-6.5.0
ISOs deliver Puppet 6 application RPMs.

firewalld Support
^^^^^^^^^^^^^^^^^

As of SIMP 6.5.0, preliminary ``firewalld`` support within the SIMP ecosystem
is now available.

* **New simp-simp_firewalld module**: SIMP now includes ``simp-simp_firewalld``
  which provides a profile class and defined type to manage the system's
  ``firewalld`` with "safe" defaults and safety checks for ``firewalld`` rules.
* **firewalld support in simp-iptables for backward compatibility**:  The
  ``simp-iptables`` module has preliminary support for acting as a pass-through
  to various ``firewalld`` capabilities using the ``simp-simp_firewalld``
  module.

  * To enable ``firewalld`` mode on supported operating systems, simply set
    ``iptables::use_firewalld`` to ``true`` via Hiera.
  * EL8 systems enable ``firewalld`` mode by default.
  * Use of any of the ``iptables::listen::*`` defined types will work
    seamlessly in ``firewalld`` mode, as long as IP addresses are used
    in their ``trusted_net`` parameters.
  * Direct calls to ``iptables::rule`` in ``firewalld`` mode will emit
    a warning notification that directs the user to convert their rules to
    ``simp_iptables::rule`` types.

.. IMPORTANT::

   Be aware that ``firewalld`` rules do not support hostnames. IP addresses
   must be used. This may impact your use of any manifests that contain
   ``iptables::listen::*`` resources, including resources from some SIMP
   modules. You will have to change hostnames to IP addresses for the
   affected resources when using ``firewalld``.

Optional Dependency Handling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In SIMP 6.5.0, optional dependency handling has been integrated into ~20
additional SIMP Puppet modules. These modules explicitly identify optional,
dependent modules, all while providing safeguards to ensure the user is
notified of any such missing dependencies at compilation time. This feature
allows the user to minimize installation of unused modules in an environment,
when the user is not using SIMP to manage specific capabilities.

Key details about this feature are as follows:

* Optional module dependencies are indicated in the *metadata.json* file using
  an 'optional_dependencies' key within a 'simp' key.  For example,
  `puppet-rsyslog's metadata.json <https://github.com/simp/pupmod-simp-rsyslog/blob/7.6.2/metadata.json>`_.
* The user has complete control over installation of the optional dependency
  modules.  These dependencies will not be installed automatically when
  the module using them is installed via ``puppet module install``.
* Modules that use this feature will fail manifest compilation, if
  the user enables the optional capabilities, but the optional dependencies
  required to implement that capability are not installed in the environment.

Dependent Module Updates
^^^^^^^^^^^^^^^^^^^^^^^^

SIMP updated as many dependent modules as possible. This included major version
bumps for several of the dependent modules. These changes did not have
a significant impact on the SIMP infrastructure. The dependency version bumps
did, however, require some of the SIMP modules to update their respective
``metadata.json`` files.  These metadata changes, in turn, required SIMP module
version updates.


Security Announcements
----------------------

SIMP 6.5.0 Added mitigations for the following CVEs:

* CVE-2020-7942
* CVE-2019-14287
* CVE-2019-6477

RPM Updates
-----------

Puppet RPMs
^^^^^^^^^^^

The following Puppet RPMs are packaged with the SIMP 6.5.0 ISOs:

+----------------------+----------+
| Package              | Version  |
+======================+==========+
| ``puppet-agent``     | 6.18.0-1 |
+----------------------+----------+
| ``puppet-bolt``      | 2.29.0-1 |
+----------------------+----------+
| ``puppetdb``         | 6.12.0-1 |
+----------------------+----------+
| ``puppetdb-termini`` | 6.12.0-1 |
+----------------------+----------+
| ``puppetserver``     | 6.13.0-1 |
+----------------------+----------+

.. WARNING::

   You do **NOT** need to update your version of Puppet from 5.X to use the
   modules supplied with this version of SIMP.

   If you decide to update from 5.X, back up your server and test the upgrade
   carefully.

Removed Puppet Modules
----------------------

Unused Augeasproviders Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following packages for unused Augeasproviders Puppet modules and one
dependency were removed from the SIMP ISOs:

* ``pupmod-herculesteam-augeasproviders_apache``
* ``pupmod-herculesteam-augeasproviders_mounttab``
* ``pupmod-herculesteam-augeasproviders_nagios``
* ``pupmod-herculesteam-augeasproviders_pam``
* ``pupmod-herculesteam-augeasproviders_postgresql``
* ``pupmod-herculesteam-augeasproviders_puppet``
* ``pupmod-herculesteam-augeasproviders_shellvar``
* ``pupmod-puppetlabs-mount_providers``

Docker Modules
^^^^^^^^^^^^^^

The packages for the following Docker Puppet modules have been permanently
removed from the SIMP ISOs, because SIMP is moving towards ``podman`` support
over ``docker``.

* ``pupmod-puppetlabs-docker``
* ``pupmod-simp-simp_docker``

Out-of-date Modules
^^^^^^^^^^^^^^^^^^^

The packages for the following SIMP profile Puppet modules and one dependent
module were temporarily removed from SIMP 6.5.0 ISOs, because they were not
able to be appropriately updated in time for the release:

* ``pupmod-puppet-gitlab``
* ``pupmod-simp-simp_gitlab``
* ``pupmod-simp-simp_snmpd``

These modules are expected to be updated in future SIMP releases.

pupmod-simp-journald
^^^^^^^^^^^^^^^^^^^^

The pupmod-simp-journald package has been removed from SIMP ISOs, because
the functionality the ``simp-journald`` module provided is now provided by
the ``camptocamp-systemd`` module.  If you used ``simp-journald``, you will
need to update your manifests to use ``camptocamp-systemd``.


Fixed Bugs
----------

pupmod-simp-aide
^^^^^^^^^^^^^^^^

* Fixed a bug in Compliance Engine data.

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the auditd service was managed when the kernel was
  not enforcing auditing.
* Fixed a bug in which the facts were not properly confined.
* Fixed a bug in which ``/etc/audit/audit.rules.prev`` caused unnecessary
  flapping.
* Fixed regex substitution for bad path characters.
* Added missing ``herculesteam-augeasproviders_grub`` module dependency.

pupmod-simp-dconf
^^^^^^^^^^^^^^^^^

* Fixed a bug in ``ensure = absent`` in ``dconf::settings``.

pupmod-simp-compliance_markup
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Fixed merging bugs introduced in interim versions of the module.
* Fixed a regression introduced in interim versions of the module in which
  compliance reports were missing 'controls', 'identifiers', and 'oval-ids'.

pupmod-simp-freeradius
^^^^^^^^^^^^^^^^^^^^^^

* Fixed missing 'group_filter' option in LDAP.


pupmod-simp-iptables
^^^^^^^^^^^^^^^^^^^^

* Fixed bugs in iptables rule address normalization:

  * Ensured that all addresses are normalized when rules are processed.
  * Removed nested looped rule normalization of addresses since it is no longer
    required.
  * Fixed ``normalize_addresses()`` so that it simply grabs the netmask if
    present and slaps on the appropriate one if not.

* Fixed some bugs in the 'munge' portions of the native types.

pupmod-simp-libvirt
^^^^^^^^^^^^^^^^^^^

* Fixed issues with module data.

pupmod-simp-logrotate
^^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the 'size' parameter in the global logrotate
  configuration file was specified more than once.

pupmod-simp-network
^^^^^^^^^^^^^^^^^^^^^

* Fix a bug where both the legacy network and NetworkManager were activated in
  all cases.

pupmod-simp-nfs
^^^^^^^^^^^^^^^

* Fixed a bug in which IPv6 ``::1`` network entries were not being created in
  ``/etc/exports``.  This could cause connections over stunnel to fail under
  certain conditions.

* ``rpc.rquotad`` service configuration was erroneously written to
  ``/etc/sysconfig/nfs`` for EL7. It is now written to the correct file,
  ``/etc/sysconfig/rpc-rquotad``.
* Fixed idmapd-related bugs:

  * ``idmapd`` was erroneously only enabled when NFSv3 was allowed. ``idmapd``
    is an NFSv4 service.
  * The idmapd client was not configured to use nfsidmap.  An nfsidmap entry
    has now been added to ``/etc/request-key.conf``.

* Fixed bugs in which bidirectional communication for NFSv3 was not properly
  configured.

  * NFSv3 lockd ports on the NFS client were not explicitly configured and
    thus not allowed through the firewall.  This would have affected file
    locking using NLM.
  * ``rpcbind``, ``statd``, and ``lockd`` service names were not allowed by TCP
    wrappers for the NFS client. This would have affected server to client
    NFSv3 NSM and NLM protocol messages over TCP.

* Fixed bugs in mount options

  * Previously used the deprecated ``nfs4`` fstype.  This has been replaced with
    the ``nfs`` fstype and use of the ``nfsvers`` option to specify the version of
    NFS to use.
  * The mount option ``proto`` is now set to ``tcp`` when stunnel is enabled.

* Fixed a bug with a duplicate exec resource in ``nfs::client::mount`` when
  stunnel was enabled.

* Fixed erroneous server-only/client-only configuration that appeared to be
  able to be set independently for the NFS client and NFS server on the same
  node, but because of shared services, actually applied to the node as a
  whole.

  * Removed ``nfs::client::firewall`` and ``nfs::server::firewall``. Use
    ``nfs::firewall`` instead.
  * Removed ``nfs::server::tcpwrappers``. Use ``nfs::tcpwrappers`` instead.
  * Removed ``nfs::server::nfsv3``, ``nfs::server::lockd_arg``,
    ``nfs::server::statdarg``, ``nfs::server::statd_ha_callout``,
    ``nfs::server::rpcgssdargs``, and ``nfs::server::rpcsvcgssdargs``. Use
    appropriate parameters in the ``nfs`` class instead.

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Fixed a bug in which a local user password could not be set.

  * Moved the ``pam_unix.so`` check before the ``pam_sss.so`` check in the
    password section of the auth files otherwise it returns an "authentication
    token manipulation" error and local passwords cannot be changed.

pupmod-simp-polkit
^^^^^^^^^^^^^^^^^^

* Fixed issue with ``basic_policy`` template that resulted in malformed rules.

pupmod-simp-pupmod
^^^^^^^^^^^^^^^^^^

* Fixed a bug on EL6 nodes in which setting ``pupmod::master::generate_types``
  to ``false`` caused the catalog compilation to fail.
* Fixed a bug in puppetserver configuration in which the
  ``profiler-output-file`` parameter was incorrectly specified as
  ``profiling-output-file``.
* Fixed a bug in managing group ownership of ``puppet.conf`` when using
  Puppet Enterprise.

  * Ensured that ``pupmod::pass_two`` does not conflict with the internal
    :term:`PE` configuration code for group ownership of ``puppet.conf``.

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

* Fixed a bug where the ``IncludeConfig`` directive for ``/etc/rsyslog.d``
  allowed more than just ``.conf`` files to be parsed.

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* Removed the broken ``tasks/`` directory.

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

* Fixed bugs in the ``grub_version`` and ``init_systems`` facts.
* Fixed the ``simplib__auditd`` fact so that it detects the state of the
  running auditd process.
* Fixed ``Simplib::Systemd::ServiceName`` to accept instance services.
* Fixed an issue in the ``simplib__sshd_config`` fact that would cause the
  daemon to start on an EL6 system that did not already have it running.
* Fixed a bug in which ``simplib__firewalls`` fact was not properly confined
  and would trigger on Windows+  systems.
* Fixed an issue in ``simplib::ip::family_hash`` where the 'unknown' entries
  were not properly populated.
* Fixed bug in which ``simplib::simp_version`` did not work on Windows.
* Fixed "uninitialized constant" error with the ``reboot_notify`` custom type.

pupmod-simp-simp_options
^^^^^^^^^^^^^^^^^^^^^^^^

* Fixed :term:`PE` detection in ``simp_options::puppet::server_distribution``.

pupmod-simp-stunnel
^^^^^^^^^^^^^^^^^^^

* Added the ``stunnel::instance_purge`` class to remedy the 'floating services'
  issue.

pupmod-simp-tftpboot
^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the internal rsync operation did not match the
  documentation.
* Fixed a bug in which the internal rsync operation would flip permissions
  each puppet agent run.
* Fixed a manifest ordering issue.

pupmod-simp-tlog
^^^^^^^^^^^^^^^^

* Fixed a bug in the tcsh template.
* Added a workaround to scripts in ``/etc/profile`` to handle a bug in tlog
  that would prevent logins if the system hostname could not be found.

pupmod-simp-xinetd
^^^^^^^^^^^^^^^^^^

* Removed 'TRAFFIC' from the default ``log_on_success`` list since it may cause
  information leakage and is not supported by all service types.

rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Fixed a bug in ``simp environment new`` in which the actual failure
  messages from a failed ``setfacl --restore`` execution were not logged.
* Fixed a bug where ``simp config --dry-run`` would prompt the user to apply
  actions instead of skipping them and then writing the
  ``~/.simp/simp_conf.yaml`` file.

  * Users would answer 'no' to the unexpected apply query and then ``simp config``
    would only persist the answers to the interim answers file
    (``~/.simp/.simp_conf.yaml``).

* Fixed Puppet Enterprise support for ``simp config`` and ``simp bootstrap``.

  * Fixed a fact-loading bug that prevented the :term:`PE` fact ('is_pe') from
    being available.
  * Hardened PE-detection logic for cases in which the 'is_pe' fact is not
    yet available during ``simp config``.
  * Added support for SIMP server template Hiera data that is PE-specific.
  * Fixed a bug in which the module paths containing PE modules were not
    excluded when ``simp config`` checked for modules in the 'production'
    Puppet environment. This forced the user to remove the skeleton
    'production' environment installed by the puppet-agent RPM, in order to get
    ``simp config`` to run on a freshly installed PE system.

simp-environment-skeleton
^^^^^^^^^^^^^^^^^^^^^^^^^

* When running FakeCA in batch mode, do not request input from the user.
* Fixed a bug in which some non-script files were installed with executable
  permissions.

simp-utils
^^^^^^^^^^

* Fixed minor bugs in ``unpack_dvd``.


New Features
------------

pupmod-simp-aide
^^^^^^^^^^^^^^^^

* Updated the EL8 ciphers to be safe on FIPS systems by default.
* Removed overrides for ``aide::aliases`` on EL8 since it works properly in FIPS
  mode.
* Automatically add ``@@include`` lines to ``aide.conf``.
  Previously, when declaring ``aide::rule`` resources, it was also
  necessary to add the rule name to the ``aide::rules`` array.
* Moved the default rules to data in modules.

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^

* Allow ``auditd::space_left`` and ``auditd::admin_space_left`` to accept
  percentages on supported versions.
* Added ``INCREMENTAL_ASYNC`` to possible values for ``auditd::flush``.
* Added a ``built_in`` audit profile to the subsystem that provides ability
  to include and manage sample rulesets to be compiled into active rules.
* Ensured that kmod is audited in all STIG modes on EL7+.
* Allow users to knockout entries from arrays specified in Hiera.
* Added rules based on best practices mostly pulled from
  ``/usr/share/doc/auditd``:

  * Audit 32 bit operations on 64 bit systems
  * Audit calls to the auditd CLI commands
  * Audit IPv4 and IPv6 inbound connections
  * Optionally audit IPv4 and IPv6 outbound connections
  * Audit suspicious applications
  * Audit systemd
  * Audit the auditd configuration space
  * Ignore time daemon logs (clutter)
  * Ignore ``CRYPTO_KEY_USER`` logs (clutter)
  * Add ability to set the ``backlog_wait_time``
  * Set ``loginuid_immutable``

* Set defaults for syslog parameters if auditd version is unknown.
* Added a fact that determines the major version of auditd that is running
  on the system, ``auditd_major_version``.  This is used in hiera.yaml hierarchy
  to add module data specific to the versions.
* Added support for auditd v3.0 which is used by RedHat 8.  Most of the changes
  in auditd v3.0 were related to how the plugins are handled but there
  are a few new parameters added to ``auditd.conf``. They are set to their
  defaults according to man page of ``auditd.conf``.

  * Auditd V3.0 moved the handling of plugins into auditd from audispd.
    The following changes were made to accommodate that:

    * To make sure the parameters used to handle plugins where defined in
      one place no matter what version of auditd was used, they were moved to
      ``init.pp`` and referenced from there by the audisp manifest.
      For backwards compatibility, they remain in ``audisp.conf`` and are aliased
      in the hiera module data.
    * For backwards compatibility ``auditd::syslog`` remains defaulting to the
      value of ``simp_options::syslog`` although the two are not really the same
      thing. You might want to review this setting and set ``auditd::syslog`` to
      a setting that is appropriate for your system.

      * To enable auditd logging to syslog set the following in hiera

        .. code-block:: yaml

           ---
           auditd::syslog: true
           auditd::config::audisp::syslog::enable: true.
           # The drop_audit_logs is still there for backwards compatibility and
           # needs to be disabled.
           auditd::config::audisp::syslog::drop_audit_logs: false

      * To stop auditd logging to syslog set the following in hiera

        .. code-block:: yaml

           ---
           auditd::syslog: true
           auditd::config::plugins::syslog::enable: false.

      * Setting ``auditd::syslog`` to false will stop Puppet from managing the
        ``syslog.conf``, it will not disable auditd logging to syslog.
        Disable the syslog plugin as described above.

    * The settings for ``syslog.conf`` were updated to work for new and old
      versions of auditd.
    * Added installation of audisp-syslog package when using auditd v3.

* Added rules to monitor ``/usr/share/selinux``.

pupmod-simp-autofs
^^^^^^^^^^^^^^^^^^

This module was extensively refactored. Please read the updated README to
understand the current usage.  Notable feature/API changes:

* Updated autofs service configuration to use ``/etc/autofs.conf`` in
  addition to ``/etc/sysconfig/autofs``.
* Updated autofs.master to load content from ``/etc/auto.master.simp.d/``
  and ``/etc/auto.master.d/`` in lieu of specifying map entries directly.

  * auto.master entries are now written to files in ``/etc/auto.master.simp.d``,
    a directory fully managed by this module.
  * ``/etc/auto.master.d`` is left unmanaged by Puppet.
  * Auto-converts from old maps directory to current maps directory and
    emits a warning. This is to help the 90% of the users who aren't doing
    anything special with this module.

* Added a ``autofs::map`` defined type that allows the user to specify all
  the parameters for a ``file`` map in one place.  This resource will
  generate the appropriate resources to create both the auto.master entry
  file and the map file.
* Added ``autofs::masterfile`` defined type to replace deprecated
  ``autofs::master::map``.

  * ``autofs::masterfile`` creates an auto.master entry file in
    ``autofs::master_conf_dir``.
  * Unlike ``autofs::map::master``, ``autofs::masterfile`` does not have
    a ``content`` parameter, because a user can simply use a file resource
    to specify a custom auto.master entry file.

* Added ``autofs::mapfile`` defined type to replace deprecated
  ``autofs::master::entry``.

  * ``autofs::mapfile`` creates a mapfile for a direct mapping or one or
    more indirect mappings.
  * Unlike ``autofs::master::entry``, it does not have duplicate resource
    naming problems (wildcard or otherwise).

* ``autofs`` class changes

  * Added the following new autofs service configuration parameters:

    * ``master_wait``
    * ``mount_verbose``
    * ``mount_nfs_default_protocol``
    * ``force_standard_program_map_env``
    * ``use_hostname_for_mounts``
    * ``disable_not_found_message``
    * ``sss_master_map_wait``
    * ``use_mount_request_log_id``
    * ``auth_conf_file``
    * ``custom_autofs_conf_options``

  * Added ``master_conf_dir`` and ``master_include_dirs`` parameters to allow
    users to specify directories containing auto.master entry files.
  * Added ``maps_dir`` to specify the location of SIMP-managed maps and
    changed the directory name from ``/etc/autofs`` to ``/etc/autofs.maps.simp.d``
    for clarity.
  * Added ``maps`` to allow users to specify 'file' type maps in Hiera data.

    * Each map specifies the contents of an autofs master entry file and
      its mapping file.

  * Renamed ``options`` to ``automount_options`` for clarity.
  * Renamed ``use_misc_device`` to ``automount_use_misc_device`` for clarity.
  * Removed ``autofs::master_map_name``.

    * This parameter is not exposed in ``/etc/autofs.conf`` and does not look
      like it is intended to be changed.

  * Changed permissions of ``/etc/auto.master`` and ``/etc/sysconfig/autofs``
    to match those of the delivered RPM.

* ``autofs::ldap_auth`` class changes

  * ``autofs::ldap_auth`` is now a private class to ensure the name of the
    configuration file created by this class matches the 'auth_conf_file'
    setting in ``/etc/autofs.conf``.
  * Added ``encoded_secret`` optional parameter.  This parameter takes
    precedence when both ``secret`` and ``encoded_secret`` parameters are
    specified

* ``autofs::map::master`` has been deprecated by ``autofs::map`` or
  ``autofs::masterfile``.  Its behavior has changed from writing a section
  of ``/etc/auto.master`` to writing an autofs master entry file in
  ``autofs::master_conf_dir``.
* ``autofs::map::entry`` has been deprecated by ``autofs::map`` or
  ``autofs::mapfile``.  Its behavior has changed from writing a file in
  ``/etc/autofs`` to writing a file in ``autofs::maps_dir``.

pupmod-simp-clamav
^^^^^^^^^^^^^^^^^^

* Updated documentation to clarify what ``simp_options::clamav`` actually does
  and to note that clamav was removed from the SIMP's default class list
  in SIMP 6.5.
* Set the default for ``clamav::set_schedule::enable`` to lookup
  ``clamav::enable``, so that the class will remove the clamav schedule if
  clamav is disabled.
* Disable rsync pulls by default.

pupmod-simp-compliance_markup
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Deep merge hash values in the Hiera backend.
* Improved confinement

  * Added support for confinement in 'profiles', 'controls' and 'ces'
    (as well as 'checks').
  * Added support for arrays of potential matches in confinement blocks.
  * Added support for structured facts in confinement.
  * Updated confinement logic to ensure that all possibilities are collected.
  * Apply confinement before merging values.

* Improved performance:

  * Reduced the amount of data passed around in the Hiera backend.
  * Ensured that the Hiera backend recurses as little as possible.
  * Removed useless loops in ``list_puppet_params()``.

* Improved error handling and debugging:

  * Ignore undefined 'ces' when correlating checks and profiles.
  * Raise errors on malformed data.
  * Added debugging logs to enforcement logic.

* Removed all support for v1 data since it was experimental and removed in
  3.0.0.

* Load data from the ``compliance_markup::compliance_map`` Hiera key after
  compliance profiles in modules to allow for profile tailoring via Hiera.
  This means that uses may now override all settings from the underlying
  compliance maps across all modules to fit their environment specifics.

pupmod-simp-cron
^^^^^^^^^^^^^^^^

* Manage cron packages by default.

pupmod-simp-crypto_policy
^^^^^^^^^^^^^^^^^^^^^^^^^

This is a new module to manage, and provide information about, the system-wide
crypto policies.

pupmod-simp-dconf
^^^^^^^^^^^^^^^^^

* Allow users to set custom settings via Hiera.

pupmod-simp-deferred_resources
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Remove ``ftp`` and ``games`` users and groups when enforcing STIG compliance.

pupmod-simp-dhcp
^^^^^^^^^^^^^^^^

* Made use of rsync optional (enabled by default for backwards compatibility).
* Added support for passing in a full ``dhcpd.conf`` entry.
* Ensured that the SELinux user and type are set for the configuration files.
* Switched to using ``iptables::listen::udp`` for firewalld compatibility.

pupmod-simp-fips
^^^^^^^^^^^^^^^^

* Ensured that EL8 updates trigger updating the global system crypto policy,
  since some subsystems now ignore the local configuration by default.

pupmod-simp-freeradius
^^^^^^^^^^^^^^^^^^^^^^
* Added support for overriding post-auth in LDAP.
* Added support for overriding accounting in LDAP.
* Added support for specifying the entire file content.
* Remove ``simp_options::puppet::server`` from the default lookup logic
  for ``freeradius::v3::modules::ldap::server``. In systems that use Bolt
  to compile and apply manifests, that setting will not be available.

pupmod-simp-incron
^^^^^^^^^^^^^^^^^^

* Remove pinned versions of incron, since the upstream packages have been fixed.

pupmod-simp-iptables
^^^^^^^^^^^^^^^^^^^^

* Added preliminary support for acting as a pass-through to various
  ``firewalld`` capabilities using the ``simp-simp_firewalld`` module.

  * Using any of the ``iptables::listen::*`` defined types will work seamlessly
    in ``firewalld`` mode but direct calls to ``iptables::rule`` will fail.
  * Calls to any of the native types included in this module will result in
    undefined behavior and is not advised.
  * To enable ``firewalld`` mode on supported operating systems, simply set
    ``iptables::use_firewalld`` to ``true`` via Hiera.
  * EL 8 systems will enable ``firewalld`` mode by default.

* Improved the internal rule matching to handle most netmask and port updates.
* Added a ``exact_match`` Boolean to the ``iptables_optimize`` and
  ``ip6tables_optimize`` native types to allow for more aggressive rule
  matching.

  * This change requires that inbound rules match whatever is returned by
    ``iptables-save`` and/or ``ip6tables-save`` to prevent iptables flapping.

* Allow LOCAL-INPUT jump rule in FORWARD and INPUT chains to occur last as a
  default action through the addition of an
  ``iptables::rules::base::force_local_input`` parameter.
* Allow users to disable adding the ``SIMP:`` prefix to the rule comment.
* Allow users to disable comments on rules completely.

pupmod-simp-krb5
^^^^^^^^^^^^^^^^

* Updated SELinux hotfix for EL8.
* Migrated SELinux hotfix to ``vox_selinux::module``.

pupmod-simp-libvirt
^^^^^^^^^^^^^^^^^^^

* Split out install and service into separate classes.

pupmod-simp-logrotate
^^^^^^^^^^^^^^^^^^^^^

* Allow all log size configuration parameters to be specified in bytes,
  kilobytes, megabytes, or gigabytes.
* Added ability to specify ``maxsize`` configuration for specific log rotate rules.

pupmod-simp-named
^^^^^^^^^^^^^^^^^

* Allow users to force enabling/disabling of the chroot settings.
* Allow users to easily set the ``named_write_master_zones`` SELinux boolean in
  case they need to support dynamic DNS or zone transfers.

pupmod-simp-nfs
^^^^^^^^^^^^^^^

This module was extensively refactored. Read the updated README to
understand the current usage.  Notable feature/API changes:

* Overall changes

  * Dropped stunnel support for NFSv3.  This tunneling did not work because:

    * The NFS client sends the NFS server Network Status Manager (NSM)
      notifications via UDP, exclusively.
    * At multi-NFS-server sites, a unique rpcbind port per server is
      required in order for a NFS client to be able to tunnel its
      server-specific RPC requests to the appropriate server.

  * ``nfs`` class

    * Reworked parameters to reflect configuration of ``/etc/nfs.conf`` and,
      for limited EL7-only configuration, ``/etc/sysconfig/nfs``.  See the class
      documentation for full details.

  * Removed ``stunnel_systemd_deps`` and ``sunnel_tcp_nodelay`` parameters
    throughout the module.

    * These parameters were not consistently used in the manifest
      code (i.e., declared but not used) and were confusing.
    * The corresponding ``stunnel_socket_options`` and ``stunnel_wantedby``
      parameters in classes/defines now use defaults that were intended to be
      set by those parameters.

  * Now masks NFS services that are not needed, so they are not unnecessarily
    started when the nfs-server.service or nfs-client.target are restarted.

* ``nfs::client`` changes

  * Added support for pNFS:  Set ``blkmap`` to true to enable the pNFS service,
    nfs-blkmap.service.
  * Added ``nfs::stunnel_socket_options`` and ``stunnel_wantedby``
    parameters which provide the defaults for all
    ``nfs::client::mount instances``.

* ``nfs::client::mount`` define changes

  * ``nfs_server`` must now be specified as an IP address.  This change was
    necessary for firewalld.
  * In ``options``, changed the default mount type to ``soft`` instead of
    ``hard``.  Also removed deprecated ``intr`` option, as it has no effect.
  * Reworked the remote autodetect logic to detect a local mount based
    on IP address instead of simply whether the node is also configured
    to be an NFS server.
  * Added support for direct autofs mounts and simplified specification of
    indirect mounts.  When ``autofs_indirect_map_key`` is not specified, a
    direct mount is specified by ``name``.  When ``autofs_indirect_map_key``
    is specified, an indirect mount is specified with ``name`` as the mount
    point and ``autofs_indirect_map_key`` as the mount key.
  * Renamed ``autofs_map_to_user`` to ``autofs_add_key_subst`` to better
    reflect automount terminology. This parameter simply adds key substitution
    to the remote location, which although can be used for user home
    directories, is not restricted to that use case.
  * Renamed ``port`` to ``nfsd_port`` to be consistent with the name of that
    parameter throughout the entire module.
  * Renamed ``v4_remote_port`` to ``stunnel_nfsd_port`` for clarity and to
    be consistent with the name of that parameter throughout the entire module.
  * Exposed client stunnel configuration that was scattered throughout the
    module to this API.  User can now specify ``stunnel_socket_options`` and
    ``stunnel_verify`` for each mount.  When unspecified, the defaults from
    the ``nfs`` class are used.

* ``nfs::server`` class changes

  * Exposed server stunnel configuration that was scattered throughout the
    module to this API.  User can now specify ``stunnel_accept_address``,
    ``stunnel_nfsd_acccept_port``, ``stunnel_socket_options``,
    ``stunnel_verify``, and ``stunnel_wantedby`` in this class.  When
    unspecified, the defaults for all but ``stunnel_accept_address`` and
    ``stunnel_wantedby`` are pulled from the ``nfs`` class.
  * Added the following parameters: ``nfsd_vers4``, ``nfsd_vers4_0``,
    ``nfsd_vers4_1``, ``nfsd_vers4_2``, and ``custom_rpcrquotad_opts``.
  * Renamed ``nfsv3`` to ``nfsd_vers3`` to reflect its use in ``/etc/nfs.conf``.
  * Moved ``nfs::rpcquotad_port`` to this class and renamed ``rpcrquotadopts``
    to ``custom_rpcrquotad_opts`` for clarity.
  * Moved ``nfs::mountd_port`` to this class and removed ``rpcmountdopts``.
    Custom configuration for that daemon should now be made via
    ``nfs::custom_nfs_conf_opts`` or ``nfs::custom_daemon_args`` as
    appropriate.
  * Removed the obsolete ``nfsd_module`` parameter.

* ``nfs::server::export`` define changes

  * Added ``replicas``, ``pnfs``, and ``security_label`` parameters to
    support additional export configuration parameters.

* ``nfs::idmapd`` class changes

  * Refactored into 3 classes to support distinct NFS server and client
    configuration
  * Added ``no_strip`` and ``reformat_group`` to ``nfs::idmapd::config``
    to support additional ``/etc/idmapd.conf`` configuration parameters.

pupmod-simp-oath
^^^^^^^^^^^^^^^^

* Allow ``oath::config::user`` to be any string.
* Disabled ``show_diff`` option in ``concat`` for  ``/etc/liboath/users.oath``
  to prevent that information from being exposed in logs.

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Ensured that ``pam_tty_audit`` is optional if auditing is not enabled on the
  system.
* Added the ability to specify ``pam::limits::rules`` via Hiera.
* Ignore authconfig disable on EL8. Authconfig was replaced with authselect
  and authselect does not overwrite settings unless you select the ``--force``
  option.
* Remove installation of ``pam_pkcs11`` and ``fprintd-pam`` by default, since
  they aren't actually required for basic functionality.

pupmod-simp-polkit
^^^^^^^^^^^^^^^^^^

* Added the following classes:

  * ``polkit::install``
  * ``polkit::service``
  * ``polkit::use``

* Ensured that the polkit user is managed by default and placed into the
  supplementary group bound to the ``gid`` option on ``/proc``, if one is set.
  This is necessary to work around issues with ``hidepid`` > 0.
* Made the entire main class inert on unsupported OSs; logs a warning on the
  server that can be disabled.

pupmod-simp-pupmod
^^^^^^^^^^^^^^^^^^

* Set the default puppetserver ciphers to a safe set.
* Added better auto-tuning support for puppetserver, based on best practices.
* Added ``ReservedCodeCache`` puppetserver support.
* Removed incron support in favor of using systemd path units to run
  ``simp_generate_types``.

  * Attempts to activate the incron code will result in a warning message.

* Added mitigation for https://puppet.com/security/cve/CVE-2020-7942/
* Added optional management of the Facter configuration file.
* Removed the deprecated CA CRL pull cron job and the corresponding
  ``pupmod::ca_crl_pull_interval`` parameter.
* Removed deprecated *auth.conf* support for the legacy pki module and
  the corresponding parameters:

  * ``pupmod::master::simp_auth::legacy_cacerts_all``
  * ``pupmod::master::simp_auth::legacy_mcollective_all``
  * ``pupmod::master::simp_auth::legacy_pki_keytabs_from_host``

* Removed the deprecated ``pupmod::master::simp_auth::server_distribution``
  parameter.

pupmod-simp-resolv
^^^^^^^^^^^^^^^^^^

* Added optional management of DNS servers via nmcli.

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

* Added support for KeepAlive variables for imtcp and omfwd actions.
* Changed local rule defined type to use the same package defaults for
  action queues that are in the remote rule defined type.
* Changed remote rule defined type to use package defaults for action
  queues.
* Added a default rule to log packets dropped by firewalld to
  ``/var/log/firewall.log``.
* Added ``/var/log/firewall.log`` to SIMP's 'syslog' logrotate rule.
* Added ``logrotate::rule`` options to ``rsyslog::conf::logrotate`` class.
* Removed params pattern and migrated to data in modules.

pupmod-simp-selinux
^^^^^^^^^^^^^^^^^^^

* No longer enable or install mcstransd by default.  It is a user convenience
  feature and not required for core functionality.
* Ensured that mcstransd is added to the GID assigned to ``/proc`` if one is
  assigned on the system.

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* sssd configuration updates

  * Do not configure the ``local`` provider for EL8.
  * Use the ``files`` provider for the local domain for EL7 and later.
  * Deprecated sssd client autofs, ssh and sudo settings.  The sssd
    module configures services in ``sssd::services``.  Use that
    setting to configure those entries.
  * Configure sssd even if local and ldap domains are not configured for EL8.

* Updated ``simp::mountpoints::proc`` to ensure polkitd can be configured to
  have access to ``/proc``:

  * Assign a group and gid by default
  * Create a group by default
  * Discover these values from the system if possible

* Removed the following applications from the list of base OS applications
  installed automatically by ``simp-simp``:

  * man
  * man-pages
  * vim-enhanced
  * dos2unix
  * elinks
  * hunspell
  * lsof
  * mlocate
  * pax
  * pinfo
  * sos
  * star
  * symlinks
  * words
  * x86info

* Deprecated the ``simp::base_apps::manage_elinks_config`` parameter.

  * It no longer has any effect.

* ``simp::nsswitch`` updates

  * Updated the ``simp::nsswitch`` class to have sane defaults

    * Added support for mymachines and myhostname by default.
    * Removed all NIS references since NIS should not be in general usage any
      longer and was never natively supported by SIMP.
    * Configuration files are now common across all supported OSs since nsswitch
      "does the right thing" when it hits a module that it does not recognize.

  * Allow nsswitch overrides.

* Added chronyd support for EL8

  * Moved ntp to list of OS relevant applications for EL6 and EL7.
  * Added chronyd for EL8.

* Updated the client kickstart scripts/configuration

  * Updated the ``bootstrap_simp_client`` script to use chrony if kernel version
    is 4 or later.
  * Deprecated the ``simp::server::kickstart::runpuppet`` parameter and removed
    the old ``runpuppet`` kickstart scripts.  The ``simp_bootstrap_client``
    scripts should be used instead.

* ClamAV updates:

  * Removed ``clamav`` from the list of classes included by default in the
    SIMP scenarios.

    * This will not remove ClamAV from systems where it is installed; Puppet
      will simply stop managing it.
    * To continue managing ClamAV with Puppet, add ``clamav`` to ``simp::classes``
      in the appropriate Hiera file for that SIMP client.
    * See the ``simp-clamav`` module for information on configuring or removing
      ClamAV on a system.

  * Deprecated ``simp::server::clamav``.

    * This parameter will be removed in a future SIMP release.
    * To manage ClamAV on the SIMP server after the parameter is removed,
      manually add the ``clamav`` class to the ``simp::classes`` Array in the
      SIMP server's Hiera file.


pupmod-simp-simp_banners
^^^^^^^^^^^^^^^^^^^^^^^^

* Removed all OS support statements from ``metadata.json`` since this is simply a
  data-only module.


pupmod-simp-simp_bolt
^^^^^^^^^^^^^^^^^^^^^

* Added plan to install puppet-agent on target nodes.
* Configured Bolt to request a pseudo TTY for SSH sessions if specified.
* Configured new logs to be appended to the log file instead of overwriting.

pupmod-simp-simp_firewalld
^^^^^^^^^^^^^^^^^^^^^^^^^^

This is a new SIMP module that provides a profile class and defined type to
manage the system's ``firewalld`` with "safe" defaults and safety checks for
``firewalld`` rules.  It uses the ``puppet-firewalld`` module to update the
system's ``firewalld`` configuration.

pupmod-simp-simp_ipa
^^^^^^^^^^^^^^^^^^^^

* Make the IPA server optional in the ``join`` task.  It is perfectly valid
  to not specify a server when doing an IPA client install and instead
  rely on DNS auto discovery.

pupmod-simp-simp_nfs
^^^^^^^^^^^^^^^^^^^^

* The following parameters had to be changed from hostnames or IP addresses
  to only IP addresses due to use of firewalld on EL8:

  * ``simp_nfs::home_dir_server``
  * ``simp_nfs::mount::home::nfs_server``

pupmod-simp-simp_options
^^^^^^^^^^^^^^^^^^^^^^^^

* The ``simp_options::clamav`` catalyst has been deprecated.

  * As of SIMP 6.5, SIMP's ``clamav`` class is no longer included in the class
    list of the SIMP scenarios. So, this catalyst is not needed to disable it.
  * To have SIMP manage ClamAV on your system, add the ``clamav`` class to
    your system's class list.
  * See the SIMP ``clamav`` module README for information on managing ClamAV.

* ``simp_options::puppet::server`` and ``simp_options::puppet::ca`` are now
  optional.

  * These are no longer required at all times due to support for Bolt. Code that
    used these parameters will correctly fail and require users to add them to
    their configuration.

* Updated ``simp_options::ldap`` to require the 'master' and 'uri' parameters if
  ``simp_options::puppet::server`` is not defined.

pupmod-simp-simp_rsyslog
^^^^^^^^^^^^^^^^^^^^^^^^

* Add support for firewalld log message collection.
* Deep merge ``simp_rsyslog::log_collection``.


pupmod-simp-simpkv
^^^^^^^^^^^^^^^^^^

This is a new SIMP module that provides an abstract library that allows Puppet
to access one or more key/value stores.

This module provides

* a standard Puppet language API (functions) for using key/value stores
* a configuration scheme that allows users to specify per-application use
  of different key/value store instances
* adapter software that loads and uses store-specific interface software
  provided by the simpkv module itself and other modules
* a Ruby API for the store interface software that developers can implement
  to provide their own store interface
* a file-based store on the local filesystem and its interface software.

  * Future versions of this module will provide a distributed key/value store.

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

Facts Changes
"""""""""""""

Added the following facts:

+----------------------------------+----------------------------------------+
| Fact                             | Description                            |
+==================================+========================================+
| ``simplib__auditd``              | Returns a hash of auditd status.       |
+----------------------------------+----------------------------------------+
| ``simplib__firewalls``           | Return an array of known firewall      |
|                                  | commands that are present on the       |
|                                  | system.                                |
+----------------------------------+----------------------------------------+
| ``simplib__mountpoints``         | Returns a hash of mountpoints of       |
|                                  | particular interest to SIMP modules.   |
+----------------------------------+----------------------------------------+
| ``simplib__efi_enabled``         | Returns ``true`` if the host is using  |
|                                  | EFI.                                   |
+----------------------------------+----------------------------------------+
| ``simplib__secure_boot_enabled`` | Returns ``true`` if the host is using  |
|                                  | UEFI Secure Boot.                      |
+----------------------------------+----------------------------------------+

Deprecated the following facts:

* ``tmp_mounts`` fact.  Use ``simplib__mountpoints``, instead.


Function Changes
""""""""""""""""

Added the following functions:

+----------------------------------------------+---------------------------------------------+
| Function                                     | Description                                 |
+==============================================+=============================================+
| ``simplib::debug::inspect``                  | Enhanced version of ``simplib::inspect``.   |
+----------------------------------------------+---------------------------------------------+
| ``simplib::debug::classtrace``               | Prints a trace of all catalog resources     |
|                                              | traversed to get to the current point.      |
+----------------------------------------------+---------------------------------------------+
| ``simplib::debug::stacktrace``               | Prints a trace of all files traversed to    |
|                                              | get to the current point.                   |
+----------------------------------------------+---------------------------------------------+
| ``simplib::ip::family_hash``                 | Takes an IP address or array of IP          |
|                                              | addresses and returns a hash with the       |
|                                              | addresses broken down by family. The        |
|                                              | returned hash also contains additional      |
|                                              | helpful metadata.                           |
+----------------------------------------------+---------------------------------------------+
| ``simplib::module_metadata::os_blacklisted`` | Determine if the passed module metadata     |
|                                              | indicates that the current OS has been      |
|                                              | blacklisted.                                |
+----------------------------------------------+---------------------------------------------+
| ``simplib::module_metadata::os_supported``   | Determine if the passed module metadata     |
|                                              | indicates that the current OS is supported. |
+----------------------------------------------+---------------------------------------------+
| ``simplib::module_metadata::assert``         | Adds an assertion based on whether the OS   |
|                                              | is supported or blacklisted.                |
+----------------------------------------------+---------------------------------------------+
| ``simplib::caller``                          | Determines what called a function.          |
+----------------------------------------------+---------------------------------------------+
| ``simplib::passgen::gen_password_and_salt``  | Generates a password and salt.              |
+----------------------------------------------+---------------------------------------------+
| ``simplib::passgen::gen_salt``               | Generates a salt.                           |
+----------------------------------------------+---------------------------------------------+
| ``simplib::passgen::get``                    | Retrieves a generated password and any      |
|                                              | stored attributes.                          |
+----------------------------------------------+---------------------------------------------+
| ``simplib::passgen::list``                   | Retrieves the list of generated passwords   |
|                                              | with attributes and the list of sub-folders |
|                                              | stored at a ``simplib::passgen`` folder.    |
+----------------------------------------------+---------------------------------------------+
| ``simplib::passgen::remove``                 | Removes a generated password, history and   |
|                                              | stored attributes.                          |
+----------------------------------------------+---------------------------------------------+
| ``simplib::passgen::set``                    | Sets a generated password with attributes.  |
+----------------------------------------------+---------------------------------------------+
| ``simplib::safe_filename``                   | Convert a string into a filename that is    |
|                                              | 'path safe'.                                |
+----------------------------------------------+---------------------------------------------+

Updated the following functions:

* ``simplib::passgen``

  * Added simpkv mode.

    * Runs in legacy mode (default) or in a simpkv mode.
    * simpkv mode is **EXPERIMENTAL**.
    * When in simpkv mode, ``simplib:passgen`` uses ``simp-simpkv`` for
      password persistence.
    * simpkv mode is enabled by setting ``simplib::passgen::simpkv`` to
      ``true`` in hieradata.
    * If you enable simpkv mode in a system that already has passwords
      generated via the legacy code, currently, **all passwords will be
      regenerated**.
    * Added ``simpkv_options`` parameter to ``simplib::passgen`` for use in
      simpkv mode.

  * Enhanced ``simplib::passgen`` operation when in simpkv mode

    * Stores 'complexity' and 'complex_only' setting in the password's simpkv
      metadata, so that the password can be regenerated with the same
      characteristics.
    * Regenerates the password if the requested 'complexity' or 'complex_only'
      setting differs from the setting used for the latest persisted password.
    * Stores up to the lastest 10 <password,salt> pairs in the password's
      simpkv metadata.

  * Added a ``gen_timeout_seconds`` password option.  Previously this was
    hardcoded to 30 seconds.

  * Added ability to set the user and group for legacy
    ``simplib::passgen`` files.
  * Changed the default permissions on legacy ``simplib::passgen`` files
    to the user running the catalog compile.  This will allow bolt to set
    permissions correctly.

* ``simplib::gen_random_password``:

  * Intersperse special characters among the alpha-numeric characters,
    when 'complexity' is 1 or 2 and 'complex_only' is ``false``.
    Previously, this function grouped the all alpha-numeric characters
    together and grouped all special characters together.  This generated
    passwords that were not suitable for user passwords, as they would fail
    the cracklib/libpwquality complexity checks.

* ``simplib::assert_metadata``:

  * Added ``blacklist`` option. This allows functionality to deliberately
    fail on an OS that is listed in the module's ``metadata.json``, but is not
    necessarily supported by all parts of the given module.

New data type aliases
"""""""""""""""""""""

Added ``Simplib::Systemd::ServiceName`` for valid systemd service names.

pupmod-simp-stunnel
^^^^^^^^^^^^^^^^^^^

* Set default for ``stunnel::connection::ssl_version`` to TLSv1.2 for EL8
  compatibility.
* Set default for ``stunnel::instance::ssl_version`` to TLSv1.2 for EL8
  compatibility.
* Set the ``stunnel::connection::app_pki_crl parameter`` to ``undef`` by
  default due to issues with pointing the setting to an absent directory in EL8.
* Set the ``stunnel::instance::app_pki_crl``` parameter to ``undef`` by default
  due to issues with pointing the setting to an absent directory in EL8.
* Updated valid ``ssl_version`` entries.


pupmod-simp-sudo
^^^^^^^^^^^^^^^^

* Added parameters for ``sudo::default_entry`` and ``sudo::alias`` defined
  types.
* CVE-2019-14287 mitigation

  * Do not allow the use of userid or group id of '-1' when 'ALL' or '%ALL' are
    used in the runas section of a sudo user specification and the version of
    sudo is earlier than 1.8.28.
  * See  https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-14287
    for more information.

* Deep merge ``user_specifications`` by default.

pupmod-simp-svckill
^^^^^^^^^^^^^^^^^^^

* Updated the ``svckill`` provider to work with different Puppet ``service``
  provider implementations.

  * If after a Puppet upgrade you find that ``svckill`` is trying to kill
    system services that it previously ignored, you need ``simp-svckill``
    version 3.6.1 or later to fix the problem.

* Updated service lists.

pupmod-simp-swap
^^^^^^^^^^^^^^^^

* Disable ``dynamic_swappiness`` by default.
* Set static system swappiness to 60 by default.


pupmod-simp-tcpwrappers
^^^^^^^^^^^^^^^^^^^^^^^

* Enhanced behavior to do nothing when tcpwrappers is not supported by the OS.

pupmod-simp-tpm2
^^^^^^^^^^^^^^^^

* Removed the option for managing tools, ``tpm2::manage_tpm2_tools``.
  Tools can be managed or not by removing them from the package list.
  Note that the tools package is needed to determine the status of the TPM.
* Added support for setting ``tabrm_options`` for connecting to the simulator.


pupmod-simp-useradd
^^^^^^^^^^^^^^^^^^^

* Added explicit support for setting the rescue/emergency shell on systemd
  systems.


rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Allow users to set the SIMP_ENVIRONMENT environment variable to change the
  initial environment from 'production' to a custom value, when running
  ``simp config`` or ``simp bootstrap``.
* ``simp config`` changes

  * Ensured that ``simp config`` uses the ``simp::classes`` parameter instead
    of ``classes`` by default, but accept both ``simp::classes`` and
    ``classes`` as valid existing configurations.
  * Removed deprecated ``--non-interactive`` option.  Use ``--force-defaults``
    instead.

* Added ``simp kv`` command family to allow users to manage and inspect
  entries in a simpkv key/value store
* ``simp passgen`` changes

  * Split into sub-commands for ease of use:

    * ``simp passgen envs``: List environments that may have ``simplib::passgen``
      passwords.
    * ``simp passgen list``: List names of ``simplib::passgen`` passwords.
    * ``simp passgen remove``: Remove ``simplib::passgen`` passwords.
    * ``simp passgen set``: Set ``simplib::passgen`` passwords.
    * ``simp passgen show``:  Show ``simplib::passgen`` passwords and other
      stored attributes.

  * Updated to work with simpkv-enabled ``simplib::passgen``.  Automatically
    detects whether ``simplib::passgen`` is operating in legacy mode or
    simpkv mode in the specified environment, and then executes password
    operations using the appropriate mechanism for that mode.
  * When setting passwords, disabled libpwquality/cracklib validation of
    user-entered passwords, by default, because not all passwords managed
    by ``simplib::passgen`` are user passwords.  This validation can be
    re-enabled with the ``--validate`` option of the ``simp passgen set``
    command.

  * Added the following command line options when creating passwords

    * ``--[no-]auto-gen``: Whether to auto-generate new passwords.
    * ``--complexity``: Password complexity to use when a password is
      auto-generated. Corresponds to the complexity option of
      ``simplib::passgen``.
    * ``--[no-]complex-only``: Whether to only use only complex characters
      when a password is auto-generated. Corresponds to the complex_only
      option of ``simplib::passgen``.
    * ``--[no-]validate``: Enabled validation of new passwords with
      libpwquality/cracklib.
    * ``--length``: Password length to use when a password is auto-generated.

  * Added ``--[no-]details`` option when showing password information.  When
    enabled, all available password information is displayed, not just the
    current and previous password values.

simp-environment-skeleton
^^^^^^^^^^^^^^^^^^^^^^^^^

* Ensured that the server hieradata defaults have ``simp::server`` in the
  ``simp::classes`` array. Otherwise, it will never get picked up.
* Replace ``classes`` with ``simp::classes`` and ``simp::server::classes`` as
  appropriate in example Hiera YAML files.
* FakeCA Updates

  * Added the CA code directly into the project to allow the code to work
    on newer OS versions
  * Allow users to specify an alternate output directory via a KEYDIST
    environment variable.
  * Consolidate the certificate request and revocation code.
  * Certificate revocation now runs in linear time.

* Changed permissions for files and directories to be world readable.
* Add a PE-suitable puppet YAML data template.


simp-gpgkeys
^^^^^^^^^^^^

* Added the CentOS 8 and EPEL 8 GPG keys.
* Removed Fedora 25 and 26 GPG keys.
* Updated puppetlabs GPG key.

simp-rsync-skeleton
^^^^^^^^^^^^^^^^^^^

* Added mitigation for CVE-2019-6477 to the sample, RedHat 7 ``named.conf``.

  * See  https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-6477
    for more information.

* Removed ``rndc.key`` files from sample named configuration to prevent users
  from accidentally using a published, sample secret key.

  * The ``named`` service will create a key if one does not exist using the
    correct defaults for the system.

Known Bugs
----------

Nothing significant at this time.

The SIMP project in JIRA can be used to `file bugs`_.

.. _file bugs: https://simp-project.atlassian.net


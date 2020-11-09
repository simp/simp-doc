.. _changelog-latest:
.. _changelog-6.5.0:

SIMP Community Edition (CE) 6.5.0-Beta
======================================

.. raw:: pdf

  PageBreak

.. contents::
  :depth: 2

.. raw:: pdf

  PageBreak


OS compatibility
----------------

.. contents::
  :depth: 2
  :local:

This release is known to work with:

  * CentOS 6.10 x86_64
  * CentOS 7.0 2003 x86_64
  * CentOS 8.2 2004 x86_64 — :ref:`client systems only<changelog-6-5-0-el8-client-only>`
  * OEL 6.10 x86_64
  * OEL 7.8 x86_64
  * OEL 8.2 x86_64 — :ref:`client systems only<changelog-6-5-0-el8-client-only>`
  * RHEL 6.10 x86_64
  * RHEL 7.8 x86_64
  * RHEL 8.2 x86_64 — :ref:`client systems only<changelog-6-5-0-el8-client-only>`


Important OS compatibility limitations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

OS compatibility is subject to the following limitations:



.. _changelog-6-5-0-el8-client-only:

EL8 support is CLIENT ONLY
""""""""""""""""""""""""""

This release introduces client-only EL8 support in the core Puppet modules.

* EL8 support is limited to managing EL8 Puppet *agents* with the core Puppet
  modules.
* All Puppet modules provided as core dependencies of the :package:`simp` RPM
  support EL8.

This release does NOT support EL8 for:

* Managing an EL8 SIMP Server
* Installing SIMP from an EL8 ISO.
* Using the :program:`unpack_dvd` script on modular yum repositories found on
  EL8 OS ISOs

.. rubric:: Additional limitations with EL8

* Not all modules provided by the :package:`simp-extras` RPM have been updated
  for EL8.
* EL8 updates to the remaining :package:`simp-extras` modules will be phased
  in over future SIMP releases.
* Support for managing an EL8 SIMP/Puppet server and installing from
  EL8 ISOs will be provided in a later SIMP release (SIMP 6.6.0).

* In SIMP 6.5.0,
  :ref:`there are known issues<changelog-6-5-0-el8-client-limitations>` with
  PXE kickstarting and unpacking ISOs as yum mirrors for EL8 clients.  These
  issues particularly affect network-isolated environments.

  * For details, see: :ref:`changelog-6-5-0-el8-client-limitations`.


Support for managing EL6 is drawing down
""""""""""""""""""""""""""""""""""""""""

  * EL6 maintenance support is EOL for both RHEL 6 and CentOS 6, and upstream
    vendor support will end on 30 November 2020.
  * **New Puppet modules may not support EL6.**
  * Some optional Puppet modules (provided by the :package:`simp-extras` RPM)
    no longer support EL6. In particular, this affects :pupmod:`simp/autofs`,
    :pupmod:`simp/nfs`, and :pupmod:`simp/simp_nfs`.  If you need those
    capabilities on EL6, use earlier versions of these modules in EL6-specific
    Puppet environments.


Breaking Changes
----------------

.. contents::
  :depth: 2
  :local:

IPTables Rule Refinement
^^^^^^^^^^^^^^^^^^^^^^^^

.. IMPORTANT::

   IPTables does NOT have breaking changes out of the box.

A new parameter, :code:`iptables::precise_match` was added that performs higher
precision matching on :program:`iptables` rules to detect the need to restart
:program:`iptables`.

It is highly recommended that you set :code:`iptables::precise_match: true` in
:term:`Hiera` so that minor changes, such as subnet updates or single port
changes, will appropriately restart
:program:`iptables`.

If you enable precision matching, do so with care since you may find that
:program:`iptables` rule updates are propagated that you thought had previously
been applied.

It is highly recommended that you migrate to :code:`firewalld` if at all
possible. See the relevant section below for more details.


Deprecated Puppet 3 API Functions Removed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All SIMP-provided Puppet 3 API functions (originally deprecated in SIMP 6.4.0)
have now been removed in order to fully support Puppet 6. The affected
functions and their replacements (when available) are listed in sub-sections
below.

Puppet 3 Functions Removed from simp/compliance_markup
""""""""""""""""""""""""""""""""""""""""""""""""""""""

+---------------------------+-------------------------------------------+-------------------------------------------+
| Puppet 3 API Function     | Replacement                               | Replacement Source                        |
+===========================+===========================================+===========================================+
| :code:`compliance_map`    | :code:`compliance_markup::compliance_map` | :pupmod:`simp/compliance_markup` >= 3.0.0 |
+---------------------------+-------------------------------------------+-------------------------------------------+

Puppet 3 Functions Removed from simp/simp_apache
""""""""""""""""""""""""""""""""""""""""""""""""

+------------------------------+-------------------------------------------+-------------------------------------+
| Puppet 3 API Function        | Replacement                               | Replacement Source                  |
+==============================+===========================================+=====================================+
| :code:`apache_auth`          | :code:`simp_apache::auth`                 | :pupmod:`simp/simp_apache` >= 6.0.1 |
+------------------------------+-------------------------------------------+-------------------------------------+
| :code:`apache_limits`        | :code:`simp_apache::limits`               | :pupmod:`simp/simp_apache` >= 6.0.1 |
+------------------------------+-------------------------------------------+-------------------------------------+
| :code:`munge_httpd_networks` | :code:`simp_apache::munge_httpd_networks` | :pupmod:`simp/simp_apache` >= 6.0.1 |
+------------------------------+-------------------------------------------+-------------------------------------+

Puppet 3 Functions Removed from simp/simplib
""""""""""""""""""""""""""""""""""""""""""""

.. IMPORTANT::

   Most (but not all) of the Puppet 3 API functions in the table below have
   replacements. If any function that has been removed without a replacement is
   essential to you, let us know by submitting a feature request at
   https://simp-project.atlassian.net.

+----------------------------------+--------------------------------------------+-----------------------------------+
| Puppet 3 API Function            | Replacement                                | Replacement Source                |
+==================================+============================================+===================================+
| :code:`array_include`            | Puppet language `in`_ operator *or* Puppet | Puppet >= 5.2.0                   |
|                                  | built-in functions :code:`any` or          |                                   |
|                                  | :code:`all`                                |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`array_size`               | Puppet built-in function :code:`length`    | Puppet >= 5.5.0                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`array_union`              | Puppet language `+ (concatenation)`_       | Puppet >= 5.0.0                   |
|                                  | operator, combined with Puppet built-in    |                                   |
|                                  | function :code:`unique`                    |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`bracketize`               | :code:`simplib::bracketize`                | :pupmod:`simp/simplib` >= 3.15.0  |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`generate_reboot_msg`      | None                                       | N/A                               |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`get_ports`                | None                                       | N/A                               |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`h2n`                      | None                                       | N/A                               |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`host_is_me`               | :code:`simplib::host_is_me`                | :pupmod:`simp/simplib` >= 3.15.0  |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`inspect`                  | :code:`simplib::inspect`                   | :pupmod:`simp/simplib` >= 3.3.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`ipaddresses`              | :code:`simplib::ipaddresses`               | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`ip_is_me`                 | :code:`simplib::host_is_me` (checks        | :pupmod:`simp/simplib` >= 3.15.0  |
|                                  | hostnames and IP addresses)                |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`ip_to_cron`               | :code:`simplib::ip_to_cron`                | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`join_mount_opts`          | :code:`simplib::join_mount_opts`           | :pupmod:`simp/simplib` >= 3.8.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`localuser`                | None                                       | N/A                               |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`mapval`                   | None                                       | N/A                               |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`nets2cidr`                | :code:`simplib::nets2cidr`                 | :pupmod:`simp/simplib` >= 3.7.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`nets2ddq`                 | :code:`simplib::nets2ddq`                  | :pupmod:`simp/simplib` >= 3.8.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`parse_hosts`              | :code:`simplib::parse_hosts`               | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`passgen`                  | :code:`simplib::passgen`                   | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`rand_cron`                | :code:`simplib::rand_cron`                 | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`simp_version`             | :code:`simplib::simp_version`              | :pupmod:`simp/simplib` >= 3.15.0  |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`simplib_deprecation`      | :code:`simplib::deprecation`               | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`slice_array`              | Puppet built-in :code:`slice`              | Puppet >= 4.0.0                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`strip_ports`              | :code:`simplib::strip_ports`               | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`to_integer`               | Puppet built-in :code:`Integer` *or*       | :code:`Integer`: Puppet >= 4.0.0; |
|                                  | :code:`simplib::to_integer`                | :code:`simplib::to_integer`:      |
|                                  |                                            | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`to_string`                | Puppet built-in :code:`String`             | :code:`String`: Puppet >= 4.0.0;  |
|                                  | *or* :code:`simplib::to_string`            | :code:`simplib::to_string`:       |
|                                  |                                            | :pupmod:`simp/simplib` >= 3.5.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_array_member`    | :code:`simplib::validate_array_member`     | :pupmod:`simp/simplib` >= 3.8.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_array_of_hashes` | Use a custom Puppet data type              | Puppet >= 4.0.0                   |
|                                  | such as :code:`Array[Hash]`                |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_between`         | Puppet data types :code:`Integer` or       | :pupmod:`simp/simplib` >= 3.8.0   |
|                                  | :code:`Float` *or*                         |                                   |
|                                  | :code:`simplib::validate_between`          |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_bool_simp`       | Use Puppet :code:`Boolean` data type       | Puppet: >= 4.0.0;                 |
|                                  | *or* :code:`simplib::validate_bool`        | :pupmod:`simp/simplib` >= 3.8.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_deep_hash`       | :code:`simplib::validate_deep_hash`        | :pupmod:`simp/simplib` >= 3.8.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_float`           | Use Puppet :code:`Float` data type         | Puppet: >= 4.0.0;                 |
|                                  | *or* a check using :code:`is_float`        | :code:`is_float`:                 |
|                                  | from :pupmod:`puppetlabs/stdlib`           | :pupmod:`puppetlabs/stdlib` >=    |
|                                  |                                            | 2.2.0                             |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_macaddress`      | Use :code:`Simplib::Macaddress` data type  | :pupmod:`simp/simplib` >= 3.7.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_net_list`        | Use :code:`Simplib::Netlist` data type     | :pupmod:`simp/simplib` >= 3.5.0   |
|                                  | *or* :code:`simplib::validate_net_list`    |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_port`            | Use :code:`Simplib::Port` data type *or*   | :pupmod:`simp/simplib` >= 3.5.0   |
|                                  | :code:`simplib::validate_net_list`         |                                   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_re_array`        | :code:`simplib::validate_re_array`         | :pupmod:`simp/simplib` >= 3.7.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_sysctl_value`    | :code:`simplib::validate_sysctl_value`     | :pupmod:`simp/simplib` >= 3.7.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_umask`           | Use :code:`Simplib::Umask` data type       | :pupmod:`simp/simplib` >= 3.7.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+
| :code:`validate_uri_list`        | :code:`simplib::validate_sysctl_value`     | :pupmod:`simp/simplib` >= 3.7.0   |
+----------------------------------+--------------------------------------------+-----------------------------------+

.. _in:                https://puppet.com/docs/puppet/6.18/lang_expressions.html#in
.. _+ (concatenation): https://puppet.com/docs/puppet/6.18/lang_expressions.html#+-(concatenation)

Puppet 3 Functions Removed from simp/ssh
""""""""""""""""""""""""""""""""""""""""

+--------------------------------+---------------------------------+------------------------------+
| Puppet 3 API Function          | Replacement                     | Replacement Source           |
+================================+=================================+==============================+
| :code:`ssh_autokey`            | :code:`ssh::autokey`            | :pupmod:`simp/ssh` >= 6.2.0  |
+--------------------------------+---------------------------------+------------------------------+
| :code:`ssh_global_known_hosts` | :code:`ssh::global_known_hosts` | :pupmod:`simp/ssh` >= 6.2.0  |
+--------------------------------+---------------------------------+------------------------------+

Primary API Changed in Optional Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following SIMP modules from the :package:`simp-extras` RPM have had breaking
API changes:

* :pupmod:`simp/autofs`
* :pupmod:`simp/nfs`
* :pupmod:`simp/simp_nfs`
* :pupmod:`simp/simp_snmpd`

The specific changes made are described in detail in the
:ref:`New Features section<changelog-6-5-0-new-features>`.

.. _changelog-6.5.0-el6-support-dropped-from-some-optional-puppet-modules:

EL6 Support Dropped from Some (Optional) Puppet Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following optional SIMP modules have dropped support for EL6:

* :pupmod:`simp/autofs`
* :pupmod:`simp/nfs`
* :pupmod:`simp/simp_nfs`

If you need EL6 for a client node, place it in an environment with
older versions of the appropriate modules.


Significant Updates
-------------------

.. contents::
  :depth: 2
  :local:

.. _changelog-6.5.0-el8-client-support:

EL8 SIMP Client Node Support
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This release provides support for managing software on EL8 agents.

This includes all (appropriate) Puppet modules provided by the :package:`simp`
RPM, and a subset of the Puppet modules provided by the :package:`simp-extras`
RPM.

* The remaining changes required for an EL8 SIMP server and ISO will be
  available in the next SIMP minor release.
* EL8 updates to the remaining, optional, Puppet modules will be phased in
  over future SIMP releases. This includes the following SIMP modules:

  * :pupmod:`simp/gdm`
  * :pupmod:`simp/gnome`
  * :pupmod:`simp/hirs_provisioner`
  * :pupmod:`simp/mate`
  * :pupmod:`simp/simp_gitlab`
  * :pupmod:`simp/simp_pki_service`
  * :pupmod:`simp/tuned`
  * :pupmod:`simp/vnc`
  * :pupmod:`simp/x2go`


Full Puppet 6 Support and Puppet 6 Default Components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All SIMP Puppet modules now work with both Puppet 5 and Puppet 6, and the
SIMP-6.5.0 ISOs deliver Puppet 6 application RPMs.

firewalld Support
^^^^^^^^^^^^^^^^^

As of SIMP 6.5.0, preliminary :program:`firewalld` support within the SIMP
ecosystem is now available.

* **New simp/simp_firewalld module**: SIMP now includes
  :pupmod:`simp/simp_firewalld` which provides a profile class and defined type
  to manage the system's :program:`firewalld` with "safe" defaults and safety
  checks for :program:`firewalld` rules.
* **firewalld support in simp/iptables for backward compatibility**:  The
  :pupmod:`simp/iptables` module has preliminary support for acting as a
  pass-through to various :program:`firewalld` capabilities using the
  :pupmod:`simp/simp_firewalld` module.

  * To enable 'firewalld' mode on supported operating systems, simply set
    :code:`iptables::use_firewalld` to :code:`true` via :term:`Hiera`.
  * EL8 systems enable 'firewalld' mode by default.
  * Use of any of the :code:`iptables::listen::*` defined types will work
    seamlessly in 'firewalld' mode, as long as IP addresses are used in their
    :code:`trusted_net` parameters.
  * Direct calls to :code:`iptables::rule` in 'firewalld' mode will emit a
    warning notification that directs the user to convert their rules to
    :code:`simp_iptables::rule` types.

.. IMPORTANT::

   Be aware that :program:`firewalld` rules do not support hostnames; IP
   addresses must be used. This may impact any manifests that contain
   :code:`iptables::listen` resources, including resources from some SIMP
   modules. You will have to change any hostnames to IP addresses for the
   affected resources when using :program:`firewalld`.


The table below is a list of the SIMP resource parameters impacted by the lack
of hostname support by :program:`firewalld`.

* Many of these parameters default to :code:`simp_options:trusted_nets`, when it
  is available.
* Each network element can be specified as a network (CIDR notation), an IP address,
  :code:`'ALL'` or :code:`'any'`.
* 'or' in the table below indicates the default value that will be used if the
  previous value is not defined.

+---------------------------------------------------+----------------------------------------+
| Parameter                                         | Default Value                          |
+===================================================+========================================+
| :code:`freeradius::v3::conf::trusted_nets`        | :code:`simp_options::trusted_nets`     |
|                                                   | or :code:`['127.0.0.1','::1']`         |
+---------------------------------------------------+----------------------------------------+
| :code:`krb5::kdc::firewall::trusted_nets`         | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1','::1']`        |
+---------------------------------------------------+----------------------------------------+
| :code:`krb5::kdc::realm::trusted_nets`            | :code:`krb5::kdc::trusted_nets`        |
|                                                   |  or :code:`simp_options::trusted_nets` |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`libreswan::trusted_nets`                   | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1/32']`           |
+---------------------------------------------------+----------------------------------------+
| :code:`nfs::client::mount::nfs_server`            | N/A                                    |
+---------------------------------------------------+----------------------------------------+
| :code:`nfs::server::trusted_nets`                 | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`ntpd::trusted_nets`                        | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1','::1']`        |
+---------------------------------------------------+----------------------------------------+
| :code:`postfix::server::trusted_nets`             | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`pupmod::master::trusted_nets`              | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1','::1']`        |
+---------------------------------------------------+----------------------------------------+
| :code:`rsync::server::trusted_nets`               | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`rsyslog::trusted_nets`                     | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1/32']`           |
+---------------------------------------------------+----------------------------------------+
| :code:`simp::puppetdb::trusted_nets`              | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`simp_apache::ssl::trusted_nets`            | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1','::1']`        |
+---------------------------------------------------+----------------------------------------+
| :code:`simp_apache::conf::allowroot`              | :code:`['127.0.0.1','::1']`            |
+---------------------------------------------------+----------------------------------------+
| :code:`simp_nfs::home_dir_server`                 | N/A                                    |
+---------------------------------------------------+----------------------------------------+
| :code:`simp_nfs::mount::home::nfs_server`         | N/A                                    |
+---------------------------------------------------+----------------------------------------+
| :code:`simp_openldap::server::conf::trusted_nets` | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`ssh::server::conf::trusted_nets`           | :code:`['ALL']`                        |
+---------------------------------------------------+----------------------------------------+
| :code:`stunnel::connection::trusted_nets`         | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`stunnel::instance::trusted_nets`           | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+
| :code:`vsftpd::trusted_nets`                      | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1','::1']`        |
+---------------------------------------------------+----------------------------------------+
| :code:`xinetd::service::trusted_nets`             | :code:`simp_options::trusted_nets`     |
|                                                   |  or :code:`['127.0.0.1']`              |
+---------------------------------------------------+----------------------------------------+


Optional Dependency Handling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In SIMP 6.5.0, optional dependency handling has been integrated into ~20
additional SIMP Puppet modules. These modules explicitly identify optional,
dependent modules, all while providing safeguards to ensure the user is
notified of any such missing dependencies at compilation time. This feature
allows the user to minimize installation of unused modules in an environment,
when the user is not using SIMP to manage specific capabilities.

Key details about this feature are as follows:

* Optional module dependencies are indicated in the :file:`metadata.json` file
  using an 'optional_dependencies' key within a 'simp' key.  For example,
  `simp/rsyslog's metadata.json <https://github.com/simp/pupmod-simp-rsyslog/blob/7.6.2/metadata.json>`_.
* The user has complete control over installation of the optional dependency
  modules.  These dependencies will not be installed automatically when
  the module using them is installed via :code:`puppet module install`.
* Modules that use this feature will fail manifest compilation, if
  the user enables the optional capabilities, but the optional dependencies
  required to implement that capability are not installed in the environment.

Dependent Module Updates
^^^^^^^^^^^^^^^^^^^^^^^^

SIMP updated as many dependent modules as possible. This included major version
bumps for several of the dependent modules. These changes did not have
a significant impact on the SIMP infrastructure. The dependency version bumps
did, however, require some of the SIMP modules to update their respective
:file:`metadata.json` files.  These metadata changes, in turn, required SIMP
module version updates.


Security Announcements
----------------------

.. contents::
  :depth: 2
  :local:

SIMP 6.5.0 Added mitigations for the following CVEs:

* :cve:`CVE-2020-7942`
* :cve:`CVE-2019-14287`
* :cve:`CVE-2019-6477`

RPM Updates
-----------

Puppet RPMs
^^^^^^^^^^^

The following Puppet RPMs are packaged with the SIMP 6.5.0 ISOs:

+-----------------------------+----------+
| Package                     | Version  |
+=============================+==========+
| :package:`puppet-agent`     | 6.18.0-1 |
+-----------------------------+----------+
| :package:`puppet-bolt`      | 2.29.0-1 |
+-----------------------------+----------+
| :package:`puppetdb`         | 6.12.0-1 |
+-----------------------------+----------+
| :package:`puppetdb-termini` | 6.12.0-1 |
+-----------------------------+----------+
| :package:`puppetserver`     | 6.13.0-1 |
+-----------------------------+----------+

.. WARNING::

   You do **NOT** need to update your version of Puppet from 5.X to use the
   modules supplied with this version of SIMP.

   If you decide to update from 5.X, back up your server and test the upgrade
   carefully.

Removed Puppet Modules
----------------------

.. contents::
  :depth: 2
  :local:

Unused Augeasproviders Modules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following packages for unused Augeasproviders Puppet modules and one
dependency were removed from the SIMP ISOs:

* :package:`pupmod-herculesteam-augeasproviders_apache`
* :package:`pupmod-herculesteam-augeasproviders_mounttab`
* :package:`pupmod-herculesteam-augeasproviders_nagios`
* :package:`pupmod-herculesteam-augeasproviders_pam`
* :package:`pupmod-herculesteam-augeasproviders_postgresql`
* :package:`pupmod-herculesteam-augeasproviders_puppet`
* :package:`pupmod-herculesteam-augeasproviders_shellvar`
* :package:`pupmod-puppetlabs-mount_providers`

Docker Modules
^^^^^^^^^^^^^^

The packages for the following Docker Puppet modules have been permanently
removed from the SIMP ISOs, because SIMP is moving towards :program:`podman`
support over :program:`docker`.

* :package:`pupmod-puppetlabs-docker`
* :package:`pupmod-simp-simp_docker`

Out-of-date Modules
^^^^^^^^^^^^^^^^^^^

The packages for the following SIMP profile Puppet modules and one dependent
module were temporarily removed from SIMP 6.5.0 ISOs, because they were not
able to be appropriately updated in time for the release:

* :package:`pupmod-puppet-gitlab`
* :package:`pupmod-simp-simp_gitlab`
* :package:`pupmod-simp-simp_snmpd`

These modules are expected to be updated in future SIMP releases.

pupmod-simp-journald
^^^^^^^^^^^^^^^^^^^^

The :package:`pupmod-simp-journald` package has been removed from SIMP ISOs,
because the functionality the :pupmod:`simp/journald` module provided is now
provided by the :pupmod:`camptocamp/systemd` module. If you used
:pupmod:`simp/journald`, you will need to update your manifests to use
:pupmod:`camptocamp/systemd`.


Fixed Bugs
----------

.. contents::
  :depth: 2
  :local:

pupmod-simp-aide
^^^^^^^^^^^^^^^^

* Fixed a bug in Compliance Engine data.

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the :program:`auditd` service was managed when the kernel
  was not enforcing auditing.
* Fixed a bug in which the facts were not properly confined.
* Fixed a bug in which :file:`/etc/audit/audit.rules.prev` caused unnecessary
  flapping.
* Fixed regex substitution for bad path characters.
* Added missing :pupmod:`herculesteam/augeasproviders_grub` module dependency.

pupmod-simp-dconf
^^^^^^^^^^^^^^^^^

* Fixed a bug in :code:`ensure = absent` in :code:`dconf::settings`.

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

* Fixed a bug in which the module did not check for :program:`firewalld`
  availability when :code:`iptables::use_firewalld` was set to :code:`true`.

  * The module now ensures that systems that do not have :code:`firewalld`
    do not attempt to configure it.

* Fixed bugs in :program:`iptables` rule address normalization:

  * Ensured that all addresses are normalized when rules are processed.
  * Removed nested looped rule normalization of addresses since it is no longer
    required.
  * Fixed :code:`normalize_addresses()` so that it simply grabs the netmask if
    present and slaps on the appropriate one if not.

* Fixed some bugs in the :code:`munge()` portions of the native types.

pupmod-simp-libvirt
^^^^^^^^^^^^^^^^^^^

* Fixed issues with module data.

pupmod-simp-logrotate
^^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the 'size' parameter in the global :program:`logrotate`
  configuration file was specified more than once.

pupmod-simp-network
^^^^^^^^^^^^^^^^^^^^^

* Fix a bug where both the legacy network and :program:`NetworkManager` were
  activated in all cases.

pupmod-simp-nfs
^^^^^^^^^^^^^^^

* Fixed a bug in which IPv6 '::1' network entries were not being created in
  :file:`/etc/exports`.  This could cause connections over :program:`stunnel`
  to fail under certain conditions.

* :program:`rpc.rquotad` service configuration was erroneously written to
  :file:`/etc/sysconfig/nfs` for EL7. It is now written to the correct file,
  :file:`/etc/sysconfig/rpc-rquotad`.
* Fixed :program:`idmapd`-related bugs:

  * :program:`idmapd` was erroneously only enabled when NFSv3 was allowed.
    :program:`idmapd` is an NFSv4 service.
  * The :program:`idmapd` client was not configured to use :program:`nfsidmap`.
    An :program:`nfsidmap` entry has now been added to
    :file:`/etc/request-key.conf`.

* Fixed bugs in which bidirectional communication for NFSv3 was not properly
  configured.

  * NFSv3 lockd ports on the NFS client were not explicitly configured and
    thus not allowed through the firewall.  This would have affected file
    locking using NLM.
  * :program:`rpcbind`, :program:`statd`, and :program:`lockd` service names
    were not allowed by TCP Wrappers for the NFS client. This would have
    affected server to client NFSv3 NSM and NLM protocol messages over TCP.

* Fixed bugs in mount options

  * Previously used the deprecated 'nfs4' fstype.  This has been replaced with
    the 'nfs' fstype and use of the 'nfsvers' option to specify the version of
    NFS to use.
  * The mount option 'proto' is now set to 'tcp' when :code:`stunnel` is
    enabled.

* Fixed a bug with a duplicate exec resource in :code:`nfs::client::mount` when
  :program:`stunnel` was enabled.

* Fixed erroneous server-only/client-only configuration that appeared to be
  able to be set independently for the NFS client and NFS server on the same
  node, but because of shared services, actually applied to the node as a
  whole.

  * Removed :code:`nfs::client::firewall` and :code:`nfs::server::firewall`.
    Use :code:`nfs::firewall` instead.
  * Removed :code:`nfs::server::tcpwrappers`. Use :code:`nfs::tcpwrappers`
    instead.
  * Removed :code:`nfs::server::nfsv3`, :code:`nfs::server::lockd_arg`,
    :code:`nfs::server::statdarg`, :code:`nfs::server::statd_ha_callout`,
    :code:`nfs::server::rpcgssdargs`, and :code:`nfs::server::rpcsvcgssdargs`.
    Use appropriate parameters in the :code:`nfs` class instead.

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Fixed a bug in which a local user password could not be set.

  * Moved the 'pam_unix.so' check before the 'pam_sss.so' check in the
    password section of the auth files otherwise it returns an
    ``authentication token manipulation`` error and local passwords cannot be
    changed.

pupmod-simp-polkit
^^^^^^^^^^^^^^^^^^

* Fixed issue with :code:`basic_policy` template that resulted in malformed
  rules.

pupmod-simp-pupmod
^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the module could not determine the appropriate Puppet
  configuration for Puppet >= 6.19.0 from the internal :code:`Puppet.settings`
  method, because the 'master' section was renamed  to 'server'.
* Fixed a bug on EL6 nodes in which setting :code:`pupmod::master::generate_types`
  to :code:`false` caused the catalog compilation to fail.
* Fixed a bug in :program:`puppetserver` configuration in which the
  'profiler-output-file' parameter was incorrectly specified as
  'profiling-output-file'.
* Fixed a bug in managing group ownership of :file:`puppet.conf` when using
  Puppet Enterprise.

  * Ensured that :code:`pupmod::pass_two` does not conflict with the internal
    :term:`PE` configuration code for group ownership of :file:`puppet.conf`.

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

* Fixed the default security collection string for :program:`firewalld` rules.
* Fixed a bug where the 'IncludeConfig' directive for :file:`/etc/rsyslog.d`
  allowed more than just :file:`.conf` files to be parsed.

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* Ensure that the :program:`sudoers` rule for removing the Puppet SSL directory
  is not created when running from Bolt, since the directory target is changed
  at each Bolt run and will result in non-idempotency.
* Fixed a bug in which the 'gpgkey' and 'baseurl' configuration strings were
  required for the local YUM repositories managed by
  :code:`simp::yum::repo::local_os_updates` and :code:`simp::yum::repo::local_simp`.

  - Both are optional in the :code:`yumrepo` type if they already exist on disk.

* Removed the broken :file:`tasks/` directory.

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

* Fixed the use of :code:`simplib::debug::inspect` when using Bolt.
* Fixed bugs in the :code:`grub_version` and :code:`init_systems` facts.
* Fixed the :code:`simplib__auditd` fact so that it detects the state of the
  running :program:`auditd` process.
* Fixed :code:`Simplib::Systemd::ServiceName` to accept instance services.
* Fixed an issue in the :code:`simplib__sshd_config` fact that would cause the
  daemon to start on an EL6 system that did not already have it running.
* Fixed a bug in which :code:`simplib__firewalls` fact was not properly confined
  and would trigger on Windows+  systems.
* Fixed an issue in :code:`simplib::ip::family_hash` where the 'unknown' entries
  were not properly populated.
* Fixed bug in which :code:`simplib::simp_version` did not work on Windows.
* Fixed ``uninitialized constant`` error with the :code:`reboot_notify` custom
  type.

pupmod-simp-simp_options
^^^^^^^^^^^^^^^^^^^^^^^^

* Fixed :term:`PE` detection in :code:`simp_options::puppet::server_distribution`.

pupmod-simp-simp_snmpd
^^^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the PID file option was missing from the default options
  for the :program:`snmpd` daemon in EL6.  The daemon failed to start without
  this option.

* Fixed a bug where the default for access security level was incorrectly set.

  * The default access security level is now by the new parameter
    :code:`simp_snmpd::defvacmlevel` instead of
    :code:`simp_snmpd::defsecuritylevel`.
  * :code:`simp_snmpd::defsecuritylevel` sets the default security
    level for the client.

* Added a missing dependency on :pupmod:`simp/tcpwrappers`.

pupmod-simp-stunnel
^^^^^^^^^^^^^^^^^^^

* Added the :code:`stunnel::instance_purge` class to remedy the
  'floating services' issue.

pupmod-simp-tftpboot
^^^^^^^^^^^^^^^^^^^^

* Fixed a bug in which the internal :program:`rsync` operation did not match the
  documentation.
* Fixed a manifest ordering issue.

pupmod-simp-tlog
^^^^^^^^^^^^^^^^

* Fixed a bug in the :program:`tcsh` template.
* Added a workaround to scripts in :file:`/etc/profile` to handle a bug in
  :program:`tlog` that would prevent logins if the system hostname could
  not be found.

pupmod-simp-tpm2
^^^^^^^^^^^^^^^^

* Fixed a bug where the :program:`tpm2_*` commands could return nothing which
  would trigger an error in further logic.

pupmod-simp-xinetd
^^^^^^^^^^^^^^^^^^

* Removed 'TRAFFIC' from the default :code:`log_on_success` list since it may
  cause information leakage and is not supported by all service types.

rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Fixed a bug in which :command:`simp config` did not allow DNS domains that
  did not include at least one dot character.  Domains are now validated per
  RFC 3696.
* Fixed a bug where :command:`simp config` recommended the wrong SSSD domain,
  when the SIMP server was not the LDAP server.  It recommended the 'Local'
  domain, when the appropriate SIMP-created domain with the 'local' (EL6) or
  'files' (EL7) provider is named 'LOCAL'.
* Fixed a bug in :command:`simp environment new` in which the actual failure
  messages from a failed :command:`setfacl --restore` execution were not logged.
* Fixed a bug where :command:`simp config --dry-run` would prompt the user to
  apply actions instead of skipping them and then writing the
  :file:`~/.simp/simp_conf.yaml` file.

  * Users would answer 'no' to the unexpected apply query and then
    :program:`simp config` would only persist the answers to the interim
    answers file (:file:`~/.simp/.simp_conf.yaml`).

* Fixed Puppet Enterprise support for :command:`simp config` and
  :command:`simp bootstrap`.

  * Fixed a fact-loading bug that prevented the :term:`PE` fact (:code:`is_pe`)
    from being available.
  * Hardened PE-detection logic for cases in which the :code:`is_pe` fact is
    not yet available during :command:`simp config`.
  * Added support for SIMP server template Hiera data that is PE-specific.
  * Fixed a bug in which the module paths containing PE modules were not
    excluded when :command:`simp config` checked for modules in the 'production'
    Puppet environment. This forced the user to remove the skeleton
    'production' environment installed by the :package:`puppet-agent` RPM, in
    order to get :command:`simp config` to run on a freshly installed PE system.

simp-environment-skeleton
^^^^^^^^^^^^^^^^^^^^^^^^^

* When running FakeCA certification-generation scripts in batch mode, do not
  request input from the user.
* Fixed a bug in which some non-script files were installed with executable
  permissions.

simp-utils
^^^^^^^^^^

* Fixed minor bugs in :program:`unpack_dvd`.


.. _changelog-6-5-0-new-features:

New Features
------------

.. contents::
  :depth: 2
  :local:

pupmod-simp-aide
^^^^^^^^^^^^^^^^

* Updated the EL8 ciphers to be safe on FIPS systems by default.
* Removed overrides for :code:`aide::aliases` on EL8 since it works properly
  in FIPS mode.
* Automatically add '@@include' lines to :file:`aide.conf`.  Previously, when
  declaring :code:`aide::rule` resources, it was also necessary to add the
  rule name to the :code:`aide::rules` array.
* Moved the default rules to data in modules.

pupmod-simp-auditd
^^^^^^^^^^^^^^^^^^

* Allow :code:`auditd::space_left` and :code:`auditd::admin_space_left` to
  accept percentages on supported versions.
* Added 'INCREMENTAL_ASYNC' to possible values for :code:`auditd::flush`.
* Added a :code:`built_in` audit profile to the subsystem that provides ability
  to include and manage sample rulesets to be compiled into active rules.
* Ensured that :program:`kmod` is audited in all STIG modes on EL7+.
* Allow users to knockout entries from arrays specified in Hiera.
* Added rules based on best practices mostly pulled from
  :file:`/usr/share/doc/auditd`:

  * Audit 32 bit operations on 64 bit systems
  * Audit calls to the :program:`auditd` CLI commands
  * Audit IPv4 and IPv6 inbound connections
  * Optionally audit IPv4 and IPv6 outbound connections
  * Audit suspicious applications
  * Audit systemd
  * Audit the :program:`auditd` configuration space
  * Ignore time daemon logs (clutter)
  * Ignore 'CRYPTO_KEY_USER' logs (clutter)
  * Add ability to set the 'backlog_wait_time'
  * Set 'loginuid_immutable'

* Set defaults for syslog parameters if :program:`auditd` version is unknown.
* Added a fact that determines the major version of :program:`auditd` that is
  running on the system, :code:`auditd_major_version`.  This is used in the
  :file:`hiera.yaml` hierarchy to add module data specific to the versions.
* Added support for :program:`auditd` v3.0 which is used by RedHat 8.  Most of
  the changes in :program:`auditd` v3.0 were related to how the plugins are
  handled but there are a few new parameters added to :file:`auditd.conf`. They
  are set to their defaults according to :program:`man` page of
  :file:`auditd.conf`.

  * :program:`auditd` V3.0 moved the handling of plugins into :program:`auditd`
    from :program:`audispd`.  The following changes were made to accommodate
    that:

    * To make sure the parameters used to handle plugins where defined in
      one place no matter what version of :program:`auditd` was used, they were
      moved to :file:`init.pp` and referenced from there by the :code:`audisp`
      manifest.  For backwards compatibility, they remain in :file:`audisp.conf`
      and are aliased in the Hiera module data.
    * For backwards compatibility :code:`auditd::syslog` remains defaulting to
      the value of :code:`simp_options::syslog` although the two are not really
      the same thing. You might want to review this setting and set
      :code:`auditd::syslog` to a value that is appropriate for your system.

      * To enable :program:`auditd` logging to syslog, set the following in
        Hiera

        .. code-block:: yaml

           ---
           auditd::syslog: true
           auditd::config::audisp::syslog::enable: true.
           # The drop_audit_logs is still there for backwards compatibility and
           # needs to be disabled.
           auditd::config::audisp::syslog::drop_audit_logs: false

      * To stop :program:`auditd` logging to syslog set the following in Hiera

        .. code-block:: yaml

           ---
           auditd::syslog: true
           auditd::config::plugins::syslog::enable: false.

      * Setting :code:`auditd::syslog` to :code:`false` will stop Puppet from
        managing the :file:`syslog.conf`, it will not disable :program:`auditd`
        logging to syslog.  Disable the syslog plugin as described above.

    * The settings for :file:`syslog.conf` were updated to work for new and old
      versions of :program:`auditd`.
    * Added installation of :package:`audisp-syslog` package when using
      :program:`auditd` V3.

* Added rules to monitor :file:`/usr/share/selinux`.

pupmod-simp-autofs
^^^^^^^^^^^^^^^^^^

This module was extensively refactored. Please read the updated :file:`README.md`
to understand the current usage.  Notable feature/API changes:

* Updated :program:`autofs` service configuration to use :file:`/etc/autofs.conf`
  in addition to :file:`/etc/sysconfig/autofs`.
* Updated :file:`/etc/autofs.master` to load content from
  :file:`/etc/auto.master.simp.d/` and :file:`/etc/auto.master.d/` in lieu of
  specifying map entries directly.

  * 'auto.master' entries are now written to files in
    :file:`/etc/auto.master.simp.d`, a directory fully managed by this module.
  * :file:`/etc/auto.master.d` is left unmanaged by Puppet.
  * Auto-converts from old maps directory to current maps directory and
    emits a warning. This is to help the 90% of the users who aren't doing
    anything special with this module.

* Added a :code:`autofs::map` defined type that allows the user to specify all
  the parameters for a 'file' map in one place.  This resource will
  generate the appropriate resources to create both the 'auto.master' entry
  file and the map file.
* Added :code:`autofs::masterfile` defined type to replace deprecated
  :code:`autofs::master::map`.

  * :code:`autofs::masterfile` creates an 'auto.master' entry file in
    :code:`autofs::master_conf_dir`.
  * Unlike :code:`autofs::map::master`, :code:`autofs::masterfile` does not have
    a :code:`content` parameter, because a user can simply use a :code:`file`
    resource to specify a custom 'auto.master' entry file.

* Added :code:`autofs::mapfile` defined type to replace deprecated
  :code:`autofs::master::entry`.

  * :code:`autofs::mapfile` creates a mapfile for a direct mapping or one or
    more indirect mappings.
  * Unlike :code:`autofs::master::entry`, it does not have duplicate resource
    naming problems (wildcard or otherwise).

* :code:`autofs` class changes

  * Added the following new :program:`autofs` service configuration parameters:

    * :code:`master_wait`
    * :code:`mount_verbose`
    * :code:`mount_nfs_default_protocol`
    * :code:`force_standard_program_map_env`
    * :code:`use_hostname_for_mounts`
    * :code:`disable_not_found_message`
    * :code:`sss_master_map_wait`
    * :code:`use_mount_request_log_id`
    * :code:`auth_conf_file`
    * :code:`custom_autofs_conf_options`

  * Added :code:`master_conf_dir` and :code:`master_include_dirs` parameters to
    allow users to specify directories containing 'auto.master' entry files.
  * Added :code:`maps_dir` to specify the location of SIMP-managed maps and
    changed the directory name from :file:`/etc/autofs` to
    :file:`/etc/autofs.maps.simp.d` for clarity.
  * Added :code:`maps` to allow users to specify 'file' type maps in Hiera data.

    * Each map specifies the contents of a 'auto.master` entry file and its
      corresponding mapping file.

  * Renamed :code:`options` to :code:`automount_options` for clarity.
  * Renamed :code:`use_misc_device` to :code:`automount_use_misc_device` for
    clarity.
  * Removed :code:`autofs::master_map_name`.

    * This parameter is not exposed in :file:`/etc/autofs.conf` and does not
      look like it is intended to be changed.

  * Changed permissions of :file:`/etc/auto.master` and
    :file:`/etc/sysconfig/autofs` to match those of the delivered RPM.

* :code:`autofs::ldap_auth` class changes

  * :code:`autofs::ldap_auth` is now a private class to ensure the name of the
    configuration file created by this class matches the 'auth_conf_file'
    setting in :file:`/etc/autofs.conf`.
  * Added :code:`encoded_secret` optional parameter.  This parameter takes
    precedence when both :code:`secret` and :code:`encoded_secret` parameters
    are specified.

* :code:`autofs::map::master` has been deprecated by :code:`autofs::map` or
  :code:`autofs::masterfile`.  Its behavior has changed from writing a section
  of :file:`/etc/auto.master` to writing an `auto.master` entry file
  in :code:`autofs::master_conf_dir`.
* :code:`autofs::map::entry` has been deprecated by :code:`autofs::map` or
  :code:`autofs::mapfile`.  Its behavior has changed from writing a file in
  :file:`/etc/autofs` to writing a file in :code:`autofs::maps_dir`.

pupmod-simp-clamav
^^^^^^^^^^^^^^^^^^

* Updated documentation to clarify what :code:`simp_options::clamav` actually
  does and to note that :code:`clamav` was removed from the SIMP's default class
  list in SIMP 6.5.
* Set the default for :code:`clamav::set_schedule::enable` to lookup
  :code:`clamav::enable`, so that the class will remove the 'clamscan'
  :program:`cron` job if management of ClamAV is disabled.
* Disable SIMP's :program:`rsync` pulls by default.

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
  * Removed useless loops in :code:`list_puppet_params()`.

* Improved error handling and debugging:

  * Ignore undefined 'ces' when correlating checks and profiles.
  * Raise errors on malformed data.
  * Added debugging logs to enforcement logic.

* Removed all support for v1 data since it was experimental and removed in
  3.0.0.

* Load data from the :code:`compliance_markup::compliance_map` Hiera key
  after compliance profiles in modules to allow for profile tailoring via
  Hiera. This means that uses may now override all settings from the underlying
  compliance maps across all modules to fit their environment specifics.

pupmod-simp-cron
^^^^^^^^^^^^^^^^

* Manage :program:`cron` packages by default.

pupmod-simp-crypto_policy
^^^^^^^^^^^^^^^^^^^^^^^^^

This is a new module to manage, and provide information about, the system-wide
crypto policies.

pupmod-simp-dconf
^^^^^^^^^^^^^^^^^

* Allow users to set custom settings via Hiera.

pupmod-simp-deferred_resources
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Remove 'ftp' and 'games' users and groups when enforcing STIG compliance.

pupmod-simp-dhcp
^^^^^^^^^^^^^^^^

* Made use of SIMP's :program:`rsync` operation optional (enabled by default for
  backwards compatibility).
* Added support for passing in a full :file:`dhcpd.conf` entry.
* Ensured that the SELinux user and type are set for the configuration files.
* Switched to using :code:`iptables::listen::udp` for :program:`firewalld`
  compatibility.

pupmod-simp-fips
^^^^^^^^^^^^^^^^

* Ensured that EL8 updates trigger updating the global system crypto policy,
  since some subsystems now ignore the local configuration by default.

pupmod-simp-freeradius
^^^^^^^^^^^^^^^^^^^^^^
* Added support for overriding 'post-auth' in LDAP.
* Added support for overriding 'accounting' in LDAP.
* Added support for specifying the entire file content.
* Removed :code:`simp_options::puppet::server` from the default lookup logic
  for :code:`freeradius::v3::modules::ldap::server`. In systems that use Bolt
  to compile and apply manifests, that setting will not be available.

pupmod-simp-incron
^^^^^^^^^^^^^^^^^^

* Remove pinned versions of :package:`incron`, since the upstream packages have
  been fixed.

pupmod-simp-iptables
^^^^^^^^^^^^^^^^^^^^

* Added preliminary support for acting as a pass-through to various
  :program:`firewalld` capabilities using the :pupmod:`simp/simp_firewalld`
  module.

  * Using any of the :code:`iptables::listen::*` defined types will work
    seamlessly in 'firewalld' mode but direct calls to
    :code:`iptables::rule` will fail.
  * Calls to any of the native types included in this module will result in
    undefined behavior and is not advised.
  * To enable 'firewalld' mode on supported operating systems, simply set
    :code:`iptables::use_firewalld` to :code:`true` via Hiera.
  * EL 8 systems will enable 'firewalld' mode by default.

* Improved the internal rule matching to handle most netmask and port updates.
* Added a :code:`exact_match` Boolean to the :code:`iptables_optimize` and
  :code:`ip6tables_optimize` native types to allow for more aggressive rule
  matching.

  * This change requires that inbound rules match whatever is returned by
    :program:`iptables-save` and/or :program:`ip6tables-save` to prevent
    :program:`iptables` flapping.

* Allow 'LOCAL-INPUT' jump rule in 'FORWARD' and 'INPUT' chains to occur last as
  a default action through the addition of an
  :code:`iptables::rules::base::force_local_input` parameter.
* Allow users to disable adding the 'SIMP:' prefix to the rule comment.
* Allow users to disable comments on rules completely.

pupmod-simp-krb5
^^^^^^^^^^^^^^^^

* Updated SELinux hotfix for EL8.
* Migrated SELinux hotfix to :code:`vox_selinux::module`.

pupmod-simp-libreswan
^^^^^^^^^^^^^^^^^^^^^

* Removed unused :code:`libreswan::use_certs_parameter` parameter.
* Added support for IKEv2 Mobility (RFC-4555) and mobile client connections.
* Added additional settings for DNS and Domains for Libreswan v3.23+.

pupmod-simp-libvirt
^^^^^^^^^^^^^^^^^^^

* Split out install and service into separate classes to give users more
  flexibility on what they manage with the module.

pupmod-simp-logrotate
^^^^^^^^^^^^^^^^^^^^^

* Allow all log size configuration parameters to be specified in bytes,
  kilobytes, megabytes, or gigabytes.
* Added ability to specify 'maxsize' configuration for specific
  :program:`logrotate` rules.

pupmod-simp-named
^^^^^^^^^^^^^^^^^

* Allow users to force enabling/disabling of the :program:`chroot` settings.
* Allow users to easily set the :code:`named_write_master_zones` SELinux boolean in
  case they need to support dynamic DNS or zone transfers.

pupmod-simp-nfs
^^^^^^^^^^^^^^^

This module was extensively refactored. Read the updated :file:`README.md` to
understand the current usage.  Notable feature/API changes:

* Overall changes

  * Dropped :program:`stunnel` support for NFSv3.  This tunneling did not work
    because:

    * The NFS client sends the NFS server Network Status Manager (NSM)
      notifications via UDP, exclusively.
    * At multi-NFS-server sites, a unique rpcbind port per server is
      required in order for a NFS client to be able to tunnel its
      server-specific RPC requests to the appropriate server.

  * :code:`nfs` class

    * Reworked parameters to reflect configuration of :file:`/etc/nfs.conf` and,
      for limited EL7-only configuration, :file:`/etc/sysconfig/nfs`.  See the
      class documentation for full details.

  * Removed :code:`stunnel_systemd_deps` and :code:`stunnel_tcp_nodelay`
    parameters throughout the module.

    * These parameters were not consistently used in the manifest
      code (i.e., declared but not used) and were confusing.
    * The corresponding :code:`stunnel_socket_options` and
      :code:`stunnel_wantedby` parameters in classes/defines now use defaults
      that were intended to be set by those parameters.

  * Now masks NFS services that are not needed, so they are not unnecessarily
    started when the :program:`nfs-server.service` or
    :program:`nfs-client.target` are restarted.

* :code:`nfs::client` changes

  * Added support for pNFS:  Set :code:`blkmap` to true to enable the pNFS
    service, :program:`nfs-blkmap.service`.
  * Added :code:`nfs::stunnel_socket_options` and :code:`stunnel_wantedby`
    parameters which provide the defaults for all :code:`nfs::client::mount`
    instances.

* :code:`nfs::client::mount` define changes

  * :code:`nfs_server` must now be specified as an IP address.  This change was
    necessary for :program:`firewalld`.
  * In :code:`options`, changed the default mount type to 'soft' instead of
    'hard'.  Also removed deprecated 'intr' option, as it has no effect.
  * Reworked the remote autodetect logic to detect a local mount based
    on IP address instead of simply whether the node is also configured
    to be an NFS server.
  * Added support for direct autofs mounts and simplified specification of
    indirect mounts.  When :code:`autofs_indirect_map_key` is not specified, a
    direct mount is specified by :code:`name`.  When
    :code:`autofs_indirect_map_key` is specified, an indirect mount is specified
    with :code:`name` as the mount point and :code:`autofs_indirect_map_key` as
    the mount key.
  * Renamed :code:`autofs_map_to_user` to :code:`autofs_add_key_subst` to better
    reflect automount terminology. This parameter simply adds key substitution
    to the remote location, which although can be used for user home
    directories, is not restricted to that use case.
  * Renamed :code:`port` to :code:`nfsd_port` to be consistent with the name of
    that parameter throughout the entire module.
  * Renamed :code:`v4_remote_port` to :code:`stunnel_nfsd_port` for clarity and
    to be consistent with the name of that parameter throughout the entire
    module.
  * Exposed client :program:`stunnel` configuration that was scattered
    throughout the module to this API.  User can now specify
    :code:`stunnel_socket_options` and :code:`stunnel_verify` for each mount.
    When unspecified, the defaults from the :code:`nfs` class are used.

* :code:`nfs::server` class changes

  * Exposed server :program:`stunnel` configuration that was scattered
    throughout the module to this API.  User can now specify
    :code:`stunnel_accept_address`, :code:`stunnel_nfsd_acccept_port`,
    :code:`stunnel_socket_options`, :code:`stunnel_verify`, and
    :code:`stunnel_wantedby` in this class. When unspecified, the defaults for
    all but :code:`stunnel_accept_address` and
    :code:`stunnel_wantedby` are pulled from the :code:`nfs` class.
  * Added the following parameters: :code:`nfsd_vers4`, :code:`nfsd_vers4_0`,
    :code:`nfsd_vers4_1`, :code:`nfsd_vers4_2`, and
    :code:`custom_rpcrquotad_opts`.
  * Renamed :code:`nfsv3` to :code:`nfsd_vers3` to reflect its use in
    :file:`/etc/nfs.conf`.
  * Moved :code:`nfs::rpcquotad_port` to this class and renamed
    :code:`rpcrquotadopts` to :code:`custom_rpcrquotad_opts` for clarity.
  * Moved :code:`nfs::mountd_port` to this class and removed
    :code:`rpcmountdopts`.  Custom configuration for that daemon should now be
    made via :code:`nfs::custom_nfs_conf_opts` or :code:`nfs::custom_daemon_args`
    as appropriate.
  * Removed the obsolete :code:`nfsd_module` parameter.

* :code:`nfs::server::export` define changes

  * Added :code:`replicas`, :code:`pnfs`, and :code:`security_label` parameters
    to support additional export configuration parameters.

* :code:`nfs::idmapd` class changes

  * Refactored into 3 classes to support distinct NFS server and client
    configuration
  * Added :code:`no_strip` and :code:`reformat_group` to
    :code:`nfs::idmapd::config` to support additional
    :file:`/etc/idmapd.conf` configuration parameters.

pupmod-simp-oath
^^^^^^^^^^^^^^^^

* Allow :code:`oath::config::user` to be any string.
* Disabled :code:`show_diff` option in :code:`concat` for
  :file:`/etc/liboath/users.oath` to prevent that information from being exposed
  in logs.

pupmod-simp-pam
^^^^^^^^^^^^^^^

* Ensured that 'pam_tty_audit' is optional if auditing is not enabled on the
   system.
* Added the ability to specify :code:`pam::limits::rules` via Hiera.
* Ignore :program:`authconfig` disable on EL8. Authconfig was replaced with
  :program:`authselect` and :program:`authselect` does not overwrite settings
  unless you select the :code:`--force` option.
* Remove installation of :package:`pam_pkcs11` and :package:`fprintd-pam` by
  default, since they aren't actually required for basic functionality.

pupmod-simp-polkit
^^^^^^^^^^^^^^^^^^

* Added the following classes:

  * :code:`polkit::install`
  * :code:`polkit::service`
  * :code:`polkit::use`

* Ensured that the polkit user is managed by default and placed into the
  supplementary group bound to the 'gid' option on :file:`/proc`, if one
  is set.  This is necessary to work around issues with 'hidepid' > 0.
* Made the entire main class inert on unsupported OSs; logs a warning on the
  server that can be disabled.

pupmod-simp-pupmod
^^^^^^^^^^^^^^^^^^

* Default :code:`pupmod::master::ssl_protocols` to TLSv1.2 only.
* Use :code:`$facts['certname']`, when available, in the parameters below,
  because :code:`$facts['fqdn` may not be appropriate when the system does not
  use its primary NIC/FQDN for its Puppet certificate.

  * :code:`pupmod::certname`
  * :code:`pupmod::master::ca_status_whitelist`
  * :code:`pupmod::master::admin_api_whitelist`

* Set the default :program:`puppetserver` ciphers to a safe set.
* Added better auto-tuning support for :program:`puppetserver`, based on best
  practices.
* Added 'ReservedCodeCache' :program:`puppetserver` support.
* Removed :program:`incron` support in favor of using :program:`systemd` path
  units to run :program:`simp_generate_types`.

  * Attempts to activate the :program:`incron` code will result in a warning
    message.

* Added mitigation for :cve:`CVE-2020-7942`
* Added optional management of the Facter configuration file.
* Removed the deprecated CA CRL pull :program:`cron` job and the corresponding
  :code:`pupmod::ca_crl_pull_interval` parameter.
* Removed deprecated :file:`auth.conf` support for the legacy pki module and
  the corresponding parameters:

  * :code:`pupmod::master::simp_auth::legacy_cacerts_all`
  * :code:`pupmod::master::simp_auth::legacy_mcollective_all`
  * :code:`pupmod::master::simp_auth::legacy_pki_keytabs_from_host`

* Removed the deprecated :code:`pupmod::master::simp_auth::server_distribution`
  parameter.

pupmod-simp-resolv
^^^^^^^^^^^^^^^^^^

* Added optional management of DNS servers via :program:`nmcli`.

pupmod-simp-rsyslog
^^^^^^^^^^^^^^^^^^^

* Added support for 'KeepAlive' variables for 'imtcp' and 'omfwd' actions.
* Changed local rule defined type to use the same package defaults for
  action queues that are in the remote rule defined type.
* Changed remote rule defined type to use package defaults for action
  queues.
* Added a default rule to log packets dropped by :program:`firewalld` to
  :file:`/var/log/firewall.log`.
* Added :file:`/var/log/firewall.log` to SIMP's 'syslog' :program:`logrotate`
  rule.
* Added :code:`logrotate::rule` options to :code:`rsyslog::conf::logrotate`
  class.
* Removed the :code:`filter_` rules that were present for an old (and broken)
  version of the :pupmod:`simp/simp_firewalld` module.
* Removed params pattern and migrated to data in modules.

pupmod-simp-selinux
^^^^^^^^^^^^^^^^^^^

* No longer enable or install :program:`mcstransd` by default.  It is a user
  convenience feature and not required for core functionality.
* Ensured that :program:`mcstransd` is added to the GID assigned to
  :file:`/proc` if one is assigned on the system.

pupmod-simp-simp
^^^^^^^^^^^^^^^^

* :program:`sssd` configuration updates

  * Configure the 'files' provider in lieu of the 'local' provider for EL7 and
    later.
  * Deprecated the following parameters in :code:`simp::sssd::client`:
    :code:`autofs`, :code:`ssh` and :code:`sudo`.  The :pupmod:`simp/sssd`
    module configures services in :code:`sssd::services`.  Use that
    parameter to configure those entries.
  * Configure :program:`sssd` for EL8, even if the :code:`ldap_domain` and
    :code:`local_domain` parameters of :code:`simp::sssd::client` are set to
    :code:`false`.

* Updated :code:`simp::mountpoints::proc` to ensure :program:`polkitd` can be
  configured to have access to :file:`/proc`:

  * Assign a group and gid by default.
  * Create a group by default.
  * Discover these values from the system if possible.

* Removed the following applications from the list of base OS applications
  installed automatically by :pupmod:`simp/simp`:

  * :package:`man`
  * :package:`man-pages`
  * :package:`vim-enhanced`
  * :package:`dos2unix`
  * :package:`elinks`
  * :package:`hunspell`
  * :package:`lsof`
  * :package:`mlocate`
  * :package:`pax`
  * :package:`pinfo`
  * :package:`sos`
  * :package:`star`
  * :package:`symlinks`
  * :package:`words`
  * :package:`x86info`

* Deprecated the :code:`simp::base_apps::manage_elinks_config` parameter.

  * It no longer has any effect.

* :code:`simp::nsswitch` updates

  * Updated the :code:`simp::nsswitch` class to have sane defaults.

    * Added support for 'mymachines' and 'myhostname' by default.
    * Removed all NIS references since NIS should not be in general usage any
      longer and was never natively supported by SIMP.
    * Configuration files are now common across all supported OSs since
      :program:`nsswitch` "does the right thing" when it hits a module that it
      does not recognize.

  * Allow :program:`nsswitch` overrides.

* Added :program:`chronyd` support for EL8

  * Moved :package:`ntp` to list of OS relevant applications for EL6 and EL7.
  * Added :package:`chrony` for EL8.

* Updated the client kickstart scripts/configuration

  * Updated the :program:`bootstrap_simp_client` script to use
    :program:`chronyd` if the kernel version is 4 or later.
  * Deprecated the :code:`simp::server::kickstart::runpuppet` parameter and
    removed the old, corresponding :program:`runpuppet` kickstart scripts.
    The :program:`simp_bootstrap_client` scripts should be used instead.

* ClamAV updates:

  * Removed :code:`clamav` from the list of classes included by default in the
    SIMP scenarios.

    * This will not remove ClamAV from systems where it is installed; Puppet
      will simply stop managing it.
    * To continue managing ClamAV with Puppet, add :code:`clamav` to
      :code:`simp::classes` in the appropriate Hiera file for that SIMP client.
    * See the :pupmod:`simp/clamav` module for information on configuring or
      removing ClamAV on a system.

  * Deprecated :code:`simp::server::clamav`.

    * This parameter will be removed in a future SIMP release.
    * To manage ClamAV on the SIMP server after the parameter is removed,
      manually add the :code:`clamav` class to the :code:`simp::classes` array
      in the SIMP server's Hiera file.

* :code:`simp::yum::repo*` updates:

  * Added:

    * :code:`simp::yum::repo::internet_simp` class:

      * Uses the SIMP yum repository package (:package:`simp-community-release`)
        to configure yum for SIMP's internet public repositories at
        `simp-project.com`_.
      * `simp-project.com`_ is the new host for SIMP's yum repositories.
      * `packagecloud`_ is no longer being updated.

    * :code:`simp::yum::repo::simp_release_version` function: Returns the SIMP
      release version for use in the SIMP internet yum repositories.
    * :code:`Simp::Version` data type alias for valid version strings for use in
      the SIMP internet repositories.

    * New parameters to :code:`simp::yum::repo::local_simp` and
      :code:`simp::yum::repo::local_os_updates`:

      * :code:`relative_repo_path`, :code:`baseurl`, and :code:`gpgkey`.
      * :code:`baseurl` and :code:`gpgkey` allow complete :code:`yumrepo`
        resource overrides.

  * Deprecated:

    * :code:`simp::yum::repo::internet_simp_server` and
      :code:`simp::yum::repo::internet_simp_dependencies` classes:

      * These resources are no longer useful because their API matches the OBE
        `packagecloud`_ SIMP repositories.
      * As a workaround, the classes have been modified to use
        :code:`simp::yum::repo::internet_simp` to configure the correct
        repositories at `simp-project.com`_.
      * You should switch to using :code:`simp::yum::repo::internet_simp`,
        directly, as these classes will be removed in a future release.

    * :code:`simp::yum::repo::sanitize_simp_release_slug` function: a function
      only useful to the deprecated classes.

* Added :code:`simp::puppetdb::cipher_suites` parameter to manage the
  cipher suites supported by PuppetDB's HTTP interface (Jetty).

  * Used to set :code:`puppetdb::cipher_suites`.
  * Value set to a safe set.

pupmod-simp-simp_banners
^^^^^^^^^^^^^^^^^^^^^^^^

* Removed all OS support statements from :file:`metadata.json`, since this is
  simply a data-only module.


pupmod-simp-simp_bolt
^^^^^^^^^^^^^^^^^^^^^

* Added plan to install :package:`puppet-agent` on target nodes.
* Configured Bolt to request a pseudo TTY for SSH sessions if specified.
* Configured new logs to be appended to the log file instead of overwriting.

pupmod-simp-simp_firewalld
^^^^^^^^^^^^^^^^^^^^^^^^^^

This is a new SIMP module that provides a profile class and defined type to
manage the system's :program:`firewalld` with "safe" defaults and safety checks
for :program:`firewalld` rules.  It uses the :pupmod:`puppet/firewalld` module to
update the system's :program:`firewalld` configuration.

pupmod-simp-simp_ipa
^^^^^^^^^^^^^^^^^^^^

* Make the IPA server optional in the :code:`join` task.  It is perfectly valid
  to not specify a server when doing an IPA client install and instead
  rely on DNS auto discovery.

pupmod-simp-simp_nfs
^^^^^^^^^^^^^^^^^^^^

* The following parameters had to be changed from hostnames or IP addresses
  to only IP addresses due to use of :program:`firewalld` on EL8:

  * :code:`simp_nfs::home_dir_server`
  * :code:`simp_nfs::mount::home::nfs_server`

pupmod-simp-simp_options
^^^^^^^^^^^^^^^^^^^^^^^^

* The :code:`simp_options::clamav` catalyst has been deprecated.

  * As of SIMP 6.5, SIMP's :code:`clamav` class is no longer included in the
    class list of the SIMP scenarios. So, this catalyst is not needed to
    disable it.
  * To have SIMP manage ClamAV on your system, add the :code:`clamav` class to
    your system's class list.
  * See the :pupmod:`simp/clamav` module :file:`README.md` for information on
    managing ClamAV.

* :code:`simp_options::puppet::server` and :code:`simp_options::puppet::ca` are
  now optional.

  * These are no longer required at all times due to support for Bolt. Code that
    used these parameters will correctly fail and require users to add them to
    their configuration.

* Updated :code:`simp_options::ldap` to require the :code:`master` and
  :code:`uri` parameters if :code:`simp_options::puppet::server` is not defined.

pupmod-simp-simp_rsyslog
^^^^^^^^^^^^^^^^^^^^^^^^

* Added support for :program:`firewalld` log message collection.
* Deep merge :code:`simp_rsyslog::log_collection`.
* Removed the :code:`filter_IN_99_simp_DROP` rules that were present for an old
  (and broken) version of the :pupmod:`simp/simp_firewalld` module.

pupmod-simp-simp_snmpd
^^^^^^^^^^^^^^^^^^^^^^

* Changes:

  * Updated to use :pupmod:`puppet/snmp` version 5.1.2.
  * The default configuration for this module has not changed but some settings
    are now placed in the :file:`snmpd.conf` file instead of in a subdirectory.
  * The user directory for :program:`snmpd` configuration,
    :file:`/etc/snmp/snmpd.d`, is not included by default.

    * :file:`/etc/snmp/simp_snmpd.d` is always included.
    * The new parameter :code:`simp_snmpd::include_userdir` must be set to
      :code:`true` to include :file:`/etc/snmp/snmpd.d`.

  * The configuration parameter :code:`simp_snmpd::snmpd_conf_file` has been
    renamed to :code:`simp_snmpd::service_config`.
  * The type of the :code:`simp_snmpd::services` parameter has been changed
    from a :code:`String` to an :code:`Integer`.
  * :code:`simp_snmpd::system_info`, :code:`simp_snmpd::contact`,
    :code:`simp_snmpd::location`, :code:`simp_snmpd::sysName`, and
    :code:`simp_snmpd::sysServices` have been deprecated.

    * These parameters are inert, because :pupmod:`puppet/snmpd` no longer
      supports this configuration.

* New features:

  * Added settings to allow users to change owner/group and permissions
    on configuration files:

    * :code:`simp_snmpd::service_config_dir_owner`
    * :code:`simp_snmpd::service_config_dir_group`
    * :code:`simp_snmpd::service_config_dir_perms`
    * :code:`simp_snmpd::service_config_perms`

  * Added configuration of :program:`snmpd` user and group IDs, as well
    as optional managment of the user and group:

    * :code:`simp_snmpd::snmpd_uid`
    * :code:`simp_snmpd::snmpd_gid`
    * :code:`simp_snmpd::manage_snmpd_user`
    * :code:`simp_snmpd::manage_snmpd_group`

  * Added :program:`snmpd` configuration parameters:

    * :code:`simp_snmpd::trap_service_config`
    * :code:`simp_snmpd::snmpdtrapd_options`

pupmod-simp-simpkv
^^^^^^^^^^^^^^^^^^

This is a new SIMP module that provides an abstract library that allows Puppet
to access one or more key/value stores.

This module provides

* a standard Puppet language API (functions) for using key/value stores
* a configuration scheme that allows users to specify per-application use
  of different key/value store instances
* adapter software that loads and uses store-specific interface software
  provided by the :pupmod:`simp/simpkv` module itself and other modules
* a Ruby API for the store interface software that developers can implement
  to provide their own store interface
* a file-based store on the local filesystem and its interface software.

  * Future versions of this module will provide a distributed key/value store.

pupmod-simp-simplib
^^^^^^^^^^^^^^^^^^^

Facts Changes
"""""""""""""

Added the following facts:

+--------------------------------------+--------------------------------------+
| Fact                                 | Description                          |
+======================================+======================================+
| :code:`simplib__auditd`              | Returns a hash of :program:`auditd`  |
|                                      | status.                              |
+--------------------------------------+--------------------------------------+
| :code:`simplib__firewalls`           | Return an array of known firewall    |
|                                      | commands that are present on the     |
|                                      | system.                              |
+--------------------------------------+--------------------------------------+
| :code:`simplib__mountpoints`         | Returns a hash of mountpoints of     |
|                                      | particular interest to SIMP modules. |
+--------------------------------------+--------------------------------------+
| :code:`simplib__numa`                | Returns a hash of NUMA values.       |
+--------------------------------------+--------------------------------------+
| :code:`simplib__efi_enabled`         | Returns :code:`true` if the host is  |
|                                      | using EFI.                           |
+--------------------------------------+--------------------------------------+
| :code:`simplib__secure_boot_enabled` | Returns :code:`true` if the host is  |
|                                      | using UEFI Secure Boot.              |
+--------------------------------------+--------------------------------------+

Deprecated the following facts:

* :code:`tmp_mounts` fact.  Use :code:`simplib__mountpoints`, instead.


Function Changes
""""""""""""""""

Added the following functions:

+--------------------------------------------------+--------------------------------+
| Function                                         | Description                    |
+==================================================+================================+
| :code:`simplib::debug::inspect`                  | Enhanced version of            |
|                                                  | :code:`simplib::inspect`.      |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::debug::classtrace`               | Prints a trace of all catalog  |
|                                                  | resources traversed to get to  |
|                                                  | the current point.             |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::debug::stacktrace`               | Prints a trace of all files    |
|                                                  | traversed to get to the        |
|                                                  | current point.                 |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::ip::family_hash`                 | Takes an IP address or array   |
|                                                  | of IP addresses and returns a  |
|                                                  | hash with the addresses        |
|                                                  | broken down by family. The     |
|                                                  | returned hash also contains    |
|                                                  | additional helpful metadata.   |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::module_metadata::os_blacklisted` | Determine if the passed        |
|                                                  | metadata indicates that the    |
|                                                  | current OS has been            |
|                                                  | blacklisted.                   |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::module_metadata::os_supported`   | Determine if the passed module |
|                                                  | metadata indicates that the    |
|                                                  | current OS is supported.       |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::module_metadata::assert`         | Adds an assertion based on     |
|                                                  | whether the OS is supported or |
|                                                  | blacklisted.                   |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::caller`                          | Determines what called a       |
|                                                  | function.                      |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::passgen::gen_password_and_salt`  | Generates a password and salt. |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::passgen::gen_salt`               | Generates a salt.              |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::passgen::get`                    | Retrieves a generated password |
|                                                  | and any stored attributes.     |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::passgen::list`                   | Retrieves the list of          |
|                                                  | generated passwords with       |
|                                                  | attributes and the list of     |
|                                                  | sub-folders stored at a        |
|                                                  | :code:`simplib::passgen`       |
|                                                  | folder.                        |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::passgen::remove`                 | Removes a generated password,  |
|                                                  | history and stored attributes. |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::passgen::set`                    | Sets a generated password with |
|                                                  | attributes.                    |
+--------------------------------------------------+--------------------------------+
| :code:`simplib::safe_filename`                   | Convert a string into a is     |
|                                                  | filename that 'path safe'.     |
+--------------------------------------------------+--------------------------------+

Updated the following functions:

* :code:`simplib::passgen`

  * Added 'simpkv' mode.

    * Runs in 'legacy' mode (default) or in a 'simpkv' mode.
    * 'simpkv' mode is **EXPERIMENTAL**.
    * When in 'simpkv' mode, :code:`simplib:passgen` uses :pupmod:`simp/simpkv`
      for password persistence.
    * 'simpkv' mode is enabled by setting :code:`simplib::passgen::simpkv` to
      :code:`true` in Hiera.
    * If you enable 'simpkv' mode in a system that already has passwords
      generated via the legacy code, currently, **all passwords will be
      regenerated**.
    * Added :code:`simpkv_options` parameter to :code:`simplib::passgen` for use
      in 'simpkv' mode.

  * Enhanced :code:`simplib::passgen` operation when in 'simpkv' mode

    * Stores :code:`complexity` and :code:`complex_only` setting in the
      password's simpkv metadata, so that the password can be regenerated with
      the same characteristics.
    * Regenerates the password if the requested 'complexity' or 'complex_only'
      setting differs from the setting used for the latest persisted password.
    * Stores up to the lastest 10 <password,salt> pairs in the password's
      simpkv metadata.

  * Added a :code:`gen_timeout_seconds` password option.  Previously this was
    hardcoded to 30 seconds.

  * Added ability to set the user and group for legacy
    :code:`simplib::passgen` files.
  * Changed the default permissions on legacy :code:`simplib::passgen` files
    to the user running the catalog compile.  This will allow bolt to set
    permissions correctly.

* :code:`simplib::gen_random_password`:

  * Intersperse special characters among the alpha-numeric characters,
    when :code:`complexity` is 1 or 2 and :code:`complex_only` is
    :code:`false`.  Previously, this function grouped the all alpha-numeric
    characters together and grouped all special characters together.  This
    generated passwords that were not suitable for user passwords, as they
    would fail the :package:`cracklib`/:package:`libpwquality` complexity checks.

* :code:`simplib::assert_metadata`:

  * Added :code:`blacklist` option. This allows functionality to deliberately
    fail on an OS that is listed in the module's :file:`metadata.json`, but is
    not necessarily supported by all parts of the given module.

New data type aliases
"""""""""""""""""""""

Added :code:`Simplib::Systemd::ServiceName` for valid :program:`systemd` service
names.

pupmod-simp-stunnel
^^^^^^^^^^^^^^^^^^^

* Set default for :code:`stunnel::connection::ssl_version` to TLSv1.2 for EL8
  compatibility.
* Set default for :code:`stunnel::instance::ssl_version` to TLSv1.2 for EL8
  compatibility.
* Set the :code:`stunnel::connection::app_pki_crl parameter` to :code:`undef` by
  default due to issues with pointing the setting to an absent directory in EL8.
* Set the :code:`stunnel::instance::app_pki_crl` parameter to :code:`undef` by
  default due to issues with pointing the setting to an absent directory in EL8.
* Updated valid :code:`ssl_version` entries.


pupmod-simp-sudo
^^^^^^^^^^^^^^^^

* Added parameters for :code:`sudo::default_entry` and :code:`sudo::alias`
  defined types.
* :cve:`CVE-2019-14287` mitigation.

  * Do not allow the use of user id or group id of '-1' when 'ALL' or '%ALL' are
    used in the runas section of a :program:`sudo` user specification and the
    version of :program:`sudo` is earlier than 1.8.28.

* Deep merge :code:`user_specifications` by default.

pupmod-simp-svckill
^^^^^^^^^^^^^^^^^^^

* Updated the :code:`svckill` provider to work with different Puppet
  :code:`service` provider implementations.

  * If after a Puppet upgrade you find that :code:`svckill` is trying to kill
    system services that it previously ignored, you need :pupmod:`simp/svckill`
    version 3.6.1 or later to fix the problem.

* Updated service lists.

pupmod-simp-swap
^^^^^^^^^^^^^^^^

* Disable :code:`dynamic_swappiness` by default.
* Set the static system swappiness to 60 by default.


pupmod-simp-tcpwrappers
^^^^^^^^^^^^^^^^^^^^^^^

* Enhanced behavior to do nothing when TCP Wrappers is not supported by the OS.

pupmod-simp-tpm2
^^^^^^^^^^^^^^^^

* Removed the option for managing tools, :code:`tpm2::manage_tpm2_tools`.
  Tools can be managed or not by removing them from the package list.
  Note that the tools package is needed to determine the status of the TPM.
* Added support for setting :code:`tabrm_options` for connecting to the
  simulator.


pupmod-simp-useradd
^^^^^^^^^^^^^^^^^^^

* Added explicit support for setting the rescue/emergency shell on systemd
  systems.


rubygem-simp-cli
^^^^^^^^^^^^^^^^

* Updated the instructions provided in the local user lockout warning message
  in the bootstrap lock file.

  * Simplified instructions to create resources via Hiera.
  * Tell the user to check that they can :command:`ssh` into the server with the
    new user after bootstrap but before rebooting. This step is imperative to
    ensure that the user can also get through Puppet-managed authentication!

* Updated SIMP internet repositories configured by :command:`simp config`.

  * Now uses `simp-project.com`_ repositories via the new
    :code:`simp::yum::repo::internet_simp` class.
  * The `packagecloud`_ repositories are no longer being updated.

* Allow users to set the 'SIMP_ENVIRONMENT' environment variable to change the
  initial environment from 'production' to a custom value, when running
  :command:`simp config` or :command:`simp bootstrap`.
* :command:`simp config` changes

  * Ensured that :command:`simp config` uses the :code:`simp::classes` parameter
    instead of :code:`classes` by default, but accept both :code:`simp::classes`
    and :code:`classes` as valid existing configurations.
  * Removed deprecated :code:`--non-interactive` option.  Use
    :code:`--force-defaults` instead.

* Added :command:`simp kv` command family to allow users to manage and inspect
  entries in a simpkv key/value store
* :command:`simp passgen` changes

  * Split into sub-commands for ease of use:

    * :command:`simp passgen envs`: List environments that may have
      :code:`simplib::passgen` passwords.
    * :command:`simp passgen list`: List names of :code:`simplib::passgen`
      passwords.
    * :command:`simp passgen remove`: Remove :code:`simplib::passgen` passwords.
    * :command:`simp passgen set`: Set :code:`simplib::passgen` passwords.
    * :command:`simp passgen show`:  Show :code:`simplib::passgen` passwords
      and other stored attributes.

  * Updated to work with simpkv-enabled :code:`simplib::passgen`.  Automatically
    detects whether :code:`simplib::passgen` is operating in 'legacy' mode or
    'simpkv' mode in the specified environment, and then executes password
    operations using the appropriate mechanism for that mode.
  * When setting passwords, disabled :package:`libpwquality`/:package:`cracklib`
    validation of user-entered passwords, by default, because not all passwords
    managed by :code:`simplib::passgen` are user passwords.  This validation
    can be re-enabled with the :code:`--validate` option of
    :command:`simp passgen set`.

  * Added the following command line options when creating passwords

    * :code:`--[no-]auto-gen`: Whether to auto-generate new passwords.
    * :code:`--complexity`: Password complexity to use when a password is
      auto-generated. Corresponds to the :code:`complexity` option of
      :code:`simplib::passgen`.
    * :code:`--[no-]complex-only`: Whether to only use only complex characters
      when a password is auto-generated. Corresponds to the :code:`complex_only`
      option of :code:`simplib::passgen`.
    * :code:`--[no-]validate`: Enables validation of new passwords with
      :package:`libpwquality`/:package:`cracklib`.
    * :code:`--length`: Password length to use when a password is auto-generated.

  * Added :code:`--[no-]details` option when showing password information.  When
    enabled, all available password information is displayed, not just the
    current and previous password values.

* Updated :package:`HighLine` from version 1.7.8 to 2.0.3.

simp-adapter
^^^^^^^^^^^^

* Removed logic to ensure any existing, global :file:`hiera.yaml.simp` file is not
  removed on upgrade from simp-adapter <= 0.0.6.

  * This is not an issue when upgrading from SIMP 6.4.0 to SIMP 6.5.0 (i.e.,
    :package:`simp-adapter` version 1.0.1 to version 2.0.0).
  * If for some reason you are upgrading from :package:`simp-adapter` version
    <= 0.0.6, manually save off :file:`/etc/puppetlabs/puppet/hiera.yaml.simp`
    prior to the upgrade, and then restore that file after the upgrade is
    complete.

simp-environment-skeleton
^^^^^^^^^^^^^^^^^^^^^^^^^

* Ensure that :program:`firewalld` is used by default in the applicable SIMP
  scenarios.
* Ensured that the server Hiera defaults have :code:`simp::server` in the
  :code:`simp::classes` array. Otherwise, it will never get picked up.
* Replace :code:`classes` with :code:`simp::classes` and
  :code:`simp::server::classes` as appropriate in example Hiera YAML files.
* FakeCA updates

  * Added the CA code directly into the project to allow the code to work
    on newer OS versions
  * Allow users to specify an alternate output directory via a 'KEYDIST'
    environment variable.
  * Consolidate the certificate request and revocation code.
  * Certificate revocation now runs in linear time.

* Changed permissions for files and directories to be world readable.
* Add a PE-suitable Puppet server YAML data template.


simp-gpgkeys
^^^^^^^^^^^^

* Added the CentOS 8 and EPEL 8 GPG keys.
* Removed Fedora 25 and 26 GPG keys.
* Updated puppetlabs GPG key.

simp-rsync-skeleton
^^^^^^^^^^^^^^^^^^^

* Added mitigation for :cve:`CVE-2019-6477` to the sample, RedHat 7 :file:`named.conf`.

* Removed :file:`rndc.key` files from sample named configuration to prevent
  users from accidentally using a published, sample secret key.

  * The :program:`named` service will create a key if one does not exist using
    the correct defaults for the system.

* Updated the :file:`README` in :file:`rsync/RedHat/Global/tftpboot/linux-install`.

  * It now explains which boot files for the :term:`TFTP` boot server are
    required when :code:`tftpboot::use_os_files` is set to :code:`false`.

simp-utils
^^^^^^^^^^

* Added (optional) :code:`--unpack-pxe [DIR]` option to the
  :program:`unpack_dvd` script.

  * Added (optional) :code:`--environment ENV` to set the PXE rsync
    environment.
  * Added a new :code:`--[no-]unpack-yum` (enabled by default), to permit users
    to disable the RPM unpack.
  * To enable unpacking PXE tftpboot files, run with :code:`--unpack-pxe`.
  * To disable unpacking RPMs/yum repos, run with :code:`--no-unpack-yum`.
  * See :command:`unpack_dvd --help` for details.

* Overhauled :command:`unpack_dvd --help`; output now fits on 80-character PTY
  consoles.


Known Bugs and Limitations
--------------------------

Below are bugs and limitations known to affect this release. If you discover
additional problems, please `submit an issue`_ to let use know.

.. contents::
  :depth: 2
  :local:

.. _changelog-6-5-0-el8-client-limitations:

Special considerations with EL8 clients
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


Network-isolated EL8 clients require EPEL8 and EL8 Base/Updates dnf mirrors
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

Because there is no SIMP 6.5 EL8 server release, there is no accompanying EL8
ISO or package tarball that can be used to create a self-hosted dnf repository
for SIMP-specific EL8 packages.

In order to provide the necessary packages to EL8 agents on a network-isolated
SIMP 6.5 infrastructure, admins must ensure that dnf repo mirrors are available
for:

  * EL8 Base/Updates
  * `EPEL 8 <https://download.fedoraproject.org/pub/epel/8/Everything/x86_64/>`_
  * `Puppet EL8 <http://yum.puppet.com/puppet/el/8/x86_64/>`_


unpack_dvd does not (re-)create modular repos for EL8 dnf repos (:jira:`SIMP-8614`)
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

EL8 introduces `modular package repositories
<https://docs.pagure.org/modularity/>`_. When unpacking an EL8 ISO to populate
a yum repository, SIMP 6.5.0's :program:`unpack_dvd` script does not recognize
or correctly package repository modules.  Consequently, EL8 Puppet agents
applying catalogs that require modular EL8 packages may encounter errors like
the following:

.. code-block:: none

   Error: /Stage[main]/Simp_apache::Install/Package[httpd]/ensure: change from 'purged' to 'latest' failed: Could not update: Execution of '/usr/bin/dnf -d 0 -e 1 -y install httpd' returned 1: No available modular metadata for modular package 'httpd-2.4.37-21.module_el8.2.0+382+15b0afa8.x86_64', it cannot be installed on the system
   Error: No available modular metadata for modular package


.. _submit an issue: https://simp-project.atlassian.net
.. _simp-project.com: https://simp-project.com
.. _packagecloud: https://packagecloud.io/simp-project


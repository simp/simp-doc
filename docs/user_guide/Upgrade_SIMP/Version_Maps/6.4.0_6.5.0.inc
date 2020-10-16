.. _upgrade-6.4.0-to-6.5.0:

DEMONSTRATION: New Formatting roles, and how they look:
-------------------------------------------------------

.. admonition:: This page demonstrates a bunch of new style roles
  :class: "neutral-bg"

  This page has been updated to demonstrate how using ReStructuredText roles
  could keep a document consistent and avoid getting overwhelmed by
  double-backticks.

  Inline with double backticks: ``inline code``

  .. list-table:: Roles already built into Sphinx
       :header-rows: 1

       * - Role
         - Example
         - Code
       * - code
         - :code:`simp/simplib`
         - ``:code:`simp/simplib```
       * - command
         - :command:`yum update -y`
         - ``:command:`yum update -y```
       * - file
         - :file:`/etc/simp/version.{x}.{y}`
         - ``:file:`/etc/simp/version.{x}.{y}```
       * - program
         - :program:`mcstransd`
         - ``:program:`mcstransd```

  .. list-table:: Custom roles, created for simp-doc
       :header-rows: 1

       * - Role
         - Example
         - Code
       * - github link
         - :github:`simp/simp-doc`
         - ``:github:`simp/simp-doc```
       * - jira
         - :jira:`SIMP-8464`
         - ``:jira:`SIMP-8464```
       * - package
         - :package:`simp-utils`
         - ``:package:`simp-utils```
       * - pupmod
         - :pupmod:`simp/simplib`
         - ``:pupmod:`simp/simplib```


Upgrading from SIMP-6.4.0 to SIMP-6.5.0
---------------------------------------

.. WARNING::

   You **MUST** be on at least version 6.4.0 to upgrade directly to 6.5.0 due to
   changes to the deployment methods starting with 6.4.0.

.. contents:: :local:
   :depth: 3

.. _before-upgrading-to-6.5.0:

Before Upgrading to 6.5.0
^^^^^^^^^^^^^^^^^^^^^^^^^

Survey EL6 Systems
""""""""""""""""""

.. WARNING::

   Maintenance Updates for EL6 are EOL, and will stop on November 30 2020 for
   both `CentOS <https://wiki.centos.org/About/Product>`_ and `RHEL
   <https://access.redhat.com/support/policy/updates/errata#Maintenance_Support_2_Phase>`_.

   The SIMP 6.5.0 changelog contains a
   :ref:`list of (optional) Puppet modules that no longer support EL6 <changelog-6.5.0-el6-support-dropped-from-some-optional-puppet-modules>`.
   Prior to upgrading, identify any EL6 nodes that still require these modules and either:

     1. Update the affected EL6 nodes to EL7, or
     2. Manage the affected EL6 nodes from a :ref:`SIMP 6.4.0 environment<Deploying SIMP Environments>`.

Ensure Local Modules work with Puppet 6
"""""""""""""""""""""""""""""""""""""""

This release updates :program:`puppet-agent` to Puppet 6.  Make sure all of your
own Puppet modules work with Puppet 6 prior to upgrading.

.. NOTE::

   You may choose to stay on Puppet 5 by not upgrading the :package:`puppet-agent` and
   :package:`puppetserver` packages.

   If you choose to stay on Puppet 5, please check the :file:`metadata.json` for new
   modules to ensure that they still support Puppet 5 prior to using them.

Update NFS settings
"""""""""""""""""""

The NFS Puppet modules have been updated to only accept IP addresses for the
NFS server in order to support :program:`firewalld` integration.  If your site
currently references NFS servers by hostname or FQDN, please change them to IP
addresses before upgrading.  This affects the parameters
:code:`simp_nfs::home_dir_server` and :code:`nfs::client::mount::nfs_server`.

In Hiera:

.. code-block:: yaml

   simp_nfs::home_dir_server: '<change to server IP>'

In Puppet code:

.. code-block:: ruby

   nfs::client::mount { 'my directory':
     nfs_server =>  '<change to server IP>',
     # ...
   }


Update DNS RNDC key
"""""""""""""""""""

(The following only needs to be done if you are using
:package:`pupmod-simp-named` to configure your DNS server and it copies the
files using rsync:)

On the SIMP Puppet server, remove the :file:`rndc.key` from the rsync
directories:

.. code-block:: sh

   find /var/simp/environments/ -name rndc.key -exec rm -f {} \;

On your DNS servers, view the :file:`/etc/rndc.key`.  If the algorithm is
``hmac-md5``, generate a new rndc key:  Replace ``<keyname>`` with the name
in the key field in your :file:`/etc/rndc.key` files.  (It should be the same
as the name used in your :file:`/etc/named.conf`.)

.. code-block:: sh

   rndc-confgen -a -k <keyname> -b 256

Set :code:`svckill::mode` to ``warning``
"""""""""""""""""""""""""""""""""""""""""

Puppet recently made changes to how it internally enumerates services, with the
consequence that, if :code:`svckill::mode` is set to ``enforcing``, it is
possible that it could kill critical system services between updating to the
new Puppet version and deploying the new Puppet modules.

As a safety precaution before upgrading, set :code:`svckill::mode` from
``enforcing``  to ``warning`` until SIMP 6.5.0's Puppet modules have been
deployed.

In Hiera:

.. code-block:: yaml

   svckill::mode: 'warning'


Uninstall out-of-date Puppet modules
""""""""""""""""""""""""""""""""""""

This only affects user with a :ref:`Local deployment
scenario<ug-sa-env-deployment-scenarios--local>`.  The following list of
optional Puppet modules required by the
:package:`simp-extras` RPM were not updated for SIMP 6.5.0.

If they are present, they will cause :command:`yum update` to fail. If you
installed them by installing :package:`simp-extras`, then updating
:package:`simp-extras` will automatically resolve the dependencies.  Otherwise
you must do a :command:`yum remove` of any of the following rpms installed on
your system:

  - :package:`pupmod-simp-journald`
  - :package:`pupmod-simp-simp_snmpd`
  - :package:`pupmod-simp-simp_gitlab`
  - :package:`pupmod-puppetlabs-docker`

Note that this will **not** remove the local git repositories created for
each Puppet module during previous SIMP RPM installations.  The git
repositories can still be referenced from SIMP 6.4 Puppetfiles and
:ref:`deployed into a SIMP environment<Deploying SIMP Environments>` that uses
SIMP 6.4 modules.

Upgrading to 6.5.0
^^^^^^^^^^^^^^^^^^

After resolving all applicable items in :ref:`Before Upgrading to 6.5.0 <before-upgrading-to-6.5.0>`,
follow the :ref:`Incremental Upgrades <ug-incremental-upgrades>` instructions.

During/after the upgrade, you might see the following error during Puppet agent
runs:

.. code-block:: text

   Error: /Stage[main]/Aide/Exec[update_aide_db]: Failed to call refresh: Command exceeded timeout
   Error: /Stage[main]/Aide/Exec[update_aide_db]: Command exceeded timeout

This indicates that the rebuilding of the AIDE database has timed out. This
is expected because many changes have been made to both the OS and Puppet modules.
Follow whatever process you normally employ to update the AIDE database after
the SIMP 6.5.0 updates have been made to the system.

After Upgrading to 6.5.0
^^^^^^^^^^^^^^^^^^^^^^^^

Set :code:`svckill::mode` back to ``enforcing`` (if applicable)
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

If you changed :code:`svckill::mode` from ``enforcing`` to ``warning`` before
the upgrade, you can set it back to ``enforcing`` after the upgrade is
complete.

Mcstransd syslog errors
"""""""""""""""""""""""

If the :program:`mcstransd` daemon is enabled on a system, changes to how
:file:`/proc` was mounted to allow :program:`polkit` to work can cause
:program:`mcstransd` to send a lot of errors to the system log.

:pupmod:`simp-selinux` has been updated with a fix for this, but it no longer
manages :program:`mcstransd` by default.  To manage :program:`mcstransd` with
the :pupmod:`simp-selinux` module, set the following in Hiera:

.. code-block:: yaml

   selinux::manage_mcstrans_service: true

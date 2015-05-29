Changelog
=========

.. code-block:: bash

  5.1.0-Alpha Fri Mar 20 2015 SIMP Team ================ SIMP 5.1.0-Alpha
  ================ --------- Changelog --------- .. raw:: pdf PageBreak ..
  contents:: .. raw:: pdf PageBreak SIMP 5.1.0-Alpha ================
  \*\*Package\*\*: 5.1.0-Alpha This release is known to work with: \* RHEL
  7.0 and 7.1 x86\_64 \* CentOS 7.0 x86\_64 Upgrade Guidance
  ---------------- Fully detailed upgrade guidance can be found in the
  \*\*Upgrading SIMP\*\* portion of the \*User's Guide\*. The following is
  an excerpt of the most relevant steps to an upgrade for the impatient.
  .. WARNING:: You must have at least \*\*2GB\*\* of \*\*free\*\* RAM on
  your system to upgrade to this release. .. NOTE:: Upgrading from
  releases older than 5.0 is not supported. Expectations ~~~~~~~~~~~~
  Before you begin, please be aware that the following actions will take
  place as a result of the migration script: \* The \*puppet-server\* RPM
  will be removed \* The \*puppetserver\* RPM will be installed (no,
  that's not a typo) \* \*\*ALL\*\* SIMP Puppet code will be migrated into
  a new \*simp\* environment \* This will be located at
  \*/etc/puppet/environments/simp\* \* A backup of your running
  environment will be made available at
  \*/etc/puppet/environments/pre\_migration.simp\* \* You will find
  timestamped directories under the \*pre\_migration.simp\* directory that
  correspond to runs of the migration script \* Your old files will be in
  a \*backup\_data\* directory and will be linked to a local bare Git
  repository in the same space Upgrade Procedure ~~~~~~~~~~~~~~~~~ The
  following is the basic upgrade procedure for moving to SIMP 5.1. 1) Copy
  the SIMP ISO onto your system. a) We will refer to it as
  \*SIMP\_Update.iso\* for the rest of this procedure. 2) Extract the new
  \*simp-utils\* package using the following command: .. code-block:: bash
  isoinfo -i SIMP\_Update.iso -R -x \\ \`isoinfo -i SIMP\_Update.iso -Rf
  \| grep noarch/simp-utils\` \\ > simp-utils-update.rpm 3) Install the
  new \*simp-utils\* RPM .. code-block:: bash yum -y localupdate
  simp-utils\*.rpm 4) Unpack the DVD .. code-block:: bash
  /usr/local/bin/unpack\_dvd SIMP\_Update.iso 5) Run the migration script
  (this may take some time, do \*\*not\*\*, hit CTRL-C!) .. code-block::
  bash /usr/share/simp/upgrade\_script/migrate\_to\_environments 6) Run
  the Puppet Agent .. code-block:: bash puppet agent -t 7) Stop the new
  puppetserver service (it may be running) .. code-block:: bash service
  puppetserver stop 8) Remove any left over PID files .. code-block:: bash
  rm /var/run/puppetserver/puppetserver 9) Kill any running \*puppet
  master\* processes .. code-block:: bash pkill -f 'puppet master' 10)
  Wait 10 seconds to let things finalize if necessary .. code-block:: bash
  sleep 10 11) Start the new Puppet Server .. code-block:: bash service
  puppetserver start If all went well, you should now be fully upgraded to
  the new Clojure-based Puppet Server with a functioning \*simp\*
  environment. Security Announcements ---------------------- CVEs
  Addressed ~~~~~~~~~~~~~~ RPM Updates ----------- Numerous RPMs were
  updated in the creation of this release. Several were included due to
  our use of \*repoclosure\* to ensure that RPM dependencies are met when
  releasing a DVD. Fixed Bugs ---------- \* pupmod-aide - Change the call
  to the \*rsyslog\* init script to the \*service\* command to seamlessly
  support both RHEL6 and RHEL7. \* pupmod-apache - Remove the
  apache\_version fact and simply use the version controls built into the
  Apache configuration language. - Update all custom functions to properly
  scope definitions. - Ensure that mod\_ldap is installed in SIMP >= 5.0.
  \* pupmod-clamav - Change the call to the \*rsyslog\* init script to the
  \*service\* command to seamlessly support both RHEL6 and RHEL7. \*
  pupmod-common - Ensure that the \*passgen()\* function fails on invalid
  scenarios. This prevents the accidental cration of empty passwords. -
  Allow the value \*2\* to be used for \*rp\_filter\* in
  \*common::sysctl\*. \* pupmod-dhcp - Change the call to the \*rsyslog\*
  init script to the \*service\* command to seamlessly support both RHEL6
  and RHEL7. \* pupmod-simp-elasticsearch - Ensured that Elasticsearch
  works properly with the new version of Apache. - Removed our default ES
  tuning since the default works better for LogStash. - Ensure that Puppet
  manages the Elasticsearch logging file. \* pupmod-simp-logstash - Fix
  issues with both TCPWrappers and IPTables when used with LogStash. \*
  pupmod-nfs - Updated the \*mountd\* port to be \*20048\* by default for
  SELinux issues in RHEL7. \* pupmod-ntp - Ensure that \*restrict\*
  entries use DDQ format. \* pupmod-openldap - Change the call to the
  \*rsyslog\* init script to the \*service\* command to seamlessly support
  both RHEL6 and RHEL7. \* pupmod-openscap - Change the call to the
  \*rsyslog\* init script to the \*service\* command to seamlessly support
  both RHEL6 and RHEL7. \* pupmod-ssh - Updated to use the new
  augeasproviders module dependencies. - Added a function
  \*ssh\_format\_host\_entry\_for\_sorting()\* that will properly sort SSH
  \*Host\* entries for inclusion with concat. \* pupmod-stunnel - Had a
  variable \*\*options\*\* in \*stunnel.erb\* that should have been scoped
  as \*\*@options\*\*. \* pupmod-sudosh - Change the call to the
  \*rsyslog\* init script to the \*service\* command to seamlessly support
  both RHEL6 and RHEL7. \* pupmod-sysctl - Removed support for the old
  parsed-file provider and moved to using the new Augeas-based provider.
  \* pupmod-tftpboot - Purging of non-Puppet-managed items in
  \*pxelinux.cfg\* is now optional. \* simp - Fixed several logic issues
  in \*simp config\*. New Features ------------ \* pupmod-augeasproviders
  - This was updated to 2.1.3. - The update to 2.1.3 caused the addition
  of all of the pupmod-augeasproviders modules below. \*
  augeasproviders\_apache - Imported 2.1.3 to support the Augeasproviders
  stack. \* augeasproviders\_base - Imported 2.1.3 to support the
  Augeasproviders stack. \* augeasproviders\_core - Imported 2.1.3 to
  support the Augeasproviders stack. \* augeasproviders\_grub - Imported
  2.1.3 to support the Augeasproviders stack. \* augeasproviders\_mounttab
  - Imported 2.1.3 to support the Augeasproviders stack. \*
  augeasproviders\_nagios - Imported 2.1.3 to support the Augeasproviders
  stack. \* augeasproviders\_pam - Imported 2.1.3 to support the
  Augeasproviders stack. \* augeasproviders\_postgresql - Imported 2.1.3
  to support the Augeasproviders stack. \* augeasproviders\_puppet -
  Imported 2.1.3 to support the Augeasproviders stack. \*
  augeasproviders\_shellvar - Imported 2.1.3 to support the
  Augeasproviders stack. \* augeasproviders\_ssh - Imported 2.1.3 to
  support the Augeasproviders stack. \* augeasproviders\_sysctl - Imported
  2.1.3 to support the Augeasproviders stack. \* pupmod-richardc-datacat -
  Incorporated the \*richardc/datacat\* module into the core for user
  convenience. \* pupmod-freeradius - Split the Freeradius module based on
  version so that it can be properly selected against the \*installed\*
  version of Freeradius. This may take two runs to coalesce. \*
  pupmod-puppetlabs-inifile - Updated to version 1.2.0 \* pupmod-pki - Now
  generate a system RSA public key against the passed private key. \*
  pupmod-puppetlabs-postgresql - Initial import of the Puppet Labs
  PostgreSQL module. - Modifications were made to support the SIMP concat.
  \* pupmod-puppetlabs-puppetdb - New import of the Puppet Labs PuppetDB
  module. \* pupmod-puppetlabs-stdlib - Updated to version 4.5.1 Known
  Bugs ---------- \* Setting pwdReset to 'true' in LDAP does not force a
  user to reset their password like it is supposed to. This works with
  FreeIPA and we are looking to move to support that system in the future.
  \* SSSD is currently broken and will allow logins via SSH even if your
  password has expired. This has been noted by Red Hat and is in the
  pipeline. Their suggestion it to move to FreeIPA from OpenLDAP. We are
  looking to do this in the future. \* If you are running libvirtd, when
  svckill runs it will always attempt to kill dnsmasq unless you are
  deliberately trying to run the dnsmasq service. This does \*not\*
  actually kill the service but is, instead, an error of the startup
  script and causes no damage to your system.

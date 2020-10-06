.. _upgrade-6.4.0-to-6.5.0:

Upgrading from SIMP-6.4.0 to SIMP-6.5.0
---------------------------------------

.. WARNING::

   You **MUST** be on at least SIMP 6.4.0 to upgrade to 6.5.0 because of
   changes to the deployment methods in SIMP 6.4.0.

Before Upgrading to 6.5.0
^^^^^^^^^^^^^^^^^^^^^^^^^
#. EL6 Systems

   The changelog has a list of Puppet modules that do not support EL6 any longer.
   Identify EL6 machines that still need these modules and either update them
   to EL7 or create a :ref:`SIMP 6.4.0 environment<Deploying SIMP Environments>`
   for these machines.

   .. NOTE::

     CentOS is stopping support of EL6 in Nov 2020.

#. Puppet 6

   Puppet is being updated to Puppet 6 in this release.  Make sure all of your own
   modules work with Puppet 6.

#. Update NFS settings

   The NFS modules have been updated to only accept IP addresses for the NFS server
   value.  If hostnames are being used they will have to be changed to IP addresses.

   The following parameters are affected.

   .. code-block:: ruby

     simp_nfs::home_dir_server: <change to server  ip>

     nfs::client::mount { 'my directory':
       nfs_server =>  <change to server ip>,
       ...
     }


#. Update DNS RNDC key.

   The following only needs to be done if you are using ``simp-named``
   to configure your DNS server and it copies DNS configuration files from 
   SIMP's rsync directories.

   On the SIMP puppet server remove the rndc.key file from the rsync directories:

   .. code-block:: sh

     find /var/simp/environments/ -name rndc.key -exec rm -f {} \;


   On each DNS server, view the /etc/rndc.key file.  If the algorithm is hmac-md5, generate
   a new RNDC key and then replace its <keyname> with the name in the key field in the /etc/named.conf
   on that server.

   .. code-block:: sh

     rndc-confgen -a -k <keyname> -b 256

#. SVCKILL mode setting

   If svckill is set to 'enforcing' mode set it to 'warning' until the new modules
   have been deployed.

   In Puppet made changes to how it reports systemd services in its internal API on . This significantly
   impacted ``simp-svckill``.  If ``svckill::mode`` is set to 'enforcing' in older versions of
   ``simp-svckill``, it is possible system services could be killed between updating Puppet and
   deploying the new modules which could cause the system to reboot.

   .. code-block:: yaml

     # Change the setting in hiera
     svckill::mode: 'warning'

#. Uninstall out of date puppet modules.

   This only affect user with a :ref:`Local deployment
   scenario<ug-sa-env-deployment-scenarios--local>`.
   The following list of modules in the simp-extras module were not updated and will
   cause yum update to fail if they are not removed.  If you installed them
   by installing simp-extras, then updating simp-extras will take care the dependencies.
   Otherwise you will need to do a yum remove of any of the following rpms
   installed on your system:

   - pupmod-simp-journald
   - pupmod-simp-simp_snmpd
   - pupmod-simp-simp_gitlab
   - pupmod-puppetlabs-docker

  This will not remove the local git repositories and they can still be deployed to
  in 6.4 environments.

Upgrading to 6.5.0
^^^^^^^^^^^^^^^^^^

After taking care of all the items in the Before section
follow the :ref:`Incremental Upgrade instruction <ug-incremental-upgrades>`.

The following error might be seen:

.. code-block:: sh

  Error: /Stage[main]/Aide/Exec[update_aide_db]: Failed to call refresh: Command exceeded timeout
  Error: /Stage[main]/Aide/Exec[update_aide_db]: Command exceeded timeout

This indicates that the rebuilding of the AIDE database has timed out.  This is not
unusual because so many changes have been made to the OS and puppet modules.
Use whatever means are normally employed to update the AIDE database after an
update has been made to the system.

After Upgrading to 6.5.0
^^^^^^^^^^^^^^^^^^^^^^^^

#. SVCKILL

   Once the upgrade is complete the svckill::mode can be set back to enforcing:
      
   .. code-block:: yaml

     # Change the setting in hiera
     svckill::mode: 'enforcing'

#. Mcstransd syslog errors

   If the mcstransd daemon is enabled on a system, changes to how */proc* was mounted
   to allow polkit to work can cause mcstransd to send a lot of errors
   to the system log.  ``simp-selinux`` has a fix for this when it manages mcstrand,
   but no longer manages mcstransd by default.  To enable management of mcstransd to
   pick up the fix, set the following in hiera:

   .. code-block:: yaml

     selinux::manage_mcstrans_service: true


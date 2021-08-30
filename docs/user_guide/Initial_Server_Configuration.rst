.. _ug-initial_server_configuration:

Initial SIMP Server Configuration
=================================

Introduction to the SIMP Utility
--------------------------------

The ``simp`` command provides a CLI intended to make the
configuration of the :term:`SIMP server` straightforward and repeatable.
In these instructions, we will be using the ``config`` and ``bootstrap``
options of the ``simp`` command

For a list of the commands ``simp`` provides, type :command:`simp help`. Type
:command:`simp <Command> --help` for more information on a specific command.

* :command:`simp config` sets up configuration required to bootstrap the SIMP server
  with Puppet.  It asks questions, generates configuration files, and applies
  preliminary server configuration based on the answers.  It records the options
  chosen in a file, :file:`/root/.simp/simp_conf.yaml` and generates a log file
  under :file:`/root/.simp/`.

  * You can use the ``--dry-run`` option to step through the questions without
    changing anything and then run :command:`simp config  -a /root/.simp/simp_conf.yaml`
    to apply the changes.

  * :command:`simp config` uses the ``production`` :term:`Puppet Environment` by
    default. If you want to use a different initial environment, see
    :ref:`howto-use-an-alternate-simp-config-environment`.

* :command:`simp bootstrap` uses several targeted Puppet runs to configure the rest
  of the system and generates a log file under :file:`/root/.simp/`.

For more details about initial configuration provided by :command:`simp
config`, see :ref:`gsg-advanced-configuration`.

Configuring the SIMP Server
---------------------------

.. WARNING::

   Puppet has problems when hostnames contain capital letters (`SERVER-1809`_) — do not use them!

   .. _SERVER-1809: https://tickets.puppetlabs.com/browse/SERVER-1809

#. Log on as a user that can gain ``root`` access and ``sudo`` to ``root``.

   * **If you installed from the ISO**

     * Log in as ``simp``.
     * Run  :command:`sudo su - root`.

   * **If you installed from RPM**

     * Create a local user that can escalate to ``root`` and use it to access the ``root`` account.

#. Run :command:`simp config` and configure the system as prompted.

   * These settings will be used to set up files appropriate for bootstrapping the system.

     * For each setting:

       * Press *Enter* to keep the recommended value or enter your desired value.

  * For more details about :command:`simp config`'s installation variables and actions, see
    :ref:`gsg-advanced-configuration`.

  .. NOTE::

     If you see a message about 'simp bootstrap' being 'locked', follow the steps in
     :ref:`ug-prevent-lockout`:

.. _ug-initial_server_configuration-run_bootstrap:

3. Run :command:`simp bootstrap`.

   If your SIMP server is on a virtual machine, or slow system, the default timeout for the
   Puppet server to start (5 minutes) may be too short.  You will want to extend this time by using
   the ``-w`` option.

   For example, to extend the timeout to 10 minutes:

   .. code:: console

      simp bootstrap -w 10

   .. NOTE::

      If the bootstrap progress bars of each Puppet run are of equal length, a problem has probably
      occurred due to an error in SIMP configuration. Refer to the previous step and make sure that
      all configuration options are correct.

      You can debug issues by either looking at the log files in :file:`/root/.simp` or by running
      :command:`puppet agent -t --masterport=8150 --agent_disabled_lockfile /opt/puppetlabs/server/data/puppetserver/state/bootstrap.lock`.

#. Run :command:`reboot` to restart your system and apply the necessary kernel
   configuration items.

After rebooting, SIMP-managed security settings have been applied and the SIMP server is ready for
site-specific configuration.

To ``su`` to ``root`` from the  ``simp`` user, you must now use :command:`sudo su - root`.

Next Steps
----------

* To continue configuring the system, move on to the next section in the :ref:`simp-user-guide`,
  :ref:`Client_Management`.

The following are links to other information in the user guide that are answers to questions sometimes
asked at this time:

* To learn more details about how your system has just been configured see :ref:`gsg-advanced-configuration`.
* To add additional simp modules or you own site modules to the environment :ref:`Updating an Existing Environment<ug-environments-deploying-from-local-repositories>`
* To extract the Full OS to the yum server see :ref:`howto-unpack-dvd`

.. include::  Initial_Server_Configuration/Prevent_Lockout_on_Puppetserver.inc

.. include::  Initial_Server_Configuration/Advanced_Configuration.inc

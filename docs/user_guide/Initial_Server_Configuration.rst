.. _ug-initial_server_configuration:

Initial SIMP Server Configuration
=================================

Introduction to the SIMP Utility
--------------------------------

The ``simp`` command provides a CLI intended to make the
configuration of the :term:`SIMP server` straightforward and repeatable.
In these instructions, we will be using the ``config`` and ``bootstrap``
options of the ``simp`` command

For a list of the commands ``simp`` provides, type ``simp help``. Type
``simp <Command> --help`` for more information on a specific command.

- ``simp config`` sets up configuration required to bootstrap the SIMP server
  with Puppet.  It asks questions, generates configuration files, and applies
  preliminary server configuration based on the answers.  It records the options
  chosen in a file, ``/root/.simp/simp_conf.yaml`` and generates a log file
  under ``/root/.simp/``.

  * You can use the ``--dry-run`` option to step through the questions without
    changing anything and then run ``simp config  -a /root/.simp/simp_conf.yaml``
    to apply the changes.

- ``simp bootstrap`` uses several targeted Puppet runs to configure the rest
  of the system and generates a log file under ``/root/.simp/``.

For more details about initial configuration provided by ``simp config`` see
:ref:`gsg-advanced-configuration`.

Configuring the SIMP Server
---------------------------

.. WARNING::

   Puppet has problems when hostnames contain capital letters
   (`SERVER-1809`_) — do not use them!

   .. _SERVER-1809: https://tickets.puppetlabs.com/browse/SERVER-1809

#. Log on as a user that can gain ``root`` access and ``su`` to ``root``.

   - If you installed from the ISO, it created the ``simp`` user.
     Log in with ``simp`` and run  ``su -``.
   - If you installed from RPM, create a privileged user or log in as ``root``.
     There will be instructions later about how to configure access for the
     privileged user on the SIMP server, so that after bootstrap, you are not
     locked out of the server.  This step is **essential** on cloud instances.

#. Run ``simp config`` and configure the system as prompted.  (The ``--dry-run``
   option will run through all of the prompts without applying any changes to
   the system.)

   -  ``simp config`` will prompt you with the follow:

      - ``Ready to create the SIMP omni-environment?`` Enter ``yes``.
      - ``Ready to start the questionaire?`` Enter ``yes``.

   -  ``simp config`` will then prompt you for system settings and apply them as
      appropriate for bootstrapping the system. When applicable, ``simp config``
      will present you with a recommendation for each setting. For each question:

      - Press *Enter* to keep a recommended value.
      - Otherwise, enter your desired value.

   -  When the questionnaire is finished and you are prompted with

      -  ``Ready to apply?`` Enter ``yes`` to continue.

   -  ``simp config`` then applies the information and generates its
      configuration files.


      .. Important::

         If you have installed SIMP from RPM and see the following failure, go
         to the :ref:`ug-prevent-lockout`  section and follow the steps to
         configure a user that has ``su -`` capability.

           ``'simp bootstrap' has been locked due to potential login lockout.``

             ``* See /root/.simp/simp_bootstrap_start_lock for details``

   - For more details about ``simp config``'s installation variables and
     actions, see :ref:`gsg-advanced-configuration`.


#. Run ``simp bootstrap``.

   If your SIMP server is a virtual machine in a cloud, the default
   timeout for the Puppet server to start (5 minutes) may be too short.
   You will want to extend this time by using the ``-w`` option.  For
   example, to extend that timeout to 10 minutes:

   ``simp bootstrap -w 10``


   .. NOTE::

      If the bootstrap finishes quickly and the progress bars of each Puppet run
      are of equal length, it is very likely that  a problem has occurred due to
      an error in SIMP configuration. Refer to the previous step and make sure
      that all configuration options are correct.

      If this happens, you can debug by either looking at the log files or by
      running ``puppet agent -t --masterport=8150``.

#. Run ``reboot`` to restart your system and apply the necessary kernel
   configuration items.


When your systems comes back up, SIMP-managed security settings have been applied
and the SIMP server (``puppetserver``) is ready for site-specific configuration.
To ``su`` to ``root`` from the  ``simp`` user, you must now use ``sudo su -t root``.

Next steps:

* To continue configuring the system, move on to the next section in the
  :ref:`simp-user-guide`, :ref:`Client_Management`.
* To learn more details about what the ``simp`` utility just did to your system,
  see :ref:`gsg-advanced-configuration`.

Optional: Extract the full OS RPM Package Set
---------------------------------------------

The SIMP ISO only provides enough RPM packages to run a basic system. If you
require additional stock OS packages, you can extract additional packages from
vendor ISOs using the following procedure:

#. Log on as ``simp`` and run ``su -`` to gain root access.
#. Run ``puppet agent -t`` to ensure system consistency.
#. Copy the appropriate vendor OS ISO(s) to the server and unpack using the
   ``unpack_dvd`` utility. This will create a new directory tree under
   ``/var/www/yum/<OperatingSystem>`` suitable for serving to clients.

   Run: ``unpack_dvd CentOS-RHEL_MAJOR_VERSION-x86_64-DVD-####.iso``

#. Ensure that subsequent :term:`yum` operations are aware of the new RPM
   packages by refreshing the system's yum cache:

   Run: ``yum clean all; yum makecache``

.. include::  Initial_Server_Configuration/Prevent_Lockout_on_Puppetserver.inc

.. include::  Initial_Server_Configuration/Advanced_Configuration.inc

.. _ug-initial_server_configuration:

Initial SIMP Server Configuration
=================================

Using the SIMP Utility
----------------------

In these instructions, we will be using the ``config`` and ``bootstrap`` of the
``simp`` command.  The ``simp`` command provides a CLI intended to make the
initial configuration of the SIMP server straightforward and repeatable.

.. NOTE::

   For a list of the commands ``simp`` provides, type ``simp help``. Type
   ``simp <Command> --help`` for more information on a specific command.

Configuring the SIMP Server
---------------------------

.. IMPORTANT::

   Correct time—synchronized across all systems—is **critical** to the proper
   functioning of SIMP (and Puppet in general).

.. TIP::

   If a Puppet agent receives errors regarding certificate validation while
   connecting to the Puppet server, compare the time on the server and
   agent to make sure they are synchronized.

.. WARNING::

   Puppet has problems when hostnames contain capital letters
   (`SERVER-1809`_) — do not use them!

   .. _SERVER-1809: https://tickets.puppetlabs.com/browse/SERVER-1809

.. NOTE::

  This section assumes that:

   * You started by :ref:`gsg-installing_simp_from_an_iso`
   * You have logged in using the ``simp`` local user account (created by
     the ISO installation)

   Use the appropriate user for your environment if you installed via an
   alternate method.

#. Log on as ``simp`` and run ``su -`` to gain root access.
#. Run ``simp config`` and configure the system as prompted.

    - ``simp config`` will prompt you for system settings and then apply them as
      appropriate for bootstrapping the system.

    - When applicable, ``simp config`` will present you with a recommendation
      for each setting.
        - Press *Enter* to keep a recommended value.
        - Otherwise, enter your desired value.

    - ``simp config`` generates a log file under ``/root/.simp/`` with details
      of the configurations selected and actions taken.

   .. NOTE::

       For details about ``simp config``'s installation variables and actions,
       see :ref:`gsg-advanced-configuration`.

   .. TIP::

      There are two ``simp config`` options that are particularly useful:

      * ``--dry-run`` will run through all of the prompts without
        applying any changes to the system. This is useful to:

        - become familiar with the variables set by ``simp config`` without
          applying them
        - generate a configuration file to use as a template for subsequent
          ``simp config`` runs

      * ``-a <Config File>`` will load and apply
        a previously-generated configuration (aka the 'answers' file) in lieu of
        prompting for settings.

        - This is useful to run on systems that will be rebuilt often.
        - Please note, however: if you edit the answers file, only configuration
          settings for which you would be prompted by ``simp config`` can be
          modified in that file—any changes made to settings that ``simp
          config`` automatically determines will be ignored.

   .. NOTE::

     For a list of additional options, type ``simp config --help``.

#. When the questionnaire is finished and you are prompted with ``Ready to
   apply?``, enter ``yes`` to continue.

   This will apply changes to the system, which may take some time.

   .. NOTE::

      After ``simp config`` is applied, three SIMP configuration files will have
      been generated:

      #. ``/root/.simp/simp_conf.yaml``: File containing  all your ``simp
         config`` settings; can include additional settings related to ones
         you entered and other settings required for SIMP.

      #. ``/etc/puppetlabs/code/environments/simp/data/simp_config_settings.yaml``:
         File containing global Hiera data relevant to SIMP clients and the SIMP
         server.

      #. ``/etc/puppetlabs/code/environments/simp/data/hosts/<server_fqdn>.yaml``:
         The SIMP server's host-specific Hiera configuration.

#. Run ``simp bootstrap``.

   * ``simp bootstrap`` uses several targeted Puppet runs to configure the rest
     of the system.
   * It generates a detailed log file under ``/root/.simp/``.


   .. NOTE::

      For a list of additional options, type ``simp bootstrap --help``.

   .. NOTE::

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

Optional: Extract the full OS RPM Package Set
---------------------------------------------

The SIMP ISO only provides enough RPM packages to run a basic system. If you
did not install via ISO, or you require additional stock packages, you can
extract additional packages from vendor ISOs using the following procedure:

#. Log on as ``simp`` and run ``su -`` to gain root access.
#. Run ``puppet agent -t`` to ensure system consistency.
#. Copy the appropriate vendor OS ISO(s) to the server and unpack using the
   ``unpack_dvd`` utility. This will create a new directory tree under
   ``/var/www/yum/<OperatingSystem>`` suitable for serving to clients.

   Run: ``unpack_dvd CentOS-RHEL_MAJOR_VERSION-x86_64-DVD-####.iso``

#. Ensure that subsequent :term:`yum` operations are aware of the new RPM
   packages by refreshing the system's yum cache:

   Run: ``yum clean all; yum makecache``

.. include::  Initial_Server_Configuration/Advanced_Configuration.inc

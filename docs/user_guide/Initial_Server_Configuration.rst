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

   Correct time across all systems is important to the proper functioning of
   SIMP (and Puppet in general).

   If a Puppet agent receives errors regarding certificate validation while
   connecting to the Puppet server, compare the time on the Puppet server and
   agent and ensure they are synchronized.

.. WARNING::

   Puppet has problems when hostnames contain capital letters
   (`SERVER-1809`_) — do not use them!

   .. _SERVER-1809: https://tickets.puppetlabs.com/browse/SERVER-1809

.. NOTE:: The remainder of this section assumes that:

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
     for each setting.  To keep a recommended value, press *Enter*.  Otherwise,
     enter your desired value.

   - ``simp config`` generates a log file in ``/root/.simp`` containing details
     of the configuration selected and actions taken.

  .. NOTE::

      For more details about the installation variables set by ``simp config``
      and the corresponding actions, see :ref:`gsg-advanced-configuration`.

  .. NOTE::

     For a list of additional options, type ``simp config --help``.

     There are two options that are particularly useful:

     - ``simp config --dry-run`` will run through all of the ``simp config``
       prompts without applying any changes to the system. This is the option
       to run to become familiar with the variables set by ``simp config`` or
       generate a configuration file to be used as a template for subsequent
       ``simp config`` runs.

     - ``simp config -a <Config File>`` will load a previously generated
       configuration (aka the 'answers' file) in lieu of prompting for
       settings, and then apply the settings.  This is the option to run for
       systems that will be rebuilt often. Please note, however, if you edit
       the answers file, only configuration settings for which you would be
       prompted by ``simp config`` can be modified in that file.  Any changes
       made to settings that ``simp config`` automatically determines will be
       ignored.

#. When the questionnaire is finished and you are prompted with ``Ready to
   apply?``, enter ``yes`` to continue.

   * This will apply changes to the system, which may take some time.

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

#. Run ``simp bootstrap``

   - ``simp bootstrap``  generates a log file in ``/root/.simp`` containing
     details of the bootstrap operation.

  .. NOTE::

     For a list of additional options, type ``simp bootstrap --help``.

   .. NOTE::

      - If your SIMP server is a virtual machine in a cloud, the default
        timeout for the puppet server to start (5 minutes) may be too short.
        You will want to extend this time by using the ``-w`` option.  For
        example, to extend that timeout to 10 minutes:

        ``simp bootstrap -w 10``

      - If progress bars of each puppet run are of equal length and the
        bootstrap finishes quickly, a problem has occurred. This is most
        likely due to an error in SIMP configuration. Refer to the previous
        step and make sure that all configuration options are correct.

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

#. Update your system using :term:`yum`. The updates applied will depend on
   what ISO you initially used.

   Run: ``yum clean all; yum makecache``

.. include::  Initial_Server_Configuration/Advanced_Configuration.inc

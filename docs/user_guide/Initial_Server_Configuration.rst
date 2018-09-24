.. _ug-initial_server_configuration:

Initial SIMP Server Configuration
=================================

Using the SIMP Utility
----------------------

In these instructions we will be using the ``config`` and ``bootstrap``
commands of the SIMP Utility, ``simp``.   The SIMP Utility provides a CLI
intended to make the system initial configuration straightforward and
repeatable.

.. NOTE::

   For a list of the commands ``simp`` provides, type ``simp help``. Type
   ``simp help <Command>`` for more information on a specific command.

Configuring the SIMP Server
---------------------------

.. IMPORTANT::

   Correct time across all systems is important to the proper functioning of
   SIMP and Puppet in general.

   If a user has trouble connecting to the Puppet server and errors regarding
   certificate validation appear, check the Puppet server and client times to
   ensure they are synchronized.

.. WARNING::

   Keep in mind as the installation process begins that Puppet does not
   work well with capital letters in host names. Therefore, they should
   not be used.

For the remainder of the document, we will assume that you use the ISO
installation method and that you are logging in using a ``simp`` local user.
Use the appropriate user for your environment if you installed via an alternate
method.

#. Log on as ``simp`` and run ``su -`` to gain root access.
#. Type ``simp config`` and configure the system as prompted.

  - ``simp config`` will prompt you for system settings and then apply them as
    appropriate for bootstrapping the system.

  - When applicable, ``simp config`` will present you with a
    recommendation for each setting (variable).  To keep a recommended
    value, press *Enter*. Otherwise, enter your desired value.

  - ``simp config``  generates a log file in ``/root/.simp`` containing details
    of the configuration selected and actions taken.

  - For more details about the installation variables set by ``simp config``
    and the corresponding actions, see :ref:`gsg-advanced-configuration`.

  - For a list of additional options, type ``simp help config``.

    - ``simp config --dry-run`` will run through all of the ``simp config``
      prompts without applying any changes to the system. This is the
      option to run to become familiar with the variables set by
      ``simp config`` or generate a configuration file to be used as
      a template for subsequent ``simp config`` runs.

    - ``simp config -a <Config File>`` will load a previously generated
      configuration (aka the 'answers' file) in lieu of prompting for
      settings, and then apply the settings.  This is the option to run
      for systems that will be rebuilt often. Please note, however,
      if you edit the answers file, only configuration settings for
      which you would be prompted by ``simp config`` can be modified
      in that file.  Any changes made to settings that ``simp config``
      automatically determines will be ignored.

.. NOTE::

   Once ``simp config`` has been run, three SIMP configuration files
   will be generated:

   - ``/root/.simp/simp_conf.yaml``: File containing  all your
     ``simp config`` settings; can include additional settings related
     to ones you entered and other settings required for SIMP.

   - ``/etc/puppetlabs/code/environments/simp/hieradata/simp_config_settings.yaml``:
     File containing global Hiera data relevant to SIMP clients and the SIMP
     server.

   - ``/etc/puppetlabs/code/environments/simp/hieradata/hosts/<server_fqdn>.yaml``:
     SIMP server host specific Hiera configuration.

#. Type ``simp bootstrap``

.. NOTE::

   If progress bars are of equal length and the bootstrap finishes quickly, a
   problem has occurred. This is most likely due to an error in SIMP
   configuration. Refer to the previous step and make sure that all
   configuration options are correct.

   If this happens, you can debug by either looking at the log files or by
   running ``puppet agent -t --masterport=8150``.

#. Type ``reboot`` to reboot and apply the necessary kernel configuration items.

Optional: Extract the full OS Package Set
-----------------------------------------

The SIMP ISO attempts to contain everything that you need to run a base system.
However, if you did not install via ISO, or your require additional stock
packages, you can use the following procedure to extract the vendor ISOs.

#. Log on as ``simp`` and run ``su -`` to gain root access.
#. Run puppet for the first time.

   Type: ``puppet agent -t``

#. Copy the appropriate vendor OS ISO(s) to the server and unpack using the
   ``unpack_dvd`` utility. This creates a new tree under
   ``/var/www/yum/<OperatingSystem>`` suitable for serving to clients.

   Type: ``unpack_dvd CentOS-RHEL_MAJOR_VERSION-x86_64-DVD-####.iso``

#. Update your system using :term:`yum`. The updates applied will depend on
   what ISO you initially used.

   Type: ``yum clean all; yum makecache``

.. include::  Initial_Server_Configuration/Advanced_Configuration.inc


.. _howto-set-up-and-utilize-bolt:

HOWTO Set up and Utilize Bolt
=============================

This section details the steps required to set up and utilize :term:`Bolt`.
Bolt is an open source task runner developed by Puppet that facilitates
execution of tasks on demand, allowing them to be "pushed" on an as needed
basis instead of waiting for Puppet agent to run on a remote system in a
"pull" fashion.

The ``simp_bolt`` module is intended to configure both Bolt controllers that
manage Bolt tasks and Bolt target systems where the tasks are executed. The two
parts can be used independently but are intended to work in unison, ensuring
that the correct parameters are specified and available on both systems.

Setting up a Bolt Target
------------------------

The target portion of SIMP Bolt ensures that an account with appropriate
permissions exists on the target systems. To configure a target system,
the ``simp_bolt`` class must be included on the target node and
``simp_bolt::bolt_target`` must be set to ``true`` via :term:`Hiera`.

There are a variety of other parameters that are either optional or have
default values specified that can be overridden in Hiera.  A few of the
significant parameters and their default values are:

.. code-block:: yaml

   ---
   simp_bolt::target_user_name:                 'simp_bolt'
   simp_bolt::target_user_home:                 '/var/local/simp_bolt'
   simp_bolt::target::create_user:              false
   simp_bolt::target::disallowed_users:         ['root']
   simp_bolt::target::user_password:            undef
   simp_bolt::target::user_ssh_authorized_keys: undef
   simp_bolt::target::user_allowed_from:        'puppet_server'

The ``target_user_name`` parameter specifies the name of the account on the
**target** system that Bolt will use to logon to the system when connecting
from a Bolt controller. On target systems, the ``target_user_home`` parameter
specifies the full path to the home directory of the local account used by
Bolt, and the directory will be created if it does not exist. This directory
is used by Bolt to stage temporary files on the target system.

.. NOTE::

   By default, SIMP disables running files from the ``/tmp`` mount, so the home
   directory should not be on the ``/tmp`` partition.

The ``create_user`` parameter indicates whether the specified user account
should be managed as a Puppet resource. If an existing account is utilized,
this can be left with the default value of ``false``; however, if a new account
is required, an opt-in strategy requiring this parameter be set to ``true`` has
been implemented.

SIMP's Bolt configuration uses :term:`ssh` by default to connect from a Bolt
controller to a target system. Although both parameters are identified as
optional in manifest and undefined by default, it is required that at least one
of ``user_password`` or ``user_ssh_authorized_keys`` be configured for
authentication from remote systems if ``create_user`` is ``true``.

The ``disallowed_users`` parameter specifies accounts not permitted to be
managed by SIMP Bolt. This is intended to prevent system accounts, such as
``root``, from be configured in such a fashion as to render a system
inoperable.

The ``user_allowed_from`` parameters resolves to the puppet server generating
the manifest by default. It should be set to the Bolt controller(s) that will
manage the target system. This is intended as a security feature to ensure
access to an account with ``sudo`` privileges is restricted to selected
sources.

Setting up a Bolt Controller
----------------------------

The controller portion of SIMP Bolt configures a system to run Bolt for
execution of tasks on the remote systems. To configure a controller system, the
``simp_bolt`` class must be included on the target node and
``simp_bolt::bolt_controller`` must be set to ``true`` via :term:`Hiera`.

With just these settings, Bolt will be installed on the controller system and
it should be able to execute tasks on a target system. Once again there are a
number other parameters that can be specified to refine or customize its
performance, including of the following significant parameters and their
default values:

.. code-block:: yaml

   ---
   simp_bolt::controller::local_user_name:           undef
   simp_bolt::controller::local_user_home:           undef
   simp_bolt::controller::config::disable_analytics: true
   simp_bolt::controller::config::config_hash:       undef

The ``local_user_name`` parameter specifies the account to be used on the
controller to issue Bolt commands. SIMP does not create or manage this
account but does use it to set file permissions.

The ``local_user_home`` parameter is used to determine where files associated
with Bolt should be saved. If ``local_user_name`` or ``local_user_home`` are
not specified, SIMP will default to creating the files in
``/var/local/simp_bolt`` with ``root`` ownership but world readable permissions
so the files can be used as a template for other users to copy to their home
directory.

By default, SIMP opts-out of the Bolt analytics data collection to comply with
best practices and :term:`NIST information limiting requirements`. To opt-in,
change the ``disable_analytics`` parameter to ``false``.

The optional ``config_hash`` parameter can used to specify the desired content
of the `bolt.yaml configuration file`_.  If this parameter is specified, all
other configuration parameters will be ignored.

Using Bolt with Existing Puppet Modules
---------------------------------------

Once Bolt is installed, it can be used execute tasks on remote systems.  The
`Bolt documentation`_ provides detailed instructions on how to use Bolt for
basic commands.  The remainder of this section will focus on using Bolt to
manage and apply existing Puppet modules.

To view a list of modules available to Bolt, execute the following command
as the local user on the Bolt controller:

.. code-block:: bash

  bolt puppetfile show-modules

The output of this command should be a list of modules. To download additional
modules from the Puppet Forge or a Git repository, create a :term:`Puppetfile`
in the Bolt project directory for the local user on the controller. This will
be ``~{local_user_name}/.puppetlabs/bolt`` if it was specified; if not it would
be wherever the ``/var/local/simp_bolt/puppetlabs/bolt`` directory was copied
from the template. To specify modules to install, add them the ``Puppetfile``,
using the following format:

.. code-block:: puppet

   # To specify modules from the Puppet Forge
   mod 'puppetlabs-stdlib', '5.2.0'
   mod 'simp-simplib', '3.13.0'

   # To specify modules from a Git repository
   mod 'simp-simplib', git: 'https://github.com/simp/pupmod-simp-simplib.git', ref: '3.13.0'

Then execute the command:

.. code-block:: bash

  bolt puppetfile install

to download and install the specified modules.

To configure Hiera for Bolt, create a ``hiera.yaml`` in the Bolt project
directory, updating as necessary.

.. code-block:: yaml

   ---
   version: 5

   defaults:  # Used for any hierarchy level that omits these keys.
     datadir: data  # This path is relative to the environment -- <ENVIRONMENT>/data
     data_hash: yaml_data  # Use the built-in YAML backend.

   hierarchy:
     - name: "Per-node data"                   # Human-readable name.
       path: "nodes/%{trusted.certname}.yaml"  # File path, relative to datadir.
                                      # ^^^ IMPORTANT: include the file extension!

     - name: "Per-OS defaults"
       path: "os/%{facts.os.family}.yaml"

     - name: "Common data"
       path: "common.yaml"

Hiera data can then be specified as needed by making a ``data`` directory in
the Bolt project directory and then creating the appropriate YAML files in the
directory.

To apply a module to a Bolt target, create a manifest file, such as ``site.pp``,
in the Bolt project directory.  In its simplest form, the manifest would
call the desired module and consist of:

.. code-block:: puppet

   include simplib

The manifest can then be applied to target systems with the command:

.. code-block:: bash

  bolt apply site.pp --nodes 'comma, separated, list, of, target, nodes'

As mentioned previously, Bolt is configured to use ssh as its transport
mechanism to remote systems so it may be necessary to troubleshoot the
connection.  Some of the common issues could be:

  * No entry for the target system in the known hosts file,
  * The private key file corresponding to the public
    ``user_ssh_authorized_keys`` may not be available, or
  * The ``--password`` option should be specified to prompt for a password when
    connecting to the target system.

.. NOTE::

   Users should verify that an ``ssh`` connection can be established from the
   controller system to the target system as the ``target_user_name`` prior to
   trying to execute a Bolt command.

.. _Bolt documentation: https://puppet.com/docs/bolt/latest/bolt.html
.. _bolt.yaml configuration file: https://puppet.com/docs/bolt/latest/bolt_configuration_options.html

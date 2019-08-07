HOWTO Customize Settings for SSH
================================

By default, SIMP will include the ``ssh`` class from the ``simp-ssh`` module
under most of the :ref:`SIMP scenarios <simp scenarios>`.  If it is absent
from the scenario you have selected, add the following to :term:`Hiera` to
include the class:

.. code-block:: yaml

   simp::classes:
     - 'ssh'

To exclude the SIMP ``ssh`` class so that you can manage SSH configuration
by other means, refer to :ref:`Disable SSH Management <disable_ssh_management>`.

The SIMP ``ssh`` class is configured to automatically include the
``ssh::server`` and ``ssh::client`` classes. These classes, in turn, manage
the SSH daemon settings for incoming connections and the SSH client settings
for outgoing connections, respectively. They configure SSH with reasonable
defaults for the OS and environment.

If you want to disable SIMP management of either the SSH server or client
settings, you can do so through Hiera.  For example, to disable SIMP management
of both:

.. code-block:: yaml

   ssh::enable_client: false
   ssh::enable_server: false


Managing Settings for the SSH Server
------------------------------------

The ``ssh::server`` class is responsible for configuring ``sshd``.  It delegates
configuration of specific ``sshd`` settings found in ``sshd_config`` to
``ssh::server::conf``.  Detailed descriptions of the these settings are provided
in the ``sshd_config(5)`` man page.

The ``ssh::server::conf`` class uses a number of SIMP-specific parameters, such
as whether the system is FIPS enabled, has a firewall, or utilizes LDAP. This
allows seamless integration of SSH with other SIMP-managed applications in the
larger SIMP environment.  For example, ``ssh::server::conf`` ensures the
appropriate fallback ciphers are used, ensures proper authentication is
configured, and allows SSH traffic through the firewall.  More detailed
information is provided in the ``simp-ssh`` module README file.

Configuring ``ssh::server::conf`` from Hiera
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``ssh::server::conf`` provides default configuration for key ``sshd_config``
settings and a mechanism to set most other settings.

To customize the SSH server, edit the parameters of ``ssh::server::conf`` using
Hiera or :term:`ENC`.

.. NOTE::

   Unlike many SIMP modules, these customizations cannot be made
   directly with a resource-style class declaration ― they *must* be
   made via automatic parameter lookup provided by Hiera or an ENC.
   Examples using Hiera are provided for illustrative purposes.

In Hiera:

.. code-block:: yaml

   ssh::server::conf::port: 2222
   ssh::server::conf::passwordauthentication: false

Starting with version **6.8.0** of the ``simp-ssh`` module, multiple ports
can be specified to listen for incoming SSH connections. So the
``ssh::server::conf::port`` parameter in the previous example could be set
as follows in Hiera to listen on multiple ports:

.. code-block:: yaml

   ssh::server::conf::port: [22, 2222, 22222]

Managing Additional Settings with ``sshd_config``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Starting with version **6.7.0** of the ``simp-ssh`` module, you can manage
additional settings not explicitly mapped to ``ssh::server::conf`` parameters,
using the ``ssh::server::conf::custom_entries`` parameter.  For example, to specify
configuration for ``AllowAgentForwarding`` and ``AuthorizedPrincipalsCommand``
``sshd`` settings, you would include Hiera such as the following:

.. code-block:: yaml

  ssh::server::conf::custom_entries:
    AllowAgentForwarding: "yes"
    AuthorizedPrincipalsCommand: "/usr/local/bin/my_command"

There are a few limitations with ``ssh::server::conf::custom_entries`` that
need to be noted:

* *No setting validation*:
    This parameter is **not validated**. Be careful to only specify settings
    that are allowed for your particular SSH daemon and avoid duplicate
    declaration of settings already specified.  Invalid options may cause the
    ``sshd`` service to fail on restart. Duplicate settings will result in
    duplicate Puppet resources (i.e., manifest compilation failures).

* *No direct MATCH entry support*:
     Due to their complexity, ``Match`` entries are not supported.  However,
     you can add them using the ``sshd_config_match`` resource from the
     `herculesteam-augeasproviders_ssh`_ module.  Since ``simp-ssh`` uses
     this module internally, the ``sshd_config_match`` resource will be
     available to you on any node using ``simp-ssh``.


Managing Settings for the SSH Client
------------------------------------

The ``ssh::client`` class is responsible for configuring default client settings
for outgoing SSH sessions to all hosts (``Host *``).


Managing Settings for the Default Host Entry (``Host *``)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you want to customize the default settings, you must prevent ``ssh::client``
from declaring them automatically and then declare ``Host *`` settings manually.
You do this by setting ``ssh::client::add_default_entry`` to ``false`` and
using the defined type ``ssh::client::host_config_entry``.  For example:

In Hiera:

.. code-block:: yaml

   ssh::client::add_default_entry: false

In Puppet:

.. code-block:: puppet

   ssh::client::host_config_entry{ '*':
     gssapiauthentication      => true,
     gssapikeyexchange         => true,
     gssapidelegatecredentials => true,
   }


Managing Client Settings for Specific Hosts
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Different settings for particular hosts can be managed by using the defined
type ``ssh::client::host_config_entry``:

.. code-block:: puppet

   # `ancient.switch.fqdn` only understands old ciphers:
   ssh::client::host_config_entry { 'ancient.switch.fqdn':
     ciphers => [ 'aes128-cbc', '3des-cbc' ],
   }


Managing Additional Settings with ``ssh_config``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Starting with version **6.4.0** of the ``simp-ssh`` module, you can use the
`ssh_config`_ resource from the `herculesteam-augeasproviders_ssh`_ module to
manage settings that the module does not cover.

For instance, to ensure that the default host entry's ``RequestTTY`` option is
set to ``auto``:

.. code-block:: puppet

   # RequestTTY is not managed by ssh::client::host_config_entry
   ssh_config { 'Global RequestTTY':
     ensure => present,
     key    => 'RequestTTY',
     value  => 'auto',
   }

.. _herculesteam-augeasproviders_ssh: https://github.com/hercules-team/augeasproviders_ssh
.. _ssh_config: https://github.com/hercules-team/augeasproviders_ssh/blob/master/README.md

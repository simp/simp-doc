HOWTO Customize Settings for SSH
================================

By default, SIMP will include the ``simp-ssh`` module ``ssh`` class under all
deployment scenarios. To exclude the SIMP ``ssh`` class, refer to
:ref:`Disable SSH Managment <disable_ssh>`.

The SIMP ``ssh`` class is configured to automatically include the
``ssh::server`` and ``ssh::client`` classes. These classes manage the SSH
daemon settings for incoming connections and the SSH client settings for
outgoing connections respectively, and are configured with reasonable defaults
for the OS and environment. To override this and **disable management** of one
or both of these classes and manage them through some other mechanism, add the
following to :term:`Hiera`:

.. code-block:: yaml
   ssh::enable_client: false
   ssh::enable_server: false


Managing Settings for the SSH Server
------------------------------------

As stated above, the ``simp-ssh`` module includes the ``ssh::server`` class and
provides "sane" settings for each host's environment by default. Detailed
descriptions of the various settings are provided in the sshd_config(5) man
page.


Configuring ``ssh::server::conf`` from Hiera
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Many standard sshd_config settings for the SSH server are included with default
values in the ``ssh::server::conf`` class. To customize the SSH server, edit
the parameters of ``ssh::server::conf`` using Hiera or :term:`ENC`.

.. NOTE::

   Unlike many SIMP modules, these customizations cannot be made
   directly with a resource-style class declaration―they *must* be
   made via automatic parameter lookup provided by Hiera or ENC.
   Examples using Hiera are provided for illustrative purposes.

In Hiera:

.. code-block:: yaml

   ssh::server::conf::port: 2222
   ssh::server::conf::passwordauthentication: false


Managing Additional Settings with ``sshd_config``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Starting with version **6.4.0** of the ``simp-ssh`` module, you can use the
`sshd_config`_ resource from the ``augeasproviders_ssh`` module to manage
settings that the module does specifically define. To manage global sshd_config
settings that are not included in the ``ssh::server::conf`` class, specify them
in Hiera with the ``ssh::server::conf::custom_entries`` parameter as follows:

.. NOTE::

   Due to their complexity, ``Match`` entries are not supported and will
   need to be added using ``sshd_config_match`` resources as described in
   ``augeasproviders_ssh`` module.

.. code-block:: yaml

  ssh::server::conf::custom_entries:
    AllowAgentForwarding: "yes"
    AuthorizedPrincipalsCommand: "/usr/local/bin/my_command"
 
.. NOTE::

   This parameter is **not validated**. Be careful to only specify settings
   that are only allowed for your particular SSH daemon and avoid duplicate
   declaration of resources already specified. Invalid options may cause the
   ssh service to fail on restart.

There are also number of SIMP specific parameters, such as whether the system
is FIPS enabled, has a firewall, or utilizes LDAP. The ``ssh::server::config``
class utilizes ``simplib::lookup`` as well as retrieving parameters from the
``ssh::server::params`` class to help integrate the ``simp-ssh`` module within
the larger SIMP environment. This includes things like determining appropriate
fallback ciphers for inclusion in ``sshd_config``, ensuring proper
authentication, and that SSH traffic passes through the firewall. More
detailed information is provided in the ``simp-ssh`` module README file.


Managing Settings for the SSH Client
------------------------------------

The ``ssh::client`` class, also included by default as part of the ``simp-ssh``
module, will automatically manage client settings as the default for outgoing
SSH sessions to all hosts (``Host *``).


Managing Settings for the Default Host Entry (``Host *``)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you want to customize the default settings, you must prevent ``ssh::client``
from declaring them automatically with ``ssh::client::add_default_entry: false``
and declare ``Host *`` manually with the defined type
``ssh::client::host_config_entry``:

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

Starting with version **6.4.0** of the **simp-ssh** module, you can use the
`ssh_config`_ resource from the ``augeasproviders_ssh`` module to manage 
settings that the module does not cover.

For instance, to ensure that the default host entry's ``RequestTTY`` option is
set to ``auto``:

.. code-block:: puppet

   # RequestTTY is not managed by ssh::client::host_config_entry
   ssh_config { 'Global RequestTTY':
     ensure => present,
     key    => 'RequestTTY',
     value  => 'auto',
   }


Environments that use **simp-ssh** versions prior to **6.4.0** will not be
able to make further customizations using ``ssh_config`` resource, because it
will conflict with the internal implementation of
``ssh::client::host_config_entry``. However, users can still add extra SSH
client configurations by editing their ``$HOME/.ssh/config`` files.

.. _sshd_config: http://augeasproviders.com/documentation/examples.html#sshdconfig-provider
.. _ssh_config: http://augeasproviders.com/documentation/examples.html#sshconfig-provider

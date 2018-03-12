How do I customize settings for SSH?
------------------------------------

By default, including the **ssh** module will include ``ssh::server`` and
``ssh::client``, configured with reasonable defaults for the OS & environment:

.. code-block:: puppet

   include 'ssh'

.. NOTE::

   The examples below feature ``include 'ssh::server'`` and ``include
   'ssh::client'``, but most SIMP scenarios already include them both via
   ``ssh``


How do I manage settings for the SSH server?
=======================================================

Including `ssh::server` with the default options will manage the server with
reasonable settings for each host's environment.


Configuring ``ssh::server::conf`` from Hiera
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To customize the SSH server, edit the parameters of `ssh::server::conf` using
Hiera or ENC (Automatic Parameter Lookup).

.. NOTE::

    Unlike many SIMP modules, these customizations cannot be made
    directly with a resource-style class declaration―they *must* be made via
    APL.

In Hiera:

.. code-block:: yaml

   ssh::server::conf::port: 2222
   ssh::server::conf::ciphers:
   - 'chacha20-poly1305@openssh.com'
   - 'aes256-ctr'
   - 'aes256-gcm@openssh.com

In Puppet:

.. code-block:: puppet

   include 'ssh::server'

   # Alternative:
   # if `ssh::enable_server: true`, this will also work
   include 'ssh'


Managing additional settings with ``sshd_config``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To manage SSH server settings that aren't managed by the SIMP ``ssh`` module,
use the ``sshd_config`` resource from `augeasproviders_ssh`_.  This is what the
SIMP ``ssh`` module uses internally to manage the ``/etc/ssh/sshd_config``
file, and you can use it to set any options ``ssh::server::conf`` doesn't use.

For instance, to set the sshd `LogLevel`_ option to ``VERBOSE``:

.. code-block:: puppet

   # VERBOSE will log SSH key fingerprints used for logins
   sshd_config { 'LogLevel' : value => 'VERBOSE' }


Mixing ``ssh::server::conf`` and ``sshd_config``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some SSH server configurations may require a combination of
``ssh::server::conf`` (for options that SIMP manages) and ``sshd_config``
resources (for additional options). The following example configures the
``/etc/ssh/sshd_config`` keys ``GSSAPIAuthentication``, ``GSSAPIKeyExchange``,
and ``GSSAPICleanupCredentials`` with a value of "**yes**":

In Hiera:

.. code-block:: yaml

   # GSSAPIKeyExchange + GSSAPICleanupCredentials are managed via sshd_config
   ssh::server::conf::gssapiauthentication: true

In Puppet:

.. code-block:: puppet

   include 'ssh::server'

   sshd_config {
    default:
      ensure => 'present',
      value  => 'yes',
    ;
    # GSSAPIAuthentication is managed via `ssh::server::conf::gssapiauthentication`
    ['GSSAPIKeyExchange', 'GSSAPICleanupCredentials']:
      # use defaults
    ;
   }



How do I manage settings for the SSH client?
============================================

Including ``ssh::client`` will automatically manage client settings as the
default for all hosts (``Host *``).


Managing settings for the default Host entry (``Host *``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you want to customize the default settings, you must prevent ``ssh::client``
from declaring them automatically with ``ssh::client::add_default_entry: false``
and declare ``Host *`` manually with the defined type
``ssh::client::host_config_entry``:

.. code-block:: puppet

   class{ 'ssh::client': add_default_entry => false }

   ssh::client::host_config_entry{ '*':
     gssapiauthentication      => true,
     gssapikeyexchange         => true,
     gssapidelegatecredentials => true,
   }


Managing client settings for specific hosts
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Different settings for particular hosts can be managed by using the defined
type ``ssh::client::host_config_entry``:

.. code-block:: puppet

   # `ancient.switch.fqdn` only understands old ciphers:
   ssh::client::host_config_entry { 'ancient.switch.fqdn':
     ciphers => [ 'aes128-cbc', '3des-cbc' ],
   }


Managing additional settings with ``ssh_config``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Starting with version **6.4.0** of the **simp-ssh** module, you can use the
``sshd_config`` resource from `augeasproviders_ssh`_ to manage settings that the
module doesn't cover.

For instance, to ensure that the default host entry's ``RequestTTY`` option is
set to ``auto``:

.. code-block:: puppet

   # RequestTTY isn't managed by ssh::client::host_config_entry
   ssh_config { 'Global RequestTTY':
     ensure => present,
     key    => 'RequestTTY',
     value  => 'auto',
   }


Environments that use **simp-ssh** versions prior to **6.4.0** will not be
able to make further customizations using ``ssh_config`` resource, because it
will conflict with the internal implementation of
``ssh::client::host_config_entry``.  However, users can still add extra SSH
client configurations by editing their ``$HOME/.ssh/config`` files.

.. _augeasproviders_ssh: http://augeasproviders.com/documentation/examples.html#sshdconfig-provider

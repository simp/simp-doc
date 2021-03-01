.. _howto-set-up-and-utilize-hiera-eyaml:

HOWTO Set up and Utilize hiera-eyaml
====================================

.. contents::
  :depth: 2
  :local:

This section steps through configuring a :term:`Puppet environment`'s hierarchy
to use the ":term:`eyaml`" Hiera backend, which enables you to keep encrypted
data in otherwise plaintext :term:`Hiera` files.  The examples use ``pkcs7``,
which is the default and built-in encryption mechanism. Additional encryption
mechanisms (like :term:`GPG`) are available as plugins, distributed as
RubyGems.

For additional information on configuring and using eyaml, see the Puppet
documentation on `configuring hiera-eyaml`_ and VoxPupuli's `hiera-eyaml
documentation`_.

Installing the :package:`hiera-eyaml` gem
-----------------------------------------

A user must have the :package:`hiera-eyaml` gem installed in their Ruby
environment in order to manage secrets with the :command:`eyaml` command.

.. code-block::  bash

   $ gem install hiera-eyaml

If you are unable to install the gem from your system, you may be able to use
the version that ships with the Puppet Agent at
:file:`/opt/puppetlabs/puppet/bin/eyaml`.  You may need privileged access to
run it, depending on the :package:`puppet-agent` package and the system's umask
when it was installed.


Configuring the Hiera hierarchy to use eyaml
--------------------------------------------

At the top level of your :term:`Puppet Environment`, edit the
:file:`hiera.yaml` file and add hierarchy tiers using the :code:`eyaml`
backend.

The following is an example of an eyaml tier in the Hiera hierarchy.
Starting with SIMP 6.6.0, the :package:`simp-environment-skeleton` RPM
has been updated to provide eyaml entries like this one.

.. code-block:: yaml

  ---
  defaults:
    - datadir: data

  hierarchy:
    - name: Eyaml Data
      lookup_key: eyaml_lookup_key
      paths:
      - "hosts/%{trusted.certname}.eyaml"
      - "hosts/%{facts.fqdn}.eyaml"
      - "hosts/%{facts.hostname}.eyaml"
      options:
        pkcs7_private_key: /var/lib/puppet/keys/private_key.pkcs7.pem
        pkcs7_public_key: /var/lib/puppet/keys/public_key.pkcs7.pem
  ...

Note the :code:`lookup_key` and :code:`options` keys, and that the data files
have an :file:`.eyaml` extension.

Note also that each hierarchy tier defines its own :code:`options`, so in more
advanced situations, you can configure Hiera to decrypt data from multiple key
pairs using duplicate tiers with different keys.

Generating the keys
--------------------

Use the :command:`eyaml` command to generate the keys specified in each
hierarchy tier's :code:`options:`. Make sure the permissions on the keys are
set securely, but that the Puppet Server has access to them.

.. code-block:: console

  # mkdir -p /var/lib/puppet/keys/
  # /opt/puppetlabs/puppet/bin/eyaml createkeys \
     --pkcs7-private-key=/var/lib/puppet/keys/private_key.pkcs7.pem \
     --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem
  # cd /var/lib/
  # chown -R puppet puppet
  # chmod 500 puppet
  # chmod 400 puppet/*.pem


Make sure the keys are backed up.



Encrypting Hiera values
-----------------------

Once the environment's hierarchy is set up, data can be encrypted and stored as
values in the :file:`.eyaml` files.  The following is an example of how to
encrypt the passwords used by the puppet module :pupmod:`simp/simp_snmpd`.

The :pupmod:`simp_snmpd` module accepts a Hash containing several
credentials in the :code:`simp_snmpd::v3_users_hash` parameter, which is used
to configure the net-snmp users.

Unencrypted, this sensitive data would be exposed as plaintext in
a file like
:file:`/etc/puppetlabs/code/environments/production/data/common.yaml`, looking
something like this:

.. code-block:: yaml

  simp_snmpd::v3_users_hash:
    snmp_ro:
      authtype: 'SHA'
      privtype: 'AES'
      authpass: 'MyAuthPassw0rd'
      privpass: 'MyPrivPassw0rd'
    snmp_rw:
      authtype: 'SHA'
      privtype: 'AES'
      authpass: 'MyOtherAuthPassw0rd'
      privpass: 'MyOtherPrivPassw0rd'

This exposes the passwords to everyone with access to the production
environment's files.  If you deploy your environment from a :term:`control
repository`, they are also exposed to everyone with read access to the
repository.  To avoid this, we will safeguard the sensitive data by encrypting
it.

To encrypt the passwords, you need access to the file defined as the eyaml
backend's :code:`pkcs7_public_key`, which can be safely distributed to users.

Use the :command:`eyaml encrypt` command to use the public key and encrypt each
password string:

.. code-block:: console

  # eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyAuthPassw0rd'
  # eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyPrivPassw0rd'
  # eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyOtherAuthPassw0rd'
  # eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyOtherPrivPassw0rd'

The output for each of the commands will look something like the following:

::

  string: ENC[PKCS7,MIIBeQYJKoZIhvcNAQcDoIIBajCCAWYCAQAxggEhMIIBHQIBADAFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEAKNBCkXENUf6C0diKcV1VPvB4r8q+AFzu9E4VsR9Ch50q0UJ5sO977VXWLkX1oYbEvqPZZrmH122gvrYp1xux+W+UuFZbCzMQ7AMNe8eiJ7FvYYs79/leJIYouylfPod9G/M1SC/Lw64fhzcC7dSOru+vJan3zT1Jp/7nmsen263VBihOshbtkHKLSoJ7n96MlFqF0CrzOzxoz/p3y2591FoSXqjljCGG0PmV9FGONe1n5vUwWuy/+YQlciZEtyjyUBCZyJgaWfFh6//6vJT4G+5i0Ui1xzAtvYaDKW968Yx3ldQYy7btiRYct4Xvh6giFWDLXIE5Mnfe4fH6NwwXHDA8BgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBAXOTJRuXWXBSfxIlA9HqWfgBBhi06bLLsVsjQ2leNYg2N5]

or:

::

  block: >
    ENC[PKCS7,MIIBeQYJKoZIhvcNAQcDoIIBajCCAWYCAQAxggEhMIIBHQIBAD
    AFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEAKNBCkXENUf6C0diKcV1VPvB4r8
    q+AFzu9E4VsR9Ch50q0UJ5sO977VXWLkX1oYbEvqPZZrmH122gvrYp1xux+W
    +UuFZbCzMQ7AMNe8eiJ7FvYYs79/leJIYouylfPod9G/M1SC/Lw64fhzcC7d
    SOru+vJan3zT1Jp/7nmsen263VBihOshbtkHKLSoJ7n96MlFqF0CrzOzxoz/
    p3y2591FoSXqjljCGG0PmV9FGONe1n5vUwWuy/+YQlciZEtyjyUBCZyJgaWf
    Fh6//6vJT4G+5i0Ui1xzAtvYaDKW968Yx3ldQYy7btiRYct4Xvh6giFWDLXI
    E5Mnfe4fH6NwwXHDA8BgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBAXOTJRuX
    WXBSfxIlA9HqWfgBBhi06bLLsVsjQ2leNYg2N5]

Edit the :file:`/etc/puppetlabs/code/environments/production/data/common.eyaml`
and include the :code:`simp_snmpd::v3_users_hash` with the passwords replaced
by the encrypted values provided by the eyaml command.

.. code-block:: yaml

  ---
  simp_snmpd::v3_users_hash:
    snmp_ro:
      authtype: 'SHA'
      privtype: 'AES'
      authpass: ENC[PKCS7,MIIBeQYJKoZIhvcNAQcDoIIBajCCAWYCAQAxggEhMIIBHQIBADAFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEAKNBCkXENUf6C0diKcV1VPvB4r8q+AFzu9E4VsR9Ch50q0UJ5sO977VXWLkX1oYbEvqPZZrmH122gvrYp1xux+W+UuFZbCzMQ7AMNe8eiJ7FvYYs79/leJIYouylfPod9G/M1SC/Lw64fhzcC7dSOru+vJan3zT1Jp/7nmsen263VBihOshbtkHKLSoJ7n96MlFqF0CrzOzxoz/p3y2591FoSXqjljCGG0PmV9FGONe1n5vUwWuy/+YQlciZEtyjyUBCZyJgaWfFh6//6vJT4G+5i0Ui1xzAtvYaDKW968Yx3ldQYy7btiRYct4Xvh6giFWDLXIE5Mnfe4fH6NwwXHDA8BgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBAXOTJRuXWXBSfxIlA9HqWfgBBhi06bLLsVsjQ2leNYg2N5]
      privpass: ENC[PKCS7,MIIBeQYJKoZIhvcNAQcDoIIBajCCAWYCAQAxggEhMIIBHQIBADAFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEASZjHjGiEAaseJ3tOYCKLmAMgGpzyj6QdERI2Jo5nUaRwGTRZS11ndOodNNxt+F2hSrBX0J+U4TppFX/LxnJqrs/FeuhSrCSBkU9G2IsEcMD5k1KWvS5k81RwNO5DtXLnoEarxrAXkej6n4KAj1fU8o+6/Fe9gBP2zUtJAv+xpZp22AzOlUvjRMZo3aXNUuTLYhKrFJ6bcZHotkVM4gFcGX+AEoN6e4rQfBK9AK1ZxRjJWT675/oNW7Pa5naeucCKsFtFfWakLk5Hk7jZDqzlqlu3wpHLeRuWDqOxkM3i7WdNVKGlES3JMjTYIYNao21nACB4rj1FdqReXVsfiMYLkzA8BgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBDS6I2+phbPGn4Xc4arDsRVgBCZweW8h1nQ8Lxdysu1es2D]
    snmp_rw:
      authtype: 'SHA'
      privtype: 'AES'
      authpass: ENC[PKCS7,MIIBiQYJKoZIhvcNAQcDoIIBejCCAXYCAQAxggEhMIIBHQIBADAFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEAjxzvezqyGrgycNf26CFBsl2QNYYo0amVVNynLYYIrJtCGH9E6q5GxKkRDRtaJNM65WxmSVrzVVbNng0mB80JAw0lzQQlVDstWmYt27A1IOz1QRR3uZiBBqCULesMK2Sw50ObawMEv7U0jIWqLypBWJ4YQI1wEaebt4drJTwWSAYfh9oUS2KHYZzLWsbQ8Skiiavh9iyD8TqUdPM7AgeR83hk5o5Z/8zvn+gx3rz8bHc+vVH3Fnej9Zj3FMoRbaYJAs2iNqii7ew398rumWy3TiqeV8isiAft7HnH1r/zXDTfndgTsxyd0guu5ZFM7ecqVyS8Sqkc7nHYeR5u8o1NWTBMBgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBDFz6fVKaPmMshbKvs7Ft7IgCBUHuRUAPIVTnvtmlS1iN13wMLN0k7tNwRQav2MOSr62g==]
      privpass: ENC[PKCS7,MIIBiQYJKoZIhvcNAQcDoIIBejCCAXYCAQAxggEhMIIBHQIBADAFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEAPROZvDIFre7M3+Bs2QfG9YpXgCRaoayD80Ni2UtUcW8ffoks3f2ufIYoxqgn2DrxmastoRVyyu8Q1G/hAl9J/zg13znafT+eLHsa6ds7YqlM208VVlxYWfl/zhWEW8U3KYhzlHRo9TIXw5w5yAtpYVknF0UL5+MFhCrHKBES92PPq4hS+X0E/o0Mk1zDu24ZgvT8+BRVH+7GmvLPQ+rrT89ou3ovy/PRTu6jf2ppX9M1NFJAxB+bskEA9PMzgPshEGs85ns25mNknFrKG8R8YxejVm0l6JD5DTzWCEghnGkP799Kssem5PC8cD7BvaDJdmBrA8CnQ7iVYjILl3ltazBMBgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBAulg1vUcbivI6BGHePIF8ZgCAjWpQXSV2fG4XPBDxXWAfHXVAVvUj4RWiq7IOcmO9tZA==]

Save the file and make sure the Puppet Server has access to read it.

.. code-block:: console

  # chown root:puppet /etc/puppetlabs/code/environments/production/data/common.eyaml
  # chmod 0640 /etc/puppetlabs/code/environments/production/data/common.eyaml

Remove the unencrypted :code:`simp_snmpd::v3_users_hash` key from
:file:`/etc/puppetlabs/code/environments/production/data/common.yaml` so the
passwords are no longer visible.

Run :command:`puppet agent -t` on an agent node in the ``production``
environment where :pupmod:`simp_snmpd` is classified.  The net-snmp users'
credentials should be configured using the decrypted values.


Using :command:`eyaml edit`
---------------------------

If a user editing the :file:`.eyaml` file has access to both the private and
public keys, they can use :command:`eyaml edit file.eyaml` as a convenient
alternative to the :command:`eyaml encrypt` example in th previous section.

:command:`eyaml edit` will automatically decrypt the file and bring up an editor
to edit the values in plaintext.  After exiting the editor, any edited values
will be automatically re-encrypted in place.

It is also possible to encrypt blocks of data and entire files. See the
`hiera-eyaml documentation`_ for more details on these and other features.

.. _Configuring hiera-eyaml: https://puppet.com/docs/puppet/latest/hiera_config_yaml_5.html#configuring_a_hierarchy_level_hiera_eyaml
.. _hiera-eyaml documentation: https://github.com/voxpupuli/hiera-eyaml

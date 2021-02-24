.. _howto-set-up-and-utilize-hiera-eyaml:

HOWTO Set up and Utilize Hiera-eyaml
====================================

This section steps through configuring the :term:`hiera-eyaml`  backend
for a puppet environment.  This backend allows you to encrypt data in
the :term:`Hiera` files.

An example of how to use the :command:`eyaml` command provided by the :term:`hiera-eyaml`
gem to encrypt data is included at the end.

More details on configuring and using eyaml can be found in the puppet
documents for `Configuring hiera-eyaml`_ and in the `hiera-eyaml documentation`_.

Install the gem.
----------------

The hiera-eyaml backend has been included in Puppet Server since version 5.2.0.
If the Puppet Server does not already have the hiera-eyaml gem installed run:

.. code-block::  bash

   $ puppetserver gem install hiera-eyaml

The hiera-eyaml gem can be installed by users into their Ruby environment. This
will provide the :command:`eyaml` command used to encrypt the data.

.. code-block::  bash

   $ gem install hiera-eyaml

Set up the hiera eyaml hierarchy
--------------------------------

On the Puppet Server, in the top level of the :term:`Puppet Environment`,
:file:`/etc/puppetlabs/code/environments/<environment name>`, edit the
:file:`hiera.yaml` file and add the eyaml hierarchy.

The following is an example of an eyaml entry in the hiera hierarchy.
The :package:`simp-environment-skeleton` RPM was updated in the SIMP 6.6.0
release to contain an eyaml entries like this one.

Note the :code:`lookup_key` and :code:`options` keys.  Also note
that the files have a :code:`.eyaml` extension.

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

Generate the keys
-----------------

The keys location is configured under :code:`options` in the above hierarchy.
Use the :command:`eyaml` command to generate these keys. Make sure the permissions on the
keys are set securely, the Puppet Server has access to them and that they are backed up
once they are created.

.. code-block:: bash

  $ mkdir -p /var/lib/puppet/keys/
  $ sudo /opt/puppetlabs/puppet/bin/eyaml  createkeys \
     --pkcs7-private-key=/var/lib/puppet/keys/private_key.pkcs7.pem \
     --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem
  $ cd /var/lib/
  $ sudo chown -R puppet puppet
  $ sudo chmod  500 puppet
  $ sudo chmod  400 puppet/*.pem

Make sure the keys are backed up.

Encrypt values in hiera
-----------------------

Once the hierarchy is set up in an environment, hiera values can be encrypted
and stored in the :file:`.eyaml` files.  The following is an example of how to
encrypt the passwords used by the puppet module :pupmod:`simp/simp_snmpd`.

The :pupmod:`simp/simp_snmpd` module uses the :code:`simp_snmpd::v3_users_hash`
hiera data value to configure the net-snmp users.
Unencrypted, the hiera value would be in a common hiera file in the environment,
such as :file:`/etc/puppetlabs/code/environments/production/data/common.yaml`.
The setting would look like:

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

This would allow anyone with access to the production environment files to see the password.

To encrypt the passwords you need access to the hiera-eyaml pkcs7_public_key which
can be distributed to users.

Use the :command:`eyaml` install by the hiera-eyaml gem.  (If you do not have
access to the gem you can use the version installed for the Puppet Server at
:file:`/opt/puppetlabs/puppet/bin/eyaml`).

For each string that needs to be encrypted run the eyaml command:

.. code-block:: bash

  $ eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyAuthPassw0rd'
  $ eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyPrivPassw0rd'
  $ eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyAuthPassw0rd'
  $ eyaml encrypt --pkcs7-public-key=/var/lib/puppet/keys/public_key.pkcs7.pem  -s 'MyAuthPassw0rd'

The following is what the output will look like from each of the command:

::

  string: ENC[PKCS7,MIIBeQYJKoZIhvcNAQcDoIIBajCCAWYCAQAxggEhMIIBHQIBADAFMAACAQEwDQYJKoZIhvcNAQEBBQAEggEAKNBCkXENUf6C0diKcV1VPvB4r8q+AFzu9E4VsR9Ch50q0UJ5sO977VXWLkX1oYbEvqPZZrmH122gvrYp1xux+W+UuFZbCzMQ7AMNe8eiJ7FvYYs79/leJIYouylfPod9G/M1SC/Lw64fhzcC7dSOru+vJan3zT1Jp/7nmsen263VBihOshbtkHKLSoJ7n96MlFqF0CrzOzxoz/p3y2591FoSXqjljCGG0PmV9FGONe1n5vUwWuy/+YQlciZEtyjyUBCZyJgaWfFh6//6vJT4G+5i0Ui1xzAtvYaDKW968Yx3ldQYy7btiRYct4Xvh6giFWDLXIE5Mnfe4fH6NwwXHDA8BgkqhkiG9w0BBwEwHQYJYIZIAWUDBAEqBBAXOTJRuXWXBSfxIlA9HqWfgBBhi06bLLsVsjQ2leNYg2N5]

  OR

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

Edit the :file:`/etc/puppetlabs/code/environments/production/data/secrets/common.eyaml`
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

Save the file and make sure the puppet server can read it.

.. code-block:: bash

  $ chown root:puppet /etc/puppetlabs/code/environments/production/data/secrets/common.eyaml
  $ chmod 640 /etc/puppetlabs/code/environments/production/data/secrets/common.eyaml

Remove the :code:`simp_snmpd::v3_users_hash` key from
:file:`/etc/puppetlabs/code/environments/production/data/common.yaml` so the
passwords are no longer visible.

That completes the example.

If the user editing the eyaml file has access to both the private
and public keys the :command:`eyaml edit` option can be used which will allows you to edit the file
and add the data in plain text. The editor then encrypts the data when the file is saved.
Also blocks of data and entire files can be encrypted.  See the `hiera-eyaml documentation`_ for
more details on these and other features.

.. _Configuring hiera-eyaml: https://puppet.com/docs/puppet/latest/hiera_config_yaml_5.html#configuring_a_hierarchy_level_hiera_eyaml
.. _hiera-eyaml documentation: https://github.com/voxpupuli/hiera-eyaml

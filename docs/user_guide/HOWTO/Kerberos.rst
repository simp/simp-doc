.. _ug-howto-enable-kerberos:

HOWTO Enable Kerberos
=====================

For the latest documentation, see the documentation in the
`SIMP KRB5 Puppet Module`_.

The module helps administrators get a working :term:`KDC` in place and clients
configured to use the KDC.

The module, by default, sets up a fully functional KDC in your environment and
generates keytabs for one admin user, and all of your hosts that it can
discover via :ref:`keydist <Certificates>`.

.. IMPORTANT::

   If you want to let SIMP automatically handle all of your hosts, you should
   follow the README included with the SIMP provided ``krb5`` Puppet module and
   you should **NOT** proceed with this guide.

.. NOTE::

   The ``keydist`` discovery only works if the KDC is on the same system as
   your Puppet Server!

.. WARNING::

   For distribution of keys to work properly, you **must** add
   ``/var/simp/environments/<environment>/site_files`` to your environment's
   ``environment.conf`` file and restart the ``puppetserver`` process.

   The default in the ``simp`` environment is:

   ``modulepath = modules:/var/simp/environments/**simp**/site_files:$basemodulepath``

Beginning with krb5
-------------------

The following sections give a brief guide on how to get started with manual
Kerberos configuration and distribution of keytabs, for more information,
please see the `official Red Hat documentation`_.

Creating Admin Principals
^^^^^^^^^^^^^^^^^^^^^^^^^

ACL Configuration
"""""""""""""""""

The following Puppet code snippet will create an :term:`ACL` for your admin
user that is **probably** appropriate for your organization.

.. code:: puppet

  krb5_acl { "${facts['domain']}_admin":
    principal       => "*/admin@${facts['domain']}",
    operation_mask  => '*'
  }

Create Your Admin Principal
"""""""""""""""""""""""""""

Your first principal will be an admin principal and will be allowed to manage
the environment since it is in the ``admin`` group. This **must** be created on
the KDC system.

Run the following command, as root, to create your principal:

.. code:: bash

  # /usr/sbin/kadmin.local -r YOUR.DOMAIN -q "addprinc <username>/admin"

You can now do everything remotely using this principal. Load it using:

.. code:: bash

  # /usr/bin/kinit <username>/admin

Creating Host Principals
^^^^^^^^^^^^^^^^^^^^^^^^

Before you can really do anything with your hosts, you need to ensure that the
host itself has a keytab.

SIMP uses the ``/var/simp/environments/<client_environment>/site_files/krb5_files/files/keytabs/<client_fqdn>``
directory for each host to securely distribute keytabs to the clients.

On the KDC, generate a principal for each host in your environment using the
following command:

.. code:: bash

  # /usr/sbin/kadmin.local -r YOUR.DOMAIN -q 'addprinc -randkey host/<fqdn>'

Create Your Keytabs
"""""""""""""""""""

Then, create a separate keytab file for each of your created hosts using the
following command:

.. code:: bash

  # /usr/sbin/kadmin.local -r YOUR.DOMAIN -q 'ktadd -k <fqdn>.keytab host/<fqdn>'

Propagate the Keytabs
^^^^^^^^^^^^^^^^^^^^^

Move all of the resulting keytab files SECURELY to
``/var/simp/environments/<client_environment>/site_files/krb5_files/keytabs/<fqdn>``
on the Puppet master as appropriate for each file.

.. NOTE::

   Make sure that all of your keytab directories are readable by the group
   **puppet** and not the entire world!

Then, update your node declarations to ``include '::krb5::keytab'``.

Once the Puppet Agent runs on the clients, your keytabs will copied to
``/etc/krb5_keytabs``. The keytab matching the system ``fqdn`` will be set in
place as the default system keytab.

.. _SIMP KRB5 Puppet Module: https://github.com/simp/pupmod-simp-krb5
.. _official Red Hat documentation: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/managing_smart_cards/configuring_a_kerberos_5_server.html

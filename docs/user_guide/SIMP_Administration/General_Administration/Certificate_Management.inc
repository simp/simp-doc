.. _Certificate Management:

Certificate Management
----------------------

This section describes the two different types of certificates used in a SIMP
system and how to manage them. For information on initial certificate setup,
refer to the :ref:`Certificates` section of :ref:`Client_Management`.


Infrastructure Certificates
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Server certificates are the standard :term:`PKI` certificates assigned either
by an official :term:`CA` (preferred) or generated using the FakeCA utility
offered by SIMP. Generated certificates are placed in the ``/etc/pki/simp``
directory of all managed systems.  These certificates are set to expire
annually. To change this, edit the following files with the number of days for
the desired lifespan of the certificates:

.. NOTE::

    This assumes that the user has generated Certificates with the FakeCA
    provided by SIMP. If official certificates are being used, these settings
    **must be changed within the official CA, not on the SIMP system**.

-  ``/var/simp/environments/simp/FakeCA/CA``

-  ``/var/simp/environments/simp/FakeCA/ca.cnf``

-  ``/var/simp/environments/simp/FakeCA/default\_altnames.cnf``

-  ``/var/simp/environments/simp/FakeCA/default.cnf``

-  ``/var/simp/environments/simp/FakeCA/user.cnf``

In addition, any certificates that have already been created and signed will
have a config file containing all of its details in
``/var/simp/environments/simp/FakeCA/output/conf/``.

.. IMPORTANT::

    Editing any entries in the above mentioned config files will **not** affect
    existing certificates. Existing certificates must be regenerated if you need
    to make changes.

The following is an example of how to change the expiration time from one year
(the default) to five years for any newly created certificate.

.. code-block:: bash

   for file in $(grep -rl 365 /var/simp/environments/simp/FakeCA/)
   do
      sed -i 's/365/1825/' $file
   done


Puppet Certificates
^^^^^^^^^^^^^^^^^^^

Puppet certificates are issued and maintained strictly within Puppet.  They are
different from the server certificates and should be managed with the
``puppet cert`` utility.

For documentation on the ``puppet cert`` tool, visit the `Puppet Inc. cert manual`_.

You can find the location for the Puppet certificates on your system by running
``puppet config print ssldir``.

.. NOTE::

   By default, Puppet certificates expire every five (5) years.

.. _Puppet Inc. cert manual: https://docs.puppet.com/puppet/latest/man/cert.html

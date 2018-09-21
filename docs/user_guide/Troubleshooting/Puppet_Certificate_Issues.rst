.. _ug-puppet-certificate-issues:

Puppet Certificate Issues
=========================

Puppet Client Certificate Issues
--------------------------------

.. WARNING::

  **NEVER RUN ``puppet cert clean --all``**

Most of the time, clients will have certificate issues due to the system clock
not being properly set. Before taking any other measures, make sure that your
system clock is correct on both the master and the clients!

If you need to fix client certificate issues outside of time, first make sure
that you do not have a certificate already in place on your :term:`Puppet Server`.

.. code-block:: bash

  # puppet cert list --all

If you **do** have a certificate in place, and need to register a client with
the same name, remove that client's certificate from the system.

.. code-block:: bash

  # puppet cert clean <fqdn.of.the.client>

.. WARNING::

  If you delete the Puppet master's certificate, you will need to re-deploy
  Puppet certificates to **all** of your nodes!

.. WARNING::

  **NEVER RUN ``puppet cert clean --all``**

.. _rereg-puppet-client-certs:

Puppet Client Re-Registration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If, for some reason, you need to re-register your client with a new server,
simply run the following on your client once the server is ready.

.. code-block:: bash

  # rm -rf `puppet config print ssldir`
  # puppet agent -t

After running the puppet agent, sign off the new certificate request on the
Puppet master.

.. code-block:: bash

  # puppet cert list
  # puppet cert sign <cert req name>

Puppet Master Certificate Issues
--------------------------------

To fix the issue where the Puppet Server certificate was removed using 
``puppet cert clean``, run ``puppet cert generate <your puppetservers cert name>``
and restart the puppetserver service.


If the ``/etc/puppetlabs/puppet/ssl`` directory was removed on the Puppet master
(and you do not have a backup of it) or for some other reason you need
to regenerate all the Puppet certificates and the Puppet CA do the following:

.. WARNING::

  This is destructive to your Puppet communications. This should only be used
  if you have no other options.

1. Stop the ``puppetserver`` and ``puppetdb`` services.

2. Remove the certificates and the CA on the Puppet master and generate the new
   Puppet master and CA certificates.

.. code-block:: bash

  # rm -rf /etc/puppetlabs/puppet/ssl
  # puppet cert generate <your puppetserver cert name>

3. Clean the old certificates out from the puppetdb directory and copy the new ones
   from the puppetserver using puppetdb's ssl setup script.

.. code-block:: bash

  # rm -rf /etc/puppetlabs/puppetdb/ssl/*
  # puppetdb ssl-setup

4. Restart the ``puppetserver`` and ``puppetdb`` services

5. Remove the old certificates from each of the Puppet clients and re-register
   the client using the :ref:`rereg-puppet-client-certs` instructions.

Puppetserver and PuppetDB certificate mismatch
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the puppetserver CA has been updated and the puppetdb
certificates are not cleaned a puppet agent run produces an
error similar to this:

.. code-block:: bash

  Error: Could not retrieve catalog from remote server: Error 500 on SERVER: Server Error:
  Failed to execute '/pdb/cmd/v1?checksum=5584595ca917e6b8d5767f7ff0fd71863fdfc486&version=5
  &certname=puppet.your.domain&command=replace_facts&producer-timestamp=1521137360'
  on at least 1 of the following 'server_urls': https://puppet.your.domain:8139

You will probably need to disconnect the :term:`PuppetDB` process from the
:term:`Puppet Server`. Follow the :ref:`ht-disconnect-puppetdb` Guide to
remediate this issue.

Puppet Certificate Issues
=========================

Puppet Client Certificate Issues
--------------------------------

Most of the time, clients will have certificate issues due to the system clock
not being properly set. Before taking any other measures, make sure that your
system clock is correct on both the master and the clients!

If you need to fix client certificate issues outside of time, first make sure that you don't have a certificate already in place on your Puppet server.

.. code-block:: bash

  $ puppet cert list --all

If you **do** have a certificate in place, and need to register a client with the same name, remove that client's certificate from the system.

.. code-block:: bash

  $ puppet cert clean <fqdn.of.the.client>

.. WARNING::

  If you delete the Puppet server's certificate, you will need to re-deploy
  Puppet certificates to **all** of your nodes!

.. WARNING::

  **NEVER RUN ``puppet cert clean --all``**

Puppet Client Re-Registration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If, for some reason, you need to re-register your client with a new server,
simply run the following on your client once the server is ready.

.. code-block:: bash

  $ rm -rf `puppet config print ssldir`
  $ puppet agent -t

Puppet Server Certificate Issues
--------------------------------

.. WARNING::

  This is destructive to your Puppet communications. This should only be used
  if you have no other options.

If the Puppet server has certificate issues, regenerate the server CAs. To do
this, remove the contents of the *ssl* folder and regenerate those ``.pem``
files.

The following table lists the steps to regenerate the server CAs:

.. code-block:: bash

  $ service puppetserver stop
  $ rm -rf /etc/puppetlabs/puppet/ssl
  $ puppet cert list --all
  $ puppet cert --generate ***<fqdn>***
  $ service puppetserver start
  $ puppet agent --test

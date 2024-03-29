.. _howto-simp-enc:

HOWTO Set Up SIMP's External Node Classifier
============================================

An :term:`External Node Classifier`, ENC, can be used to determine what
Puppet environment is used by a node.

SIMP provides a simple, YAML-based ENC, :file:`/usr/local/bin/set_environment`,
available from the ``simp-utils`` RPM.  That package will already be installed
on your system, if you installed the SIMP server from a SIMP ISO or from RPM.

To use this script for your ENC, do the following as ``root``:

#. Ensure the script can be executed by the :program:`puppetserver`.

   .. code-block:: sh

      chmod g+rX /usr/local/bin/set_environment
      chgrp puppet /usr/local/bin/set_environment

#. Configure Puppet to use this script as an ENC

   Set the following in the :code:`[server]` section of :file:`/etc/puppetlabs/puppet/puppet.conf`:

   a. Add or change the line :code:`node_terminus` to :code:`exec`.
   b. Set the :code:`external_nodes` entry to :file:`/usr/local/bin/set_environment`.

   The resulting lines should look something like this:

   .. code-block:: ini

      ...
      [server]
      ...
      node_terminus = exec
      external_nodes = /usr/local/bin/set_environment
      ...

#. Add a file :file:`/etc/puppetlabs/puppet/environments.yaml` to your system.

   This file defines the mapping of nodes to environments.

   * Each rule in this file should have the form

       :code:`<regular expression or FQDN>: 'environment name'`.

   * The rules are processed from top down, and the first match wins.

   For example,

   .. code-block:: yaml

      # The puppet server will use the production environment
      'puppet.my.domain':            'production'

      # Any node in my.domain whose FQDN begins with test will use the test environment
      '/^test([0-9])+\.my\.domain/': 'test'

      # Default to the production environment in all other cases
      '/^.*$/':                      'production'

   To verify the file is properly formatted and yields the classification desired
   run the script manually with a node's FQDN.  It should return

     :code:`environment: <node's Puppet environment name>`

   Using the above :file:`environments.yaml` file, the following command should return

     :code:`environment: test`

   .. code-block:: sh

      /usr/local/bin/set_environment test11.my.domain

#. Ensure the :program:`puppetserver` can access the ENC's configuration file

   .. code-block:: sh

      chmod g+rX /etc/puppetlabs/puppet/environments.yaml
      chgrp puppet /etc/puppetlabs/puppet/environments.yaml

#. Restart the :program:`puppetserver` service

   .. code-block:: sh

      # On EL7
      systemctl restart puppetserver

For more information on ENCs, please see `Puppet's ENC documentation`_.

.. _Puppet's ENC documentation: https://puppet.com/docs/puppet/latest/nodes_external.html

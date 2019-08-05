.. _howto-simp-enc:

Set Up SIMP's External Node Classifier
=======================================

An :term:`External Node Classifier`, ENC, can be used to determine what
Puppet environment is used by a node.

.. TIP::

   For an ENC overview, please see `Puppet's ENC documentation`_.

SIMP provides a simple, YAML-based ENC, ``/usr/local/bin/set_environment``,
available from the ``simp-utils`` RPM.  That package will already be installed
on your system, if you installed the SIMP server from a SIMP ISO or from RPM.

To use this script for your ENC, do the following as ``root``:

#. Ensure the script can be executed by the ``puppetserver``.

   .. code-block:: sh

      chmod g+rX /usr/local/bin/set_environment
      chgrp puppet /usr/local/bin/set_environment

#. Configure Puppet to use this script as an ENC

   Set the following in the ``[master]`` section of
   ``/etc/puppetlabs/puppet/puppet.conf``:

   a. Add or change the line ``node_terminus`` to ``exec``.
   b. Set the ``external_nodes`` entry to ``/usr/local/bin/set_environment``.

   The resulting lines should look something like this:

   .. code-block:: ini

      ...
      [master]
      ...
      node_terminus = exec
      external_nodes = /usr/local/bin/set_environment
      ...

#. Add a file ``/etc/puppetlabs/puppet/environments.yaml`` to your system.

   This file defines the mapping of nodes to environments.

   * Each rule in this file should have the form

       ``<regular expression or FQDN>: 'environment name'``.

   * The rules are processed from top down, and the first match wins.

   For example,

   .. code-block:: yaml

      # The puppetmaster will use the production environment
      'puppet.my.domain':            'production'

      # Any node in my.domain whose FQDN begins with test will use the test environment
      '/^test([0-9])+\.my\.domain/': 'test'

      # Default to the production environment in all other cases
      '/^.*$/':                      'production'

   To verify the file is properly formatted and yields the classification desired
   run the script manually with a node's FQDN.  It should return

     ``environment: <node's Puppet environment name>``

   Using the above ``environments.yaml`` file, the following command should return

     ``environment: test``

   .. code-block:: sh

      /usr/local/bin/set_environment test11.my.domain


#. Ensure the ``puppetserver`` can access the ENC's configuration file

   .. code-block:: sh

      chmod g+rX /etc/puppetlabs/puppet/environments.yaml
      chgrp puppet /etc/puppetlabs/puppet/environments.yaml

#. Restart the ``puppetserver`` service

   .. code-block:: sh

      # On EL7
      systemctl restart puppetserver


.. _Puppet's ENC documentation: https://puppet.com/docs/puppet/latest/nodes_external.html

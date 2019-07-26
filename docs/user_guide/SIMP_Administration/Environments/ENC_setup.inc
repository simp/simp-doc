.. _ug-deploy-env-enc:

External Node Classifier
++++++++++++++++++++++++

An :term:External Node Classifier ,ENC , can be used to determine what
environment is used by a node.

SIMP provides a simple ENC in the ``simp-utils`` RPM.  It is installed
as ``/usr/local/bin/set_environment``.

To use this script for your :term:ENC do the following:

#. Ensure the script can be executed by the Puppet server.

   .. code-block:: sh

      chmod g+rX /usr/local/bin/set_environment
      chgrp puppet /usr/local/bin/set_environment

#. Configure Puppet to use this script as an :term:ENC

   a. Add, or change, the line 'node_terminus' to 'exec' in the
      [master] section of ``/etc/puppetlabs/puppet/puppet.conf``.
   b. Set the 'external_nodes' entry in the [master] section of
      puppet.conf to point to this script.

   The resulting lines should look something like this:

   .. code-block:: yaml

      ...
      [master]
      ...
      node_terminus = exec
      external_nodes = /usr/local/bin/set_environment
      ...

#. Add the file ``/etc/puppetlabs/puppet/environments.yaml`` to
   your system.

   Add a directive per line for assiging nodes in the form of
   <regular expression or fqdn>: 'environment name'

   .. code-block:: yaml

      'puppet.my.domain':      'production'
      '/^test([0-9])+\.my\.domain/':   'test'
      # Fall back to production in all cases
      '/^.*$/':                           'production'

   To verify the file is properly formatted
   and yields the classification desired run the script
   manually with a hostname.  For example,

   .. code-block:: sh

      #The following should return
      #   environment: test
      #
      /usr/local/bin/set_environment test11.my.domain

   should return ``environment: test``

#. Restart the Puppet Server process

There is more information about the ``/usr/local/bin/set_environment``
in the script.   The puppet documentation has further information on
`External Node Classifiers`_


.. _External Node Classifiers: https://puppet.com/docs/puppet/latest/nodes_external.html

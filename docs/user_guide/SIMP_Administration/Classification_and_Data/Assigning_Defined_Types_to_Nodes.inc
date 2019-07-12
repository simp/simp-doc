Assigning Defined Types to Nodes
--------------------------------

Defined types do not have the ability to receive parameters via Hiera in the
traditional sense. To include a defined type on a node, one could use
``create_resources``, but this is messy and discouraged. Instead, create your
own profile or a ``site`` module with site-specific manifests.  For example,
``/etc/puppetlabs/code/environments/production/modules/site/manifests/my_site.pp``.

.. NOTE::

   You can find a working example of this in the :ref:`PXE_Boot` section
   of the documentation

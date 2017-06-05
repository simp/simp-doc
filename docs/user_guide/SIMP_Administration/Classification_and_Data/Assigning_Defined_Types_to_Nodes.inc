Assigning Defined Types to Nodes
--------------------------------

Defined types do not have the ability to receive parameters via Hiera in the
traditional sense. To include a defined type on a node, one could use
``create_resources``, but this is messy and discouraged. Instead, create your
own profile or add a class to the SIMP ``site`` module such as:
``/etc/puppetlabs/code/environments/simp/modules/site/manifests/my_site.pp``.

.. NOTE::

   You can find a working example of this in the :ref:`PXE_Boot` section
   of the documentation

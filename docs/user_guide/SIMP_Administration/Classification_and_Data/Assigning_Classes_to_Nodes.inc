Assigning Classes to Nodes
--------------------------

Assigning classes to nodes can be done in a few ways in SIMP. First, there is a
``lookup`` function in ``/etc/puppetlabs/code/environments/simp/manifests/site.pp``
that looks for an array called ``classes`` in your hierarchy. It also looks for
an array called ``class_exclusions``, which can be used to remove classes from
the classes array. The classes that are included are the result of
``$classes - $class_exclusions``. If classes need to be added to all nodes, a
``classes`` array could be added to the ``default.yaml`` in your hieradata,
like this:

.. code-block:: yaml

   ---
   classes:
     - 'site::example_class'


A similar array could be created in any other layer in the hierarchy, and it
will be merged with the 'unique' strategy by the ``lookup`` function noted
above.

The SIMP profile module also includes other classes needed for a secure
baseline, which are discussed below in the :ref:`SIMP scenarios <simp scenarios>`
section.

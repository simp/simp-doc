Assigning Classes to Nodes
--------------------------

The SIMP profile module includes other classes needed for a secure
baseline, which are discussed below in the :ref:`SIMP scenarios <simp scenarios>`
section. The lists of included classed are controlled by the ``simp::classes``
and ``simp::server::classes`` class parameters:

* The ``simp::classes`` parameter is available for all nodes.
* The ``simp::server::classes`` parameter is available only for the SIMP server
  (Puppet master).

To preserve various levels of ordering and overrides, it is highly recommended
that you use the ``simp::classes`` and ``simp::server::classes`` class parameters
in :term:`Hiera` to manage standard class inclusions.

This allows you to use the ``--`` knockout prefix to exclude classes at any level
of your hierarchy, which, in turn, allows you to use your own modules to manage
specific applications that SIMP manages by default.

.. code-block:: yaml
   :caption: Example: Adding a Class to Standard List

   ---
   simp::classes:
     - gnome

.. code-block:: yaml
   :caption: Example: Removing a Class from Client Nodes

   ---
   simp::classes:
     - '--ntpd'

.. code-block:: yaml
   :caption: Example: Removing a Class from Client Nodes and the SIMP Server

   ---
   simp::classes:
     - '--ntpd'

   simp::server::classes:
     - '--ntpd'


Legacy Method
^^^^^^^^^^^^^

SIMP also includes class inclusion functionality via a top-level ``classes``
array in your Hiera hierarchy. Though this is no longer recommended, it is
still supported.

In this case, instead of using the knockout prefix, there is also a
``class_exclusions`` array that will be used to remove classes from the
include list.  Note, however, this array cannot be applied to any classes
included via ``simp::classes`` or ``simp::server::classes``.

Assigning Classes to Nodes
--------------------------

The SIMP profile module includes other classes needed for a secure
baseline, which are discussed below in the :ref:`SIMP scenarios <simp scenarios>`
section.

To preserve various levels of ordering and overrides, it is highly recommended
that you use the ``simp::classes`` and ``simp::server::classes`` class parameters
in :term:`Hiera` to manage standard class inclusions.

This allows you to correctly use the ``--`` knockout prefix to exclude classes
at any level of your hierarchy.

.. code-block:: yaml
   :caption: Example Class Management

   ---
   # Client node class adjustments to the SIMP scenario-specific default list
   simp::classes:
     # Add the 'gnome' class to the standard list
     - gnome
     # Remove the 'ntp' class because we want to use our own
     - '--ntp'

   # SIMP server class adjustments to its default list
   simp::server::classes:
     # Remove the 'ntp' class because we want to use our own
     - '--ntp'

Legacy Method
^^^^^^^^^^^^^

SIMP also includes class inclusion functionality via a top-level ``classes``
array in your Hiera hierarchy. Though this is no longer recommended, it is
still supported.

In this case, instead of using the knockout prefix, there is also a
``class_exclusions`` array that will be used to remove classes from the
include list.  Note, however, this array cannot be applied to any classes
included via ``simp::classes`` or ``simp::server::classes``.

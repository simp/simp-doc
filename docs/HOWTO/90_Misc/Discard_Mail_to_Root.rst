HOWTO Discard Mail to Root
==========================

In many environments, you may have a central log collection facility
for analyzing your log data. In this case, you may want to disable
the default behavior of sending all e-mail to root.

The simplest method of discarding root's e-mail is to redirect it to
``/dev/null`` on the system using the following Puppet code.

.. WARNING::
   This is a **very** brute force approach and should only be used if you are
   **absolutely sure** that you want to discard all of root's e-mail on your
   systems.

.. code-block:: puppet

   postfix::alias { 'root':
     values => '/dev/null'
   }

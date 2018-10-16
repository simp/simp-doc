.. _faq-root-login:

How can the root user login
===========================

Keeping in line with general best practice, SIMP does not allow ``root`` to
login to the system remotely or at local terminals by default.

However, there may be cases where you need to login as ``root`` for perfectly
valid reasons.

Enabling Terminal Logins
------------------------

To allow ``root`` to login at the terminal, you will need to set the
``useradd::securetty`` ``Array`` to include all ``tty`` devices from which you
wish to allow ``root`` access.

For example, to allow the ``root`` user to login at the first three virtual
consoles and the first serial device, you would place the following in
:term:`hiera`:

.. code-block:: yaml

   useradd::securetty:
     - tty0
     - tty1
     - tty2
     - ttyS0


.. IMPORTANT::

   If you are working on a system that was not installed from an ISO, you
   should do this before running ``simp bootstrap``.  Otherwise, unless you have
   set up other users, you may not be able to log into your system.


Enabling Remote SSH Logins
--------------------------

If you need to allow remote ``root`` logins over SSH (we **highly** advise
against this), you can add the following to :term:`hiera`:

.. code-block:: yaml

   ssh::server::conf::permitrootlogin: true

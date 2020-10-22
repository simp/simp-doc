.. _faq-root-login:

Enabling ``root`` Logins
========================

Keeping in line with general best practice, SIMP does not allow ``root`` to
login to the system remotely or at local terminals by default.

However, there may be cases where you need to login as ``root`` for perfectly
valid reasons.

Enabling Terminal Logins
------------------------

To allow ``root`` to login at the terminal, you will need to set the
:code:`useradd::securetty` ``Array`` to include all ``tty`` devices from which you
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


Enabling Remote SSH Logins
--------------------------

If you need to allow remote ``root`` logins over SSH (we **highly** advise against this), you can
add the following to :term:`hiera`:

.. code-block:: yaml

   ssh::server::conf::permitrootlogin: true

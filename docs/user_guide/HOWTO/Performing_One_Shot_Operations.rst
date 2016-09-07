HOWTO Perform One-Shot Operations
=================================

This section introduces the options provided for performing one-shot
commands on all Puppet-managed systems without using Puppet. This is
useful when the user needs to perform an action one time in every
location, but does not want to enforce that action over time.

Use the PSSH Utility
--------------------

:term:`Parallel Secure Shell` (PSSH) has been included in SIMP for some time, but has not been installed by
default.

The table below lists the steps to use PSSH.


Table: Use PSSH Procedure

.. note::

    There is no manual page provided with PSSH; type ``pssh --help`` for
    further explanation.

Other SSH Options
~~~~~~~~~~~~~~~~~

Using the ``-f`` option forces :term:`TTY` for SSH, which allows the user to run
sudo commands via PSSH.

Using the *-OStrictHostKeyChecking=no* option connects the user to the
target servers via SSH even if there is an issue with ``~/.ssh/known_hosts``.

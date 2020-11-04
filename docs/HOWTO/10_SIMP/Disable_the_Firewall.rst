.. _howto-disable-the-firewall:

Fully Disabling the System Firewall
===================================

Though we hope that you never actually want to do this, there may be situations where you want to
use puppet to fully disable the system firewall.

When :program:`iptables` was the only option, this was very straightforward. The introduction of
:program:`firewalld` has added a bit of complexity due to the preservation of backwards
compatibility with calls into the :code:`iptables::rules::*` :term:`defined types`.

To fully disable **all** firewalls on the system (not just management of the firewalls) set the
following via :term:`Hiera`:

.. code-block:: yaml

   iptables::enable: false
   firewalld::service_enable: false

As per usual, once this is set, Puppet will ensure that the firewall is fully disabled until the
settings are reversed.

.. IMPORTANT::

   Just setting :code:`firewalld::service_enable: false` will likely cause your system to fall back
   to using :program:`iptables`.

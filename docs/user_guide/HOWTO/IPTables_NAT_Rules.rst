HOWTO Configure iptables NAT Rules
==================================

See the documentation in the ``iptables`` module itself for general usage.

Add NAT Rules
-------------

The user may be required to add :term:`Network Address Translation` (NAT) rules
to the iptables ruleset. To achieve this using the iptables module, the
``iptables::rule`` input statement should be used.

The example below shows an iptables NAT rule.

Example of an iptables NAT Rule

.. code-block:: puppet

   iptables::rule { 'nat_global':
     table    => 'nat',
     first    => true,
     absolute => true,
     header   => false,
     content  => '
     :PREROUTING ACCEPT [0:0]
     :POSTROUTING ACCEPT [0:0]
     :OUTPUT ACCEPT [0:0]
     '
   }

   iptables::rule { 'nat_test':
     table   => 'nat',
     header  => false,
     content => '-A PREROUTING --physdev-in eth1 -j DROP'
   }

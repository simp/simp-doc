HOWTO Configure IPTables NAT Rules
==================================

See the `IPtables Module
Reference <../developers_guide/rdoc/classes/iptables.html>`__ for notes
on using the basic IPtables Module.

Add NAT Rules
-------------

The user may be required to add :term:`Network Address Translation` (NAT) rules to the IPtables ruleset. To
achieve this using the IPtables module, SIMP 1.1.3 or later is required
and the ``iptables::add_rules`` input statement should be used to affect
the appropriate changes.

The example below shows an IPtable NAT rule.

Example of an IPtable NAT Rule

.. code-block:: ruby

  iptables::add_rules { "nat_global":
     table => "nat",
     first => "true",
     absolute => "true",
     header => "false",
     content => "
     :PREROUTING ACCEPT [0:0]
     :POSTROUTING ACCEPT [0:0]
     :OUTPUT ACCEPT [0:0]
     "
   }

 iptables::add_rules { "nat_test":
     table   => "nat",
     header  => "false",
     content => "
     -A PREROUTING --physdev-in
     eth1 -j DROP
     "
   }

Denial of Service Protection
-----------------------------

SIMP takes several measures to reduce the chances of Denial of Service (DoS)
attacks. The primary measures in place are to limit traffic with IPTables
and set several kernel parameters.  The kernel parameters set
include limiting ICMP redirects, logging martian packets, ignoring ICMP
broadcast traffic, ignoring bogus ICMP errors, and enabling protection against
SYN cookies.

References: :ref:`SC-5`

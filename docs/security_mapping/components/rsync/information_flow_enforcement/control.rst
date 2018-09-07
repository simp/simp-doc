Information Flow Enforcement
----------------------------

The rsync server port (over stunnel) is open to the IP addresses defined by the
value of ``$simp_options::trusted_nets``, which for most installs is the local network.

References: :ref:`AC-4`

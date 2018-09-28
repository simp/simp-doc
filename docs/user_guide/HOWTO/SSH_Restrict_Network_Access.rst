HOWTO Restrict Network Access to SSH
====================================

Like most SIMP modules, the SSH module utilizes a ``trusted_nets`` parameter to
control access to the SSH service via both IPTables and TCPWrappers.

Since there is no way for the SIMP installation to successfully guess where you
may be connecting from, or know about your particular network architecture, it
defaults to allowing SSH connections from **any** host.

It is understandable that you may want to restrict this further. To do so, you
simply need to set the ``ssh::server::conf::trusted_nets`` to an ``Array`` of
networks or hosts from which you would like to connect.

**Example**: Set Trusted Nets to Alternate Networks via Hiera

.. code:: yaml

   ---
   ssh::server::conf::trusted_nets :
     - 1.2.3.4
     - 10.1.2.0/24
     - 192.168.0.0/16

You can find more information on ``trusted_nets`` in the
:ref:`List of Installation Variables` in the :ref:`gsg-advanced-configuration`
section of the :ref:`gsg_index`.

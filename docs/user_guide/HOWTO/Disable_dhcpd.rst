HOWTO Disable DHCP
==================

If you have an existing infrastructure and do not want to use SIMP as a DHCP
server, here is how to disable it:

In the host yaml file for your SIMP server, set the following variable in
Hiera:

.. code-block:: yaml

  simp::kickstart_server::manage_dhcp: false


Alternatively, if you don't want SIMP to be a kickstart server, either, just
remove ``simp::kickstart_server`` from your SIMP host's yaml file.

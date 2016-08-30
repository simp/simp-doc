HOWTO Disable Named
====================

If you already have an authoritative DNS server on your network, make sure your
DNS server list in ``hieradata/simp_def.yaml``  doesn't include your machine's
local IP address. SIMP should automatically turn off the ``named`` service.

However, if you are running an alternative DNS provider on SIMP and need to
keep your IP address in ``resolve.conf``, set
``simplib::resolv::named_autoconf: false`` in Hiera.

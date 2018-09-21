HOWTO Configure a Puppet Master behind a NAT
============================================

.. ATTENTION::

  This page was written for Puppet 3 and SIMP versions less than 6.

This section provides guidance for when the Puppet master is behind a
NAT but is managing hosts outside the NAT.

Your puppet server certificate must have all names in it that are used by
any client.  To update your certificates follow the guidance:

1) Add the alternative certificate names (in a comma-separated list) in /etc/puppetlabs/puppet/puppet.conf

  .. code-block:: ini

    [main]

    dns_alt_names = hostname.your.domain,hostname.your.other.domain


2) Regenerate ALL certificates on Puppet:

   https://docs.puppet.com/puppet/3.8/ssl_regenerate_certificates.html

   In Section 2 of the web page above that says update your Puppetdb
   certificates follow the instructions in Step 3, option A at this
   location:

   https://docs.puppet.com/puppetdb/2.3/install_from_source.html#step-3-option-a-run-the-ssl-configuration-script


HOWTO Configure a Puppet Server Behind a NAT
============================================

This section provides guidance for when the Puppet server is behind a
NAT but is managing hosts outside the NAT.

Your puppet server certificate must have all names in it that are used by
any client.  To update your certificates follow the guidance:

1) Add the alternative certificate names (in a comma-seperated list) in /etc/puppet/puppet.conf

    [main]

    dns_alt_names = hostname.your.domain,hostname.your.other.domain


2) Regenerate ALL certificates on Puppet:

   http://docs.puppetlabs.com/puppet/3.8/reference/ssl_regenerate_certificates.html

   In Section 2 of the web page above that says update your Puppetdb
   certificates follow the instructions in Step 3, option A at this
   location:

   http://docs.puppetlabs.com/puppetdb/2.3/install_from_source.html#step-3-option-a-run-the-ssl-configuration-script


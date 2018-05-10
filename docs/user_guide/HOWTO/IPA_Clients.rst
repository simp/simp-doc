.. _howto-ipa_clients:

HOWTO Enroll Hosts into an IPA Domain
=====================================

Host should be able to join an IPA domain with a few catches. SIMP already uses
the login stack that IPA uses (PAM, SSSD), but it also optionally manages the
same resources that IPA provides automation for, like:

=========== =============================== ===============
Technology  Related SIMP features           Related tickets
=========== =============================== ===============
``sudoers`` simp-simp and simp-sudo modules `SIMP-4898`_
``autofs``  optional simp-simp_nfs module   `SIMP-4168`_
``krb5``    optional simp-krb5 module       `SIMP-4167`_
=========== =============================== ===============

These features may work in the future, but logins via SSSD or LDAP should work
without issue, now.


Adding clients
--------------

Adding clients requires two steps:

#. Add the hosts on the IPA server, setting a one time password
#. Running ``ipa-client-install`` on the host, using the password generated in
   the previous step

   .. NOTE::

   	There may be issues running ``ipa-client-install`` on EL6 with FIPS mode
      enabled.


Add hosts to IPA
^^^^^^^^^^^^^^^^

There are two ways to complete this step:

#. Use the IPA web interface, and write down the one time password
#. Run ``ipa host-add`` on the command line and pregenerate the password

Only option 2 will be covered here.

To be able to add hosts from the command line:

#. Log onto a machine that already has joined an IPA domain
#. ``kinit`` into an account with the appropriate privileges
#. Use the script below as an example to generate host accounts in bulk

   .. code-block:: ruby

      #!/opt/puppetlabs/puppet/bin/ruby
      require 'securerandom'

      password_suffix = SecureRandom.urlsafe_base64(8)
      puts 'Looking for a file called `hosts` with lines that look like "host1.domain.local,192.168.1.3"'
      puts 'Using passwords in the form of <hostname>-' + password_suffix

      File.readlines('hosts').each do|h|
        host, ip = h.split(',')
        cmd = "ipa host-add #{host} --ip-address=#{ip} --password=#{host}-#{password_suffix}"
        puts cmd
        exit unless system(cmd)
      end

#. Set the password, add IPA server settings, and include the classes in hiera:

   .. code-block:: yaml

      # In <domain>.yaml or whichever level is most specific
      ---
      classes:
      - simp::ipa::install
      - simp::ipa::client

      simp::ipa::install::ensure: present
      simp::ipa::install::password: "%{trusted.certname}-<password from script above>"
      simp::ipa::install::server: ipa.example.domain
      simp::ipa::install::domain: example.domain
      simp::ipa::install::realm: EXAMPLE.DOMAIN


   Some optional settings that may be needed, depending on the configuration of the
   IPA server and the environment:

   .. code-block:: yaml

      ---
      # If the IPA server is a DNS server, this will allow you to use the DNS
      # SRV records to discover other IPA provided services, like LDAP and krb5
      simp_options::dns::servers:
      - <IP address of IPA server>

      # IPA's default uid's are in the millions while SIMP's max is much lower
      simp_options::uid::max: 0

#. Next time Puppet runs via cron job, your node will be part of the IPA domain.


.. _SIMP-4898: https://simp-project.atlassian.net/browse/SIMP-4898
.. _SIMP-4168: https://simp-project.atlassian.net/browse/SIMP-4168
.. _SIMP-4167: https://simp-project.atlassian.net/browse/SIMP-4167

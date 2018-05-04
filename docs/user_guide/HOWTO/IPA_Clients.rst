.. _howto-ipa_clients:

HOWTO Enroll Hosts into an IPA Domain
=====================================

Host should be able to join an IPA domain with a few catches. SIMP already uses
the login stack that IPA uses, but it also actively manages the same resources
that IPA provides automation for, like:

#. autofs
#. sudoers
#. SELinux
#. krb5

These features may work in the future, but logins via SSSD or LDAP should work
without issue.


Adding clients
--------------

Adding clients requires two steps:

#. Add the hosts on the IPA server, setting a one time password
#. Running ``ipa-client-install`` on the host, using the password generated in
   the previous step


Add hosts to IPA
^^^^^^^^^^^^^^^^

There are two ways to complete this step:

#. Use the IPA web interface, and write down the one time password
#. Run ``ipa host-add`` on the command line and pregenerate the password

Only option 2 will be covered here.

To be able to add hosts from the command line:

#. Log onto a machine that already has join an IPA domain
#. ``kinit`` into an account with the appropriate priviliges
#. Use the script below as an example to generate host accounts in bulk

   .. code-block:: ruby

      #!/opt/puppetlabs/puppet/bin/ruby
      require 'securerandom'

      password_suffix = SecureRandom.urlsafe_base64(8)
      puts 'Looking for a file called `hosts` with lines that look like "test1.domain.local,192.168.1.3"'
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

      ###### Optional settings
      # If the IPA server is a DNS server, this will allow you to use the DNS
      # SRV records to discover other IPA provided services, like LDAP and krb5
      simp_options::dns::servers:
      - <IP address of IPA server>

      # IPA's default uid's are in the millions while SIMP's max is much lower
      simp_options::uid::max: 0

#. Run puppet on each node, or wait for it to run via cron job.

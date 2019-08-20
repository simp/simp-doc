.. _howto-ipa_clients:

HOWTO Enroll Hosts into an IPA Domain
=====================================

Hosts should be able to join an IPA domain with a few catches. SIMP already uses
the login stack that IPA uses (PAM, SSSD), but it also optionally manages the
same resources that IPA provides automation for. This includes:

=========== =============================== ===============
Technology  Related SIMP features           Related tickets
=========== =============================== ===============
``sudoers`` simp-simp and simp-sudo modules `SIMP-4898`_
``autofs``  optional simp-simp_nfs module   `SIMP-4168`_
``krb5``    optional simp-krb5 module       `SIMP-4167`_
=========== =============================== ===============

The features in the above table may work in the future, but logins via SSSD or
LDAP should work without issue, now.

IPA should work in both the ``simp`` and ``simp_lite``
:ref:`scenarios<simp scenarios>`. There may be issues with logins if the
``sssd`` module is not included.


Adding IPA Clients
------------------

Adding IPA clients requires two steps:

#. Add the hosts on the IPA server, setting a one time password.
#. Join each host to the IPA domain by running ``ipa-client-install`` on the
   host with the password generated in the previous step.

The second step is automated with the ``simp-simp_ipa`` module.

   .. NOTE::

      Using ``ipa-client-install`` on EL6 with FIPS mode is not currently
      supported and will result in the following error message:
      ``Cannot install IPA client in FIPS mode``


Add Hosts to IPA
^^^^^^^^^^^^^^^^

There are two ways to complete this step:

#. Use the IPA web interface, and take note of the one time password.
#. Run ``ipa host-add`` on the command line and pre-generate the password.

Only option 2 will be covered here.

To add hosts from the command line:

#. Log onto a machine that already has joined an IPA domain.
#. ``kinit`` into an account with the appropriate privileges, (e.g., ``kinit admin``).
#. Use a script such as the example below, to add hosts in bulk:

   .. code-block:: ruby

      #!/opt/puppetlabs/puppet/bin/ruby

      # This scripts adds hosts to IPA using fqdn/IP address pairs listed in a
      # 'hosts' input file and generated passwords of the form
      #
      #   <fqdn>-<random string>
      #
      # The <random string> portion will be the same for all host passwords
      # in a specific run.
      #
      # The input file must contain lines formatted as follows:
      #
      #   <fqdn>,<IP address>
      #
      # such as
      #
      #   ws1.example.domain,192.168.1.3
      #
      require 'securerandom'

      unless File.exist?('hosts')
        $stderr.puts "ERROR: Could not find 'hosts' file."
        exit 1
      end

      password_suffix = SecureRandom.urlsafe_base64(8)
      puts 'Using one-time passwords of the form of <fqdn>-' + password_suffix

      File.readlines('hosts').each do |h|
        # skip comment lines and blank lines
        next if (h[0] == '#') || (h.strip.empty?)

        unless h.include?(',')
          $stderr.puts "WARN: Skipping malformed entry: '#{h.strip}'"
          next
        end

        fqdn, ip = h.split(',')
        fqdn.strip!
        ip.strip!
        unless !fqdn.empty? && ip && !ip.empty?
          $stderr.puts "WARN: Skipping malformed entry: '#{h.strip}'"
          next
        end

        cmd = "ipa host-add #{fqdn} --ip-address=#{ip} --password=#{fqdn}-#{password_suffix}"
        puts cmd
        unless system(cmd)
          $stderr.puts "ERROR:  Command failed '#{cmd}'"
          $stderr.puts 'Exiting!'
          exit 2
        end
      end


Join a Host to the IPA Domain
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To join the host to the IPA domain, use ``simp_ipa::client::install`` from the
``simp-simp_ipa`` Puppet module, by setting the hieradata as shown in the
examples below.

The following examples assume

* the IPA server is ``ipa.example.domain`` with an IP address of ``192.153.1.2``
* the IPA domain is ``example.domain``
* the IPA realm is ``EXAMPLE.DOMAIN``.

.. code-block:: yaml

    # In the appropriate level hieradata file
    ---
    simp::classes:
      # this will include this class in client node manifests
      - simp_ipa::client::install

    simp_ipa::client::install::ensure: present

    # Set this to the one-time password generated when the host was added to IPA.
    # This example assumes you used the example script.
    simp_ipa::client::install::password: "%{trusted.certname}-<OTP suffix>"

    # Set this to the IPA server FQDN
    simp_ipa::client::install::server: ipa.example.domain

    # Set these to match your IPA domain and realm
    simp_ipa::client::install::domain: example.domain
    simp_ipa::client::install::realm: EXAMPLE.DOMAIN


In addition to the above settings, other settings may be needed, depending on the
configuration of the IPA server and the environment:

.. code-block:: yaml

   ---
   # IPA uses both of these technologies, so they need to be enabled.
   # SSSD is already enabled in the 'simp' and 'simp_lite' scenarios.
   simp_options::sssd: true
   simp_options::ldap: true

   # These 4 parameters have to be set, even though they may be unused because
   # IPA does not, natively, set up a BIND DN or a SYNC DN.  If your IPA server
   # has those DNs and you are using a SIMP module that uses them (e.g.,
   # simp-simp_gitlab), be sure to set them to the real values.  It is likely
   # you will also have to set the commented out parameters as well!
   simp_options::ldap::bind_pw: "A-Unused-LDAP-Bind-Password"
   simp_options::ldap::bind_hash: "{SSHA}this-is-not-a-real-password-hash"
   simp_options::ldap::sync_pw: "A-Unused-LDAP-Sync-Password"
   simp_options::ldap::sync_hash: "{SSHA}this-is-not-a-real-password-hash"
   #simp_options::ldap::base_dn: FILL-ME-IN-AS-NEEDED
   #simp_options::ldap::bind_dn: FILL-ME-IN-AS-NEEDED
   #simp_options::ldap::sync_dn: FILL-ME-IN-AS-NEEDED
   #simp_options::ldap::root_dn: FILL-ME-IN-AS-NEEDED
   #simp_options::ldap::master:  FILL-ME-IN-AS-NEEDED
   #simp_options::ldap::uri:     [ FILL-ME-IN-AS-NEEDED ]

   # If the IPA server is a DNS server, this will allow you to use the DNS
   # SRV records to discover other IPA provided services, like LDAP and krb5.
   simp_options::dns::servers:
     # IP address of IPA server
     - 192.153.1.2

   # Other DNS-related settings that may fix issues that pop up.
   simp_options::dns::search:
      # IPA domain
      - example.domain
      resolv::named_autoconf: false
      resolv::caching: false

      # IPA domain
      resolv::resolv_domain: example.domain


Next time Puppet runs, your node will be part of the IPA domain and appropriate
logins should work.

IPA User Accounts
-----------------

Once a host has been joined to the IPA domain following the instructions above,
users should be able to login with SSSD or LDAP.  However, there are a few nuances
about user accounts that are worth noting:

*  Only users that are in an IPA group of type ``POSIX`` will be able to
   log into Linux systems.  You may need to add such a group on the IPA server.
   For example, to add a POSIX group named ``posixusers`` via the command line:

   .. code-block:: bash

      kinit admin
      # by default this will be a POSIX group
      ipa group-add posixusers --desc "A POSIX group for users"

*  The default UID and GID ranges are very high in IPA (generated randomly by
   default and can be in the low billions), so they are a lot higher than both
   the SIMP and SSSD default max. You have a couple of options on how to avoid
   this issue:

   * Set the start user and group number when you install the IPA server by
     using the ``--idstart`` command line option (e.g.,
     ``ipa-server-install --idstart=5000``)
   * Change the UID/GID ranges in the IPA GUI.
   * Set ``simp_options::uid::max`` to match that of your existing IPA server.

*  Users and groups still have to be added to PAM to be able to log in!  You
   will need to allow access using the ``pam::access::rule`` define from the
   ``simp-pam`` Puppet module.  For example, to allow access to the
   ``posixusers`` group created above:

   .. code-block:: puppet

      pam::access:rule { 'Allow IPA posixusers group into the system':
        users   => [ '(posixusers)' ],
        origins => [ $simp_options::trusted_nets ],
        comment => 'group for IPA users'
      }


.. _SIMP-4898: https://simp-project.atlassian.net/browse/SIMP-4898
.. _SIMP-4168: https://simp-project.atlassian.net/browse/SIMP-4168
.. _SIMP-4167: https://simp-project.atlassian.net/browse/SIMP-4167

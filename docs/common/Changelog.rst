
  SIMP-103 #close Fixed some of the typoes
 (SIMP-103) Make first set of corrections on docs. Fixed some typos on original documentation -- not complete Updated spaces and also conditional (which defaults to no tags but can be fixed)
 SIMP-112 #close
 SIMP-112 #comment Introduced env var SIMP_SKIP_NON_SIMPOS_TESTS
 SIMP-113 #close
 (SIMP-113) Skip SimpOS tests for YumRepositories - This commit enables non-SimpOS testing environments such as TravisCI to skip YumRepositories tests that rely on SimpOS-specific OS components.
 SIMP-115 #close
 SIMP-115 #comment resolved executable dependency for cracklib-check
 (SIMP-115) Fix nil Utils.validate_password tests - Before this patch, Travis CI would fail any tests that run the method Simp::Cli::Config::Utils.validate_password, due to nil variables returned missing the executable command cracklib-check. This commit updates the .travis.yml to request a sudo-enabled Travis worker that pre-installs the apt package libcrack2 (which provides cracklib-check).
 SIMP-116 #close Update pupmod-acpid for travis
 (SIMP-116) Update pupmod-acpid for travis. Update fixtures.yml, travis.yml, Gemfile.
 SIMP-117 #close Update activemq for travis
 (SIMP-117) Update pupmod-simp-activemq for travis. Updated fixtures, travis.yml, Gemfile.
 SIMP-118 #close Update pupmod-simp-aide for travis
 (SIMP-118) Update pupmod-simp-aide for travis. Updated fixtures, travis.yml, Gemfile.
 SIMP-119 #close Update pupmod-simp-auditd for travis
 (SIMP-119) Update pupmod-simp-auditd for travis. Updated fixtures, travis.yml. Gemfile.
 SIMP-120 #closes Update pupmod-simp-autofs for travis
 (SIMP-120) Update pupmod-simp-autofs for travis. Updated fixtures, travis.yml, Gemfile
 SIMP-126 #close Update pupmod-simp-clamav for travis
 (SIMP-126) Update pupmod-simp-clamav for travis. Update fixtures, travis.yml, Gemfile.
 SIMP-127 #close Update pupmod-simp-concat for travis
 (SIMP-127) Update pupmod-simp-concat for travis. Update fixtures, travis.yml, Gemfile.
 SIMP-133 #close Update dhcp for travis
 (SIMP-133) Update pupmod-simp-dhcp for travis
 SIMP-135 #close Update freeradius for travis
 (SIMP-135) Update pupmod-simp-freeradius for travis
 SIMP-136 #close Update functions for travis
 (SIMP-136) Update pupmod-simp-functions for travis
 Merge "(SIMP-136) Update pupmod-simp-functions for travis"
 SIMP-139 #close Update iptables for travis
 (SIMP-139) Update pupmod-simp-iptables for travis
 Merge "(SIMP-139) Update pupmod-simp-iptables for travis"
 SIMP-141 #close Update kibana for travis
 (SIMP-141) Update pupmod-simp-kibana for travis
 SIMP-143 #close Update libvirt for travis
 (SIMP-143) Update pupmod-simp-libvirt for travis
 SIMP-144 #close Update logrotate for travis
 (SIMP-144) Update pupmod-simp-logrotate for travis
 SIMP-148 #close Update mozilla for travis
 (SIMP-148) Update pupmod-simp-mozilla for travis
 SIMP-150 #close Update named for travis
 (SIMP-150) Update pupmod-simp-named for travis
 SIMP-151 #close Update network for travis
 (SIMP-151) Update pupmod-simp-network for travis
 SIMP-152 #close Update nfs for travis
 (SIMP-152) Update pupmod-simp-nfs for travis
 SIMP-153 #close Update nscd for travis
 (SIMP-153) Update pupmod-simp-nscd for travis
 SIMP-154 #close Update ntpd for travis
 (SIMP-154) Update pupmod-simp-ntpd for travis
 SIMP-156 #close Update openldap for travis
 (SIMP-156) Update pupmod-simp-openldap for travis
 SIMP-157 #close Update openscap for travis
 (SIMP-157) Update pupmod-simp-openscap for travis
 SIMP-158 #close Update pam for travis
 (SIMP-158) Update pupmod-simp-pam for travis
 SIMP-159 #close Update pki for travis
 (SIMP-159) Update pupmod-simp-pki for travis
 SIMP-160 #close Update polkit for travis
 (SIMP-160) Update pupmod-simp-polkit for travis
 SIMP-161 #close Update postfix for travis
 (SIMP-161) Update pupmod-simp-postfix for travis
 SIMP-162 #close Update pupmod for travis
 (SIMP-162) Update pupmod-simp-pupmod for travis
 SIMP-163 #close Update rsync for travis
 (SIMP-163) Update pupmod-simp-rsync for travis
 SIMP-164 #close Update rsyslog for travis
 (SIMP-164) Update pupmod-simp-rsyslog for travis
 SIMP-165 #close Update selinux for travis
 (SIMP-165) Update pupmod-simp-selinux for travis
 SIMP-167 #close Update pupmod simp for travis
 (SIMP-167) Update pupmod-simp-simp for travis
 SIMP-168 #close Update snmpd for travis
 (SIMP-168) Update pupmod-simp-snmpd for travis
 SIMP-170 #close Update sssd for travis
 (SIMP-170) Update pupmod-simp-sssd for travis
 SIMP-171 #close Update stunnel for travis
 (SIMP-171) Update pupmod-simp-stunnel for travis
 SIMP-172 #close Update sudo for travis
 (SIMP-172) Update pupmod-simp-sudo for travis
 SIMP-173 #close Update sudosh for travis
 (SIMP-173) Update pupmod-simp-sudosh for travis
 SIMP-174 #close Update svckill for travis
 (SIMP-174) Update pupmod-simp-svckill for travis
 SIMP-176 #close Update tcpwrappers for travis
 (SIMP-176) Update pupmod-simp-tcpwrappers for travis
 (SIMP-177) Update pupmod-simp-tftpboot for travis
 SIMP-177 Update tftpboot for travis
 SIMP-178 #close Update tpm for travis
 (SIMP-178) Update pupmod-simp-tpm for travis
 SIMP-179 #close Update upstart for travis
 (SIMP-179) Update pupmod-simp-upstart for travis - Due to a rake validate error, validation was removed from job.erb and put in separate functions. sysconfig.erb was causing similar problems, but it was being used anywhere and was deleted.
 SIMP-181 #close Update vsftpd for travis
 (SIMP-181) Update pupmod-simp-vsftpd for travis
 SIMP-182 #close Update windowmanager for travis
 (SIMP-182) Update pupmod-simp-windowmanager for travis
 SIMP-184 #close Update xwindows for travis
 (SIMP-184) Update pupmod-simp-xwindows for travis
 SIMP-188 #comment Pushed Augeasproviders
 SIMP-188 #comment Removed the Modulefile from the repo but not the spec file
 SIMP-188 #comment Updated augeasproviders_apache
 SIMP-188 #comment Updated augeasproviders_base
 SIMP-188 #comment Updated augeasproviders_core
 SIMP-188 #comment Updated augeasproviders_grub
 SIMP-188 #comment Updated augeasproviders_mounttab
 SIMP-188 #comment Updated augeasproviders_nagios
 SIMP-188 #comment Updated augeasproviders_pam
 SIMP-188 #comment Updated augeasproviders_postgresql
 SIMP-188 #comment Updated augeasproviders_puppet
 SIMP-188 #comment Updated augeasproviders_shellvar
 SIMP-188 #comment Updated augeasproviders_ssh
 SIMP-188 #comment Updated augeasproviders_sysctl
 SIMP-188 #comment Updated common
 SIMP-188 #comment Updated datacat
 SIMP-188 #comment Updated dhcp
 SIMP-188 #comment Updated elasticsearch
 SIMP-188 #comment Updated freeradius
 SIMP-188 #comment Updated functions
 SIMP-188 #comment Updated gpasswd
 SIMP-188 #comment Updated inifile
 SIMP-188 #comment Updated iptables
 SIMP-188 #comment Updated java
 SIMP-188 #comment Updated java_ks
 SIMP-188 #comment Updated logrotate
 SIMP-188 #comment Updated mysql
 SIMP-188 #comment Updated named
 SIMP-188 #comment Updated network
 SIMP-188 #comment Updated openldap
 SIMP-188 #comment Updated postgresql
 SIMP-188 #comment Updated pupmod
 SIMP-188 #comment Updated puppetdb
 SIMP-188 #comment Updated puppetlabs_apache
 SIMP-188 #comment Updated RPM spec file
 SIMP-188 #comment Updated rsync
 SIMP-188 #comment Updated stdlib
 SIMP-188 #comment Updated tftpboot
 SIMP-188 #comment Updated upstart
 (SIMP-188) Incorporating all of the gap material - This pulls in the changes from when we switched over from the old system. This is a *complete* fork which should be merged with our Apache module at some point.
 (SIMP-188) Incorporating all of the gap material. This pulls in the changes from when we switched over from the old system. We're at a later commit than I had originally thought, but it should merge in cleanly later on.
 (SIMP-188) Needed to fix the RPM spec file. Since we smashed our work together with the external module, we needed to update the spec file to handle this properly.
 Merge "(SIMP-188) Incorporating all of the gap material" into simp-master
 New .travis.yml file tests in ruby 1.9.3 and 2.0.0
 SIMP-193 #close
 SIMP-193 #close
 (SIMP-193) Supplementary cleanup - This patch cleans up some gemspec cruft after SIMP-193
 (SIMP-193) Support reading RPM files - This patch adds support for reading RPM information directly from RPM files as well as RPM spec files. It also removes some legacy style cruft
 SIMP-196 #close
 (SIMP-196) Supplementary cleanup - This patch updates the gem version before publishing to rubygems.org.
 (SIMP-208) Allow Puppet 4.X in simp-rake-helpers - Before this patch, requiring the gem 'simp-rake-helpers' would fail if PUPPET_VERSION was 4 or above. This patch relaxes the PUPPET_VERSION requirement in the gemspec for simp-rake-helpers and removed the superfluous 'require' statement in lib/simp/rake/helpers.rb that had been breaking Travis CI tests using ruby 1.9.3 with a NoMethodError since SIMP-226. This patch also increments the gem's version to 1.0.10.
 SIMP-208 #close #comment Gemfile honors PUPPET_VERSION, release 1.0.10
 SIMP-208 #comment Updated CHANGELOG for 1.0.10 release
 (SIMP-208) Update CHANGELOG for 1.0.10 release
 SIMP-210 #close validation has been removed from the templates
 SIMP-211 #close
 (SIMP-211) Remove validation erb in freeradius. Moved validation from templates/2/conf.erb and put them in separate functions. 2/conf.pp and conf/log.pp was edited to call the validation functions before it hits the template. Tests were also added for the validation functions in spec/functions/*.
 SIMP-213 #close Update apache for travis
 (SIMP-213) Update pupmod-simp-apache for travis
 (SIMP-226) Another ordering issue - This should be the last issue. Directories need to be ignored *after* the usual suspects.
 SIMP_226 #close #comment Well, that escalated quickly....
 SIMP-226 #comment Had a logic error
 SIMP-226 #comment Need to ignore directories last
 (SIMP-226) Fixed an issue with ordering - Not ignoring everything is probably a good thing.
 (SIMP-226) Update to handle 'dist' Releases - This spiraled a bit out of control as I was debugging:1) Sanely ignore any variables in Release tags in RPMs * This does mean that we probably need to start naming our RPMs by Version only. 2) Realized that any optimizations we were attempting to do were getting swallowed up by directory attributes. Now ignore directories and life is much better. 3) Base all rebuilds on dist/*rpm globs. This may rebuild *all* SRPMs in the dist directory but this is *probably* what you want anyway. 4) Cleaned up the code a bit.
 SIMP-229 #close Remove swappiness cron if value set.
 (SIMP-229)- Remove dynamic_swappiness cron job if a static value is set.
 SIMP-230 #close Moved stunnel's pid file.
 (SIMP-230) Stunnel's pid file moved to /var/run/stunnel.pid - Was /var/run/stunnel/stunnel.pid.
 SIMP-231 #comment Fixing missed tests
 SIMP-231 #comment Integrate Electrical Logstash and SIMP Logstash
 (SIMP-231) - Fixing tests - Had some Gerrit issues and some missed tests.
 (SIMP-231) Integrate the public logstash module - This pulls all of the SIMP material directly into the Logstash module from Electrical.
 SIMP-236 #comment Official path update from Martin Preisler
 (SIMP-236) Remove trailing whitespaces. This patch removes trailing whitespaces from various files.
 (SIMP-236) Update the RPM signing code - The RPM signing code needs to only use GPGv1. This patch ensures that it will nail itself to GPGv1 instead of using the default of GPGv2.
 SIMP-237 #comment Fix GPG signing code
 SIMP-237 #comment Official path update from Martin Preisler
 SIMP-237 #comment removed trailing whitespaces
 SIMP-23 #comment Gem for 1.0.3 release is ready for publication
 SIMP-248 #comment Raise error if GPG signing fails. Non-critical.
 (SIMP-248) Raise an error if GPG signing fails - This adds an exception in the case that the GPG signing command outright fails.
 (SIMP-252) Cast ima_audit to a string. Kernel_parameter does not take a boolean, it needs to be a string.
 SIMP-252 #close Cast ima_audit to string.
 SIMP-254 #close Update rysnc for global_etc.
 (SIMP-254) Update rsync to reflect global_etc changes. We no longer supply crontab or anacrontab in global_etcd, so don't rsync them in puppet.
 SIMP-255 #close #comment Stunnel pid dir and safety
 (SIMP-255) Modified stunnel's default pid dir. Moved it back to /var/run/stunnel/stunnel.pid by default. The init script only creates and chowns the pid dir if it does not exist. TV - Made the init script a bit safer all around.
 SIMP-258 #close #comment removed require in helpers.rb to fix bundler
 SIMP-263 #close #comment Added spec tests. Merge "(SIMP-263) Test that 1 rule can add >10 TCP ports"
 (SIMP-263) Test that 1 rule can add >10 TCP ports - This patch adds spec tests to detect a reported bug where iptables::add_tcp_stateful listen cannot add more than 10 ports.
 SIMP-265 #close #comment Disable IMA by default
 (SIMP-265) Disable IMA by default - While interesting and useful, IMA does add load to a system without providing an active defense mechanism, therefore it is being disabled by default.
 SIMP-267 #close
 (SIMP-267) Test IPv6 client_nets with iptables. The acceptance tests for iptables::add_tcp_stateful_listen didn't include IPv6. This patch provides those tests (which pass). This patch also expands the coverage of the basic spec tests.
 SIMP-271 #close #comment Fix legacy CRL + httpd restart
 (SIMP-271) Don't restart httpd when CRL fetched - The CRL download code used to restart after downloading the Puppet CRL. This should not be done (and probably should not have been done then!).
 SIMP-282 #close Split_port regex fix
 (SIMP-282) Small regex fix in split_port. Escaped a ]
 SIMP-287 #close #comment Force encoding of salt to UTF-8 in ruby 2.0.0
 (SIMP-287) Fix UTF-8/ASCII-8BIT encoding error - Before this patch, "simp config" would fail in EL7/Ruby 1.9+, due to a difference in the encoding of salt (ASCII-8BIT, due to a workaround for Ruby 1.8.7) and other Strings when concatenated inside Base64.encode64. This patch forces the encoding for salt to UTF8 if the version of Ruby is newer than 1.8, and bumps the gem and RPM release to 1.0.2.
 SIMP-28 #close
 (SIMP-28) Delete the publican docs. Removed directory and fixed Rakefile
 SIMP-291 #close #comment Modern Updates
 (SIMP-291) Modernize the Ciphers, MACs, and Kex - Added explicit cases for FIPS and non-FIPS mode as well as reasonable default cases for RHEL7 and below. Also added support for the KexAlgorithms to get those reasonably in line with the FIPS requirements out of the box. Had to pull updated augeas lenses directly from the Augeas project to get this to work properly. This required creating a sub-RPM for license compatibility.
 (SIMP-292) Fix usability issues in 'simp config'
 SIMP-293 #close #comment Prompts no longer drop down to the next line.
 SIMP-294 #close #comment Added special instructions for entering lists.
 SIMP-295 #close #comment Fixed speling.
 SIMP-296 #close
 SIMP-296 #comment log_servers are now optional in 'simp config'
 SIMP-296 #comment ntpd::servers are now optional in 'simp config'
 SIMP-297 #close #comment Generated passwords pause until 'enter'.
 SIMP-29 #close #comment will generate V4 docs with tags
 SIMP-29 #comment but readthedocs can't handle it
 SIMP-300 #close #comment rsync::server is now non-interactive.
 SIMP-302 #close #comment Item::YumRepositories prefers /var before /srv
 SIMP-303 #close #comment Compat with Ruby>2
 (SIMP-303) Removed pry-debugger. Pry-debugger does not work with Ruby > 2.
 SIMP-305 #comment Update to use native pkgs
 (SIMP-305) Update module to use native pkgs - This update modifies the tftpboot setup to pull as much as possible from the native system itself. Actual boot media will still need to be placed into rsync.
 SIMP-307 #close
 SIMP-30 #close Create a parse_hosts function.
 (SIMP-30) Create a parse_hosts function. Created a function to parse host strings/urls. Returns a hash of hostnames with an array of ports and an array of protocols. Modified strip_ports to use parse_hosts and added get_ports to return stripped ports.
 SIMP-315 #close #comment Be careful to update the Changelog to reflect that users need simp-ppolicy-check-password-2.4.39-0
 (SIMP-315) Update to work with the new PW Overlay - The Password Policy overlay was getting loaded into the default.ldif even if you didn't want to use it. This has been fixed. - Made the password policy overlay align with the latest SIMP build of the plugin. - This means that you *must* have version simp-ppolicy-check-password-2.4.39-0 or later available to the system being configured.
 SIMP-317 #close
 (SIMP-317) making password its own section. Also reordered sections and made a change to a file and prototyped conf.py with version number
 SIMP-320 #close #comment generate prompts no longer drop to next line.
 (SIMP-321) Allow empty log_servers Hiera parameter - This updates the system to ensure that the global template does not fail if actionSendStreamDriverPermittedPeers is an empty array.
 SIMP-321 #close #comment Fix for actionSendStreamDriverPermittedPeers being an empty array
 (SIMP-34) Add .travis.yml to rubygems-simp-cli
 SIMP-34 #close Added .travis.yml
 SIMP-35 #close Fixed License (w/link) & GFM formatting
 (SIMP-35) Fix License & GFM formatting errors
 SIMP-38 #comment Fixed 2.0.0 encoding in Utils.validate_openldap_hash
 SIMP-38 #comment resolved executable dependencies
 SIMP-40 #comment Closed outstanding subtasks, general cleanup
 SIMP-40 #comment Gem for 1.0.3 release is ready for publication
 (SIMP-40) Prepare simp-rake-helpers for release. This commit updates simp-rake-helpers to 1.0.3 and generally prepares the gem for its first release to rubygems.org. Before this update, simp-rake-helpers still used portions of the original Hoe scaffolding for some rake tasks and (nonexistent) tests. This update removes all dependencies on Hoe, and adds a lot of cleanup: - Rakefile tasks - updated dependencies, moved from Gemfile to .gemspec - The testing framework moved from Minitest to RSpec - There are now (trivial smoke) tests - The README is now markdown - History.txt is now CHANGELOG.md
 SIMP-41 #close Removed hoe artifacts from rubygem-simp-rake-helpers
 SIMP-43 #close Modernized Gemfile & .gemspec
 SIMP-44 #close Added gem build/install tasks to Rakefile
 SIMP-45 #close #comment Updated default ntp.conf settings
 SIMP-45 #comment Full closure on the NTP issue
 (SIMP-45) Prevent IPv6 ::1 spoofed addresses - This patch ensures that IPv6 spoofed addresses will be rejected by default. Catalyst: https://access.redhat.com/articles/1305723
 (SIMP-45) Updated per RH Article #1305723 - This updates the NTP module against the settings recommended in https://access.redhat.com/articles/1305723
 SIMP-47 #comment Gem update to handle GPG issues
 SIMP-47 #comment RPM spec correction to account for gem version bump
 SIMP-47 #comment updated rubygem-simp-rake-helpers to 1.0.6
 (SIMP-47) Corrected the RPM spec files - So version, much bad.
 (SIMP-47) Fixed issues with GPG signing - The new format that we're using for GPG signing broke compatibility with the support Gem. This adds the appropriate patches.
 SIMP-4 #comment removed publican docs
 SIMP-4 Intial commit of reST docs (w/Travis tests)
 SIMP-54 #close Added trivial tests to rubygem-simp-rake-helpers
 SIMP-56 #close
 (SIMP-56) use hiera as default for iptables::disable. Before this patch, iptables::disable did not reference the hiera global 'use_iptables' (which is set from the intial questionnaire by simp config), nor honor the iptables::disable parameter with IPv4 firewalls. With this commit, iptables::disable now controls the management of IPv4 firewall rules and refences hiera('use_iptables') for its default value. The module has been expanded to comply with as much of the New Module Layout as possible under SIMP 4.2.X and SIMP 5.1.X. This includes limited spec and acceptance (beaker) tests, new rake tasks, and an extensive README.md. It is possible to run beaker tests using the modules under spec/fixtures/modules (installed via `rake spec_prep`) by running acceptance tests with the environment variable setting: BEAKER_use_fixtures_dir_for_modules=yes
 SIMP-62 #close
 (SIMP-62) Update tags and grammar/spelling for versions (which may be useless) and
 (SIMP-63) Add SIMP_GIT_BRANCH to the Help message - Just adding a description of the SIMP_GIT_BRANCH environment variable to the Help message since the 'help' task overrides don't appear to be working properly.
 SIMP-63 #close
 (SIMP-64) Add missing files to manifest in .gemspec
 SIMP-64 #close
 SIMP-65 #close
 (SIMP-65) Fix ruby 2.0.0 string encoding errors - Before this patch, Ruby 2.0.0+ would fail the spec test for Simp::Cli::Config::Utils.validate_openldap_hash with an encoding error when adding string.to_s [ASCII-8BIT] and salt.to_s [UTF-8].
 SIMP-66 #close Created assets to help pupmod-simp-common run
 SIMP-70 #comment Pulled all of the legacy work in
 SIMP-71 #close
 (SIMP-71) Integrated puppetlabs-postgresql
 SIMP-99 #close #comment Fixed for SIMP5.X
 (SIMP-99) Created RPM for simp-rsync-clamav - ClamAV is under a different license than the rest of the Rsync module and needed to be split out into its own sub-RPM.
 SIMP-9 Initial upload of the 'site' module
 This commit merges in our past work to puppetlabs-postgresql which made

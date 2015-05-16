#!/usr/bin/env ruby

# A script to convert a all users in a running LDAP instance over to
# InetOrgPerson entries.

require 'ldap'
require 'erb'

# LDAP Stuff
domain = 'your.domain'
base_dn = domain.split('.').map{|x| x = "dc=#{x}"}.join(',')
ldap_server = "puppet.#{domain}"
ldap_admin_user = "cn=LDAPAdmin,ou=People,#{base_dn}"

print "Enter LDAP Admin Password: "
stty_settings = %x{stty -g}
begin
  %x{stty -echo}
  password = gets
  password.chomp!
ensure
  %x{stty #{stty_settings}}
end

# Get the appropriate LDAP entries
conn = LDAP::SSLConn.new(ldap_server,389,true)
conn.set_option(LDAP::LDAP_OPT_PROTOCOL_VERSION, 3)
conn.set_option(LDAP::LDAP_OPT_TIMELIMIT,30)
conn.simple_bind(ldap_admin_user,password)

search_string = '(!(objectClass=inetOrgPerson))'
conn.search("ou=People,#{base_dn}",LDAP::LDAP_SCOPE_SUBTREE,search_string,'*',false,30) { |entry|
  new_entry = entry.to_hash

  dn = new_entry.delete('dn')
  dn = dn.first

  next unless new_entry['uid']

  new_entry['objectClass'].delete('account')
  new_entry['objectClass'] << 'inetOrgPerson'

  uid = new_entry['uid'].first
  gn,sn = uid.split('.')
  if not sn then
    sn = gn[1..-1]
    gn = gn[0].chr
  end

  gn.capitalize!
  sn and sn.capitalize!

  new_entry['givenName'] = ["#{gn}"]
  new_entry['sn'] = ["#{sn}"]
  new_entry['cn'] = ["#{gn} #{sn}"]
  new_entry['mail'] = ["#{uid}@#{domain}"]

  conn.delete(dn)

  conn.add(dn,new_entry)
}

exit

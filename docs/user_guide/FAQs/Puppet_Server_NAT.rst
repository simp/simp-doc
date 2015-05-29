Puppet Server Behind a NAT
==========================

This section provides guidance for when the Puppet server is behind a
NAT but is managing hosts outside the NAT.

To resolve this issue, open the */etc/puppet/manifests/vars.pp* file and
rename the **$puppet\_servers** variable to
**$puppet\_server\_hosts\_mod**. Then, create a new **$puppet\_servers**
variable and point it to *template('site/nat\_ip\_switch.erb')*.

The entries in *vars.pp* should look like the following example.

Example Sample Entries in vars.pp

.. code-block:: Ruby

           $puppet_server_hosts_mod = "puppet.$dns_domain|1.2.3.4 puppet2.$dns_domain|2.3.4.5"
           $puppet_servers = template('site/nat_ip_switch.erb')
          

Create a */etc/puppet/modules/site/templates/nat\_ip\_switch.erb* file
with the content shown in the next example. Change the appropriate
portions of the content to meet the needs of the user environment.

    **Important**

    Ensure that the *.erb* file is owned by *root.puppet* and mode
    *640*.

Source Create the nat\_ip\_switch.erb

.. code-block:: Ruby

            <%
            # Edit this variable to provide the IP address mappings.
            # The left-hand side should contain the internal addresses.
            # The right-hand side should contain the external addresses.
            t_ipmap = {
                "1.2.3.4" => "10.10.10.10",
                "2.3.4.5" => '10.2.3.4'
            }

            # Edit this regex to match the hosts.
            # This is done with a Regexp; the user can use whichever is preferred.
            # Pure IP matching would be faster using the IPAddr class.
            t_inside_nets = Regexp.new("^5\.*")

            t_pupsrvs = puppet_server_hosts_mod.split(/\s|,|;/)

            # Change the ipaddress variable to the host that the regexp above is matching.
            if not t_inside_nets.match(ipaddress) then
              t_pupsrvs.each_index do |t_i|
                t_vals = t_pupsrvs[t_i].split(/\|/)
                if t_ipmap.include?(t_vals.last) then
                  t_vals[-1] = t_ipmap[t_vals.last]
                  t_pupsrvs[t_i] = t_vals.join('|')
                end
              end

              t_pupsrvs = t_pupsrvs.join(' ')
            end
            -%>
            <%= t_pupsrvs -%>
          

Run **puppet agent -t** on the client to receive the appropriately
mapped NAT address of the Puppet server.

If the user cannot connect to the NAT'd Puppet server, change the values
in the */etc/hosts* directory to the correct values and try running
**puppet agent -t** again.

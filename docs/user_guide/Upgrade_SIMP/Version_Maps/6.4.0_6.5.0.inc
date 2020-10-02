.. _upgrade-6.4.0-to-6.5.0:

Upgrading from SIMP-6.4.0 to SIMP-6.5.0
---------------------------------------

.. WARNING::

   You **MUST** be on at least SIMP 6.4.0 to upgrade to 6.5.0 because of
   changes to the deployment methods in SIMP 6.4.0.

Before You Begin
^^^^^^^^^^^^^^^^
1)  Changes to  ``pupmod-simp-iptables`` and the addition of the firewalld modules
    mean that fully qualified domain Names can no longer be used in firewall
    rules.  The entries for servers names in modules that also open up firewall
    access need to be changed to IP Addresses.  This includes
      NFS server settings:  


      Error: /Stage[main]/Aide/Exec[update_aide_db]: Failed to call refresh: Command exceeded timeout
      Error: /Stage[main]/Aide/Exec[update_aide_db]: Command exceeded timeout


      New Error when upgrading:  Error: Could not retrieve catalog from remote server: Error 500 on SERVER: Server Error: no parameter named 'precise_match' (file: /etc/puppetlabs/code/environments/production/modules/iptables/manifests/init.pp, line: 155) on Iptables_optimize[/etc/sysconfig/iptables] (file: /etc/puppetlabs/code/environments/production/modules/iptables/manifests/init.pp, line: 155) on node puppet.simp.test   But the parameter is of course there

Jenkins Scripts
===============

Jenkins is the tool that was chosen to perform integration tests on the
SIMP system. Jenkins is an application that monitors execution of
specific jobs. Below are the scripts that are used to generate VMs from
a SIMP ISO and build a basic SIMP test system.

Preconfiguration
----------------

This chapter assumes that the following conditions are met before
attempting to use the scripts below within Jenkins:

-  Jenkins is installed and you can create new jobs through the
   dashboard

-  Libvirt is installed

-  The jenkins user is part of the kvm group

-  The rubygem rubygem-net-scp is installed

-  There is at least 30G of harddrive space available for each VM you
   wish to create

-  A DHCP server exists and is available with an entry for any VMs you
   expect to create

-  You will be able to ssh into the VMs you create as root

-  The following preconfigured files exist in /srv/info:

   -  simp\_conf.csv.<vm\_name> - the output of the "simp config"
      command specific for the VM, one should exist for every VM

   -  <reverseip>.db.<vm\_name> - set up for the reverse file, one
      should exist for each VM on the host machine

   -  <domain name>.db..<vm\_name> - for forward, one should exist for
      each VM on the host machine

   -  named.conf.<vm\_name> - configured for the VM, one should exist
      for each VM on the host machine

   -  <domain\_name> - containing the VM's zones information

   -  dhcpd.conf.<vm\_name> - containing all entries for any clients the
      VM will have, one should exist for each VM on the host machine

   -  hosts - file that will replace /etc/hosts on the VM

   -  jenkins.pub - the jenkins user public key

   -  pupser.pp.<vm\_name> - puppet\_server node definition containing
      all necessary tftpboot information, one should exist for each VM
      on the host machine

   -  ldifs - a folder containing a script that will run the ldapadd
      command correctly and an ldiff containing any users you wish to
      add

   -  ksfiles.sh - a script that configures the config files in
      /var/www/ks;

      .. code-block:: Bash

          ip=<ipaddress>
          dist=<distribution>
          pushd .

          # Update the kickstart files
          sed -i "s/#KSSERVER#/${ip}/g" /var/www/ks;/*.cfg
          sed -i "s/#YUMSERVER#/${ip}/g" /var/www/ks;/*.cfg
          sed -i "s/#BOOTPASS#/<MD5 Hashed grub password>/g" /var/www/ks;/*.cfg
          sed -i "s/#ROOTPASS#/>MD5 Hashed root password>/g" /var/www/ks;/*.cfg
          sed -i "s/#LINUXDIST#/${dist}/g" /var/www/ks;/*.cfg
          chown root.apache /var/www/ks;/*
          chmod 640 /var/www/ks;/*
          chcon --reference=/var/www/ks;/diskdetect_puppet_server.sh /var/www/ks;/*.cfg

          # Change server name to client name (fqdn) in togen file
          sed -i 's/<server_fqdn>/<client_fqdn>/g' /etc/puppet/Config/FakeCA/togen
          cd /etc/puppet/Config/FakeCA
          ./gencerts_nopass.sh auto
          popd

          # Check /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/rhel<version>-<arch>
          echo "Check that the boot image(s) are owned by root.nobody and perms are 644."
          echo ""
          chgrp -h nobody /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*
          chgrp nobody /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*/initrd.img
          chgrp nobody /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*/vmlinuz
          chmod 644 /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*/initrd.img
          chmod 644 /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*/vmlinuz
          ls -ld /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*
          echo ""
          ls -l /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*/initrd.img
          echo ""
          ls -l /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/tftpboot/linux-install/*/vmlinuz
          echo ""

          # Compare pupser.pp with /etc/puppet/manifests/nodes/puppet_servers.pp
          diff /srv/info/pupser.pp.u6s40cent /etc/puppet/manifests/nodes/puppet_servers.pp
          cp   /srv/info/pupser.pp.u6s40cent /etc/puppet/manifests/nodes/puppet_servers.pp

          # add users
          /srv/info/ldifs/ladd.cmd /srv/info/ldifs/users.ldif

          # add alias
          echo "alias root='sudo sudosh'" >> /etc/bashrc
                           

    **Note**

    The followilng modifications were made to ISO the before using it to
    create a VM:

    Change the bootprotocol to dhcp
        ::

            sed -i 's/network --bootproto=static --ip=192.168.0.111 --netmask=255.255.255.0 --gateway=192.168.1.1 --nodns --hostname=puppet.change.me/network --bootproto=dhcp/g' ks/dvd/include/common_ks_base
                       

    Prevent the system from forcing a root password change
        ::

            sed -i 's/chage -d 0 root;//g' ks/dvd/*.cfg
                   

    Use simp-big by default instead of just simp
        ::

            sed -i 's/default simp$/default simp-big/g' isolinux/isolinux.cfg
                       

Create a VM
-----------

String Parameters

-  simp\_ver - The version of SIMP (2.0.X, 4.0.X, etc.)

-  os\_dist - The operating system distribution (CentOS or RedHat)

-  vm\_name - The name of the VM you will be creating

-  vm\_mac - The MAC address of the VM you will be creating

-  build\_dir - The directory where your SIMP ISO is stored

.. code-block:: Bash

    #!/bin/bash
    cd ${build_dir}/SIMP-${simp_ver}
    if [ "${simp_ver}" == "2.0.X" ]; then variant="rhel5.4"; else variant="rhel6"; fi
    ISO="`ls SIMP-*${os_dist}*.iso`"
    EXISTS=`virsh --connect qemu:///system list --all | grep ${vm_name}`
    EXISTSOFF=`virsh --connect qemu:///system list --all | grep ${vm_name} | grep "shut off"`
    ignore='false'
    echo "Virsh list:"
    virsh --connect qemu:///system list --all

    if [ "${1}" == '-i' ]; then ignore='true'; fi

    if [ ! -d /var/lib/jenkins/VM ]; then mkdir /var/lib/jenkins/VM; fi

    if [ "${ignore}" == 'true' ] || [ ! -d "/var/lib/jenkins/VM/${vm_name}" ]; then
       mkdir /var/lib/jenkins/VM/${vm_name}
    elif [ -f "/var/lib/jenkins/VM/${vm_name}/Disk1" ] && [ ! "${EXISTS}" == "" ]; then
       echo "VM ${vm_name} already exists, overwriting with the latest..."
    if [ "${EXISTSOFF}" == "" ]; then
       echo "Destroying ${vm_name}"
       virsh --connect qemu:///system destroy ${vm_name}
    fi
    if [ -f "/var/lib/jenkins/VM/${vm_name}/Disk1.base" ]; then
       echo "Removing old snapshots"
       rm -rf /var/lib/jenkins/VM/${vm_name}/Disk1 /var/lib/jenkins/VM/${vm_name}/Disk1_Test
       mv /var/lib/jenkins/VM/${vm_name}/Disk1.base /var/lib/jenkins/VM/${vm_name}/Disk1
    fi
       echo "Undefining ${vm_name}"
       virsh --connect qemu:///system undefine ${vm_name}
    else
       echo "Creating VM..."
    fi

    echo "Starting installation of the ${vm_name} VM via the ${ISO}"

    /usr/bin/virt-install --connect qemu:///system -n "${vm_name}" -r 1024 --vcpus=1 --vnc --noautoconsole --os-variant=${variant} --os-type=linux -w bridge:br0 -m ${vm_mac} --disk=path="/var/lib/jenkins/VM/${vm_name}/Disk1",size=30,sparse='false' -v --accelerate --sound --cdrom=${build_dir}/SIMP-${simp_ver}/${ISO}
    wait

    SUCCESS=`/usr/bin/virsh --connect qemu:///system autostart ${vm_name}`
    echo ${SUCCESS}
    echo "Installing ${vm_name}"
    f [ "${SUCCESS}"=="Domain ${vm_name} marked as autostarted" ]; then
     still_running () { ps -f -C qemu-kvm | grep ${vm_name} | grep 'no-reboot' >& /dev/null; return ${?}; };
       while still_running; do echo -n '>'; sleep 5; done; echo;
     sleep 5;
     echo "Starting ${vm_name}";
     /usr/bin/virsh --connect qemu:///system start ${vm_name};
      echo "Waiting for VM to start...";
      still_rebooting () { test "`echo "^" | telnet ${vm_name} 22 2> /dev/null | grep Connected`" = ""; };
      while still_rebooting; do echo -n '>'; sleep 5; done; echo;
      virsh --connect qemu:///system autostart --disable ${vm_name};
      echo "Ready!";
      exit 0;
    else
      exit 1;
    fi

    echo ""; echo "Virsh list:"
    virsh --connect qemu:///system list --all
            

Setup VM
--------

String Parameters

-  vm\_name - The name of the vm that was created using the previous
   script

-  vm\_ip - IP address of the VM that was just created

Password Parameter

-  vm\_pass - The password for root

.. code-block:: Bash

    #!/bin/bash
    Clears the ip from known_hosts.
    ssh-keygen -R ${vm_ip}
           

.. code-block:: Ruby

    require 'rubygems'
    require 'net/scp'

    Net::SSH.start(ENV['vm_name'], 'root', :password => ENV['vm_pass'], 
    :auth_methods => "password", :encryption => "aes256-cbc") do |ssh|
     ssh.exec!("mkdir /root/.ssh") do|ch, stream, data|
       puts data
     end
     ssh.exec!("chmod -R 700 /root/.ssh") do|ch, stream, data|
       puts data
     end
     ssh.exec!("mkdir /srv/info") do|ch, stream, data|
       puts data
     end
    end
    puts "Copying over configuration files..."

    Net::SCP.start(ENV['vm_name'], 'root', :password => ENV['vm_pass'], 
    :auth_methods => "password", :encryption => "aes256-cbc") do |scp|
      scp.upload!("/srv/info/jenkins.pub", "/root/.ssh/authorized_keys") do |ch, name|
      end
      puts "Copied jenkins public key to VM."
      scp.upload!("/srv/info", "/srv/", :recursive => true) do |ch, name|
      end
      %x(ls /srv/info).each do |x|
        puts "Copied #{x.chomp} to VM."
      end
      scp.upload!("/srv/isos", "/srv/", :recursive => true) do |ch, name|
      end  
      %x(ls /srv/isos).each do |x|
        puts "Copied #{x.chomp} to VM."
      end
    end

    Net::SSH.start(ENV['vm_name'], 'root', :password => ENV['vm_pass'], 
    :auth_methods => "password", :encryption => "aes256-cbc") do |ssh|
      puts "simp config -a /srv/info/simp_conf.csv.#{ENV['vm_name']}"
      ssh.exec!("chmod -R 750 /srv/info/") do|ch, stream, data|
        puts data
      end
      ssh.exec!("simp config -a /srv/info/simp_conf.csv.#{ENV['vm_name']}") do|ch, stream, data|
        puts data
      end
      puts "Bootstraping..."
      ssh.exec!("simp bootstrap -v --no-track") do|ch, stream, data|
        puts data
      end
      puts "Installing java..."
      ssh.exec!("yum install -y java simp-mit rubygem-cucumber rubygem-rspec rubygem-net-ssh") do|ch, stream, data|
        puts data
      end
      puts "Rebooting.."
      ssh.exec!("shutdown -r +1") do|ch, stream, data|
         puts data
      end
    end 
           

.. code-block:: Bash

    #!/bin/bash
    virsh --connect qemu:///system destroy ${vm_name}
    sleep 60
    virsh --connect qemu:///system start ${vm_name}

    echo "Waiting for VM to restart..."
    still_rebooting () { test "`telnet ${vm_ip} 22 2> /dev/null | grep Connected`" = ""; }
    while still_rebooting; do echo -n '>'; sleep 5; done
    echo -e "\nVM restarted!"

    # The key will have changed after setup; remove again.
    ssh-keygen -R ${vm_ip}
        

.. code-block:: Ruby

    # DHCP Setup
    require 'rubygems'
    require 'net/ssh'

    Net::SSH.start(ENV['vm_name'], 'root', :password => ENV['vm_pass'], 
    :auth_methods => "password", :encryption => "aes256-cbc") do |ssh|
      puts "Configuring DHCP..."
      ssh.exec!("cat /srv/info/dhcpd.conf.#{ENV['vm_name']} > /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/dhcpd/dhcpd.conf") do |ch, stream, data|
        puts data
      end
      puts "Configuring hostfile..."
      ssh.exec!("cat /srv/info/hosts > /etc/hosts") do |ch, stream, data|
        puts data
      end
    end
           

.. code-block:: Ruby

    # DNS Setup
    require 'rubygems'
    require 'net/ssh'

    Net::SSH.start(ENV['vm_name'], 'root', :password => ENV['vm_pass'], 
    :auth_methods => "password", :encryption => "aes256-cbc") do |ssh|
     puts "Renaming your.domain to simp.dev..."
     ssh.exec!("mv /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/your.domain /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev") do |ch, stream, data| puts data
     end
     puts "Configuring named.conf..."
     ssh.exec!("cat /srv/info/named.conf.#{ENV['vm_name']} > /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/named.conf") do |ch, stream, data| puts data
     end
     puts "Configuring simp.dev zone..."
     ssh.exec!("mv /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/zones/your.domain /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/zones/simp.dev") do |ch, stream, data| puts data
     end
     ssh.exec!("cat /srv/info/simp.dev > /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/zones/simp.dev") do |ch, stream, data| puts data
     end
     puts "Configuring reverse lookup..."
     ssh.exec!("mv /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/reverse/0.0.10.db /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/reverse/<reverseip>.db") do |ch, stream, data| puts data
     end
     ssh.exec!("cat /srv/info/<reverseip>.db.#{ENV['vm_name']} > /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/reverse/<reverseip>.db") do |ch, stream, data| puts data
     end
     puts "Configuring forward lookup..."
     ssh.exec!("mv /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/forward/your.domain.db /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/forward/simp.dev.db") do |ch, stream, data| puts data
     end
     ssh.exec!("cat /srv/info/simp.dev.db.#{ENV['vm_name']} > /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/forward/simp.dev.db") do |ch, stream, data| puts data
     end
     ssh.exec!("chown root.named /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/reverse/<reverseip>.db /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/zones/simp.dev /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/named.conf /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/forward/simp.dev.db") do |ch, stream, data| puts data
     end
     ssh.exec!("chmod 640 /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/reverse/<reverseip>.db /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/zones/simp.dev /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/etc/named.conf /var/simp/rsync/CentOS/RHEL_MAJOR_VERSION/domains/simp.dev/named/var/named/forward/simp.dev.db") do |ch, stream, data| puts data
     end
    end
        

.. code-block:: Bash

    # Setup Kickstart
    require 'rubygems'
    require 'net/ssh'

    Net::SSH.start(ENV['vm_name'], 'root', :password => ENV['vm_pass'], 
    :auth_methods => "password", :encryption => "aes256-cbc") do |ssh|
     puts "Setting up server to be able to kickstart clients..."
     ssh.exec!("/srv/info/ksfiles.sh") do |ch, stream, data| puts data
     end
    end
        

Test Your Configuration
---------------------==

.. code-block:: Bash

    #!/bin/bash
    if [ `ps -ef | grep puppet | grep -v grep | grep -v Rack | wc -l` -gt 0 ]; then
      echo "Waiting for current puppet run to complete..."; 
    fi
    while [ `ps -ef | grep puppet | grep -v grep | grep -v Rack | wc -l` -gt 0 ]; do 
      sleep 5; done
    echo -e "\nPuppet Agent Run - First Pass"
    echo "-------------------------------"
    puppet agent -t
    rtn1=${?}
    echo "First Pass Return Code: ${rtn1}"
    echo -e "\nPuppet Agent Run - Second Pass"
    echo "-------------------------------"
    puppet agent -t
    rtn2=${?}
    echo -n "Second Pass Return Code: ${rtn2}"
    if [ ${rtn2} -eq 0 -o ${rtn2} -eq 2 ]; then 
      echo " - Successful Puppet Run"; else return 1; 
    fi
        

Test a Specific Module
---------------------=

String Parameters

-  test\_mod - The name of the module you wish to test

.. code-block:: Bash

    yum install -y pupmod-${test_mod}-test

    if [ ! -d ${WORKSPACE}/junit ]; then mkdir ${WORKSPACE}/junit; fi
     
    if [ -f /usr/share/simp/tests/modules/${test_mod}/mit_tests/Rakefile ]; then
       echo "Testing ${test_mod}..."
       cd /usr/share/simp/tests/modules/${test_mod}/mit_tests
       if [ ! -d ./results.xml ]; then 
         rm -f results.xml
         mkdir results.xml; chmod 755 results.xml; chgrp puppet results.xml
       fi
       rake testall:junit
       cp /usr/share/simp/tests/modules/${test_mod}/mit_tests/results.xml/*.xml ${WORKSPACE}/junit/
       sleep 5
    fi
        

Create a Client VM
------------------

String Parameters

-  simp\_ver - The version of simp that the client will have loaded on
   it (2.0.X, 4.0.X, etc.)

   cli\_name - The name of the client VM you want to create

   cli\_mac - The MAC address of the client VM, this should match an
   entry that was placed in the dhcp.conf that was created on your
   server VM

.. code-block:: Bash

    #!/bin/bash
    d1=`date`
    if [ "${simp_ver}" == "2.0.X" ]; then variant="rhel5.4"; else variant="rhel6"; fi
    EXISTS=`virsh --connect qemu:///system list --all | grep ${cli_name}`
    EXISTSOFF=`virsh --connect qemu:///system list --all | grep ${cli_name} | grep "shut off"`

    echo "Virsh list:"
    virsh --connect qemu:///system list --all

    if [ ! -d /var/lib/jenkins/VM ]; then mkdir /var/lib/jenkins/VM; fi

    if [ ! -d "/var/lib/jenkins/VM/${cli_name}" ]; then
      mkdir /var/lib/jenkins/VM/${cli_name}
    elif [ -f "/var/lib/jenkins/VM/${cli_name}/Disk1" ] && [ ! "${EXISTS}" == "" ]; then
      echo "VM ${cli_name} already exists, overwriting with the latest..."
      if [ "${EXISTSOFF}" == "" ]; then
        echo "Destroying ${cli_name}"
        virsh --connect qemu:///system destroy ${cli_name}
      fi
      if [ -f "/var/lib/jenkins/VM/${cli_name}/Disk1.base" ]; then
        echo "Removing old snapshots"
        rm -rf /var/lib/jenkins/VM/${cli_name}/Disk1 /var/lib/jenkins/VM/${cli_name}/Disk1_Test
        mv /var/lib/jenkins/VM/${cli_name}/Disk1.base /var/lib/jenkins/VM/${cli_name}/Disk1
      fi
      echo "Undefining ${cli_name}"
      virsh --connect qemu:///system undefine ${cli_name}
    else
      echo "Creating VM..."
    fi

    /usr/bin/virt-install --connect qemu:///system -n "${cli_name}" -r 512 --vcpus=1 --vnc --noautoconsole --os-variant=${variant} --os-type=linux -w bridge:br0 -m ${cli_mac} --disk=path="/var/lib/jenkins/VM/${cli_name}/Disk1",size=30,sparse='true',bus='virtio' -v --accelerate --pxe

    wait

    SUCCESS=`/usr/bin/virsh --connect qemu:///system autostart ${cli_name}`
    echo ${SUCCESS}
    echo "Installing ${cli_name}"
    if [ "${SUCCESS}"=="Domain ${cli_name} marked as autostarted" ]; then
      still_running () { ps -f -C qemu-kvm | grep ${cli_name} | grep 'no-reboot' >& /dev/null; return ${?}; }
      while still_running; do
        echo -n '>'
        sleep 5
      done
      echo
      sleep 5
      echo "Starting ${cli_name}";
      /usr/bin/virsh --connect qemu:///system start ${cli_name}
      echo "Waiting for VM to start..."
      still_rebooting () { test "`echo "^" | telnet ${cli_name} 22 2> /dev/null | grep Connected`" = ""; }
      while still_rebooting; do
        echo -n '>'
        sleep 5
      done
      echo
      virsh --connect qemu:///system autostart --disable ${cli_name};
      echo "VM Build started: ${d1}"
      echo "VM Build   ended: `date`"
      exit 0
    else
      exit 1
    fi
    echo ""; echo "Virsh list:"
    virsh --connect qemu:///system list --all

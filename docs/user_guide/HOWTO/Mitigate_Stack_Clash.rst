HOWTO Mitigate Stack Clash
==========================

The Stack Clash exploit can be mitigated by setting stack and address limits
in PAM.

.. WARNING::
  Setting stack and address limits in PAM affects ALL processes on the system.
  It is *very* important that you do not limit space needed by critical
  applications.

.. NOTE::
  Setting PAM limits will NOT guarantee immunity from Stack Clash exploits,
  but will reduce the chance of large-footprint attacks.  It is *highly*
  recommended you upgrade your kernel or obtain a vendor patch to protect
  your machines from attack.  Setting limits in PAM is a last ditch effort
  when upgrading is not feasible.

Before applying limits to your system, estimate the amount of stack and address
space used by critical applications.  We have created a `space estimation`_
script to assist with limit calculation.

We have run the script on a SIMP system with many of the component modules
installed, and have created the following class with defaults based off of the
results.  You may find you need to adjust limits or supplement the class with
additional limits.

.. code-block:: puppet

  # Mitigate suceptibility to the 'stack clash' exlpoit by limiting
  # the stack size and address size for local and remote users.
  #
  # These limitations do NOT guarantee immunity from the exploit, but
  # reduce the chance of large-footprint attacks.
  #
  # NOTE: Before applying this to your system, you should estimate
  # the amount of stack and address space used by authorized
  # applications.  You may find you need to adjust limits or augment
  # this list to best suit your system.
  #
  # A tool for calculating the largest consumers of stack and address
  # space can be found here:
  #   https://gist.github.com/8f28ac8d908b3379fa9cee97b910ac54.git
  #
  # @param ignore_list
  #   Any prameter in this list will be given unlimited stack and
  #   address space.
  #
  #   Default: Ignore root, dbus, gdm. Root and dbus serve critical
  #   roles and are unlimited for obvious reasons. GDM is a stack
  #   heavy application that must be unlimited, or users run the
  #   risk of loosing GUI access to their system. 
  #
  # @param stack_limit
  #   The max stack size, in KB, that applications not in the
  #   ignore_list will be limited to.
  #
  # @param address_limit
  #   The max address size, in KB, that applications not in the
  #   ignore_list will be limited to. 
  #
  # @author SIMP Team
  #
  class simp::pam_limits::stack_clash(
    Array[String] $ignore_list   = ['root','dbus','gdm'],
    Integer       $stack_limit   = 262144,
    Integer       $address_limit = 4194304
  ){
  
    pam::limits::rule { 'ignore_stack':
      domains => $ignore_list,
      type    => '-',
      item    => 'stack',
      value   => 'unlimited',
      order   =>   1
    }
  
    pam::limits::rule { 'ignore_as':
      domains => $ignore_list,
      type    => '-',
      item    => 'as',
      value   => 'unlimited',
      order   =>   1
    }
  
    pam::limits::rule { 'limit_stack':
      domains => ['*'],
      type    => '-',
      item    => 'stack',
      value   => $stack_limit,
      order   =>  999
    }
  
    pam::limits::rule { 'limit_as':
      domains => ['*'],
      type    => '-',
      item    => 'as',
      value   => $address_limit,
      order   =>  999
    }
  
  }

.. _space estimation: https://gist.github.com/8f28ac8d908b3379fa9cee97b910ac54.git

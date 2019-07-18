.. _ug-prevent-lockout:

Prevent Lockout From Puppetserver During RPM Installation
---------------------------------------------------------


Per security policy, SIMP, by default, disables login via ssh for all
users, including 'root', and beginning with SIMP 6.0.0 (when
useradd::securetty is empty), disables root logins at the console.  So,
to prevent lockout in systems for which no administrative user account
has yet been created or both console access is not available and the
administrative user's ssh access has not yet been enabled, you should
configure a local user for this server to have both su and ssh
privileges.

``simp config`` will issue a warning  if it thinks this situation has occured.
The warning looks like:

|  'simp bootstrap' has been locked due to potential login lockout.
|  * See /root/.simp/simp_bootstrap_start_lock for details


If you have access to the console and have enabled console access by setting
``useradd::securetty`` in :term:`Hiera` to a valid tty you can simply remove
the file ``/root/.simp/simp_bootstrap_start_lock`` and run ``simp bootstrap``.

Otherwise follow the instructions below to enable login from a local account.


Configure Local User for Access
===============================

This example creates a local module, ``mymodule``.

#. Create a local user account, as needed, using useradd.  This examples uses
   ``userx``.

#. Create a local puppet module under the ``production`` environment and add
   a manifest to enable su and allow ssh access for the user you created:

   .. code-block:: bash

      $ sudo mkdir -p /etc/puppetlabs/code/environments/production/``mymodule``/manifests


   Create a manifest in this directory, this example cals it ``local_user``.pp,  and add the
   following code:

   .. code-block:: ruby

      class mymodule::local_user (
      Boolean $pam = simplib::lookup('simp_options::pam', { 'default_value' => false }),
      ) {
        if $pam {
          include '::pam'

          pam::access::rule { 'allow_userx':
            users   => ['userx'],
            origins => ['ALL'],
            comment => 'The local user, used to remotely login to the system in the case of a lockout.'
          }
        }

        sudo::user_specification { 'default_userx':
          user_list => ['userx'],
          runas     => 'root',
          cmnd      => ['/bin/su root', '/bin/su - root']
        }
      }

#. Make sure the permissions are correct on the module:

   .. code-block:: bash

      $ sudo chown -R root:puppet  /etc/puppetlabs/code/environments/production/mymodule
      $ sudo chmod -R g+rX  /etc/puppetlabs/code/environments/production/mymodule

#. Add the module to the puppetserver's yaml file class list:

   Edit the puppetserver's yaml file,
   ``/etc/puppetlabs/code/environments/production/data/<FQDN of your puppetserver>.yaml``
   and add the module to your puppet servers class list.

   .. code-block:: yaml

      classes:
        ...
        - mymodule::local_user

#. If the local user is configured to login with pre-shared keys
   instead of a password, copy the authorized_keys file for that
   user to /etc/ssh/local_keys/<username>.  For example,

   .. code-block:: bash

      $ sudo cp ~userx/.ssh/authorized_keys /etc/ssh/local_keys/userx


#. Add the module to the Puppetfile in the production environment:

   Edit the Puppetfile used to deploy the modules,
   ``/etc/puppetlabs/code/environments/production/Puppetfile``,  and add a line
   under the section that says "Add you own Puppet modules here"

   .. code-block:: yaml

      mod 'mymodule'; :local => true




.. _ug-prevent-lockout:

Prevent Lockout from the SIMP Server during RPM Installation
------------------------------------------------------------

Per security policy, SIMP, by default, disables login via ``ssh`` for all users,
including ``root``, and beginning with SIMP 6.0.0, disables ``root`` logins at
the console by default.  So, if one of the following scenarios applies, you
should configure a local user for this server to have both ``su`` and ``ssh``
privileges, in order to prevent lockout from the system:

* Console access is available but not allowed for ``root`` and no other
  administrative user account has yet been created.

  * This can happen when SIMP is installed from RPM and the user accepts
    ``simp config``'s default value for ``useradd:securetty`` (an empty array).

* Both console access is not available and the administrative user's ``ssh``
  access has not yet been enabled (permanently) via Puppet.

  * This can happen when SIMP is installed from RPM on cloud systems.

``simp config`` will issue a warning if it thinks this situation may be
possible and writes a lock file to prevent ``simp bootstrap`` from running.
The warning looks like:

|  ``'simp bootstrap' has been locked due to potential login lockout.``
|  ``* See /root/.simp/simp_bootstrap_start_lock for details``


If you have access to the console, have the ``root`` password, and have enabled
``root`` console access by setting ``useradd::securetty`` in :term:`Hiera` to a
valid tty (e.g., ``tty0``), you can simply remove the file
``/root/.simp/simp_bootstrap_start_lock`` and continue with the bootstrap
process.

Otherwise follow the instructions below to enable login from a local account,
and then remove ``/root/.simp/simp_bootstrap_start_lock`` and continue with the
bootstrap process.


Configure Local User for Access
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This example creates a manifest in a local module, ``mymodule``, in the
``production`` Puppet environment.  See `Puppet module documentation`_ for
information on how to create a Puppet module.

#. Create a local user account, as needed, using ``useradd``.  This example
   assumes the local user is ``userx``.

   * Be sure to set the user's password if the user is logging in with a password.
   * SIMP is configured to create a home directory for the user, if it does
     not exist when the user first logs in.

#. Create a ``local_user.pp`` manifest in ``mymodule/manifests`` to enable
   ``sudo su - root`` and allow ``ssh`` access for the user you created/selected:

   .. code-block:: ruby

      class mymodule::local_user (
      Boolean $pam = simplib::lookup('simp_options::pam', { 'default_value' => false }),
      ) {

        sudo::user_specification { 'default_userx':
          user_list => ['userx'],
          runas     => 'root',
          cmnd      => ['/bin/su root', '/bin/su - root']
        }

        if $pam {
          include '::pam'

          pam::access::rule { 'allow_userx':
            users   => ['userx'],
            origins => ['ALL'],
            comment => 'The local user, used to remotely login to the system in the case of a lockout.'
          }
        }
      }

#. Create a mymodule/metadata.json file.  It should look something like the following:
   See `Puppet metadata documentation`_ for more information on metadata.json files.

   .. code-block:: yaml

     {
       "name": "mymodule",
       "version": "0.0.1",
       "author": "Your name or group here",
       "summary": "Configures Local User for sudo access",
       "license": "Apache-2.0",
       "source": "Your gitlab url or local",
       "dependencies": [
         {
           "name": "simp/pam",
           "version_requirement": ">= 6.0.0 "
         },
         {
           "name": "simp/sudo",
           "version_requirement": ">= 5.1.0 "
         },

       ],
     }

#. Make sure the permissions are correct on the module:

   .. code-block:: bash

      sudo chown -R root:puppet  /etc/puppetlabs/code/environments/production/modules/mymodule
      sudo chmod -R g+rX  /etc/puppetlabs/code/environments/production/modules/mymodule

#. Add the module to the SIMP server's host YAML file class list:

   Edit the SIMP server's YAML file,
   ``/etc/puppetlabs/code/environments/production/data/<SIMP server FQDN>.yaml``
   and add the ``mymodule::local_user`` to the ``classes`` array:

   .. code-block:: yaml

      classes:
        - mymodule::local_user

#. If the local user is configured to login with pre-shared keys instead of a
   password (typical cloud configuration), copy the ``authorized_keys`` file for
   that user to the SIMP-managed location for authorized keys ``/etc/ssh/local_keys``:

   .. code-block:: bash

      sudo mkdir -p /etc/ssh/local_keys
      sudo chmod 755 /etc/ssh/local_keys
      sudo cp ~userx/.ssh/authorized_keys /etc/ssh/local_keys/userx
      sudo chmod 644 /etc/ssh/local_keys/userx


#. Add the module to the ``Puppetfile`` in the ``production`` environment:

   Edit the ``Puppetfile`` used to deploy the modules,
   ``/etc/puppetlabs/code/environments/production/Puppetfile``,  and add a line
   under the section that says "Add you own Puppet modules here"

   .. code-block:: yaml

      mod 'mymodule', :local => true

.. _Puppet module documentation: https://puppet.com/docs/puppet/5.5/modules.html

.. _Puppet metadata documentation: https://puppet.com/docs/puppet/latest/modules_metadata.html#metadatajson-example

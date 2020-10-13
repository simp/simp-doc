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
valid tty (e.g., ``tty0``), the login lockout warning is not applicable. After
you address any other issues identified in ``/root/.simp/simp_bootstrap_start_lock``
(see :ref:`ug-other-bootstrap-lock-issues`), you can simply remove the file and
continue with the bootstrap process.

Otherwise, follow the instructions in the subsections below to create a local
user who can successfully access the system and sudo to ``root``.


Configure Local User for Access
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This example creates a manifest in a local module, ``mymodule``, in the
``production`` Puppet environment.  See `Puppet module documentation`_ for
information on how to create a Puppet module.

This example also assumes you are logged in as ``root``.

#. Create a local user account, as needed, using ``useradd``.  This example
   assumes the local user is ``userx``.

   * Be sure to set the user's password if the user is logging in with a password.
   * SIMP is configured to create a home directory for the user, if it does
     not exist when the user first logs in.

#. Create a ``local_user.pp`` manifest in ``mymodule/manifests`` to enable
   ``sudo su - root`` and allow ``ssh`` access for the user you created/selected:

   a. Create the ``mymodule`` module directory and its ``manifests`` sub-directory

      .. code-block:: sh

         $ mkdir -p /etc/puppetlabs/code/environments/production/modules/mymodule/manifests

   b. Create ``mymodule/manifests/local_user.pp`` with the following content:

      .. code-block:: ruby

         class mymodule::local_user (
           Boolean $pam = simplib::lookup('simp_options::pam', { 'default_value' => false }),
         ) {

           sudo::user_specification { 'default_userx':
             user_list => ['userx'],
             runas     => 'root',
          #   passwd    => false,   # only needed if user logs in without a password
             cmnd      => ['/bin/su root', '/bin/su - root']
           }

           if $pam {
             include 'pam'

             pam::access::rule { 'allow_userx':
               users   => ['userx'],
               origins => ['ALL'],
               comment => 'The local user, used to remotely login to the system in the case of a lockout.'
             }
           }
         }

   #. Uncomment out the ``passwd`` line in ``sudo::user_specification`` if the local
      user is configured to login with pre-shared keys instead of a password
      (typical cloud configuration).

#. Create a ``mymodule/metadata.json`` file.

   * See `Puppet metadata documentation`_ for more information on metadata.json files.
   * It should look something like the following:

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
           "name": "simp/pam"
         },
         {
           "name": "simp/simplib"
         },
         {
           "name": "simp/sudo"
         }
       ]
     }

#. Make sure the permissions are correct on the module:

   .. code-block:: bash

      $ chown -R root:puppet  /etc/puppetlabs/code/environments/production/modules/mymodule
      $ chmod -R g+rX  /etc/puppetlabs/code/environments/production/modules/mymodule

#. Add the module to the SIMP server's host YAML file class list:

   Edit the SIMP server's YAML file,
   ``/etc/puppetlabs/code/environments/production/data/hosts/<SIMP server FQDN>.yaml``
   and add the ``mymodule::local_user`` to the ``simp::classes`` array:

   .. code-block:: yaml

      simp::classes:
        - mymodule::local_user

#. If the local user is configured to login with pre-shared keys instead of a
   password (typical cloud configuration), copy the ``authorized_keys`` file for
   that user to the SIMP-managed location for authorized keys ``/etc/ssh/local_keys``:

   .. code-block:: bash

      $ mkdir -p /etc/ssh/local_keys
      $ chmod 755 /etc/ssh/local_keys
      $ cp ~userx/.ssh/authorized_keys /etc/ssh/local_keys/userx
      $ chmod 644 /etc/ssh/local_keys/userx


#. Add the module to the ``Puppetfile`` in the ``production`` environment:

   Edit the ``Puppetfile`` used to deploy the modules,
   ``/etc/puppetlabs/code/environments/production/Puppetfile``,  and add a line
   under the section that says "Add you own Puppet modules here"

   .. code-block:: yaml

      mod 'mymodule', :local => true


.. _ug-other-bootstrap-lock-issues:

Resolve Other Issues in Bootstrap Lock File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The bootstrap lock file can identify other issues, in addition to user lockout.
If any other issues are identified in ``/root/.simp/simp_bootstrap_start_lock``,
address those issues before removing the file.

  * Currently, the only other issue ``simp config`` will identify is a possible
    misconfiguration of YUM repositories. ``simp config`` will lock out
    bootstrap if it cannot find a few of the key packages needed for
    bootstrapping.  Fix your yum repository configuration and then verify the
    fix using the verification instructions in the lock message.

Resume Bootstrap Operation and Verify User Access
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. IMPORTANT::

   DO NOT REBOOT BEFORE VERIFYING USER ACCESS

#. Remove ``/root/.simp/simp_bootstrap_start_lock``.
#. Run ``simp bootstrap``.
#. Run ``puppet agent -t`` to verify that there are no warning or error
   messages related to ``mymodule``.

   * You will see a reboot notification which is expected and not an issue.
   * You may see warning/errors related to other modules that manage
     services you have not completely set up, such as ``named``. These are
     expected.
   * You may see ``svckill`` warnings about services found that would
     be killed if ``svckill::mode`` was set to 'enforcing'. You can
     address those later, after you have examined those services and
     determined which, if any, are necessary for your system.
     See :ref:`Services_dying`.

#. Verify user accesss

  a. Verify your new user can ssh into the system.
  b. Verify your new user can sudo to ``root`` by executing ``sudo su - root``

  .. WARNING::

     If your new user cannot ssh into the server and sudo to ``root``, do not
     reboot the server until you resolve the problem!

Finalize Bootstrap Operation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To finalize the bootstrap operation, reboot the server, log back in,
wait for the ``puppetserver`` service to start and accept connections
(can take over a minute), and then as ``root`` run ``puppet agent -t``.


.. _Puppet module documentation: https://puppet.com/docs/puppet/latest/modules.html

.. _Puppet metadata documentation: https://puppet.com/docs/puppet/latest/modules_metadata.html#metadatajson-example

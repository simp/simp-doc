.. _ug-prevent-lockout:

Prevent Lockout from the SIMP Server during RPM Installation
------------------------------------------------------------

By default, SIMP:

  * Disables remote logins for all users.
  * Disables ``root`` logins at the console.

If either of the following scenarios applies, you must enable ``sudo`` and
``ssh`` access for a local user. If you do not do this, you may lose access to
your system.

Scenario 1:
  Console access is available, but not allowed.  Only the ``root`` user (and
  no other user account) is available.

    * This generally occurs when SIMP is installed from RPM and the user accepts
      ``simp config``'s default value for ``useradd:securetty`` (an empty array).

Scenario 2:
  Console access is not available, and the administrative user's ``ssh``
  access has not yet been enabled permanently via Puppet.

    * This generally occurs when SIMP is installed from RPM on cloud systems.

In either of these scenarios, ``simp config`` will issue the following warning
and write a lock file to prevent ``simp bootstrap`` from running:

.. code-block:: text

  'simp bootstrap' has been locked due to potential login lockout.
  * See /root/.simp/simp_bootstrap_start_lock for details

The remainder of this document provides instructions on creating a local user
that has the appropriate level of system access.

After you address all issues identified in
``/root/.simp/simp_bootstrap_start_lock`` (see
:ref:`ug-other-bootstrap-lock-issues`), you should remove the file and
:ref:`continue with the bootstrap
process<ug-initial_server_configuration-run_bootstrap>`.


Configure Local User for Access
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This example creates a ``local_system_access`` :term:`Puppet module` in the
``production`` :term:`Puppet environment`.

* **If you already have an unprivileged account:**

   * Replace ``userx`` with your current **non-root** username throughout the
     example code.

* **If you do NOT already have an unprivileged account:**

   * Create a local user account, using ``useradd``.

     * This example assumes the local user is named ``userx``.
     * Be sure to set the user's password if the user is logging in with a password!


#. Run ``sudo su - root``

#. Set your ``umask``.

   .. code-block:: sh

      umask 022

#. Create a ``local_system_access`` puppet module directory and change to the directory.

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/production/modules
      mkdir -p local_system_access/manifests
      cd local_system_access

#. Add the following to a new ``manifests/local_user.pp`` file to enable ``sudo su - root``
   and allow ``ssh`` access for the user you created/selected:

   .. code-block:: ruby

      class local_system_access::local_user (
        Boolean $pam = simplib::lookup('simp_options::pam', { 'default_value' => false }),
      ) {

        sudo::user_specification { 'default_userx':
          user_list => ['userx'],
          runas     => 'root',
          # ONLY NEEDED IF YOUR USER DOES NOT USE A PASSWORD
          passwd    => false,
          cmnd      => ['/bin/su root', '/bin/su - root']
        }

        if $pam {
          include 'pam'

          pam::access::rule { 'allow_userx':
            users   => ['userx'],
            origins => ['ALL'],
            comment => 'Local user for lockout prevention'
          }
        }
      }

#. Add the following to a new ``metadata.json`` file to enable proper
   recognition of your module by the puppet server:

   .. code-block:: yaml

      {
        "name": "local_system_access",
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

   (See the `Puppet metadata documentation`_ for more information on metadata.json files.)

#. Make sure the permissions are correct on the module:

   .. code-block:: bash

      chown -R root:puppet $PWD
      chmod -R g+rX $PWD

#. Add the module to the SIMP server's host YAML file class list:

   .. code-block:: bash

      cd /etc/puppetlabs/code/environments/production/data/hosts

   Add ``local_system_access::local_user`` to the ``simp::classes:`` array in ``<SIMP server FQDN>.yaml``

   .. code-block:: yaml

      simp::classes:
        - local_system_access::local_user
        # Do NOT remove other items in this array
        # Make sure your whitespace lines up (spaces, not tabs)

#. Add the ``local_system_access`` module to the ``Puppetfile`` in the ``production`` environment:

   Edit ``/etc/puppetlabs/code/environments/production/Puppetfile``, and add the
   following line under the section that says *"Add your own Puppet modules here"*

   .. code-block:: ruby

      mod 'local_system_access', :local => true


If Your Local User Uses an SSH Public Key
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the local user has an SSH public key available, copy the ``authorized_keys``
file for that user to the SIMP-managed location for authorized keys
``/etc/ssh/local_keys``:

   .. code-block:: bash

      mkdir -p /etc/ssh/local_keys
      chmod 755 /etc/ssh/local_keys
      cp ~userx/.ssh/authorized_keys /etc/ssh/local_keys/userx
      chmod 644 /etc/ssh/local_keys/userx


.. _ug-other-bootstrap-lock-issues:

Resolve Other Issues in the Bootstrap Lock File
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If any other issues are identified in ``/root/.simp/simp_bootstrap_start_lock``,
you must address them before removing the file.

  * Currently, the only other issue ``simp config`` will identify is a possible
    misconfiguration of YUM repositories. ``simp config`` will lock out
    bootstrap if it cannot find a few of the key packages needed for
    bootstrapping. Fix your yum repository configuration and then verify the
    fix using the verification instructions in the lock message.

Resume Bootstrap Operation and Verify User Access
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. WARNING::

   **DO NOT REBOOT BEFORE VERIFYING USER ACCESS USING AN ALTERNATE TERMINAL OR
   SSH SESSION**

#. Remove the lock file and bootstrap the system

   * ``rm /root/.simp/simp_bootstrap_start_lock``
   * ``simp bootstrap``
   * ``puppet agent -t``

   The following items are not failures and can be ignored. All other errors or
   warnings should be addressed prior to proceeding:

     * Reboot notifications.
     * Warning/errors related to modules that manage services you have not
       completely set up, such as ``named``.
     * ``svckill`` warnings regarding services found that would be killed if
       ``svckill::mode`` was set to ``enforcing``.

       * See :ref:`Services_dying`.

#. Verify user accesss

   * Using a **NEW SSH SESSION OR TERMINAL** (do NOT close your working
     session)

     * Log in as ``userx``
     * ``sudo su - root``

  .. WARNING::

     If your new user cannot ssh into the server and sudo to ``root``

     * DO NOT reboot the server until you resolve the problem!
     * DO NOT log out of your primary work terminal unti you resolve the
       problem!

Finalization
^^^^^^^^^^^^

Reboot your system to enact the kernel-level enforcement changes:

* ``reboot``

Re-verify system access:

* Log back in as ``userx``
* ``sudo su - root``

.. _Puppet module documentation: https://puppet.com/docs/puppet/latest/modules.html
.. _Puppet metadata documentation: https://puppet.com/docs/puppet/latest/modules_metadata.html#metadatajson-example

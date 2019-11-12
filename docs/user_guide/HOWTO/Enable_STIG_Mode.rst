HOWTO Enable STIG Mode on a SIMP System
=======================================

Enabling :term:`STIG` mode in SIMP involves using the :ref:`SIMP Compliance
Engine` to apply the STIG-specific :term:`SIMP compliance profile`.

Like everything in :term:`Puppet`, STIG-mode only applies to those nodes that
actively include the correct settings.

Assumptions
-----------

#. You have a fully functional SIMP system up and running per the instructions
   in :ref:`gsg_installation_options`.

#. The :term:`FQDN` of your node is ``stig.your.domain`` and that you only want
   to enable STIG mode for this node. You can use :term:`Hiera` to enable it for
   all nodes if you so choose.

#. You are using the ``production`` :term:`Puppet Environment`

#. You have a regular user named ``stiguser`` that will be used for remote
   access to your system and for escalation of privileges to ``root``. This is
   due to the fact that, by default, SIMP does not allow remote ``root`` access.

Setting up Hiera
----------------

First, we need to create the file
``/etc/puppetlabs/code/environments/production/data/hosts/stig.your.domain.yaml``.
The next few sections describe what should be added to this file to ensure that
:term:`STIG` mode will be activated and that your user can properly login to the
system.

Allowing ``stiguser`` to login
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: yaml

   pam::access::users:
     stiguser:
       origins:
         - ALL

Allow ``stiguser`` to escalate to ``root``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: yaml

   sudo::user_specifications:
     stiguser_su:
       user_list:
         - stiguser
       cmnd:
         - ALL
       passwd: false

Configure selinux to allow ``stiguser`` to run privileged commands
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: yaml

   selinux::login_resources:
     stiguser:
       seuser: staff_u
       mls_range: "s0-s0:c0.c1023"

Place the system in STIG-enforcing mode
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Now that you have ensured that ``stiguser`` can access your system as well
as escalate to an administrative user without being blocked by SELinux, you are
ready to enable STIG-enforcing mode.

First, include the SIMP Compliance Engine backend in the hierachy defined
in the environments hiera.yml,
/etc/puppetlabs/code/environments/production/hiera.yml.
Place it under the hierachy tag just before default:

.. code-block:: yaml
   :emphasize-lines: 11,12

   ---
   version: 5
   defaults:
     datadir: data
     data_hash: yaml_data

   hierarchy:

   ...

   - name: SIMP Compliance Engine
     lookup_key: compliance_markup::enforcement

   - name: General data
     paths:
     - "default.yaml"
     - "common.yaml"

   ...

Then  add the following to the ``stig.your.domain.yaml`` file that we
have been editing:

.. code:: yaml

   compliance_markup::enforcement: disa_stig


Next Steps
----------

Applying the changes
^^^^^^^^^^^^^^^^^^^^

At this point, your system is ready to apply the STIG enforcement settings. To
begin enforcement, simply run ``puppet agent -t`` on the ``stig.your.domain``
node or wait for the next scheduled run of ``puppet``.

Escalating privileges
^^^^^^^^^^^^^^^^^^^^^

Remote system access should work as you would expect but there is a new caveat
to how you would normally run ``sudo`` to access the ``root`` account.

Since the STIG requires that all users be in an SELinux context, you will need
to ensure that all administrative users are ``staff_u`` users as we did above.

Once this is complete, you must tell ``sudo`` what context you wish to
transition into when running commands.

The simplest invocation is as follows:

.. code:: bash

   [stiguser@localhost ~]$ sudo -r unconfined_r su - root

For additional information see the `vendor documentation on confined and unconfined users`_
and/or `Dan Walsh's blog`_.

.. _Dan Walsh's blog: https://danwalsh.livejournal.com/66587.html
.. _vendor documentation on confined and unconfined users: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/selinux_users_and_administrators_guide/sect-security-enhanced_linux-targeted_policy-confined_and_unconfined_users

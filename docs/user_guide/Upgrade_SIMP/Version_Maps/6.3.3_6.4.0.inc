.. _upgrade-6.3.3-to-6.4.0:

Upgrading from SIMP-6.3.3 to SIMP-6.4.0
---------------------------------------

.. WARNING::

   You **MUST** be on at least SIMP 6.3.3 to upgrade to 6.4.0 due to issues in
   the RPMs prior to the 6.3.3 release.

   If you need to upgrade from 6.2.0 to 6.3.X, please read
   :ref:`upgrade-6.2.0-to-6.3.0` with care.

As indicated by gradual updates in previous :ref:`Changelogs`, SIMP has been
working on becoming more friendly to alternative modes of deployment and
maintenance. The team has specifically been focusing on fully supporting
:term:`r10k` and :term:`Code Manager` as methods of installation and
maintenance over time.

SIMP 6.4.0 is the release where we make this happen!

We have taken care to make no technologically breaking changes and, in keeping
with the version description in the :ref:`ug-general-upgrade-instructions`, have
not made changes that affect the API of modules provided by the supported
module set.

That said, there are quite a few procedural items that need to be taken into
account when upgrading to SIMP 6.4.0 so that things work generally like you are
used to.

To get a full accounting of the major changes in behavior for 6.4.0, please see
the :ref:`changelog-6.4.0` Changelog.

.. IMPORTANT::

   This upgrade has a great potential for deleting files you need.  Read the
   instructions carefully and back up your system prior to the upgrade.

RPM Name Changes and the Consequences
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

simp-rsync
""""""""""

``simp-rsync`` has been replaced by ``simp-rsync-skeleton``.

.. NOTE::

   Due to the nature of RPM uninstallation activities, to avoid ruining any
   active files in ``/var/simp/environments/simp/rsync``, we had to create a
   final version of ``simp-rsync`` that is generally dependency free. Nothing
   requires this RPM since it is not technically required any longer, but you
   **MUST** upgrade this RPM if you want to ensure that no files are
   accidentally removed from the ``rsync`` space on your SIMP server.

If you are not using a :term:`Puppet Environment` named ``simp`` then you can
safely remove the ``simp-rsync`` RPM after upgrading.

FakeCA
""""""

``simp-environment`` has been renamed to ``simp-environment-skeleton``.

Previously, the ``simp-environment`` RPM would place helpful ``FakeCA`` files
into ``/var/simp/environments/simp/FakeCA``. These files were for convenience
and their presence, or absence, does not affect a running system in any way.

Upon upgrade, the ``simp-environment`` RPM will be removed and you will find
that various ``FakeCA`` files are either missing or have ``.rpmsave`` appended
to them if they were modified configuration files.

You may run the following script as ``root`` to restore the ``FakeCA``
functionality:

.. code-block:: bash

   #!/bin/sh

   cd /var/simp/environments/simp
   \cp -r /usr/share/simp/environment-skeleton/secondary/FakeCA .

   cd FakeCA
   for x in *.rpmsave; do
     mv $x `basename $x .rpmsave`
   done

Upgrade Steps
^^^^^^^^^^^^^

These steps for upgrading are for use in SIMP systems that are not currently using
:term:`r10k` or :term:`Code Manager`.

Update OS and SIMP RPMS
"""""""""""""""""""""""

Update the repos for the :term:`OS` and SIMP and run ``yum update``.

The following errors/warnings are expected:

.. code-block:: bash

   warning: /var/simp/environments/simp/FakeCA/togen saved as /var/simp/environments/simp/FakeCA/togen.rpmsave
   warning: /var/simp/environments/simp/FakeCA/ca.cnf saved as /var/simp/environments/simp/FakeCA/ca.cnf.rpmsave
   libsemanage.semanage_direct_remove_key: Unable to remove module simp-environment at priority 400. (No such file or directory).
   /usr/sbin/semodule:  Failed!

.. NOTE::

   The old :term:`SELinux` module, ``simp-environment``, had to be removed
   before installing the new SELinux module, ``simp`` to avoid conflicts when
   the ``simp`` module from the ``simp-selinux-policy`` package is installed.
   Therefore, the error message above is emitted when the ``simp-environment``
   RPM tries to remove the already-removed SELinux module.

If you are using the ``FakeCA`` in the ``simp`` environment, follow the steps
in the ``FakeCA`` section above to restore the needed files.

Create the Puppetfile
"""""""""""""""""""""

At this point in the upgrade, the SIMP-packaged Puppet modules have been
installed in ``/usr/share/simp/modules`` and imported into local Git
repositories under ``/usr/share/simp/git/puppet_modules``.  Now you need to
create the ``Puppetfile`` for :term:`r10k` to use to deploy modules to the
environment.

The environment name in this example is ``myenv``.  Run the following commands
as ``root`` to create a default ``Puppetfile``:

.. code-block:: bash

   # Change to the top level puppet environment directory for your environment.
   cd /etc/puppetlabs/code/environments/myenv

   # Generate the main Puppetfile with local module entries
   simp puppetfile generate --skeleton --local-modules myenv > Puppetfile

   # Generate the Puppetfile for the SIMP modules.
   simp puppetfile generate > Puppetfile.simp

   chown root:puppet Puppetfile Puppetfile.simp
   chmod 640 Puppetfile Puppetfile.simp

Edit the main Puppetfile:

* Make sure all the local modules you created and have in the modules
  directory have ``:local => true`` entries. This includes the ``site``
  module, which is no longer delivered as an RPM.
* Delete the ``local => true`` entries for any obsolete modules that you
  do not need (e.g. ``simpcat``).

.. WARNING::

   Any module not listed in the ``Puppetfile`` will be deleted from the modules
   directory.

The following shows three examples of how to include your modules in the ``Puppetfile``:

* ``site``: a module that is not a Git repository and resides in the ``modules`` directory
* ``mymodule``: a Git repository that resides in a directory on the Puppet server
* ``mysrvmod``: a Git repository on a GitLab/GitHub type server.

.. code-block:: yaml

   mod 'site',
     :local => true

   mod 'mymodule',
     :git => 'file:///usr/share/mymodules/mymodule',
     :tag => '1.1.1'

   mod 'mysrvmod',
     :git => 'https://gitserver.my.domain/mygitproject/mysrvmod.git',
     :tag => '1.0.1'

.. IMPORTANT::

   Make sure all the  modules under your environments modules directory that
   you created and that are not SIMP modules are listed in the ``Puppetfile``.
   This includes the ``site`` module, which SIMP no longer delivers.

.. WARNING::

   Any module not listed in the ``Puppetfile`` **will be deleted** from the
   modules directory.

Deploy the Puppet Modules to the Environment
""""""""""""""""""""""""""""""""""""""""""""

Run the following commands to deploy the puppet modules to the environment:
(The environment in this example is ``myenv``.  Replace this with the name of the
environment you are updating.)

.. code-block:: bash

   # Make sure the umask is set properly

   # Set the umask and Run r10k as the puppet group to make sure the modules
   # to make sure the permissions and ownership are correct on the modules
   ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
   --puppetfile /etc/puppetlabs/code/environments/myenv/Puppetfile \
   --moduledir /etc/puppetlabs/code/environments/myenv/modules' )

Run Puppet
""""""""""

Run ``puppet agent -t`` on all your systems in the environment to apply the update.

Other Environments
""""""""""""""""""

If you have more than one environment, you need to create a Puppetfile
and deploy the modules for each environment.

Adding SIMP Extra Modules After The Upgrade
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you had modules installed from the ``simp-extras`` package, they will have
been updated with ``yum update``, a local Git repository will have been created
for them under ``/usr/share/simp/modules``, and they will
have been included in the ``Puppet.simp`` file when it was generated.

If, after the upgrade you decide to install a new SIMP module,
you must update the ``Puppetfile.simp`` file and deploy the modules again.

For example if you want to install the ``snmp`` and ``snmpd`` modules:

.. code-block:: bash

   yum install pupmod-puppet-snmp pupmod-simp-snmpd
   # After running yum you will see the modules under
   # /usr/share/simp/modules but not in your environment.

   # Now update the Puppetfile.simp and deploy the environment again.
   cd /etc/puppetlabs/code/environments/myenv
   simp puppetfile generate > Puppetfile.simp
   ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
   --puppetfile /etc/puppetlabs/code/environments/myenv/Puppetfile \
   --moduledir /etc/puppetlabs/code/environments/myenv/modules' )

If you add one of your own modules, manually edit the ``Puppetfile`` and deploy
again.  You do not need to run ``simp puppetfile generate`` with the skeleton
option again.

Remember, if you do not add the module to the ``Puppetfile``
it will get erased the next time the ``Puppetfile`` is deployed!

Upgrading If R10K Is Already Being USED
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

UNDER CONSTRUCTION


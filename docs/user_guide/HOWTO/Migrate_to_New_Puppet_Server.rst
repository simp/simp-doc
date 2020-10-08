.. _howto-migrate-to-new-puppet-server:

HOWTO Migrate to a New Puppet Server
====================================

Creating a new server and migrating clients
-------------------------------------------

The recommended method for upgrading **major** breaking changes (``X`` bump) is
to create a new Puppet Server and migrate your data and clients to it. This
process follows the path of least destruction; we will guide you through how to
back up the existing Puppet server, create a new server, and transfer your
clients.

#. Set up a new Puppet server that will house your new SIMP environment.

   .. NOTE::

      You must ensure that this node can be reached by any client that is to be
      migrated. The new system will not interfere with your existing Puppet
      system unless you specifically configure it to do so.

   .. IMPORTANT::

      Do **NOT** destroy your old Puppet server until everything has been
      successfully migrated and is in production under the new server.

#. Consider vital services other than Puppet that are housed on your current
   Puppet server node (e.g., DNS, DHCP, LDAP, custom kickstart, YUM, NFS, etc.).
   You may choose to keep many of these services running on your old Puppet
   server node. Anything not preserved must be migrated to a new system.


Back Up the Existing Puppet Server
----------------------------------

Prior to any modifications to your infrastructure, we **highly** recommend
following :ref:`ug-howto-back-up-the-puppet-master`.


Create a New Server
-------------------

Obtain an `official SIMP ISO <https://download.simp-project.com/simp/ISO/>`_ or point your
server at the latest `YUM Repositories: <https://download.simp-project.com/simp/yum/releases/latest>`_
and follow the :ref:`gsg_iso_installation_options` or
:ref:`gsg-installing_simp_from_a_repository` as appropriate.

Follow the :ref:`Client_Management` guide, and set up services as needed.
Remember, you can opt-out of any core services (DNS, DHCP, etc.)  you want your
clients or old Puppet server to run! If you want the new Puppet server to run
services the existing Puppet server ran, you may be able to use the backup of
the ``rsync`` directories from the old system.

.. WARNING::

   Do not blindly drop ``rsync`` (or other) materials from the old Puppet
   server onto the new one. The required structures for these components may
   have changed.

When you :ref:`ug-apply-certificates` you may wish to transfer client certs to
the new server.  If you are using the FakeCA and still wish to preserve the
certificates, follow the :ref:`ug-apply-certificates-official-certificates`
guidance, and treat the existing Puppet server as your 'proper CA'.


Promote the New Puppet Server and Transfer Your Clients
-------------------------------------------------------

Follow the :ref:`ug-howto-change-puppet-masters` guide to begin integration
of your new Puppet server into the existing environment.

.. NOTE::

   You should *always* start migration with a small number of
   **least critical** clients!


Retire the Old Puppet Server
----------------------------

Once you have transferred the management of all your clients over to
the new Puppet server, you may safely retire the old Puppet server.

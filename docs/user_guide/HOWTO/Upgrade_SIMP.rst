.. _ug-howto-upgrade-simp:

HOWTO Upgrade SIMP
==================

Method One: New Server Creation And Client Migration (recommended)
------------------------------------------------------------------

The most secure way to upgrade SIMP is to create a new Puppet Server and
migrate your data and clients to it.  This method mitigates multitudes of
maladies stemming from 'in-place' rpm upgrades, which suffer from potential
data clobbering, kernel corruption, pathing differences, versioning
frustration, etc.  This process follows the path of least destruction; we
will guide you through how to back up the existing Puppet server, create a
new server, and transfer your clients.

To begin this process, first consider the ramifications of adding a new
node into your environment.

* You must allocate hardware and network space. It is *highly* recommended
  that you virtualize your new Puppet server node, if possible.  Virtual
  nodes are easy to migrate and back up.  You do *not* need to sandbox the
  new Puppet server node at any point during this process.  It will not
  interfere with your existing environment, out of the box.

* Preserve your old Puppet server node.  There is no need to be destructive
  unless you absolutely need to re-claim the old node.  This gives you a
  fallback 'just in case', and allows you to migrate small numbers of clients
  over time, while retaining a security posture in the rest of your
  infrastructure.

* Consider vital services other than Puppet that are housed on your current
  Puppet server node (eg. dns, dhcp, ldap, custom kickstart, yum, nfs, etc.).
  You may wish to keep many of these services running on your old Puppet
  server node.  Anything you wish to wrap in with the new node will need
  to be re-created or migrated.

.. note::

  Creating a new Puppet server does *not* imply re-kicking your clients,
  even if some core services move to the new Puppet node.  All software
  configurations can be updated in Puppet, as needed.

Back Up The Existing Puppet Server
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prior to any modifications to your infrastructure, we *highly* recommend
following :ref:`ug-howto-back-up-the-puppet-master` (whether you are preserving
your existing Puppet server, or not!)

Create A New Server
~~~~~~~~~~~~~~~~~~~

Obtain an `official iso <https://simp-project.com/ISO/SIMP/>`_ and follow
the :ref:`simp-installation-guide`.

Follow the :ref:`Client_Management` guide, and set up services as needed.
Remember, you can opt-out of any core services (dns, dhcp, etc.)  you want
your clients or old Puppet server to run! If you want the new Puppet server
to run services the existing Puppet server ran, do yourself a favor and
utilize the rsync backup created in the previous step to rapidly deploy.

When you :ref:`ug-apply-certificates` you may wish to transfer client certs
to the new server.  If you are using FakeCA and still wish to preserve
the certificates, follow the 'Installing Official Certificates' guidance,
and treat the existing Puppet server as your 'proper CA'.

Promote The New Puppet Server and Transfer Your Clients
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Follow the :ref:`ug-howto-change-puppet-servers` guide to begin integration
of your new Puppet server into the existing environment.

.. note::

  You should *always* start migration with a small number of 'least' critical
  clients!

Method Two: In-place Upgrade Of An Existing Server
--------------------------------------------------

Coming soon!

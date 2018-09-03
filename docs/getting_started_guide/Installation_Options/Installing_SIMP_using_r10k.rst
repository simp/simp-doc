Installing SIMP Using r10k or Code Manager
==========================================

.. contents:: Contents:
  :local:

:term:`r10k` and :term:`Code Manager` are products that automate the development
and deployment of a :term:`Puppet` infrastructure. SIMP supports the usage of these
tools, with a little tweaking.

Read the introduction documentation on whichever of these technologies that is
being used:

* Code Manager: https://docs.puppet.com/pe/latest/code_mgr.html
* r10k: https://github.com/puppetlabs/r10k/blob/master/README.mkd

.. NOTE::
   r10k will be used to reference both r10k itself and Code Manager throughout
   this document. If you are using Code Manager, skip to
   `Setting Up Your Control Repo`_

.. IMPORTANT::
   This document will assume the SIMP server has internet access.
   If your system does not have internet access, you will need to adjust paths
   to point to your internal mirrors.

.. NOTE::
   This method does *not* modify your system's partitioning scheme or
   encryption scheme to meet any regulatory policies. If you want an example of
   what that should look like either see the :ref:`simp-installation-guide` or
   check out the `Kickstart`_ files in the `simp-core Git repository`_.


Preparing Your System
^^^^^^^^^^^^^^^^^^^^^

Follow the :ref:`preparing_for_non_rpm_install` guide.

Installation of r10k
^^^^^^^^^^^^^^^^^^^^

r10k is a Ruby gem that only needs to be installed on hosts running
``puppetserver``.  This gem can be installed in one of two ways:

* Gem install into the ``puppetserver`` gem set.

  .. code-block:: bash

     $ /opt/puppetlabs/puppet/bin/gem install r10k

* RPM package install of SIMP-provided r10k RPMs into ``/usr/share/simp``
  without modifying the ``puppetserver`` gem set.  These RPMs are available
  from the `official SIMP YUM repositories`_, and, beginning with SIMP 6.2.0
  the SIMP ISO.

  .. code-block:: bash

     $ yum install simp-vendored-r10k


``r10k`` can be used by calling the executable

.. code-block:: bash

   # If installed from Puppet gem
   $ /opt/puppetlabs/puppet/bin/r10k help

or

.. code-block:: bash

   # If installed from simp-vendored-r10k
   $ /usr/share/simp/bin/r10k help

.. _Setting Up Your Control Repo:

Setting Up Your Control Repository
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Follow the :ref:`howto-setup-a-simp-control-repository` guide.

Minimum Classes For Classification
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Server
------

Open Source
~~~~~~~~~~~

To manage the puppetserver, include the following classes:

* ``simp``
* ``simp::server``
* ``pupmod::master``

PE
~~

In a PE environment, The SIMP Server will normally be the Master of Masters (MoM).
Currently, Compile Masters (CMs) are not automatically supported out of the box,
and require extra configuration to ensure they remain in sync.

* ``simp``
* ``simp::server``


Agents
------

Agents will require the ``simp`` class at a minimum. SIMP ships with
'scenarios', which are essentially pre-bundled groups of modules that profile
nodes for various tasks.  See the :ref:`Classification and Data` documentation
for more information. Depending on the function of your production environment,
and your choice of scenario, you will want to populate Hiera with required
parameters.  See :ref:`gsg-advanced-configuration` for a list of base parameters and
their description.


Running Puppet For The First Time
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP doesn't configure the puppetserver to listen on the typical port and CA
port, so the first time the puppet agent is run, you may have to specify the
``ca_port`` and ``server``. An example:

.. code-block:: bash

   $ puppet agent -t --ca_port 8141 --server puppet.your.domain

SIMP also provides a provisioning script called `runpuppet`_. Run this script
during provisioning and it will (provided autosign is configured) attempt to
connect to your puppetserver as defined in ``simp_options`` and run puppet a few
times in order to get the new system in order.

.. WARNING::
   SIMP, by default, implements ``tcpwrappers`` and PAM access restrictions.
   The root user should always be able to log in at a console, but if there is no
   console, like in `AWS`_, be sure to add a user to the PAM whitelist and give
   it sudo powers:

   .. code-block:: puppet

      pam::access::rule { 'ec2user':
        origins    => ['ALL'],
        permission => '+',
        users      => ['ec2user']
      }
      sudo::user_specification { 'ec2user':
        user_list => ['ec2user'],
        cmnd      => ['ALL']
      }

  SIMP also moves the location of the ssh authorized_keys file to
  ``/etc/ssh/local_keys/%u``, so copy it there before logging out.


Notes About SIMP Infrastructure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP, when installed from the ISO, moves packages into ``/var/www/yum`` and
creates a ``yum`` repo in itself. SIMP modules, notably the ``simp::yum`` class,
assumes this. You will have to set ``simp::yum::os_update_url`` to a CentOS
Updates URL.


.. _official SIMP YUM repositories: https://packagecloud.io/simp-project
.. _AWS: https://aws.amazon.com/
.. _documentation of a control repo online: https://docs.puppet.com/pe/latest/cmgmt_control_repo.html
.. _Kickstart: http://pykickstart.readthedocs.io/en/latest
.. _runpuppet: https://github.com/simp/pupmod-simp-simp/blob/master/manifests/server/kickstart/runpuppet.pp
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/CentOS/7/x86_64/DVD/ks
.. _simp-core: https://github.com/simp/simp-core/

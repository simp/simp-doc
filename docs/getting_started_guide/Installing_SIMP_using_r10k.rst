Installing SIMP using r10k or Code Manager
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
  this document. If you are using Code Manager, skip to `Setting up your Control Repo`_

.. IMPORTANT::
  This document will assumes the SIMP server has internet access.
  If your system does not have Internet access, you will need to adjust any paths
  to point to your internal mirrors.

.. NOTE::
   This method does *not* modify your system's partitioning scheme or
   encryption scheme to meet any regulatory policies. If you want an example of
   what that should look like either see the :ref:`simp-installation-guide` or
   check out the `Kickstart`_ files in the `simp-core Git repository`_.

Installation of r10k
^^^^^^^^^^^^^^^^^^^^

On the system intended to be the Puppet server, run the following command to
install the ``r10k`` ruby gem into the vendor ruby that comes with the
```puppet-agent`` AIO package:

.. code-block:: bash

  $ /opt/puppetlabs/puppet/bin/gem install r10k

``r10k`` can be used by calling the absolute path of the executable (unless
added to ``$PATH``):

.. code-block:: bash

  $ /opt/puppetlabs/puppet/bin/r10k help


Setting up your Control Repo
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP uses a :term:`Puppetfile` and distributes some of a :term:`Control Repo` in
the ``simp-environment`` package, or also in the `simp-core`_ repository.:

On the filesystem of an installed SIMP system:

.. code-block:: bash

  $ tree -L 1 /usr/share/simp/environments/simp/
  /usr/share/simp/environments/simp/
  ├── environment.conf
  ├── FakeCA
  ├── hieradata
  ├── manifests
  └── modules

Our git repo:

.. code-block:: bash

  $ tree -L 1 src/assets/simp-environment/environments/simp
  src/assets/simp-environment/environments/simp
  ├── environment.conf
  ├── FakeCA/
  ├── hieradata/
  └── manifests/

A Control Repo contains the modules, hieradata, and roles/profiles required for
an infrastructure, and keeping it all in a git repo creates a workflow for
updating and developing on your Puppet infrastructure.

The modules are defined in a :term:`Puppetfile`, which SIMP uses as a development
tool. We keep an up-to-date Puppetfile in the core of our repo, which you can
download using this snippet:

.. code-block:: bash

  $ curl -o Puppetfile https://github.com/simp/simp-core/blob/<release>/Puppetfile.stable

The example Puppetfile is labeled *stable*, meaning that the versions of the
modules it contains are the ones contained in the last SIMP release. You can go
to any previous release and download a Puppetfile with references to older
modules from the git history of the ``simp-core`` repo.

Our Puppetfile pulls down every dependency SIMP needs, which is currently
contains more than just modules. Open up the ``Puppetfile`` that was just
downloaded and remove the lines from ``moduledir 'src'`` to
``moduledir 'src/puppet/modules'``. That should just leave the Puppet modules.

Change into the directory you're using from above and run ``git init`` to create
a control repo. Puppet, Inc has some great `documentation of a control repo online`_.

Keydist, Rsync, and the Alternate Module Path
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP uses an alternative module path, located at ``/var/simp/environments/<environment>``
and set in each environment's ``environment.conf``.
Currently, it contains the rsync assets and PKI data, custom to each host. Here
is an example from a fresh install:

.. code-block:: bash

  $ tree -L 1 /var/simp/environments/production
  /var/simp/environments/production
  ├── rsync
  └── site_files

* ``rsync`` is a tree that stores data that will be copied over to modules. We
  have chosen rsync for these applications because of how it handles large files
  and large amounts of files. See :ref:`rsync_justification`.
* ``site_files`` is a place to store private files that may not belong in the
  control repo or another data source. And example of this would be host-based
  x509 certificates (which are used heavily by SIMP).

Each of these directories need to be created manually, except rsync, which
requires a bit more work:

.. code-block:: bash

  $ mkdir -p /var/simp/environments/production/{site_files/modules/pki_files/files/keydist}
  $ chown root.puppet /var/simp/environments/<environment>/site_files
  $ chmod -R g+rX /var/simp/environments/<environment>/{site_files,simp_autofiles}

The rsync folder has a few tricks. When installed from an ISO, this data and
folder structure is laid out by the ``simp-rsync`` rpm. When installing using the
method described in this document, the git repo will have to be cloned and
manipulated:

.. code-block:: bash

  $ git clone https://github.com/simp/simp-rsync.git /tmp/simp-rsync
  $ mv -f /tmp/simp-rsync/environments/simp/rsync /var/simp/environments/<environment>/
  $ ln -s /var/simp/environments/<environment>/rsync/RedHat /var/simp/environments/<environment>/rsync/CentOS
  $ chmod u+rwx,g+rX,o+rX /var/simp{,/environments,/environments/production}

Rsync's primary use case in a base SIMP infrastructure is to distribute ClamAV
databases. If ``simp_options::clamav`` is set to true, rsync and this step are
required.

Install ``clamav-update`` and download the latest database using the following
config and commands, replacing <environment> with your environment.

.. code-block:: bash

  $ cat << EOF > /tmp/freshclam.conf
  DatabaseDirectory /var/simp/environments/<environment>/rsync/Global/clamav
  DatabaseMirror database.clamav.net
  Bytecode yes
  EOF


.. code-block:: bash

  $ yum install -y clamav-update
  $ freshclam -u root --config-file=/tmp/freshclam.conf

Be careful when copying the first rsync environment around. There are hidden
files in each folder that represents and rsync share called ``.shares``. There
is a fact in the ``simp`` modules that checks for that files. The fact is then
ingested by ``simp::server::rsync_shares`` and rsync shares are created on the
Puppet server.


Minimum classes for classification
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


Server
------

Open Source
~~~~~~~~~~~

Because SIMP will manage the puppetserver, just include the following classes:

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

Other agent will require the ``simp`` class or profile at a minimum. Other
classes or profiles may be included on top of this baseline for the desired
functionality. Check the ``simp`` scenario for a full list of classes.


Running Puppet for the first time
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
  ``/etc/ssh/local_keys/%u``, so copy it there bofore logging out.


Notes about SIMP Infrastructure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP, when installed from the ISO, moves packages into ``/var/www/yum`` and
creates a ``yum`` repo in itself. SIMP modules, notably the ``simp::yum`` class,
assumes this. You will have to set ``simp::yum::os_update_url`` to a CentOS
Updates URL.


.. _AWS: https://aws.amazon.com/
.. _documentation of a control repo online: https://docs.puppet.com/pe/latest/cmgmt_control_repo.html
.. _Kickstart: http://pykickstart.readthedocs.io/en/latest
.. _runpuppet: https://github.com/simp/pupmod-simp-simp/blob/master/manifests/server/kickstart/runpuppet.pp
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/CentOS/7/x86_64/DVD/ks
.. _simp-core: https://github.com/simp/simp-core/

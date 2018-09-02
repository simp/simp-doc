.. _preparing_for_non_rpm_install:

Preparing For Non-RPM Install
=============================

Keydist, Rsync, and The Alternate Module Path
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP uses an alternative module path, ``/var/simp/environments/<environment>/``
, which is set in each environment`s ``environment.conf``.  Currently, it
contains rsync assets and PKI data, custom to each host.  Here is an example
from a fresh install:

.. code-block:: bash

  $ tree -L 1 /var/simp/environments/production
  /var/simp/environments/production
  ├── rsync
  └── site_files

* ``rsync`` is a tree that stores data that will be copied over to modules.  We
  have chosen rsync for these applications because of how it handles large
  files and large amounts of files.  See :ref:`rsync_justification`
* ``site_files`` is a place to store private files that may not belong in the
  control repo or another data source.  An example of this would be host-based
  x509 certificates (which are used heavily by SIMP).

Create the ``site_files``, ``simp_autofiles``, and ``keydist`` directories:

.. code-block:: bash

  $ mkdir -p /var/simp/environments/production/{site_files/modules/pki_files/files/keydist}
  $ chown root.puppet /var/simp/environments/<environment>/site_files
  $ chmod -R g+rX /var/simp/environments/<environment>/{site_files,simp_autofiles}

The rsync directory is special.  When installed from an ISO via RPM, the rsync
data and folder structure is laid out in a particular manner.  Clone the rsync
repository and modify it to make it equivalent to RPM install:

.. code-block:: bash

  $ git clone https://github.com/simp/simp-rsync.git /tmp/simp-rsync
  $ mv -f /tmp/simp-rsync/environments/simp/rsync /var/simp/environments/<environment>/
  $ ln -s /var/simp/environments/<environment>/rsync/RedHat /var/simp/environments/<environment>/rsync/CentOS
  $ chmod u+rwx,g+rX,o+rX /var/simp{,/environments,/environments/production}

.. warning::

  Be careful when copying the first rsync environment around.  There are hidden
  files in each folder, including rsync .shares files.  There is a fact in the
  simp module that checks for those files.  The fact is ingested by
  ``simp::server::rsync_shares`` and rsync shares are created on the Puppet
  server.

If ``simp_options::clamav`` is set to true, the following step is required,
otherwise you can skip it.

Install clamav-update and download the latest database using the following
config and commands, replacing ``<environment>`` with your environment.

.. code-block:: bash

  $ cat << EOF > /tmp/freshclam.conf
  DatabaseDirectory /var/simp/environments/<environment>/rsync/Global/clamav
  DatabaseMirror database.clamav.net
  Bytecode yes
  EOF

.. code-block:: bash

  $ yum install -y clamav-update
  $ freshclam -u root --config-file=/tmp/freshclam.conf


Other Miscellany
^^^^^^^^^^^^^^^^

You may need to bring in the SIMP dependencies repository:

.. code-block:: bash

  $ curl -s https://packagecloud.io/install/repositories/simp-project/6_X_Dependencies/script.rpm.sh.rpm | bash

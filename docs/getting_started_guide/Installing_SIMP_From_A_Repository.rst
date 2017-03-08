.. _gsg-installing_simp_from_a_repository:

Installing SIMP From A Repository
=================================

Using the `official SIMP YUM repositories`_ is the simplest method for getting
up and running with a SIMP system. If you are using a virtual infrastructure,
such as `AWS`_, `Microsoft Azure`_, `Google Cloud`_, or your own internal VM
stack, this is the method that you will almost definitely want to use.

.. NOTE::
   This method does *not* modify your system's partitioning scheme or
   encryption scheme to meet any regulatory policies. If you want an example of
   what that should look like either see the :ref:`simp-installation-guide` or
   check out the `Kickstart`_ files in the `simp-core Git repository`_.


Enable EPEL
-----------

.. code-block:: bash

   $ sudo yum install epel-release -y
   $ sudo yum install pygpgme yum-utils

Install The SIMP-Project Repository
-----------------------------------

Add the following to ``/etc/yum.repos.d/simp-project.repo``, replacing 
``6`` with the appropriate version of SIMP. If the repo file does not exist,
create it. The repo file contents for ``SIMP 6.X`` is shown below.

If you don't know what versions map together, please see the
:ref:`faq-simp_version_guide`.

.. code-block:: bash

  [simp-project_6_X]
  name=simp-project_6_X
  baseurl=https://packagecloud.io/simp-project/6_X/el/$releasever/$basearch
  gpgcheck=1
  enabled=1
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

Install The SIMP-project_dependencies Repository
------------------------------------------------

.. NOTE::
   The repository may contain items from external vendors, most notably Puppet,
   Inc. and EPEL but may also contain non-SIMP project files that have been
   compiled for distribution.

Add the following to ``/etc/yum.repos.d/simp-project_dependencies.repo``,
replacing ``6`` with the appropriate version of SIMP. If the repo file
does not exist, create it. The repo file for ``SIMP 6.X`` is shown below.

If you don't know what versions map together, please see the
:ref:`faq-simp_version_guide`.

.. NOTE::
   The **whitespace** and **alignment** shown before the additional ``gpgkey``
   values **must be preserved**

.. code-block:: bash

  [simp-project_6_X_dependencies]
  name=simp-project_6_X_dependencies
  baseurl=https://packagecloud.io/simp-project/6_X_Dependencies/el/$releasever/$basearch
  gpgcheck=1
  enabled=1
  gpgkey=https://raw.githubusercontent.com/NationalSecurityAgency/SIMP/master/GPGKEYS/RPM-GPG-KEY-SIMP
         https://yum.puppetlabs.com/RPM-GPG-KEY-puppetlabs
         https://yum.puppetlabs.com/RPM-GPG-KEY-puppet
         https://apt.postgresql.org/pub/repos/yum/RPM-GPG-KEY-PGDG-94
         https://getfedora.org/static/352C64E5.txt
  sslverify=1
  sslcacert=/etc/pki/tls/certs/ca-bundle.crt
  metadata_expire=300

Rebuild The Yum Cache
---------------------

.. code-block:: bash

   $ sudo yum makecache

Install the SIMP Server
-----------------------

1. Select the simp-adapter package appropriate for the version of Puppet
   you will be using

   * **simp-adapter-foss**:  Version appropriate for FOSS Puppet
   * **simp-adapter-pe**:   Version appropriate for Puppet Enterprise

2. Install the simp-adapter package

.. code-block:: bash

   $ sudo yum install -y simp-adapter-foss

3. Install the remaining SIMP packages

.. code-block:: bash

   $ sudo yum install -y simp

.. NOTE::
   The ``simp`` RPM installs the SIMP core Puppet modules. Breaking changes in
   these modules trigger a breaking change update in SIMP itself.

   There are a large number of additional 'extra' modules that may be
   individually installed. Search for ``pupmod`` via ``yum`` to discover what
   is available.

   If you wish to install all of the extra modules, you can simply run ``sudo
   yum install -y simp-extras``

Configure and Bootstrap the SIMP Server
---------------------------------------

1. Login as root
2. Type ``simp config`` and configure the system as prompted.

  * ``simp config`` will prompt you for system settings and then apply the
    smallest settings subset that is required to bootstrap the system.
  * When applicable, ``simp config`` will present you with a
    recommendation for each setting (variable).  To keep a recommended
    value, press *Enter*. Otherwise, enter your desired value.
  * ``simp config``  generates a log file containing details of the
    configuration selected and actions taken.
  * For more details about the installation variables set by ``simp config``
    and the corresponding actions, see :ref:`Initial_Configuration`.
  * For a list of additional options, type ``simp help config``.

    * ``simp config --dry-run`` will run through all of the ``simp config``
      prompts without applying any changes to the system. This is the
      option to run to become familiar with the variables set by
      ``simp config`` or generate a configuration file to be used as
      a template for subsequent ``simp config`` runs.
    * ``simp config -a <Config File>`` will load a previously generated
      configuration in lieu of prompting for settings, and then apply the
      settings.  This is the option to run for systems that will be rebuilt
      often.

.. NOTE::
   Once ``simp config`` has been run, three SIMP configuration files will be
   generated:

   * ``/root/.simp/simp_conf.yaml``: File containing  all your ``simp config``
     settings; can include additional settings related to ones you entered and
     other settings required for SIMP.
   * ``/etc/puppetlabs/code/environments/simp/hieradata/simp_config_settings.yaml``:
     File containing global hieradata relevant to SIMP clients and the SIMP
     server.
   * ``/etc/puppetlabs/code/environments/simp/hieradata/hosts/<host>.yaml``:
     SIMP server host YAML file.

3. Type ``simp bootstrap``

.. NOTE::
   If progress bars are of equal length and the bootstrap finishes quickly, a
   problem has occurred. This is most likely due to an error in SIMP
   configuration. Refer to the previous step and make sure that all
   configuration options are correct.

4. Reboot your system

.. code-block:: bash

   $ reboot

Bootstrap SIMP Clients
----------------------

Use the ``runpuppet`` script from the newly created SIMP server to bootstrap
your clients. That script can be aquired in one of two ways:

1. Use a SIMP server as a kickstart server, see :ref:`Client_Management` for
   details on how to take advantage of SIMP to make this easier.

2. If another server is to be used as a kickstart server, you can still use our
   distributed and tested provisioning script, ``runpuppet``.

   Add the ``simp::server::kickstart::runpuppet`` class to you kickstart server
   node to use ``runpuppet``. The file can be placed in an exising web server by
   setting the ``location`` parameter. Here's an example that could be placed
   in a kickstarting profile class:

   .. code-block:: puppet

     class { 'simp::server::kickstart::runpuppet':
       location => '/var/www/web/server/path/runpuppet'
     }

.. NOTE::
   This would be the general technique that you would use to auto-bootstrap
   your clients via ``user-data`` scripts in cloud environments.

   Be ready to sign your client credentials as systems check in with the
   server!

Run the script on a client. This example assumes the first option from above:

.. code-block:: bash

   $ curl http://<puppet.server.fqdn>/ks/runpuppet | bash

.. _official SIMP YUM repositories: https://packagecloud.io/simp-project
.. _AWS: https://aws.amazon.com/
.. _Microsoft Azure: https://azure.microsoft.com
.. _Google Cloud: https://cloud.google.com
.. _Kickstart: http://pykickstart.readthedocs.io/en/latest
.. _simp-core Git repository: https://github.com/simp/simp-core/tree/master/build/distributions/CentOS/7/x86_64/DVD/ks

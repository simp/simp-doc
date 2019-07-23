Pre-Release Checklist
=====================

The bulk of the work to release both :term:`EL` 6 and :term:`EL` 7 versions of
a SIMP ISO is to verify that each ISO is ready for release. Below is
the list of verifications that must be executed **for each ISO**, before
proceeding with the release of that ISO. If any of these steps fail,
the problem identified must be fixed before you can proceed with the tag
and release steps.

.. contents:: :local:

Update Policy Evaluation Response Reports
-----------------------------------------

Since one of the main goals of SIMP is to assist with compliance of various
standards, we should add a response to the latest security scans that we use.

Given that most scanners are only one view on the world and often are not
flexible enough to meet all possible solutions to a given policy, it is
expected that there will be explanations of both false positives as well as
helpful material on why the SIMP framework is compliant for the benefit of our
users.

These scans should be added, as applicable, to the
:ref:`security-conop-evaluation-artifacts` section of the documentation.

Verify RPMs are available in PackageCloud
-----------------------------------------

This check is to verify that all artifacts used to create the ISO
exist as signed RPMs in `PackageCloud`_.  This will include:

* SIMP-owned Puppet modules
* Other Puppet modules
* SIMP utility RPMs (``rubygem-simp-cli``, ``simp-adapter``, ``simp-utils``,
  etc.)
* ``simp-doc``
* SIMP application RPMs
* External vendor application RPMs
* OS RPMs

For nearly all the projects listed in ``Puppetfile.tracking``, you can verify that
the RPMs for those projects exist by executing the ``pkg:check_published`` Rake command:

#. Checkout the ``simp-core`` project.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git
      cd simp-core

#. Verify the ``Puppetfile.tracking`` file contains the component tags
   for the release.

#. Execute the ``pkg:check_published`` Rake command

   .. code-block:: bash

      bundle exec rake pkg:check_published > check_published.out

#. Examine the ``check_published.out`` content to verify that, except
   for the ``simp-doc`` project, no projects lists
   ``RPM Publish Required:`` or ``Git Release Tag Required:``.  What
   you should see are lines such as::

     ...
     Found Existing Remote RPM: pupmod-simp-stunnel-6.1.0-0.noarch.rpm
     Found Existing Remote RPM: pupmod-simp-sudo-5.0.3-0.noarch.rpm
     ...

   .. IMPORTANT::

      If you see a message like
      ``Warning:  Unable to generate build-specific YUM cache``, your
      results are invalid, as connection to `PackageCloud`_ failed.

#. Manually verify the appropriate ``simp-doc`` RPM exists at `PackageCloud`_.


For the external vendor RPMs
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Upload all vendor RPMs to the ``VERSION_Dependencies`` repository in
  `PackageCloud`_. Any existing RPMs will not be overwritten.

  * ``package_cloud push simp-project/VERSION_Dependencies/el/OS_MAJOR_VERSION /path/to/packages``

.. WARNING::

   **DO NOT** push any Core Operating System RPMs up to PackageCloud, those
   should be retrieved from official vendor sources.


Verify a valid Puppetfile exists
--------------------------------

This check is to verify that that ``Puppetfile.tracking`` file for the
``simp-core`` project is complete and accurate:

* It includes all the SIMP-owned Puppet modules, other Puppet modules
  that are dependencies of SIMP-owned Puppet modules, and utilities
  to configure the SIMP system when installed from ISO.

* The URL for each artifact corresponds to the tag for its signed,
  published RPM.

Verify the Changelog.rst
------------------------

This check is to verify that the ``simp-core`` Changelog.rst has
been updated:

* Manually inspect

Verify the dependencies.yaml
----------------------------

This check is to verify that ``simp-core/build/rpm/dependencies.yaml``
contains the correct adjustments to the RPM dependencies, obsoletes,
requires, and/or release fields for any of the components listed
in the ``Puppetfile.tracking`` file.

Manually inspect the file to verify there are entries for

* All non-SIMP Puppet modules that have more dependencies listed in
  their ``metadata.json`` files than are actually required on a SIMP
  system. Each entry must list all the relevant dependencies in a
  ``:requires`` element.
* Any component that has changed name (e.g. ``pupmod-saz-timezone``
  changing to ``pupmod-simp-timezone``). Each entry must list the
  package and version obsoleted in an ``:obsoletes`` element.
* Any component for which for which the RPM release field must be
  specified (e.g. a component with a RPM-packaging-only change).
  Each entry must list a ``:requires`` element.

Verify the simp-core RPMs can be created
----------------------------------------

This check verifies that an RPM can be generated for ``simp-core``:

.. code-block:: bash

   git clone https://github.com/simp/simp-core.git
   cd simp-core/src/assets/simp
   bundle update
   bundle exec rake pkg:rpm

.. NOTE::

   This command will build the RPM for the OS of the server
   on which it was executed.

Verify simp-core tests pass
---------------------------

This check verifies that the ``simp-core`` unit and acceptance test
have succeeded.

To verify that the ``simp-core`` unit tests have succeeded, examine
the test results in `TravisCI`_.

   * Navigate to the project's TravisCI results page and verify the
     tests for the development branch to be tagged and released have
     passed.  For our project, this page is
     https://travis-ci.org/simp/simp-core/branches

     .. IMPORTANT::

        If the tests in TravisCI fail, you **must** fix them before
        proceeding.  The automated release procedures will only
        succeed, if the unit tests succeed in TravisCI.

To verify that the ``simp-core`` acceptance tests have succeeded

#. Checkout the ``simp-core`` project for the last SIMP release.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git
      cd simp-core

#. Run the default ``simp-core`` acceptance tests

   .. code-block:: bash

       bundle update
       bundle exec rake beaker:suites

.. NOTE::

   If the GitLab instance for ``simp-core`` is current (it is sync'd
   every 3 hours), you can look at the latest acceptance test results
   run by GitLab, instead.  The results will be at
   https://gitlab.com/simp/simp-core/pipelines.


Verify ISOs can be created
--------------------------

This check verifies that SIMP ISOs for CentOS 6 and CentOS 7 can be
built from the local ``simp-core`` clone  and RPMs pushed to PackageCloud.
For CentOS 6 and CentOS 7:

#. Login to a machine that has `Docker`_ installed and the ``docker``
   service running.

   .. IMPORTANT::

      In our development environment, the version of Docker
      that is available with CentOS works best.

#. Checkout the ``simp-core`` project for the last SIMP release.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git
      cd simp-core
#. Populate ``simp-core/ISO`` directory with CentOS 6/7 distribution ISOs

   .. code-block:: bash

      mkdir ISO
      cp /net/ISO/Distribution_ISOs/CentOS-6.9-x86_64-bin-DVD*.iso ISO/
      cp /net/ISO/Distribution_ISOs/CentOS-7-x86_64-1708.iso ISO/

#. Build each ISO for CentOS 6 and CentOS 7.  For example,

   .. code-block:: bash

      bundle update
      SIMP_BUILD_docs=no \
      SIMP_BUILD_verbose=yes \
      SIMP_PKG_verbose=yes \
      bundle exec rake beaker:suites[rpm_docker]

   .. IMPORTANT::

      #. By default, the ``default.yml`` for the ``rpm_docker`` suite
         builds an ISO for CentOS 7.  You must manually edit the
         ``default.yml`` file to disable the ``el7-build-server``
         instead of the ``el6-build-server``, in order to create
         a CentOS 6 ISO.

      #. The most reliable way to build each ISO is from a clean checkout
         of ``simp-core``.

#. Verify none of the RPMs in the ISO that SIMP would have generated
   are signed by the SIMP development GPG key. For example, for a
   CentOS 7 build:

   .. code-block:: bash

      cd build/distributions/CentOS/7/x86_64/SIMP/RPMS/noarch

      # The 7da6f216 key ID may change as the SIMP signing keys get updated over time
      # The output of this command should be *EMPTY*
      rpm -q --qf '%{NAME}-%{VERSION}-%{RELEASE} %{SIGPGP:pgpsig} %{SIGGPG:pgpsig}\n' -p * | grep -v 7da6f216

Verify SIMP ISO boot options work
---------------------------------

This hefty check verifies that a server booted from the SIMP ISO can
be bootstrapped for the 'simp' scenario and following boot options:

* Using default boot option
* Using disk encryption boot option
* Using FIPS disabled boot option
* Using disk encryption and FIPS disabled boot options
* Using simp-prompt option
* Using simp-prompt and disk encryption boot options
* Using simp-prompt and FIPS disabled boot options
* Using simp-prompt, disk encryption, and FIPS disabled boot options
* Using linux-min boot option
* Using linux-min and disk encryption boot options
* Using linux-min and FIPS disabled boot options
* Using linux-min, disk encryption, and FIPS disabled boot options

For the default boot options with/without encryption and the FIPS
disabled boot option with/without encryption test cases, the
`simp-packer`_ project is the easiest way to verify a SIMP VM can be
booted from the ISO and bootstrapped.  Otherwise, the check has to be done
manually:

* Boot a VM with the SIMP ISO
* Select the appropriate boot options
* Once the server boots, login to the server as root
* Bootstrap the system

  .. code-block:: bash

     simp config
     simp bootstrap
     reboot

* Login to the server as root and run ``puppet agent -t`` until the
  results are stable
* Verify the server is/is not in FIPS mode by inspecting `/proc/sys/crypto/fips_enabled`
* Verify the appropriate disk is/is not encrypted by executing

  .. code-block:: bash

     blkid

* Verify the appropriate disk partitioning

  .. code-block:: bash

     lsblk

.. IMPORTANT::

   For the ``linux-min`` test cases, the only verification required is
   verification that the server boots up.

Verify component interoperability
---------------------------------

This check verifies, with ``simp-core`` and ``pupmod-simp-simp``
acceptance tests, that this aggregation of module versions interoperate.
(These tests provide extensive, cross-component, integration tests.)

.. NOTE::
   If ``simp-core`` and ``pupmod-simp-simp`` acceptance tests have
   effectively already passed on one of our continuous integration
   platforms (e.g., in GitLab), you can skip this painful step.
   However, you must be sure that the tests were run with the correct
   component versions.

#. Checkout the ``simp-core`` project.

   .. code-block:: bash

      git clone https://github.com/simp/simp-core.git
      cd simp-core

#. Verify the ``Puppetfile.tracking`` file contains the component tags
   for the release.

#. Run the default ``simp-core`` acceptance tests

   .. code-block:: bash

       bundle update
       bundle exec rake beaker:suites

#. Checkout the version of ``pupmod-simp-simp`` corresponding to this
   ``simp-core`` version

   .. code-block:: bash

       bundle exec rake deps:checkout
       cd src/puppet/modules/pupmod-simp-simp

#. Create a ``.fixtures.yml`` file that sets the version of
   each dependency to the version contained in the
   ``Puppetfile.tracking`` file for this ISO release.

#. Run **all** the functioning acceptance tests with and without FIPS
   mode enabled

   .. code-block:: bash

      bundle update

      BEAKER_fips=yes bundle exec rake beaker:suites
      bundle exec rake beaker:suites

      BEAKER_fips=yes bundle exec rake beaker:suites[base_apps]
      bundle exec rake beaker:suites[base_apps]

      BEAKER_fips=yes bundle exec rake beaker:suites[no_simp_server]
      bundle exec rake beaker:suites[no_simp_server]

      BEAKER_fips=yes bundle exec rake beaker:suites[scenario_one_shot]
      bundle exec rake beaker:suites[scenario_one_shot]

      BEAKER_fips=yes bundle exec rake beaker:suites[scenario_poss]
      bundle exec rake beaker:suites[scenario_poss]

      BEAKER_fips=yes bundle exec rake beaker:suites[scenario_remote_access]
      bundle exec rake beaker:suites[scenario_remote_access]

Verify otherwise untested capabilities
--------------------------------------
This check verifies that all other major capabilities (not otherwise
tested in acceptance/simp-packer tests) do function as advertised:

.. todo:: Detailed test procedures need to be included in this section

.. NOTE::

   In order to speed time to market, the goal is to automate as many of
   these manual tests as possible!

* A SIMP client can be PXE booted using the kickstart files from the
  SIMP ISO
* A SIMP client can use the SIMP server for DNS
* A 'simp_lite' client operates with a SIMP server

  - login operations (PAM, LDAP, local user)
  - NFS operations (home directory)
  - logging operations (rsyslog)
  - auditing operations

* A 'simp_poss' client operates with a SIMP server
* The SIMP server can be converted from FIPS enabled to FIPS
  disabled mode.
* The SIMP server can be converted from Selinux enforcing to Selinux
  permissive.
* The SIMP server can be converted from Selinux permissive to Selinux
  enforcing.
* A local user with sudo privileges can be created and login to both
  the SIMP server and client on CentOS 6 and CentOS 7.
* An LDAP user user in the ``administrators`` group can login to both
  the SIMP server and client on CentOS 6 and CentOS 7.
* Local and LDAP users can change their passwords on both the SIMP
  server and client on CentOS 6 and CentOS 7.
* The Rsyslog rules from ``simp_rsyslog``, ``syslog`` and
  SIMP application modules (``aide``, ``tlog``, etc.) result
  in application log messages being written to the correct local
  and remote log files.

  .. NOTE::

     Although the ``simp_rsyslog`` and ``syslog`` modules have
     excellent acceptance tests, neither has a full-system test
     to verify integration with actual log producers.  The tests
     for these modules use ``logger`` as a mock message sender.

* The compliance map reports for a full SIMP system are accurate.

  - No reports list non-compliant configuration that is really a
    parameter mismatches. (Parameter tested differs from parameter
    that should have been tested; value tested differs from actual
    values allowed, etc.)
  - SIMP server and SIMP client reports are generated.

* ``simp-utils`` executables that are not tested otherwise work as
  advertised

  - ``unpack_dvd``
  - ``gen_ldap_update``
  - ``updaterepos``

* The :ref:`howto-guides` are still correct.

Verify SIMP server RPM install
------------------------------

This check verifies that CentOS 6 and CentOS 7 SIMP servers can be
installed using the set of RPMs contained in the SIMP ISOs
The verification steps largely follow the details in
:ref:`gsg-installing_simp_from_a_repository`.  All RPMs except
the ``simp-core`` RPM should be able to be pulled from `PackageCloud`_.

Verify SIMP server RPM upgrade
------------------------------

This check verifies that the set of RPMs in the SIMP ISO can upgrade
the last full SIMP release.

#. Bring up a CentOS server that was booted from the appropriate SIMP
   ISO and for which ``simp config`` and ``simp bootstrap`` has been
   run.

   .. NOTE::

      If the VirtualBox for the last SIMP ISO was created by the
      `simp-packer`_ project, you can simply setup the appropriate
      VirtualBox network for that box and then bring up that
      bootstrapped image with ``vagrant up``.

#. Copy the SIMP and system RPMs packaged in the SIMP ISO to the
   server and install with yum.

   - FIXME Should put RPMs into appropriate updates repos, run
     something like the following

     .. code-block:: bash

        cd <updates dir>
        createrepo .
        chown -R root.apache ./*
        find . -type f -exec chmod 640 {} \;
        find . -type d -exec chmod 750 {} \;
        yum clean all;
        yum make cache
        yum update

#. Verify ``puppet agent -t`` runs cleanly
#. Verify no custom content is removed by the upgrade
   (e.g., ``environments/production/modules/site/manifests``, content in
   ``environments/production/data``)

Verify SIMP server R10K install
-------------------------------

This check verifies that CentOS 6 and CentOS 7 SIMP servers can be
installed via :term:`r10k`.  Since this capability is already automatically
tested in a ``simp-core`` acceptance test, all verification is handled by
`Verify simp-core tests pass`_.


.. _Docker: https://www.docker.com
.. _GitHub: https://github.com
.. _PackageCloud: https://packagecloud.io/simp-project
.. _TravisCI: https://travis-ci.org
.. _simp-packer: https://github.com/simp/simp-packer
.. _simp-project: http://download.simp-project.com/simp/ISO

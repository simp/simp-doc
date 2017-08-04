Pre-Release Checklist
=====================

The bulk of the work to release both CentOS 6 and CentOS 7 versions of
a SIMP ISO is to verify that each ISO is ready for release. Below is
the list of verifications that must be executed *for each ISO* before
proceeding with the release of that ISO. If any of these steps fail,
the problem identified must be fixed befor you can proceed with the tag
and release steps.

* `Verify RPMs are available in packagecloud`_
* `Verify a valid Puppetfile.stable exists`_
* `Verify the Changelog.rst`_
* `Verify the simp-core RPMs can be created`_
* `Verify simp-core unit tests pass`_
* `Verify ISOs can be created`_
* `Verify SIMP ISO boot options work`_
* `Verify component interoperability`_
* `Verify otherwise untested capabilities`_
* `Verify SIMP server RPM install`_
* `Verify SIMP server RPM upgrade`_

Verify RPMs are available in packagecloud
-----------------------------------------

This check is to verify that all artifacts used to create the ISO
exist as signed RPMs in `packagecloud`_.   This will include:

* SIMP-owned Puppet modules
* Forked Puppet modules
* Utility RPMs (``rubygem-simp-cli``, ``simp-adapter``, ``simp-utils``,
  etc.)
* ``simp-doc``
* OS RPMs?

Procedure:
FILL-ME-IN (Has the pull of the RPMs using ``Puppetfile.stable`` already
been updated in Trevor's build script?)

Verify a valid Puppetfile.stable exists
---------------------------------------

This check is to verify that that ``Puppetfile.stable`` file for the
``simp-core`` project is complete and accurate:

* It includes all the SIMP-owed Puppet modules, other Puppet modules
  that are depenencies of SIMP-owned Puppet modules, and utilities
  to configure the SIMP system when installed from ISO.

* The URL for each artifact corresponds to the tag for its signed,
  published RPM.

Verify the Changelog.rst
------------------------

This check is to verify that the ``simp-core`` Changelog.rst has
been updated:

* Manually inspect
* Has the generation of the Changelog.rst from component
  CHANGELOGs been automated in any fashion?

Verify the simp-core RPMs can be created
----------------------------------------

This check verifies that CentOS 6 and CentOS 7  ``simp-core`` RPMs can
be generated:

.. code-block:: bash

   git clone https://github.com/simp/simp-core.git``
   cd simp-core/src
   bundle update
   bundle exec rake pkg:rpm[epel-6-x86_64]
   bundle exec rake pkg:rpm[epel-7-x86_64]

Verify simp-core unit tests pass
--------------------------------

This check verifies that the ``simp-core`` unit tests have succeeded
in `TravisCI`_.

   * Navigate to the project's TravisCI results page and verify the
     tests for the development branch to be tagged and released have
     passed.  For our project, this page is
     https://travis-ci.org/simp/simp-core/branches

     .. IMPORTANT::

        If the tests in TravisCI fail, you **must** fix them before
        proceeding.  The automated release procedures will only
        succeed, if the unit tests succeed in TravisCI.

Verify ISOs can be created
--------------------------

This check verifies that SIMP ISOs for CentOS 6 and CentOS 7 can be
built from the local simp-core RPM and RPMs pushed to packagecloud.

* FIXME - The magic happens here....Trevor's build script?

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

For the default boot option and and FIPS disabled boot option
test cases, the `simp-packer`_ project is the easiest way to
verify a SIMP VM can be booted from the ISO and bootstrapped.  Otherwise,
the check has to be done manually:

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

Verify component interoperability
---------------------------------

This check verifies with ``pupmod-simp-simp`` acceptance tests that this
aggregation of module versions interoperate. (These tests provide
extensive, cross-component, integration tests.)

.. NOTE::
   If ``pupmod-simp-simp`` acceptance tests have effectively 
   already been passed with the correct versions of modules
   (e.g., in GitLab), you can skip this painful step.

#. Determine the version of ``pupmod-simp-simp`` to be used in this
   SIMP ISO release.  This version can be pulled from the
   ``Puppetfile.stable``.

#. Checkout that version of the ``pupmod-simp-simp`` project.
   For this discussion, we will assume it is ``4.0.0``.

   .. code-block:: bash

      git clone https://github.com/simp/pupmod-simp-simp.git``
      cd pupmod-simp-simp
      git fetch -t origin
      git checkout tags/4.0.0

#. Create a ``.fixtures.yml`` file that sets the versions of
   each dependency to the version contained in the
   ``Puppetfile.stable`` file for this ISO release.

#. Run the acceptance tests with and without FIPS mode enabled

   .. code-block:: bash

      bundle update
      BEAKER_fips-yes bundle exec rake beaker:suites
      bundle exec rake beaker:suites

Verify otherwise untested capabilities
--------------------------------------
This check verifies that all other major capabilities (not otherwise
tested in acceptance/simp-packer tests) do function as advertised:

* A client can be PXE booted using the kickstart files from the
  SIMP ISO
* A client can use the SIMP server for DNS
* A SIMP ISO can be bootstrapped for the 'simp-lite' scenario with
  default boot options
* A 'simp-lite' client operates with a SIMP server

  - login operations (PAM, LDAP, local user)
  - NFS operations (home directory)
  - logging operations (rsyslog)
  - auditing operations

* A SIMP ISO can be bootstrapped for the 'poss' scenario with
  default boot options
* The SIMP server can be converted from FIPS enabled to FIPS
  disabled mode.
* The SIMP server can be converted from FIPS disabled to FIPS
  enabled mode.
* What else?

FILL-ME-IN  test procedures

Verify SIMP server RPM install
------------------------------
This check verifies that CentOS 6 and CentOS 7 SIMP servers can be
installed using the set of RPMs contained in the SIMP ISOs
The verification steps largely follow the details in
:ref:`gsg-installing_simp_from_a_repository`.  All RPMs except
the ``simp-core`` RPM should be able to be pulled from `packagecloud`_.

Verify SIMP server RPM upgrade
------------------------------
This check verifies that the set of RPMs in the SIMP ISO can upgrade
the last full SIMP release.

#. Bring up a CentOS server that was booted from the appropriate SIMP
   ISO and for which ``simp config`` and ``simp bootstrap`` has been
   run.  (Reminder: The `simp-packer`_ project is the easiest way to
   create a SIMP VM that has been bootstrapped.)

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
   (e.g., ``environments/simp/modules/site/manifests``, content in
   ``environments/simp/hieradata``)

.. _GitHub: https://github.com
.. _packagecloud: https://packagecloud.io/simp-project
.. _simp-project: http://simp-project.com/ISO/SIMP
.. _simp-packer: https://github.com/simp/simp-packer
.. _TravisCI: https://travis-ci.org

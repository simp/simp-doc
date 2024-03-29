.. _gsg-building_simp_from_source:

Building SIMP From Source
=========================

Getting Started
---------------

Please have your environment prepared as specified by
:ref:`gsg-environment_preparation` and have the necessary packages mirrored in
accordance with :ref:`gsg-mirroring_packages` before continuing.

All of the following items assume that you are running as ``build_user`` in the
``simp_build_centos8`` container.

Download the CentOS/RedHat installation media:

  * Refer to :file:`release_mappings.yaml` to determine the distribution ISO
    compatible with the version of SIMP you want to build.
    :file:`release_mappings.yaml` is maintained the :github:`simp/simp-core`
    repository in the :file:`build/distributions/<distribution>/<release>/<arch>`
    directory.

Updating release_mappings.yaml
""""""""""""""""""""""""""""""""

It is very possible the versions outlined in the `release_mappings.yaml` are out of date,
as such you may need to download a newer version of the installation media and create a new
section inside of the file. You can either modify one of the existing release versions or
create an entirely new one to house the information for the new iso. We recommend simply copying
one of the existing releases, renaming it, and modifying the iso section as necessary.

The size of the iso can be obtained with:

.. code-block:: bash
   ls -la </path/to/dvd*.iso>

The checksum can be obtained with:

.. code-block:: bash
   sha256sum </path/to/dvd*.iso>

After obtaining the information necessary, ensure that the iso name is correct, modify the
build command to have the correct iso name and release version, and ensure that the OS version is correct.

.. NOTE::

   The build process is handled by :github:`simp/rubygem-simp-rake-helpers`. If you
   discover issues, that is where you will want to look for answers.

Generating The ISO
------------------

Repository Setup
^^^^^^^^^^^^^^^^

Clone simp-core:

.. code-block:: bash

   git clone https://github.com/simp/simp-core
   cd simp-core

Check out your desired branch of SIMP:

* To check out a stable SIMP release, check out a tag (*Recommended*):

.. code-block::

   git checkout tags/6.6.0-1

* To check out an unstable SIMP release, check out the latest ``master``:

.. code-block::

   git checkout master

Run ``bundle`` to make sure that all of the build tools and dependencies are
installed and up to date:

.. code-block::

   bundle install

Make an ``ISO`` directory, and copy in the CentOS/RHEL installation media:

.. code-block:: bash

   mkdir ISO
   cp </path/to/dvd*.iso> ISO

ISO Setup
^^^^^^^^^

Next, you need to prepare your local packages for inclusion in the final ISO.
This example builds off of CentOS 8, but all distributions work the same way.

Change to the target distribution directory:

.. code-block:: bash

   cd build/distributions/CentOS/8/x86_64

You will see two directories :file:`DVD` and :file:`yum_data`.

Boot and Kickstart Customization (Optional)
"""""""""""""""""""""""""""""""""""""""""""

The :file:`DVD` directory holds information that is used to provide ISO boot
options in both :term:`UEFI` and :term:`BIOS` boot modes. It also contains a
:file:`ks` directory that is used as the automated :term:`kickstart` for
hands-off provisioning of the initial server.

Package Customization
"""""""""""""""""""""

The :file:`yum_data` directory is where you will modify the settings to include
your own packages in the ISO.

Change to the :file:`yum_data` directory:

.. code-block:: bash

   cd yum_data

You will now see a :file:`reposync` directory. Any YUM repository placed into
this directory will be copied onto the final ISO using the following rules:

1. If the :file:`reposync` directory has the exact same name as a directory already on the ISO

   * Remove the ISO directory and copy in the :file:`reposync` directory

2. Otherwise

   * Add the :file:`reposync` directory as a subdirectory of :file:`SimpRepos`

For example, if the original ISO has the following directory structure:

.. code-block::

   /BaseOS
   /AppStream

And the :file:`reposync` directory contains the following repositories:

.. code-block::

   /BaseOS
   /appstream
   /SimpRepos/puppet

The resulting ISO will contain the following:

.. code-block::

   /BaseOS (the reposync version)
   /AppStream (the ISO version)
   /SimpRepos/appstream (case matters)
   /SimpRepos/puppet

Add the Repositories from Pulp
""""""""""""""""""""""""""""""

You can now add the repositories that you mirrored in
:ref:`gsg-mirroring_packages` to the :file:`reposync` directory.

.. NOTE::

   Check the kickstart files in the :file:`DVD` directory to see what
   repositories will be used by default.

.. code-block:: bash
   mkdir reposync/SimpRepos
   mv /tmp/_download_path/*/BaseOS reposync
   mv /tmp/_download_path/*/AppStream reposync
   mv /tmp/_download_path/*/* reposync/SimpRepos

At this point, the :file:`reposync/SimpRepos` directory may contain both a :file:`puppet`
and :file:`puppet6` directory. If you wish to use ``puppet`` version 6 by
default, move the :file:`puppet` directory to :file:`puppet7` and rename
:file:`puppet6` to :file:`puppet`.

.. code-block:: bash

   mv reposync/puppet reposync/puppet7
   mv reposync/puppet6 reposync/puppet

Modifying the Repositories (Optional)
"""""""""""""""""""""""""""""""""""""

At this point, you may perform the following actions:

1. Add your own additional repositories
2. Update repositories that do **not** have groups or modules present

   * Repos with groups will contain a :file:`*-comps.xml` file in the
     :file:`repodata` directory
   * Repos with modules will contain a :file:`*-modules.yaml` file in the
     :file:`repodata` directory

Verify the Upstream Vendor Repositories
"""""""""""""""""""""""""""""""""""""""

The SIMP component build process first attempts to use an upstream YUM
repository to pull down a matching build artifact. Failing that, a local copy is
built for packaging. This copy is **authoritative** and will override anything
from :file:`reposync`.

The optional :file:`repos/` directory under :file:`yum_data/` holds a selection of
repositories that will be used for fetching upstream SIMP RPMs from the official
sources. These files should be regular :term:`YUM` repository configuration files.

Once downloaded, non-SIMP files will be housed in a :file:`packages` directory
and the :file:`packages.yaml` file will be updated to reflect the download
source for auditing purposes. If you need to re-download the files, simply erase
the :file:`packages` and :file:`packages.yaml` files.

Build the ISO
^^^^^^^^^^^^^

You are now ready to build the ISO!

To do so, run the following, substituting ``6.6`` with the expected build
version from :file:`release_mappings.yaml`:

.. code-block:: bash

   cd </path/to>/simp-core
   SIMP_BUILD_prompt=yes \
   SIMP_BUILD_reposync_only=yes \
   SIMP_BUILD_distro=CentOS,8Stream,x86_64 \
   bundle exec rake build:auto[$PWD/ISO,<release_version>] #The release_version is the desired release number defined in release_mappings.yaml

Once the process completes, you should have a bootable SIMP ISO, in:
:file:`build/distributions/<OS>/<rel>/<arch>/SIMP_ISO/`

You can download it as follows (using CentOS 8 as an example):

.. code-block:: bash

   podman cp simp_build_centos8:/home/build_user/simp-core/build/distributions/CentOS/8/x86_64/SIMP_ISO .

Rebuilding the ISO
""""""""""""""""""

If the ISO build fails for any reason or you simply want to re-build the ISO because you've made a change
you need to run the following commands to clean up the previous run:

.. code-block:: bash
   rm -rf build/distributions/CentOS/8/x86_64/DVD_Overlay
   rm -rf build/distributions/CentOS/8/x86_64/SIMP*

After running these commands the ISO build can be safely re-run.

Other Build Directories of Note
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following directories exist at the same level as :file:`SIMP_ISO/` and may
be of use:

* :file:`DVD_Overlay`

  * The SIMP product RPMs and artifacts as a ``tar`` file. This is extracted
    into the ISO after all other modifications have occurred.

* :file:`SIMP_ISO_STAGING`

  * The 'staging' directory for the ISO. This is essentially the final ISO in
    'unpacked' form and a useful place to look if you think something is
    missing or incorrect.


After You Build
---------------

You may have noticed that a development GPG key has been generated for the
build.

This key is only valid for one week from generation and has been specifically
generated for packages compiled specifically for your ISO build. If all of your
packages were downloaded via Pulp, then there should be no packages on your ISO
that need the development GPG key.

Doing this allows you to have a validly signed set of RPMs while reducing the
risk that you will have invalid RPMs distributed around your infrastructure.

.. NOTE::

   If you need to build and sign your RPMs with your own key, you can certainly
   do so using the ``rpm --resign`` command.

The new development key will be placed at the root of your ISO and will be
called ``RPM-GPG-KEY-SIMP_dev``. This key can be added to your clients, or
served via a web server, if you need to install from a centralized :term:`yum`
repository.

Please see the `Red Hat Guide to Configuring Yum and Yum Repositories`_ for
additional information.

.. _Red Hat Guide to Configuring Yum and Yum Repositories: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/8.3_release_notes/distribution-of-content-in-rhel-8#package_management_with_yum_dnf

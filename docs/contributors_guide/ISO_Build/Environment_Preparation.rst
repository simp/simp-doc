.. _gsg-environment_preparation:

Environment Preparation
=======================

Getting Started
---------------

.. WARNING::

   Use a **non-root** user for building SIMP!

Ensure Sufficient Entropy
^^^^^^^^^^^^^^^^^^^^^^^^^

The SIMP build generates keys and signs many packages, so it reads a lot of
random data.  On EL9 and EL10, the kernel random number generator doesn't
block once it has been initialized, so you rarely need to do anything here.
On EL10, :file:`/proc/sys/kernel/random/entropy_avail` always reads 256, so
that value doesn't tell you whether the build will be slow.

If key generation or signing stalls on EL8, run :program:`rngd`.  The
:package:`rng-tools` package is in the BaseOS repository on EL8, EL9 and
EL10:

.. code-block:: bash

   sudo dnf install -y rng-tools
   sudo systemctl enable --now rngd

Use :program:`rngd` rather than :program:`haveged`.  :package:`haveged` is
only available from EPEL, and SIMP is replacing :pupmod:`simp/haveged` with
:pupmod:`simp/rngd`.

Setup a Container Management System
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are a lot of different methods for running containers (we like
:program:`podman`) so you will need to find one that works for your operating
system.

While most of our development occurs on SIMP-managed systems, we have had luck
using WSL in Windows and Docker in OS X.

Examples in the documentation will use :program:`podman` and :program:`buildah`.

Ensure Sufficient Space
^^^^^^^^^^^^^^^^^^^^^^^

Building the ISO takes quite a bit of space so make sure you have at least 30G
of free space available.

If you use a container, you need to make sure that wherever you are hosting your
container has enough space available.

If your default location does not have enough space, you may need to change your
:code:`graphroot` in :file:`$HOME/.config/containers/storage.conf`.

Install OpenBolt
^^^^^^^^^^^^^^^^

The build uses :term:`OpenBolt`, the community fork of Puppet Bolt, to start
the :program:`pulp` container that mirrors the packages.  This approach handles
modular repositories on EL8+.

The :package:`openbolt` package is in the OpenVox repository.  Install the
repository release package, then OpenBolt:

.. code-block:: bash

   sudo dnf install -y https://yum.voxpupuli.org/openvox9-release-el-$(rpm -E %rhel).noarch.rpm
   sudo dnf install -y openbolt

OpenBolt installs its own Ruby, :program:`bundle` and :program:`bolt` into
:file:`/opt/puppetlabs/bolt/bin`.

Setup Your Build Container
^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP needs to be built using the **exact same distribution** that the ISO
targets.  The examples below use EL8, the only release that the ISO build
supports so far.  Images for EL9 and EL10 are published as well: change
``el8`` to ``el9`` or ``el10``.

The build images run as ``build_user`` and have :github:`simp/simp-core`
cloned into :file:`/home/build_user/simp-core`.  They have Ruby 3.2 (OpenVox 8)
and Ruby 4.0 (OpenVox 9) installed, and Ruby 3.2 is the default.

Use an Existing Image
"""""""""""""""""""""

For the fastest start, use a published image.  The images are rebuilt every
week:

.. code-block:: bash

   podman pull ghcr.io/simp/simp-el8-build:latest
   podman run -id --name simp_build_el8 ghcr.io/simp/simp-el8-build:latest
   podman exec -it simp_build_el8 su -l build_user

Build a Fresh Container
"""""""""""""""""""""""

If you need changes that have not been published yet, build the image from the
Dockerfiles in the :file:`build/Dockerfiles` directory of
:github:`simp/simp-core`.  To make Ruby 4.0 the default, add
``--build-arg ruby_version=4.0`` to the :program:`buildah` command.

.. code-block:: bash

   git clone https://github.com/simp/simp-core
   cd simp-core/build/Dockerfiles
   buildah build --layers -f SIMP_EL8_Build.dockerfile -t simp_build_el8 .
   podman run -id --name simp_build_el8 simp_build_el8
   podman exec -it simp_build_el8 su -l build_user

Inside the container, check out the :github:`simp/simp-core` tag that you want
to build, as described in :ref:`gsg-building_simp_from_source`.

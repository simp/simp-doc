.. _gsg-environment_preparation:

Environment Preparation
=======================

Getting Started
---------------

.. WARNING::

   Use a **non-root** user for building SIMP!

Ensure Sufficient Entropy
^^^^^^^^^^^^^^^^^^^^^^^^^

The SIMP build generates various keys and does quite a bit of package
signing. As such, your system must be able to keep its entropy pool
full at all times. If you check ``/proc/sys/kernel/random/entropy_avail``
and it shows a number below **1024**, then you should either make sure that
``rngd`` is running and pointed to a hardware source (preferred) or install
and use **haveged**.

.. code-block:: bash

   sudo yum install haveged
   sudo systemctl start haveged
   sudo systemctl enable haveged

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

Install Puppet Bolt
^^^^^^^^^^^^^^^^^^^

The latest build system uses :github:`puppetlabs/bolt` to spin up
:program:`pulp` containers for repository mirroring.

This was done to work with modular repositories in EL8+.


Setup Your Build Container
^^^^^^^^^^^^^^^^^^^^^^^^^^

SIMP needs to be built using the same distribution that you are trying to build.
While the method for doing this is up to you, we recommend that you use the
build-related Dockerfiles in the :github:`simp/simp-core` project under the
:file:`build/Dockerfiles` subdirectory.

The following provides an example of how to build and start the image:

.. code-block:: bash

   git clone https://github.com/simp/simp-core
   cd simp-core/build/Dockerfiles
   buildah bud --layers -f SIMP_EL8_Build.dockerfile -t el8build
   podman run -id --name el8build el8build
   podman exec -it el8build bash
   su - build_user

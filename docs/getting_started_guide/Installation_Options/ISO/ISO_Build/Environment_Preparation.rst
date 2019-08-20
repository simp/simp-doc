.. _gsg-environment_preparation:

Environment Preparation
=======================

Getting Started
---------------

.. WARNING::

   Please use a **non-root** user for building SIMP!

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

Set Up Ruby
-----------

We highly recommend using :term:`RVM` to make it easy to develop and test
against several versions of :term:`Ruby` at once without damaging your
underlying Operating System.

RVM Installation
^^^^^^^^^^^^^^^^

The following commands, taken from the `RVM Installation Page`_ can be used to
install :term:`RVM` for your user.

.. code-block:: bash

   gpg2 --keyserver hkp://keys.gnupg.net --recv-keys \
       409B6B1796C275462A1703113804BB82D39DC0E3 \
       7D2BAF1CF37B13E2069D6956105BD0E739499BDB
   \curl -sSL https://get.rvm.io | bash -s stable --ruby=2.4.4
   source ~/.rvm/scripts/rvm


Set the Default Ruby
^^^^^^^^^^^^^^^^^^^^

You will want to use :term:`Ruby` ``2.4.4`` as your default :term:`RVM` for SIMP
development.

.. code-block:: bash

   rvm use --default 2.4.4

.. NOTE::

   Once this is done, you can simply type ``rvm use 2.4.4``.



Bundler
^^^^^^^

The next important tool is `Bundler`_. Bundler makes it easy to install Gems
and their dependencies. It gets this information from the Gemfile found in the
root of each repo. The Gemfile contains all of the gems required for working
with the repo. More info on Bundler can be found on the
`Bundler Rationale Page`_ and more information on Rubygems can be found at
`Rubygems.org`_.

.. code-block:: bash

   rvm all do gem install bundler

Set Up Docker
-------------

Docker is typically provided by an OS repository.  You may need to enable that
repository depending on your distribution.

.. code-block:: bash

   sudo yum install docker

The Docker package may not provide a `dockerroot` group.  If it does not exist
post installation, create it:

.. code-block:: bash

   sudo groupadd dockerroot

Allow your (non-root) user to run docker:

.. code-block:: bash

   sudo usermod -aG dockerroot <user>

When you build your system make sure you set the default size for the docker
container or the ISO build may not work properly.

To do this on a :term:`EL` system, set the following in
``/etc/sysconfig/docker-storage`` and restart the ``docker`` service.

.. NOTE::

   You will need to start and stop docker once before adding in this option or
   the service will fail to start.

.. code-block:: bash

   DOCKER_STORAGE_OPTIONS= --storage-opt dm.basesize=100G


.. NOTE::

   You may need to log out and log back in before your user is able to run as
   ``dockerroot``.

As root, edit ``/etc/docker/daemon.json`` and change the ownership of the
docker daemon socket:

.. code-block:: json

   {
     "live-restore": true,
     "group": "dockerroot"
   }

Start the docker daemon:

.. code-block:: bash

   sudo systemctl start docker
   sudo systemctl enable docker

Build Your Build Containers
---------------------------

The `simp-core`_ project provides suitable build Dockerfiles for both
:term:`EL` 6 and :term:`EL` 7 in the ``build/Dockerfiles`` directory.

These work well for building both :term:`CentOS` 6 and 7 artifacts and the
usage is noted at the top of those files.

Unfortunately, getting this to work with :term:`RHEL` has proven to be a
challenge so you should use the Dockerfiles to see what packages you need to
install on your local host to be able to successfully build.

A simple way to get a quick list is to run ``grep "yum .* -y"`` on the
appropriate Dockerfile.

.. _Bundler Rationale Page: https://bundler.io/rationale.html
.. _Bundler: https://bundler.io/
.. _RVM Installation Page: https://rvm.io/rvm/install
.. _RVM: https://rvm.io/
.. _Rubygems.org: https://guides.rubygems.org/what-is-a-gem/
.. _simp-core: https://github.com/simp/simp-core

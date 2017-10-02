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

  $ yum install haveged
  $ systemctl start haveged

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

   $ gpg2 --keyserver hkp://keys.gnupg.net --recv-keys \
       409B6B1796C275462A1703113804BB82D39DC0E3
   $ \curl -sSL https://get.rvm.io | bash -s stable --ruby=2.1.9
   $ source ~/.rvm/scripts/rvm

Set the Default Ruby
^^^^^^^^^^^^^^^^^^^^

You'll want to use :term:`Ruby` 2.1.9 as your default :term:`RVM` for SIMP
development.

.. code-block:: bash

   $ rvm use --default 2.1.9

.. NOTE::

   Once this is done, you can simply type ``rvm use 2.1.9``.

Bundler
^^^^^^^

The next important tool is `Bundler`_. Bundler makes it easy to install Gems
and their dependencies. It gets this information from the Gemfile found in the
root of each repo. The Gemfile contains all of the gems required for working
with the repo. More info on Bundler can be found on the
`Bundler Rationale Page`_ and more information on Rubygems can be found at
`Rubygems.org`_.

.. code-block:: bash

   $ rvm all do gem install bundler

Set Up Docker
-------------

Docker is typically provided by an OS repository.  You may need to enable that
repository depending on your distribution.

.. code-block:: bash

  $ yum install docker

Allow your (non-root) user to run docker:

.. code-block:: bash

  $ usermod -aG dockerroot <user>

.. NOTE::

  You may need to log out and log back in before your user is able to run as
  dockerroot.

Edit ``/etc/docker/daemon.json`` and change the ownership of the docker daemon
socket:

.. code-block:: json

  {
    "live-restore": true,
    "group": "dockerroot"
  }

Start the docker daemon:

.. code-block:: bash

  $ systmectl start docker
  $ systemctl enable docker

.. _Bundler Rationale Page: http://bundler.io/rationale.html
.. _Bundler: http://bundler.io/
.. _RVM Installation Page: https://rvm.io/rvm/install
.. _RVM: https://rvm.io/
.. _Rubygems.org: http://guides.rubygems.org/what-is-a-gem/

Environment Preparation
=======================

Getting Started
---------------

Required Packages
^^^^^^^^^^^^^^^^^

Before we go any further, you'll want to make sure that you have a few
:term:`RPM` packages installed on your system and that your build system has
access to the Internet. If you're using :term:`Enterprise Linux` as your build
system, you will need to enable :term:`EPEL` for your system prior to
proceeding.

Required RPMs
"""""""""""""

.. code-block:: bash

  # Installing from EL 6:

    $ sudo yum install -y augeas-devel createrepo genisoimage git gnupg2 \
        libicu-devel libxml2 libxml2-devel libxslt libxslt-devel \
        mock rpmdevtools clamav gcc gcc-c++ ruby-devel

  # Installing from EL 7:

    $ sudo yum install -y augeas-devel createrepo genisoimage git gnupg2 \
        libicu-devel libxml2 libxml2-devel libxslt libxslt-devel \
        mock rpmdevtools clamav-update gcc gcc-c++ ruby-devel

  # Installing from Fedora 22/23:

    $ sudo dnf install -y augeas-devel createrepo genisoimage git gnupg \
        libicu-devel libxml2 libxml2-devel libxslt libxslt-devel \
        mock rpm-sign rpmdevtools clamav-update gcc gcc-c++ ruby-devel

.. warning::

   Please use a **non-root** user for developing SIMP

.. note::

   The SIMP build generates various keys and does quite a bit of package
   signing. As such, your systems needs to be able to keep its entropy pool
   reasonably full at all times. If you check
   /proc/sys/kernel/random/entropy_avail and it shows a number below **1024**,
   then you should either make sure that `rngd` is running and pointed to a
   hardware source (preferred) or install and use **haveged**. It is not
   recommended that you use `haveged` in production but it may be your only
   choice if building on a virtual system.

Clone the Core Repository
-------------------------

You should now move to your preferred working directory and clone the SIMP core
development repository.

.. code-block:: bash

  $ git clone https://github.com/simp/simp-core
  $ cd simp-core

Check Out Your Preferred Branch
-------------------------------

You're now ready to begin development!

However, you've probably noticed that the `master` branch of the environment is
relatively empty. To build your preferred version of SIMP, you'll need to check
out the corresponding branch.

Set Up Ruby
-----------

We highly recommend using :term:`RVM`_ to make it easy to develop and test against
several versions of :term:`Ruby` at once without damaging your underlying
:term:`Operating System`.

RVM Installation
^^^^^^^^^^^^^^^^

The following commands, taken from the `RVM Installation Page`_ can be used to
install RVM for your user.

.. code-block:: bash

   $ gpg2 --keyserver hkp://keys.gnupg.net --recv-keys \
       409B6B1796C275462A1703113804BB82D39DC0E3
   $ \curl -sSL https://get.rvm.io | bash -s stable --ruby=1.9.3 --ruby=2.1.0
   $ source ~/.rvm/scripts/rvm

Set the Default Ruby
^^^^^^^^^^^^^^^^^^^^

You'll want to use :term:`Ruby` 2.1.0 as your default :term:`RVM` for SIMP development.

.. code-block:: bash

   $ rvm use --default 2.1.0

.. note::

  Once this is done, you can simply type ``rvm use 2.1.0``.

Bundler
^^^^^^^

The next important tool is `Bundler`_. Bundler makes it easy to install Gems and
their dependencies. It gets this information from the Gemfile found in the root
of each repo. The Gemfile contains all of the gems required for working with
the repo. More info on Bundler can be found on the `Bundler Rationale Page`_ and
more information on Rubygems can be found at `Rubygems.org`_.

Configure Mock
--------------

The SIMP build process makes heavy use of Mock to create clean packages. As
such, you need to ensure that your system is ready.

Add Your User to the Mock Group
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: bash

   $ getent group mock > /dev/null || sudo newgrp mock
   $ sudo usermod -a -G mock $USER

You may need to run `newgrp` or logout and back in for the group settings to
take effect.

Prepare to Work
---------------

You are now ready to begin development!

If you want to start with the :term:`EL` 6 build

.. code-block:: bash

   $ git checkout 4.2.X

If you want to start with the :term:`EL` 7 build

.. code-block:: bash

   $ git checkout 5.1.X

Now, initialize your build environment

.. code-block:: bash

   # Grab all of your Gem dependencies
   $ bundle install

.. _Bundler Rationale Page: http://bundler.io/rationale.html
.. _Bundler: http://bundler.io/
.. _RVM Installation Page: https://rvm.io/rvm/install
.. _RVM: https://rvm.io/
.. _Rubygems.org: http://guides.rubygems.org/what-is-a-gem/

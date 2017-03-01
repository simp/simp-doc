.. _gsg-environment_preparation:

Environment Preparation
=======================

Getting Started
---------------

.. NOTE::
   You can skip setting up ``mock`` if you are generating an ISO from a release tarball!

Required Packages
^^^^^^^^^^^^^^^^^

Before we proceed, you'll want to make sure that you have a few :term:`RPM`
packages installed on your system and that your build system has access to the
Internet. If you're using :term:`Enterprise Linux` as your build system, you
will need to enable :term:`EPEL` for your system prior to proceeding.

Required RPMs
"""""""""""""

.. code-block:: bash

  # Installing from EL 6:

    $ sudo yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-6.noarch.rpm
    $ sudo yum install -y augeas-devel createrepo genisoimage git gnupg2 \
        libicu-devel libxml2 libxml2-devel libxslt libxslt-devel \
        mock rpm-sign rpmdevtools clamav gcc gcc-c++ ruby-devel

  # Installing from EL 7:

    $ sudo yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    $ sudo yum install -y augeas-devel createrepo genisoimage git gnupg2 \
        libicu-devel libxml2 libxml2-devel libxslt libxslt-devel \
        mock rpm-sign rpmdevtools clamav-update gcc gcc-c++ ruby-devel

  # Installing from Fedora 23/24:

    $ sudo dnf install -y augeas-devel createrepo genisoimage git gnupg \
        libicu-devel libxml2 libxml2-devel libxslt libxslt-devel \
        mock rpm-sign rpmdevtools clamav-update gcc gcc-c++ ruby-devel

.. WARNING::
   Please use a **non-root** user for building SIMP!

.. NOTE::
   The SIMP build generates various keys and does quite a bit of package
   signing. As such, your system mustto be able to keep its entropy pool
   full at all times. If you check ``/proc/sys/kernel/random/entropy_avail``
   and it shows a number below **1024**, then you should either make sure that
   ``rngd`` is running and pointed to a hardware source (preferred) or install
   and use **haveged**.

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

Configure Mock
--------------

.. NOTE::
   If you plan on just building from a tarball, you can skip this section.

Building SIMP from scratch makes heavy use of Mock to create clean packages. As
such, you need to ensure that your system is ready.

Add Your User to the Mock Group
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: bash

   $ getent group mock > /dev/null || sudo newgrp mock
   $ sudo usermod -a -G mock $USER

You may need to run ``newgrp`` or logout and back in for the group settings to
take effect.

Prepare to Work
---------------

You are now ready to begin development!

Clone simp-core:

.. code-block:: bash

  $ git clone https://github.com/simp/simp-core
  $ cd simp-core

Check out your desired branch of SIMP:

* To check out a stable SIMP release, check out a tag:

.. code::

   $ git fetch --tags
   $ git checkout tags/5.2.0-0

* To check out an unstable SIMP release, check out the latest 5.X or 4.X HEAD:

.. code::

   $ git checkout 5.1.X
   $ git checkout 4.2.X

.. NOTE::
   SIMP >= 5.2.X, >= 4.3.X are still developed on the ``5.1.X`` and ``4.2.X``
   branches, respectively. All future development will be made on the
   ``master`` branch.

.. WARNING::
   Any branch that is not tagged with a git tag should be treated as
   **unstable**.

Grab gem dependencies:

.. code-block:: bash

   $ bundle install

.. _Bundler Rationale Page: http://bundler.io/rationale.html
.. _Bundler: http://bundler.io/
.. _RVM Installation Page: https://rvm.io/rvm/install
.. _RVM: https://rvm.io/
.. _Rubygems.org: http://guides.rubygems.org/what-is-a-gem/

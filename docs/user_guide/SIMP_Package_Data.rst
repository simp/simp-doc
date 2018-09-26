Package Data
============

.. toctree::
   :maxdepth: 2

Base Packages
-------------

Information about the base SIMP packages is best gathered from the ``simp`` RPM
and the RPM metadata on your system.

The dependencies for the ``simp`` RPM are those that are required for basic
SIMP functionality and may be obtained as follows from an **installed system**:

.. code-block:: bash

   for x in `rpm -q --requires simp | cut -f 1 -d' '`; do
     rpm -q --qf "%{NAME} %{VERSION}\n" $x | grep -v 'not installed';
   done

The dependencies for the ``simp-extras`` RPM are those that are **not**
required for basic SIMP functionality and may be obtained as follows from an
**installed system**:

.. code-block:: bash

   for x in `rpm -q --requires simp-extras | cut -f 1 -d' '`; do
     rpm -q --qf "%{NAME} %{VERSION}\n" $x | grep -v 'not installed';
   done

External Packages
-----------------

Quite a few external packages are available to, and used by, the SIMP
infrastructure.

These are defined, with sources, per OS and architecture in the
``packages.yaml`` files under the ``build`` directory in the ``simp-core``
repository.

To find the particular package list for your version of SIMP, you can go to:

``https://github.com/simp/simp-core/blob/<version>/build/distributions/<os>/<os_version>/<arch>/yum_data/packages.yaml``

So, if you are using SIMP 6.0.0-0 on CentOS 6 and x86_64 architecture, you would navigate to:

``https://github.com/simp/simp-core/blob/6.0.0-0/build/distributions/CentOS/6/x86_64/yum_data/packages.yaml``

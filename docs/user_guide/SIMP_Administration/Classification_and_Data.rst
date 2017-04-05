.. _Classification and Data:

Classification and Data
=======================

Node Classification in SIMP
---------------------------

From the Puppet, Inc website:

  Hiera is a key/value lookup tool for configuration data, built to set
  node-specific data without repeating yourself.

SIMP uses :term:`Hiera` to attempt to make configuration of the overall system easier
for our end users by providing a simple, centralized, method for setting class
parameters using `automatic parameter lookup`_ and as a method for
`basic node classification`_.

It is **highly recommended** that you read the `Hiera Documentation`_ prior to
jumping into using a SIMP system.

Hiera in SIMP
-------------

As mentioned, SIMP users are expected to make extensive use of Hiera to set
parameters, particularly those that are deep within the code.

The default Hiera hierarchy used by SIMP looks like the following:

.. code:: yaml

   ---
   :backends:
     - 'yaml'
   :hierarchy:
     # Most specific
     - 'hosts/%{trusted.certname}'
     - 'hosts/%{facts.fqdn}'
     - 'hosts/%{facts.hostname}'
     - 'domains/%{facts.domain}'
     - '%{facts.os.family}'
     - '%{facts.os.name}/%{facts.os.release.full}'
     - '%{facts.os.name}/%{facts.os.release.major}'
     - '%{facts.os.name}'
     - 'hostgroups/%{::hostgroup}'
     - 'default'
     - 'compliance_profiles/%{::compliance_profile}'
     - 'simp_config_settings'
     - 'scenarios/%{::simp_scenario}'
     # Least specific
   :logger: 'puppet'
   :yaml:
     :datadir: '/etc/puppetlabs/code/environments/%{::environment}/hieradata'

.. WARNING::

   This may not be accurate for your version of SIMP, please check your local
   Hiera settings!

The rest of this document will use this hierarchy as a reference.

Assigning Classes to Nodes
--------------------------

Assigning classes to nodes can be done in a few ways in SIMP. First, there is a
a ``lookup`` function in ``/etc/puppetlabs/code/environments/simp/manifests/site.pp``
that looks for an array called ``classes`` in your hierarchy. It also looks for
an array called ``class_exclusions``, which can be used to remove classes from
the classes array. The classes that are included are the result of
``$classes - $class_exclusions``. If classes need to be added to all nodes, a
``classes`` array could be added to the ``default.yaml`` in your hieradata,
like this:

.. code-block:: yaml

   ---
     classes:
       - 'site::example_class'
   ---

A similar array could be created in any other layer in the hierarchy, and it
will be merged with the 'unique' strategy by the ``lookup`` function noted
above.

The SIMP profile module also includes other classes needed for a secure
baseline, which are discused below in the `simp scenarios`_ section.

Assigning Defined Types to Nodes
--------------------------------

Defined types do not have the ability to receive parameters via Hiera in the
traditional sense. To include a defined type on a node, one could use
``create_resources``, but this is messy and discouraged. Instead, create your
own profile or add a class to the SIMP ``site`` module such as:
``/etc/puppetlabs/code/environments/simp/modules/site/manifests/my_site.pp``.

.. NOTE::

   You can find a working example of this in the :ref:`PXE_Boot` section
   of the documentation

SIMP File Structure
-------------------

The default puppet environment in SIMP, located at
``/etc/puppetlabs/code/environments/simp``, contains almost
all necessary files for a Puppet infrastructure. It will look like this on a
fresh SIMP system:

.. code-block:: bash

   /etc/puppetlabs/code/environments/simp/
   ├── environment.conf
   ├── hieradata/
   ├── manifests/
   └── modules/

- ``environment.conf`` - Sets the environment to include the second SIMP modulepath.
- ``manifests/`` - Contains site.pp and all other node manifests.
- ``hieradata/`` - Default location of the yaml files which contain your node data
- ``modules/`` - Default install location of Puppet modules. Each module RPM copies files here during installation from ``/usr/share/simp/modules``.

Second Modulepath
-----------------

SIMP utilizes a second modulepath to ensure that deployment tools like r10k
don't squash keydist and some krb5 files. The path is
``/var/simp/environments/simp/site_files/``. :ref:`Certificates` are stored there.

Hiera
-----

.. code-block:: bash

   /etc/puppetlabs/code/environments/simp/hieradata/
   ├── CentOS -> RedHat/
   ├── compliance_profiles/
   ├── default.yaml
   ├── hostgroups/
   ├── hosts/
   ├── RedHat/
   ├── scenarios/
   └── simp_config_settings.yaml

- ``hieradata/hosts/`` - By populating this directory with some.host.name.yaml file, you can assign parameters to host some.host.name
- ``hieradata/domains/`` - Same principal as hosts, but domain names.
- ``hieradata/Redhat/`` - RedHat-specific hiera settings.
- ``hieradata/CentOS/`` - CentOS-specific hiera settings, symlinked to ``hieradata/Redhat/``.
- ``hieradata/hostgroups/`` - The hostgroup of a node can be computed in `site.pp`. Nodes assigned to hostgroup `$hostgroup` will read hiera from a file named `<hostgroup>.yaml` in this directory.
- ``hieradata/default.yaml`` - Settings that should be applied to the entire infrastructure.
- ``hieradata/simp_config_settings.yaml`` - Contains the variables needed to configure SIMP. Added by ``simp config``.
- ``hieradata/scenarios/`` - Directory containing SIMP Scenarios, set in ``manifests/site.pp``.

``/etc/puppetlabs/puppet/hiera.yaml`` - Hiera's config file, used to control the
hierarchy of your backends. The order of the files above mirrors that order in
the distributed hiera.yaml.

.. _simp scenarios:

SIMP Scenarios
--------------

SIMP scenarios are groups of classes, settings, and simp_options that ensure the
system is compliant and secure.

There are currently three SIMP scenarios:
- *simp*
- *simp_lite*
- *poss*

The *simp* scenario includes all security features enabled by default, including
iptables and svckill. This scenario is what stock SIMP used to look like in
previous releases.

The *simp_lite* scenario offers many security features, with a few explicity
turned off. This scenario was designed to make it easier to implement SIMP in an
existing environment, because it might not be trivial to flip SELinux to
Enforcing on all nodes.

The *poss* option is the barebones option. It only includes the ``pupmod``
class, to configure Puppet agent on clients. All of the simp_options default to
false, so SIMP will not do a lot of modification to clients through Puppet when
using this scenario.

.. NOTE::

  The SIMP or Puppet server is exempt from most of these settings, and will be
  using most features from the *simp* scenario by default. The SIMP server
  should only have services on it related to Puppet and systems management, and
  SIMP modules all work with all security features enabled. See the
  ``puppet.your.domain.yaml`` in the ``hieradata/hosts`` directory for details.

.. _Hiera Documentation: https://docs.puppet.com/hiera/3.3/complete_example.html
.. _Hiera hierachy: https://docs.puppet.com/hiera/3.3/hierarchy.html
.. _iterator: https://docs.puppet.com/puppet/latest/lang_iteration.html
.. _automatic parameter lookup: https://docs.puppet.com/hiera/3.3/puppet.html#automatic-parameter-lookup
.. _basic node classification: https://docs.puppet.com/hiera/3.3/puppet.html#assigning-classes-to-nodes-with-hiera-hierainclude
.. _structured data: https://docs.puppet.com/hiera/3.3/puppet.html#interacting-with-structured-data-from-hiera


.. _Hiera:


Hiera Overview
==============

SIMP uses Hiera to assign classes to nodes. From the Puppet, Inc
website: Hiera is a key/value lookup tool for configuration data, built
to set node-specific data without repeating yourself. It is an attempt
to make SIMP more configurable to you, the end user. It configures
Puppet in two ways: automatic parameter lookup/hiera lookup functions,
and assigning classes to nodes. The former allows you to generate
reusable code and concentrates parameter assignment to one directory.
The latter is a supplement to the failed inheritance model.


Setting Parameters
------------------

**Automatic Lookup** You can now safely declare any class on any node
with 'include', even if the class is parameterized. Before Hiera, this was
not possible. Puppet will automatically retrieve class parameters from
Hiera using keys. Add a key with a value pair to an appropriate yaml
file, say default.yaml, as such:

Adding a Key/Value Pair to Hiera Examples

.. code-block:: yaml

    ---
    classfoo::parameter_bar: "Woo"
    classfoo::parameter_baz: "Hoo"


You can then 'include classfoo' on any node, with parameter\_bar and
parameter\_baz defaulting to Woo and Hoo, respectively.

**Lookup Functions** You are not required to set up your hierarchy for
automatic variable lookup. Using three functions, you can query Hiera
for any key.

The first is ``hiera``. This uses standard priority lookup and can
retrieve values of any data type from Hiera. If no key is found, a
default should be included. ``$myvar = hiera('parameter_bar', 'Woo')``

The second is ``hiera_array``. This uses an array merge lookup. It
retrieves all array values for a given key throughout the entire
hierarchy and flattens them into a single array.

The third is ``hiera_hash``. This uses a hash merge lookup. It retrieves
all hash values for a given key throughout the entire hierarchy and
merges them into a single hash.


Assigning Classes to Nodes
--------------------------

Assigning classes to nodes is done with the ``hiera_include`` function.
Hiera does an array merge lookup on 'tags' to retrieve classes which
should be included on a node. In SIMP, we place
``hiera_include('classes')`` in ``/etc/puppetlabs/code/environments/simp/manifests/site.pp``. Since
site.pp is outside of any node definition and below all top scope
variables, every node controlled by puppet will get every class tagged
with 'classes' **in its hierarchy**. Additionally, simp\_def.yaml is in
the hierarchy of every node, so every node will receive those classes
(by default).


Assigning Defined Types to Nodes
--------------------------------

Defined types do not have the ability to receive parameters via Hiera in
the traditional sense. To include a defined type on a node, one could
use create\_resources, but this is messy and discouraged. Instead, make a
site class ``/etc/puppetlabs/code/environments/simp/modules/site/manifests/my_site.pp``.
For example, to include tftpboot linux\_model and assign\_host on your
puppet server, puppet.your.domain:


SIMP File Structure
-------------------

The default puppet environment in SIMP, located at
``/etc/puppetlabs/code/environments/simp``, contains almost
all necessary files for a Puppet infrastructure. It will look like this on a
fresh SIMP system:

.. code-block:: bash

   /etc/puppetlabs/code/environments/simp/
   ├── environment.conf
   ├── FakeCA/
   ├── hieradata/
   ├── manifests/
   ├── modules/
   └── simp_autofiles/

- ``environment.conf`` - Sets the environment to include the second SIMP modulepath.
- ``FakeCA/`` - Fake certificate authority. See :ref:`Certificates`.
- ``manifests/`` - Contains site.pp and all other node manifests.
- ``hieradata/`` - Default location of the yaml files which contain your node data
- ``modules/`` - Default install location of Puppet modules. Each module RPM copies files here during installation from ``/usr/share/simp/modules``.
- ``simp_autofiles`` - SIMP files


Second Modulepath
-----------------

SIMP utilizes a second modulepath to ensure that deployment tools like r10k
don't squash keydist and some krb5 files. The path is
``/var/simp/environments/simp/site_files/``.


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
- ``hieradata/CentOS/`` - CentOS-specific hiera settings, symlinks to ``hieradata/Redhat/``.
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

SIMP scenarios are groups of classes, setting, and simp_options that ensure the
system is compliant and secure.

There are currently three SIMP scenarios:
- *simp*
- *simp_lite*
- *poss*

The *simp* scenario includes all security features enabled by default, including
iptables and svckill. This scenario is what stock SIMP used to look like in
previous releases.

The *simp_lite* scenario offers many security features, with a few explicity
turned off. This scenario was designed to make it easier to implment SIMP in an
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
  SIMP modules all work with all security features enabled.


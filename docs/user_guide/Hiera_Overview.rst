
.. _Hiera:

Hiera Overview
==============

SIMP now uses Hiera natively instead of Extdata. From Puppet Labs
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

.. code-block:: xml

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
``hiera_include('classes')`` in ``/etc/puppet/environments/simp/manifests/site.pp``. Since
site.pp is outside of any node definition and below all top scope
variables, every node controlled by puppet will get every class tagged
with 'classes' **in its hierarchy**. Additionally, simp\_def.yaml in is
the hierarchy of every node, so every node will receive those classes
(by default).

Assigning Defined Types to Nodes
--------------------------------

Defined types do not have the ability to receive parameters via Hiera in
the traditional sense. To include a defined type on a node, one could
use create\_resources, but this is messy and discouraged. Instead, make a
site class ``/etc/puppet/environments/simp/modules/site/manifests/my_site.pp``.
For example, to include tftpboot linux\_model and assign\_host on your
puppet server, puppet.your.domain:

Add the following code to a file tftpboot.pp in your site/manifests directory:

.. code-block:: ruby

        # in /etc/puppet/environments/simp/modules/site/manifests/tftpboot.pp
        # Set KSSERVER statically or use Hiera for lookup

        class site::tftpboot {
          include 'tftpboot'

          tftpboot::linux_model { 'EL_MAJOR_VERSION':
            kernel => 'EL_MAJOR_VERSION_x86_64/vmlinuz',
            initrd => 'EL_MAJOR_VERSION_x86_64/initrd.img',
            ks     => "https://KSSERVER/ks/pupclient_x86_64.cfg --noverifyssl inst.noverifyssl",
            extra  => 'ipappend 2'
          }

          tftpboot::assign_host { 'default': model => 'EL_MAJOR_VERSION' }
        }

Then add the following code to your servers Hiera file,
 ``/etc/puppet/environments/simp/hieradata/hosts/puppet.your.domain.yaml``

.. code-block:: yaml

          ---
          classes:
            - 'site::tftpboot'


SIMP Hiera File Structure
-------------------------

- ``/etc/puppet/hiera.yaml`` Hiera's config file, used to control the
  hierarchy of your backends.
- ``/etc/puppet/environments/simp/hieradata/`` Default location of the yaml files which
  contain your node data
- ``/etc/puppet/environments/simp/hieradata/simp_classes.yaml`` The list of default classes
  to include on any SIMP system.
- ``/etc/puppet/environments/simp/hieradata/simp_def.yaml`` Contains the variables needed to
  configure a working SIMP system. Modified by simp-config.
- ``/etc/puppet/environments/simp/hieradata/hosts/`` By populating this directory with
  some.host.name.yaml file, you can assign parameters to host some.host.name
- ``/etc/puppet/environments/simp/hieradata/domains/`` Same principal as hosts, but domain
  names.
- ``/etc/puppet/manifests/`` Contains site.pp and all other node manifests.
  BE CAREFUL when modifying this directory, site.pp contains your globals.
  This directory can be used to supplement or even REPLACE Hiera, with
  nodes. Note that Hiera cannot regex hostnames to apply manifests, so a
  node manifest will have to be created here if you wish to have that
  ability.


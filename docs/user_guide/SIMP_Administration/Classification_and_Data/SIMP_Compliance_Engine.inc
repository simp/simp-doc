.. _SIMP Compliance Engine:

The SIMP Compliance Engine
--------------------------

The :term:`SIMP Compliance Engine` allows users to enable validation and
enforcement of inbuilt and/or custom compliance. :term:`SIMP Community Edition`
contains freely available profiles generally maintained by the community.
:term:`SIMP Enterprise Edition` contains additional profiles targeted more at
commercial regulations. Enforcement is done at the Puppet :term:`class
parameter` level and validation may be done against both :term:`classes` and
:term:`defined resource types`.

This capability is provided by the `simp-compliance_markup`_ Puppet module and
will be renamed at some point in the future to ``simp-compliance_engine`` for
clarity.

.. IMPORTANT::

   Your system must be configured to use at least :term:`Hiera` version 5 for
   the Compliance Engine to function properly.

   SIMP 6.3+ supports this capability out of the box but earlier versions may
   need to `perform a Hiera migration`_.

Enabling the SIMP Compliance Engine
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the Puppet implementation, the Compliance Engine is a :term:`Hiera backend`
that provides data from :term:`SIMP compliance profile` data sets for use in
configuring Puppet classes via :term:`automatic parameter lookup`.

The following example demonstrates how to enable the SIMP Compliance Engine in
an example ``hiera.yaml`` configuration.

.. code-block:: yaml

   ---
   version: 5
   defaults:
     datadir: "data"
     data_hash: "yaml_data"
   hierarchy:
     # Anything defined at this level of the hierarchy will override settings
     # in the SIMP Compliance Engine
     - name: "Higher Priority Items"
       paths:
         - "mandatory.yaml"

     - name: "SIMP Compliance Engine"
       lookup_key: "compliance_markup::enforcement"

     # Anything defined at this level of the hierarchy will be overridden by
     # the SIMP Compliance Engine.
     - name: "Lower Priority Items"
       paths:
         - "optional.yaml"

.. NOTE::

   Hierarchy lookups happen from top to bottom and stop at the first match.
   This means that if the SIMP Compliance Engine is in enforcing mode and is at
   the top of the hierarchy, then it cannot be overridden except by an
   :term:`ENC`. See the `Hiera config file syntax`_ page for additional
   information.

After configuring Hiera, you then need to include the ``compliance_markup`` class.

.. code-block:: puppet

   include 'compliance_markup'

Discovering Available Profiles
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Prior to SIMP 6.4, all profile data was embedded into the
``simp-compliance_markup`` module.  As of 6.4, the data has been split across
the modules with each SIMP module containing the module's specific profile data
in the ``SIMP`` directory.

Data for modules that are not maintained by the SIMP organization are still
housed in ``simp-compliance_markup``.

At this time, SIMP CE comes with two publicly available profiles, ``disa_stig``
and ``nist_800_53:rev4``.

A ``simp`` subcommand will eventually be provided to help discover available
profiles in different environments but, for now, you can use the following code
snippet:

.. code-block:: sh

    ruby -r yaml -e "puts Dir. \
      glob('/etc/puppetlabs/code/environments/production/modules/*/{SIMP,simp}/compliance_profiles/*.yaml'). \
      map{|x| begin YAML.load_file(x); rescue; end; }.compact. \
      map{|x| x['profiles'] && x['profiles'].keys}. \
      compact.flatten.sort.uniq"

Profile Enforcement
^^^^^^^^^^^^^^^^^^^

The selection of profiles to enforce is based on the ``lookup_key`` entry in
the ``hiera.yaml`` configuration as shown in the previous section.

In this case, the ``lookup_key``, is ``compliance_markup::enforcement`` and
should be set in Hiera as an Array of compliance profiles that you would like
to apply, in priority order.

If you wanted to conform with the requirements of the :term:`DISA STIG`, you
would apply the ``disa_stig`` profile as shown below:

.. code-block:: yaml

   ---
   compliance_markup::enforcement:
     - "disa_stig"

The next run of Puppet on the nodes to which this Hiera data applies would then
have their parameters set to meet the STIG requirements as mapped by parameter
in the associated compliance profile.

Additional profiles may be made available by various Puppet modules, such as
those provided by :term:`SIMP EE`.

Code-Level Compliance Validation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The SIMP Compliance Engine also provides a mechanism for generating reports
based on the Puppet parameters that are set at compile time and where those
differ from the parameters that are expected for a particular compliance
profile.

.. NOTE::

   Enforcement and validation are two distinct capabilities and the settings of
   one do not affect the other. This allows you to validate against multiple
   profiles independently of any particular enforcement setting.

In the following example, we want to validate against the ``nist_800_53_r4`` profile:

.. code-block:: yaml

   ---
   compliance_markup::validate_profiles:
     - "nist_800_53_r4"

By default, reports will be generated on the Puppet server and written to the
**server's** ``vardir`` which, by default, is
``/opt/puppetlabs/server/data/puppetserver/simp/compliance_reports``. Only the
latest compliance report for a host is stored. The report is a YAML format that
may easily be offloaded for analysis and processing and fully documented in the
`simp-compliance_markup`_ Puppet module.

.. _`Hiera config file syntax`: https://puppet.com/docs/puppet/latest/hiera_config_yaml_5.html
.. _`perform a Hiera migration`: https://puppet.com/docs/puppet/latest/hiera_migrate.html
.. _`simp-compliance_markup`: https://github.com/simp/pupmod-simp-compliance_markup

.. _SIMP Compliance Engine:

The SIMP Compliance Engine
--------------------------

The :term:`SIMP Compliance Engine` allows users to enable both Puppet
parameter-level enforcement and validation of compliance with regulatory and
internal policies.

This capability is provided by the `simp-compliance_markup`_ puppet module and
will be renamed at some point in the future to `simp-compliance_engine` for
clarity.

.. IMPORTANT::

   Your system must be configured to use at least :term:`Hiera` version 5 for
   the Compliance Engine to function properly.

   SIMP 6.3+ supports this capability out of the box bue earlier versions may
   need to `perform a Hiera migration`_.

Enabling the SIMP Compliance Engine
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In the Puppet implementation, the Compliance Engine is a Hiera backend that
provides lookup capabilities into specially formatted :term:`SIMP compliance
profile` data sets.

.. NOTE::

   This document does not cover creating custom compliance profile data sets.
   For additional information see the `simp-compliance_markup`_ module.

The following example demonstrates how to enable the SIMP Compliance Engine:

.. code-block:: yaml

   ---
   version: 5
   defaults:
     datadir: "data"
     data_hash: "yaml_data"
   hierarchy:
     - name: "SIMP Compliance Engine"
       lookup_key: "compliance_markup::enforcement"

Remember that hierarchy lookups happen from top to bottom and stop at the first
match. This means that if the SIMP Compliance Engine is in enforcing mode and
is at the top of the hierarchy, then it cannot be overridden except by an
:term:`ENC`. See the `Hiera config file syntax`_ page for additional
information.

Enforcing a Specific Profile
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The selection of profiles to enforce is based on the ``lookup_key`` entry in
the ``hiera.yaml`` configuration as shown in the previous section.

In this case, the ``lookup_key``, is ``compliance_markup::enforcement`` and
should be set in Hiera as an Array of compliance profiles that you would like
to apply, in priority order.

At this time, SIMP CE comes with two publicly available profiles, ``disa_stig``
and ``nist_800_53_rev4``.

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

Code Level Compliance Validation
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
latest compliance report for a host is stored. The report is a simple YAML
format that may easily be offloaded for analysis and processing.

.. _`Hiera config file syntax`: https://puppet.com/docs/puppet/latest/hiera_config_yaml_5.html
.. _`perform a Hiera migration`: https://puppet.com/docs/puppet/5.0/hiera_migrate_v3_yaml.html
.. _`simp-compliance_markup`: https://github.com/simp/pupmod-simp-compliance_markup

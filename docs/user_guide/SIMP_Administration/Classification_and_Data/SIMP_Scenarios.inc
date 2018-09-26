.. _simp scenarios:

SIMP Scenarios
--------------

SIMP scenarios are groups of classes, settings, and simp_options that ensure the
system is compliant and secure.

There are currently four SIMP scenarios:
- *simp*
- *simp_lite*
- *poss*
- *remote_access*

The *simp* scenario includes all security features enabled by default, including
iptables and svckill. This scenario is what stock SIMP used to look like in
previous releases.

The *simp_lite* scenario offers many security features, with a few explicitly
turned off. This scenario was designed to make it easier to implement SIMP in an
existing environment, because it might not be trivial to flip SELinux to
Enforcing on all nodes.

The *poss* option is the barebones option. It only includes the ``pupmod``
class, to configure Puppet agent on clients. All of the simp_options default to
false, so SIMP will not do a lot of modification to clients through Puppet when
using this scenario.

The *remote_access* scenario includes the SSH module and the authentication
stack, namely PAM and nsswitch. This scenario is useful for those who want
to retain remote access to their machine while leaving virtually everything
else untouched.

.. NOTE::

   The SIMP or Puppet master is exempt from most of these settings, and will be
   using most features from the *simp* scenario by default. The SIMP server
   should only have services on it related to Puppet and systems management,
   and SIMP modules all work with all security features enabled. See the
   ``puppet.your.domain.yaml`` in the ``hieradata/hosts`` directory for
   details.

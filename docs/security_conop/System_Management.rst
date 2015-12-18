Information System Management
=============================

This chapter contains SIMP security concepts that are related to the
management security controls in `NIST 800-53 <http://csrc.nist.gov/publications/PubsSPs.html>`__.

Risk Assessment
---------------

This section describes the process of identifying risks within a system.

SIMP Self Risk Assessment
-------------------------

Risk can be found in any system. The SIMP team is constantly evaluating
the system and the settings to minimize inherit risk. Most risks can be
mitigated by processes and procedures at the implementation level. The
following table describes the known areas in SIMP. [RA-1]

.. list-table::
  :header-rows: 1

  - * Risk
    * Possible Mitigations
  - * **Disabling Puppet**: This can cause the clients to be out of sync with the Puppet Master.
    * SIMP attempts to force a break on any locks and restart Puppet on all clients after a time of 4*runinterval (30 minutes by default). Implementations should ensure that further steps have not been taken to disable Puppet and should monitor their logs. Administrators can use the puppetlast command on the Puppet Master to detect servers that have not checked in within a reasonable time period.
  - * **Out of Date Patches**: SIMP can be built with the RPMs from CentOS or Red Hat. Those RPMs should be assumed out of date at the time a system is initially installed (if using the SIMP DVD).
    * Implementations should obtain the latest RPMs and apply them in a reasonable manner. All SIMP systems will, by default, attempt to update all packages using YUM nightly. Therefore, having an updated repository will ensure that the systems are updated on a regular basis.
  - * **Poor Account Management**: SIMP security access control is based on users being created and managed over time. Giving shell access to unnecessary users allows them the opportunity to escalate privileges.
    * Use the default LDIFs and local user modules to ensure that account settings remain restrictive. Ensure the system has policies and procedures in place to manage accounts. Finally, ensure that users are in appropriate groups with limited privileges.

Table: SIMP Risk

Vulnerability Scanning
----------------------

The SIMP development and security team performs regular vulnerability
scanning of the product using commercial and open source tools. Results
and mitigations for findings from those tools can be provided upon
request. [CA-2, RA-5]

Security Assessment and Authorization
-------------------------------------

Assessment and authorization varies by implementation. Implementations
are encouraged to use documentation artifacts provided by the SIMP team
to assist with assessment and authorization. [CA-2]

.. NOTE::

   This is an **example report template** to be used when responding to SCAP
   Scan results.

   The following is a short example from a CentOS 7 scan. You will need to
   adjust all content as appropriate.

TEMPLATE - SSG Scan - EL 7 STIG
===============================

* Scan Date: 1/1/1970
* SIMP Version: ``6.1.0-RC1``
* SSG Version: ``0.1.36``
* Data Stream: ``ssh-centos7-ds.xml``
* SIMP Enforcement Profile: ``disa_stig``

-------------------------------------------------------------------------------

**Format:**

-  Rule ID: OvalID in xccdf notation corresponding to the failed test
-  Type:
  -  Finding: Valid issues found by the scanner
  -  Alternate Implementation: Valid implementations per policy that do not
     match the scan
  -  Exception: Items that will need to be declared as a policy exception for
     the stated reason
  -  False Positive: Bugs in the scanner that should be reported
-  Notes: Provide recommended feedback to SIMP, SSG, RedHat, etc. and any other
   miscellany
-  Remediation: Provide guidance for fixing or justifying failed test

-------------------------------------------------------------------------------

* Rule ID: ``xccdf_org.ssgproject.content_rule_ensure_gpgcheck_repo_metadata``
* Type: **Exception**
* Notes:
    - **Recommend SSG Feedback**
      -  This should not be a ``high`` severity if using TLS
      -  This opens potential vulnerabilities to all client systems
      -  Discussion ongoing on SSG mailing list and `OpenSCAP/scap-security-guide#2455`_
* Remediation:

The way that YUM works means that all GPG keys become **trusted** by the entire
system. Enabling repository metadata signatures globally means that RPMs will
be trusted that come from any system with a trusted GPG key and may allow
software to be installed on systems that does not come from the vendor.

-------------------------------------------------------------------------------

* Rule ID: ``xccdf_org.ssgproject.content_rule_aide_periodic_cron_checking``
* Type: **Alternate Implementation**
* Remediation:

We use the Puppet ``cron`` resource to add the AIDE rule to the ``root`` user's
``crontab``.

System result:

.. code-block:: bash

   # crontab -l

   # Puppet Name: aide_schedule                        5 4 *
   * 0 /bin/nice -n 19 /usr/sbin/aide -C

-------------------------------------------------------------------------------

* Rule ID: ``xccdf_org.ssgproject.content_rule_aide_build_database``
* Type: **False Positive**
* Remediation:

System result:

.. code-block:: bash

   # ls /var/lib/aide/
   aide.db.gz

-------------------------------------------------------------------------------

* Rule ID: ``xccdf_org.ssgproject.content_rule_audit_rules_login_events_faillock``
* Type: **Finding**
* Remediation:

Marked as a valid finding and tracked as `SIMP-4047`_

.. _OpenSCAP/scap-security-guide#2455: https://github.com/OpenSCAP/scap-security-guide/issues/2455
.. _SIMP-4047: https://simp-project.atlassian.net/browse/SIMP-4047

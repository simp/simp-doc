.. _faq_logrotate:

Why Does Logrotate Complain About Repeated Configuration Settings
=================================================================

As of SIMP 6.2.0, SIMP-managed :program:`logrotate` rules are now in
:file:`/etc/logrotate.simp.d` instead of :file:`/etc/logrotate.d`.  The rules in
:file:`/etc/logrotate.d` are still applied, but :program:`logrotate` is configured to read
the rules in :file:`/etc/logrotate.simp.d` first.

This change was made to ensure SIMP-managed rules take precedence over
vendor-supplied rules. When multiple rules are specified for the same file, only
the first rule is applied. Any subsequent rules are discarded.

.. NOTE::

   For some versions of :program:`logrotate`, a rule with a duplicate is discarded *in
   its entirety*, even if only one of the managed log files is a duplicate.
   This means the remaining log files specified in that discarded rule will
   *not* be rotated!

Because the location of the SIMP-managed :program:`logrotate` rules has changed,
any previously existing (but now obsolete) SIMP rules will still reside in
:file:`/etc/logrotate.d`.  Although these rules cause no issues with :program:`logrotate`,
they may be confusing to system administrators.  So, you may wish to manually
remove these rules.

The following script can be used to identify obsolete SIMP :program:`logrotate` rules
and they can be removed manually as necessary.

.. code-block:: sh

   grep -rl 'managed by puppet' /etc/logrotate.d

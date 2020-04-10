.. _faq_logrotate:

Why Does Logrotate Complain About Repeated Configuration Settings
=================================================================

As of SIMP 6.2.0, SIMP-managed ``logrotate`` rules are now in
``/etc/logrotate.simp.d`` instead of ``/etc/logrotate.d``.  The rules in
``/etc/logrotate.d`` are still applied, but ``logrotate`` is configured to read
the rules in ``/etc/logrotate.simp.d``, first.

This change was made to ensure SIMP-managed rules take precedence over
vendor-supplied rules. When multiple rules are specified for the same file, only
the first rule is applied. Any subsequent rules are discarded.

.. NOTE::

   For some versions of ``logrotate``, a rule with a duplicate is discarded *in
   its entirety*, even if only one of the managed log files is a duplicate.
   This means the remaining log files specified in that discarded rule will
   *not* be rotated!

Because the location of the SIMP-managed ``logrotate`` rules has changed,
any previously existing (but now obsolete) SIMP rules will still reside in
``/etc/logrotate.d``.  Although these rules cause no issues with ``logrotate``,
they may be confusing to system administrators.  So, you may wish to manually
remove these rules.

The following script can be used to identify obsolete SIMP ``logrotate`` rules
and they can be removed manually as necessary.

.. code-block:: sh

   grep -rl 'managed by puppet' /etc/logrotate.d

.. _gsg-contributors_guide-documentation:

Documentation
=============

.. contents:: :local:
   :depth: 3

Style Guides
------------

SIMP documentation (:github:`simp-doc`)
"""""""""""""""""""""""""""""""""""""""

SIMP documentation uses `ReStructuredText roles`_ to keep formatting
consistent, automatically cross-reference glossary terms, and generate external
hyperlinks to web resources.

When available, prefer using a relevant role from the tables below over inline
formatting such as bold ``*text*`` or double-backticks (``````):

.. _ReStructuredText roles: https://www.sphinx-doc.org/en/master/usage/restructuredtext/roles.html

.. csv-filter:: Roles already built into Sphinx
     :header-rows: 1
     :widths: 20 20 25 35
     :file: documentation_custom_roles.csv
     :delim: |
     :included_cols: 0,1,2,3
     :include: {4: '(?i)^built-in$'}

.. csv-filter:: Custom roles, created for simp-doc
     :header-rows: 1
     :widths: 20 20 25 35
     :file: documentation_custom_roles.csv
     :delim: |
     :included_cols: 0,1,2,3
     :include: {4: '(?i)^simp-only$'}

.. csv-filter:: Inline formatting
     :header-rows: 1
     :widths: 20 20 25 35
     :file: documentation_custom_roles.csv
     :delim: |
     :included_cols: 0,1,2,3
     :include: {4: '(?i)^inline$'}

Markup conventions
^^^^^^^^^^^^^^^^^^

* Write admonitions in upper case (``.. NOTE::``, ``.. WARNING::``).
* Use ``.. code-block:: bash`` for shell commands.
* A named hyperlink reference needs a trailing underscore
  (```hardware requirements`_``).  Without it, the text renders as italics
  and links nowhere.
* Fix broken markup on the lines next to the ones you change.

Commands
^^^^^^^^

Readers copy and paste commands, so each one has to work exactly as written.

* Give the whole command.  Do not give a filename or a fragment and leave the
  reader to build the rest.
* Do not put placeholders such as ``<N>`` in a command when the shell can fill
  the value in.  Use :code:`$(rpm -E %rhel)` for the OS major version and
  :code:`$(arch)` for the architecture.
* When a command does not work on every OS that the page covers, give one
  labelled code block per OS.  Do not show one command for everyone and list
  the exceptions in a note.
* Run every command on each OS release that the page covers before you
  submit it.  A container is enough for package and repository commands.
* Use :program:`dnf`, not :program:`yum`, on pages that cover EL8+.

Technical claims
^^^^^^^^^^^^^^^^

* Check a claim against the system before you write it down.  For package
  relationships, run :command:`dnf repoquery --provides --obsoletes --conflicts
  <package>`.
* Say what the reader will see, then what to do about it.  Avoid sentences
  whose subject is unclear, such as "does not install it".
* A default in the docs has to match the default in the module.  When the
  docs recommend a different value, name the parameter that the reader has
  to set.

Product and package names
^^^^^^^^^^^^^^^^^^^^^^^^^

* The products are OpenVox, OpenVox Server and OpenVoxDB.
* The packages are :package:`openvox-agent`, :package:`openvox-server` and
  :package:`openvoxdb`.
* Some names did not change with the fork, such as the ``puppetserver``
  service and the paths under :file:`/etc/puppetlabs`.  Keep those as they
  are.

Links
^^^^^

* Link to the ``latest`` version of external documentation.  Pin a version
  only when the text is about that version.
* Do not link to resources for an end-of-life OS as the example to follow.
  When no current example exists, link to the closest one and say which
  releases it does not cover.
* Check that every link you add or change resolves.

Following the reader
^^^^^^^^^^^^^^^^^^^^

When you change the OS releases or the packages that a page covers, read
every page that it sends the reader to as a next step.

* Update a linked page that no longer fits, when that is in scope.
* Otherwise, add a ``.. WARNING::`` at the link that says what does not work
  yet.  Do not leave the reader to find the gap by running into it.

Before you submit
^^^^^^^^^^^^^^^^^

* Build the docs with :command:`bundle exec rake docs:html`.
* The CI build does not fail on warnings, and a reStructuredText error can
  silently drop content.  Compare the warning count with a build of the
  ``master`` branch.
* Open the pages you changed in the built HTML and check that they render as
  you intended.

SIMP Puppet modules
"""""""""""""""""""

Documentation for Puppet modules should follow the `Puppet Strings Style Guide`_.

.. _Puppet Strings Style Guide: https://puppet.com/docs/puppet/latest/puppet_strings_style.html

SIMP git repositories
"""""""""""""""""""""

SIMP project git repositories should contain a :file:`README.md` file at their
top level.



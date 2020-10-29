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

SIMP Puppet modules
"""""""""""""""""""

Documentation for Puppet modules should follow the `Puppet Strings Style Guide`_.

.. _Puppet Strings Style Guide: https://puppet.com/docs/puppet/latest/puppet_strings_style.html

SIMP git repositories
"""""""""""""""""""""

SIMP project git repositories should contain a :file:`README.md` file at their
top level.



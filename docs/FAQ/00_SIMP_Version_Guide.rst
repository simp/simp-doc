.. _faq-simp_version_guide:

SIMP Version Guide
==================

The SIMP versioning system has caused some confusion over time and this
document serves as the authoritative reference for clarification.

Top-Level SIMP for 6.X+
-----------------------

.. NOTE::

   This is the version number that you get when you run :command:`rpm -q simp`

The top level SIMP version for SIMP releases from 6.0.0 onward will be
following `Semantic Versioning 2.0.0`_.

In short, this means (from the reference):

Given a version number ``MAJOR.MINOR.PATCH``, increment the:

#. ``MAJOR`` version when you make incompatible API changes
#. ``MINOR`` version when you add functionality in a backwards-compatible manner
#. ``PATCH`` version when you make backwards-compatible bug fixes

Sub-Component Versioning
------------------------

For all versions of SIMP, sub-components generally follow `Semantic Versioning 2.0.0`_.

.. _Semantic Versioning 2.0.0: https://semver.org/spec/v2.0.0.html

.. _faq-simp_version_guide:

SIMP Version Guide
==================

The SIMP versioning system has caused some confusion over time and this
document serves as the authoritative reference for clarification.

Top-Level SIMP for 6.X+
-----------------------

.. NOTE::
  This is the version number that you get when you run `rpm -q simp`

The top level SIMP version for SIMP releases from 6.0.0 onward will be
following `Semantic Versioning 2.0.0`_.

In short, this means (from the reference):

| Given a version number `MAJOR.MINOR.PATCH`, increment the:
|
| #. MAJOR version when you make incompatible API changes
| #. MINOR version when you add functionality in a backwards-compatible manner
| #. PATCH version when you make backwards-compatible bug fixes

Top-Level SIMP for SIMP before 6.X
----------------------------------

.. NOTE::
  This is the version number that you get when you run `rpm -q simp`

The top level SIMP version for SIMP releases prior to the 6.0.0 release have
the following structure given the format `MAJOR.MINOR.PATCH`:

| #. MAJOR version when the version of EL changes
| #. MINOR version when you make incompatible API changes
| #. PATCH version when you add functionality in a backwards-compatible manner
| #. FIXES version when you make backwards-compatible bug fixes


The last releases mapped in this manner are as follows:

* 5.X => EL 7
* 4.X => EL 6

Sub-Component Versioning
------------------------

For all versions of SIMP, sub-components follow `Semantic Versioning 2.0.0`_.

.. _Semantic Versioning 2.0.0: http://semver.org/spec/v2.0.0.html

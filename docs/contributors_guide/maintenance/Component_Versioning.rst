Component Versioning
====================

Version Philosophy
------------------

SIMP follows Semantic Versioning 2.0.0 and has the following versioning
structure: ``X.Y.Z``, where

* ``X`` indicates breaking changes
* ``Y`` indicates new features
* ``Z`` indicates bug fixes.


When can a component be released?
---------------------------------

A component can be released when

* ``X``, ``Y``, or ``Z`` changes have been made.
* All dependencies of the component has been released.
* If a SIMP-owned component, all unit, acceptance, and integration tests
  pass.
* If a SIMP-owned component, the version number has been appropriately
  bumped and the corresponding changelog has been updated.

The SIMP project version/changelog files are as follows:

+-------------------+-------------------------------------+-------------------------------------+
| Component Type    | Version Files                       | Changelog Files                     |
+===================+=====================================+=====================================+
| SIMP-owned Puppet | ``metadata.json`` and ``CHANGELOG`` | ``CHANGELOG``                       |
| module            |                                     |                                     |
+-------------------+-------------------------------------+-------------------------------------+
| Ruby gem          | ``lib/simp/\*\*/version.rb``        | ``build/<name>.spec``               |
|                   | and either                          | or ``CHANGELOG.md``                 |
|                   | ``build/<name>.spec`` or            |                                     |
|                   | ``CHANGELOG.md``                    |                                     |
+-------------------+-------------------------------------+-------------------------------------+
| Other ISO-related | ``build/<name>.spec``               | ``build/<name>.spec``               |
| project           |                                     |                                     |
+-------------------+-------------------------------------+-------------------------------------+
| ``simp-doc``      | auto-generated                      | ``CHANGELOG``                       |
+-------------------+-------------------------------------+-------------------------------------+
| SIMP ISO          | ``Changelog.rst`` and               | ``Changelog.rst`` and               |
| (``simp-core``)   | ``src/assets/simp/build/simp.spec`` | ``src/assets/simp/build/simp.spec`` |
+-------------------+-------------------------------------+-------------------------------------+

What file changes require a version change?
-------------------------------------------

Any changes to mission impacting (significant) files require a new
release. In general, this includes the ``metadata.json``, ``CHANGELOG``
and ``hiera.yaml`` files for Puppet modules, as well as files in the
following directories:

* ``build/``
* ``data/``
* ``files/``
* ``functions/``
* ``lib/``
* ``manifests/``
* ``scripts/``
* ``share/``
* ``src/``
* ``templates/``
* ``types/``

Changes to the following do not typically warrant
a new release of a component:

* Any hidden file/directory (entry that begins with a ``.`` such as
  ``.rspec``, ``.gitignore``, ``.gitlab-ci.yml``)
* ``Gemfile``
* ``Gemfile.lock``
* ``Rakefile``
* ``spec/``
* ``doc/``

What version/changelog linters are available?
---------------------------------------------

In the ``simp-rake-helpers`` Ruby gem, we have the following
version/changelog-related linters for SIMP Puppet modules:

* ``pkg:create_tag_changelog``:
  Generates an appropriate annotated tag entry from a ``CHANGELOG``.
  The results should be carefully examined to ensure the output is correct,
  because processing stops at the first invalid changelog entry.

* ``pkg:compare_latest_tag``:
  Compares mission-impacting files with the latest tag and identifies
  the relevant files that have changed.  When mission-impacting files
  have changed, fails if:

  #. Latest version cannot be extracted from the top-most
     ``CHANGELOG`` entry.
  #. The latest version in the ``CHANGELOG`` (minus the release
     qualifier) does not match the version in the ``metadata.json``
     file.
  #. A version bump is required but not recorded in both the
     ``CHANGELOG`` and ``metadata.json`` files.
  #. The latest version is smaller than the latest tag (version regression).

* ``pkg::check_version``: Compares all files with the closest
  tag and logs an error if any files have changed, but the version
  has not been updated, or the versions in the ``metadata.json`` and
  ``CHANGELOG`` files do not match.

.. NOTE::

   Moving forward, these linters will be enhanced to handle the
   version/changelog nuances of the other projects SIMP releases and
   will be included as tests in all TravisCI builds.

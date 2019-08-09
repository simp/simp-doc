[![License](http://img.shields.io/:license-apache-blue.svg)](http://www.apache.org/licenses/LICENSE-2.0.html)
[![Build Status](https://travis-ci.org/simp/simp-doc.svg)](https://travis-ci.org/simp/simp-doc)

<!-- vim-markdown-toc GFM -->

* [SIMP Documentation](#simp-documentation)
* [Usage](#usage)
  * [Building the docs](#building-the-docs)
  * [Maintaining the data](#maintaining-the-data)
* [Environment variables](#environment-variables)
  * [`SIMP_FAST_DOCS=true`](#simp_fast_docstrue)
    * [`LINKCHECK_IGNORE_LIST`](#linkcheck_ignore_list)
  * [`SIMP_CORE_PATH`](#simp_core_path)
  * [`SIMP_BRANCH`](#simp_branch)
* [Managing Sphinx prerequisites with `pip`](#managing-sphinx-prerequisites-with-pip)

<!-- vim-markdown-toc -->

## SIMP Documentation

The repository for the SIMP documentation.

It is a component of the [System Integrity Management Platform](https://github.com/NationalSecurityAgency/SIMP), a
compliance-management framework built on Puppet.

If you find any issues, they can be submitted to our [JIRA](https://simp-project.atlassian.net/).

Please read our [Contribution Guide](https://simp-project.atlassian.net/wiki/display/SD/Contributing+to+SIMP)
and visit our [developer wiki](https://simp-project.atlassian.net/wiki/display/SD/SIMP+Development+Home).

## Usage

### Building the docs
```bash
# build HTML docs, dumps the resulting files in doc/;
rake docs:html

# build HTML docs, specifying a local simp-core location
SIMP_CORE_PATH=<path to local simp-core git repo> rake docs:html

# build HTML docs for a specific SIMP version, as specified by a
# tag/branch
SIMP_BRANCH='6.0.0-0' rake docs:html

# run a local web server to view HTML docs on http://localhost:5000
rake docs:server[port]
```

If you want to build the RPM on EL6 systems, you will need to ensure that the
SCL and python versions are appropriately available using the following
repositories and check the RPM spec file for the relevant required packages.

```python
[centos-sclo-sclo]
name=CentOS-6 - SCLo sclo
baseurl=http://mirror.centos.org/centos/6/sclo/$basearch/sclo/
gpgcheck=1
enabled=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-SIG-SCLo

[centos-sclo-rh]
name=CentOS-6 - SCLo rh
baseurl=http://mirror.centos.org/centos/6/sclo/$basearch/rh/
gpgcheck=1
enabled=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-SIG-SCLo

[extras]
name=CentOS-$releasever - Extras
baseurl=http://mirror.centos.org/centos/$releasever/extras/$basearch/
gpgcheck=1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-6
```

### Maintaining the data
```
# Update the RPM lists
rake docs:rpm:external

# Update the SIMP RPM list
rake docs:rpm:simp
```

## Environment variables

### `SIMP_FAST_DOCS=true`

Strips out certain sections and dynamic content that cause the build to be
slow or cumbersome:

* Excludes everything under `docs/security_mapping/*` (causes WARNINGS)
* Skips all logic to download/load the Known OS Mappings section, and
  instead replaces it with some (static) placeholder content.

This should not be used for real documentation builds, but is great for 90% of
all testing. (In testing the 'html' target, this took the build time on a fast
system from 40s to 9s.)

NOTE: When this is enabled, expect errors from `sphinx-build` in the form of:

(Sphinx 1.7:)

       WARNING: toctree contains reference to nonexisting document u'security_conop/index'

(Sphinx 1.8:)

       WARNING: toctree contains reference to excluded document u'security_mapping/index
       WARNING: undefined label: cm-2 (if the link has no caption the label must precede a section header)

#### `LINKCHECK_IGNORE_LIST`

A space-delimited list of URLs to ignore during `sphinx-build -b linkcheck`
tests.  This is useful to temporarily ignore URLs that are intermittently
reported as broken from our CI hosts (e.g., after they trip the sites' DDOS
protection), but validate from other locations.


### `SIMP_CORE_PATH`

Sets an path to a local simp-core git repository.  Can be used to test new
revisions of the simp-core `Changelog.rst`.  This defaults to `../..`
under the assumption that this repository has been checked out as part of
the `simp-core` SIMP ISO build.

```bash
SIMP_CORE_PATH=$PWD/../simp-core rake docs:html
```

### `SIMP_BRANCH`

Sets the simp-core github tag/branch for the version of docs to build,
when a local simp-core repository is not to be used. The version, release,
and changelog information will be downloaded for that version to build the
docs.  This defaults to `master`.

`SIMP_BRANCH=6.0.0-1 rake docs:html`

CAUTION:  This branch is used **only** to determine version information.
It does **not** result in the simp-docs version that corresponds to
SIMP_BRANCH to be downloaded and built.

## Managing Sphinx prerequisites with `pip`

If you are using PyPi to manage python packages, you may need to run something like the following:

```bash
# simp install
sudo pip install -q -r requirements.txt

# upgrading (needed in some cases when rst2pdf prereqs get stuck)
sudo pip install --upgrade --force-reinstall  -r requirements.txt -v
```

The prerequisites for **rst2pdf** may require extra OS packages:

```bash
# fedora 24
sudo dnf install -y libjpeg-devel zlib-devel python-devel

# centos 7
sudo yum install -y libjpeg-devel zlib-devel python-devel

# Debian 8.5 (containers)
sudo apt-get install -y python-dev zlib1g-dev libjpeg-dev

```

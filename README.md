[![License](http://img.shields.io/:license-apache-blue.svg)](http://www.apache.org/licenses/LICENSE-2.0.html) [![Build Status](https://travis-ci.org/simp/simp-doc.svg)](https://travis-ci.org/simp/simp-doc)


* [SIMP Documentation](#simp-documentation)
* [Usage](#usage)
  * [Building the docs](#building-the-docs)
  * [Maintaining the data](#maintaining-the-data)
* [Environment variables](#environment-variables)
  * [`SIMP_CHANGELOG_PATH`](#simp_changelog_path)
  * [`SIMP_BRANCH`](#simp_version)
* [Managing Sphinx prerequisites with `pip`](#managing-sphinx-prerequisites-with-pip)

## SIMP Documentation

The repository for the SIMP documentation.

It is a component of the [System Integrity Management Platform](https://github.com/NationalSecurityAgency/SIMP), a compliance-management framework built on Puppet.

If you find any issues, they can be submitted to our [JIRA](https://simp-project.atlassian.net/).

Please read our [Contribution Guide](https://simp-project.atlassian.net/wiki/display/SD/Contributing+to+SIMP) and visit our [developer wiki](https://simp-project.atlassian.net/wiki/display/SD/SIMP+Development+Home).

## Usage

### Building the docs
```bash
# build HTML docs
rake docs:html

# build HTML docs, using an alternate Changelog.rst location
SIMP_CHANGELOG_PATH=$PATH_TO_OTHER/Changelog.rst rake docs:html

# run a local web server to view HTML docs on http://localhost:5000
rake docs:server[port]
```

### Maintaining the data
```
# Update the RPM lists
rake docs:rpm:external

# Update the SIMP RPM list
rake docs:rpm:simp
```


## Environment variables

### `SIMP_CHANGELOG_PATH`

Sets an alternate path to the simp-core `Changelog.rst` file.  Use this variable to test new revisions of `Changelog.rst`.  This defaults to `../../../Changelog.rst` under the assumption that this repository has been checked out as part of the `simp-core` SIMP ISO build.

```bash
SIMP_CHANGELOG_PATH=$PWD/../simp-core/Changelog.rst rake docs:html
```

### `SIMP_BRANCH`

`SIMP_BRANCH=5.1.X rake munge:prep`

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
sudo dnf install -y libjpeg-devel zlib-devel

# centos 7
sudo yum install -y libjpeg-devel zlib-devel

# Debian 8.5 (containers)
sudo apt-get install -y python-dev zlib1g-dev libjpeg-dev

```


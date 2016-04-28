%if 0%{?el6}
%define simp_major_version 4
%else
%define simp_major_version 5
%endif

Summary: SIMP Documentation
Name: simp-doc
Version: 4.2.0
Release: 3.Alpha.2
License: Apache License, Version 2.0
Group: Documentation
Source: %{name}-%{version}-%{release}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Buildarch: noarch
Requires: links
%if 0%{?el6}
BuildRequires: scl-utils
BuildRequires: python27
%endif
BuildRequires: python-pip
BuildRequires: python-virtualenv
BuildRequires: fontconfig
BuildRequires: dejavu-sans-fonts
BuildRequires: dejavu-sans-mono-fonts
BuildRequires: dejavu-serif-fonts
BuildRequires: dejavu-fonts-common
BuildRequires: libjpeg-devel
BuildRequires: zlib-devel

%description
Documentation for SIMP %{version}-%{release}

You can access the documentation on a text-based system using the
command 'simp doc'.

Alternatively, you can read the docs at https://simp.readthedocs.org

%prep
%setup -q

%build
# We need the latest version of sphinx and rst2pdf
# Make sure we play nice with our neighbors...

%if 0%{?el6}
# We can't use the normal SCL commands in mock so we do this manually!
source /opt/rh/python27/enable
%endif

virtualenv venv
source venv/bin/activate
pip install --upgrade -r requirements.txt

sphinx-build -E -n -t simp_%{simp_major_version} -b html -d sphinx_cache docs html
sphinx-build -E -n -t simp_%{simp_major_version} -b singlehtml -d sphinx_cache docs html-single

# Rst2pdf is currently broken on references so we have to remove them.
# This should be removed when this bug is fixed.
find docs -name "*.rst" -exec sed -i 's/:ref://g' {} \;

sphinx-build -E -n -t simp_%{simp_major_version} -b pdf -d sphinx_cache docs pdf

if [ ! -s pdf/SIMP_Documentation.pdf ]; then
  echo "ERROR: Could not generate PDF"
  exit 1
else
  mv pdf/SIMP_Documentation.pdf pdf/SIMP-%{version}-%{release}.pdf
fi

if [ ! -d changelogs ]; then
  mkdir changelogs
fi

if [ ! -f pdf/SIMP_Documentation.pdf ] || [ `stat --printf="%s" pdf/SIMP_Documentation.pdf` ]; then
  echo "Error: PDF output has size 0"
  exit 1
fi

%install
# Just the Docs...

%clean
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}

%files
%defattr(0644,root,root,0755)
%doc Changelog.rst html html-single pdf ldifs

%post
# Post install stuff

%postun
# Post uninstall stuff

%changelog
* Thu Apr 28 2016 Nick Markowski <nmarkowski@keywcorp.com> - 4.2.0-3.Alpha.2
- Updated kickstart docs to use https.

* Tue Apr 05 2016 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.2.0-3.Alpha.1
- Prepare for the next release
- Changed the tftpboot docs to use https.
- Fixed a bug that was preventing PDF builds
- Updated the RPM build to fail if the resulting PDF is size 0

* Wed Dec 16 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.2.0-1
- Doc updates for 4.2.0-1

* Wed Nov 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.2.0-0
- Updated the default passwords to be easier overall

* Tue Sep 22 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.2.0-RC1
- Updated the release materials for 4.2.0-RC1

* Tue Aug 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.2.0-Beta2
- Updated the spec file to properly build the docs.

* Fri Jul 31 2015 Judy Johnson <judy.johnson@onyxpoint.com> - 4.2.0-Beta2
- Converted docs from Publican to ReStructured Text.

* Wed Mar 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.2.0-Alpha
- Added text to cover the move to the new Puppet Server and the migration to
  Environments.
- Removed some old material.

* Sat Dec 20 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-1
- Final release of 4.1.0-1

* Tue Nov 25 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-0
- Final release of 4.1.0

* Mon Sep 08 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-RC3
- Updated the changelog to add the RC3 changes.

* Wed Aug 06 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-RC2
- Updated the changelog to refelct the changes in RC2.

* Tue Jul 08 2014 Nick Markowski <nmarkowski@keywcorp.com> - 4.1.0-RC1
- Updated the user guide to reflect changes in the installation guide, primarily
  due to Hiera.

* Thu May 01 2014 Nick Markowski <nmarkowski@keywcorp.com> - 4.1.0-Beta
- Made the following modifications to the installation guide
-  Added a section outlining Hiera and updated all chapters for Hiera
-  Updated the Changelog for Beta
-  Removed user management chapter, now contained only in user guide
-  Modified previously included external content for consistency

* Thu Apr 03 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha3
- Updated the Changelog for Alpha3

* Fri Jan 10 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- Added a script for converting LDAP users to InetOrgPerson entries
  and updated the LDIFs to account for such.

* Mon Nov 25 2013 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- First release of 4.1.0-Alpha2


%if 0%{?el6}
%define simp_major_version 4
%else
%define simp_major_version 5
%endif

Summary: SIMP Documentation
Name: simp-doc
Version: 5.1.0
Release: RC1.1446213493
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
pip install --upgrade sphinx
pip install --upgrade rst2pdf
pip install --upgrade pillow
pip install --upgrade svglib

sphinx-build -E -n -t simp_%{simp_major_version} -b html       -d sphinx_cache docs html
sphinx-build -E -n -t simp_%{simp_major_version} -b singlehtml -d sphinx_cache docs html-single

# Rst2pdf is currently broken on references so we have to remove them.
# This should be removed when this bug is fixed.
find docs -name "*.rst" -exec sed -i 's/:ref://g' {} \;

sphinx-build -E -n -t simp_%{simp_major_version} -b pdf        -d sphinx_cache docs pdf

if [ ! -d changelogs ]; then
  mkdir changelogs
fi

mv pdf/SIMP_Documentation.pdf pdf/SIMP-%{version}-%{release}.pdf

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
* Tue Sep 22 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.1.0-RC1
- Preparing for the 5.1.0-RC1 release.

* Tue Aug 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.1.0-Beta2
- Updated the spec file to properly build the docs.

* Fri Jul 31 2015 Judy Johnson <judy.johnson@onyxpoint.com> - 5.1.0-Beta2
- Converted docs from Publican to ReStructured Text.

* Wed Mar 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.1.0-Alpha
- Added text to cover the move to the new Puppet Server and the migration to
  Environments.
- Removed some old material.

* Sat Dec 20 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-2
- Update the changelog for 5.0.0-2

* Mon Dec 08 2014 Kendall Moore <kmoore@keywcorp.com> - 5.0.0-1
- No longer suggest grub-crypt to encrypt passwords and instead use a simple
  ruby script.

* Tue Nov 25 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-0
- Final release of 5.0.0

* Wed Oct 29 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-RC1
- Updated the Changelog for the 5.0.0-RC1 release.

* Thu Aug 07 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-Beta
- Updated for the 5.0.0-Beta release

* Fri Jan 10 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- Added a script for converting LDAP users to InetOrgPerson entries
  and updated the LDIFs to account for such.

* Mon Nov 25 2013 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- First release of 4.1.0-Alpha2

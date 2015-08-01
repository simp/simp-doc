%define simp_major_version 4

Summary: SIMP Documentation
Name: simp-doc
Version: 4.2.0
Release: Beta2
License: Apache License, Version 2.0
Group: Documentation
Source: %{name}-%{version}-%{release}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Buildarch: noarch
Requires: links
BuildRequires: sphinx >= 1.2
BuildRequires: python-sphinx >= 0
Prefix: /usr/share/doc/simp-%{version}

%description
Documentation for SIMP %{version}-%{release}

You can access the documentation on a text-based system using the
command 'simp doc'. Alternatively, a PDF is provided in
%{prefix}/pdf

%prep
%setup

%build
sphinx-build    -n -t simp_%{simp_major_version} -b html       -d sphinx_cache docs html
sphinx-build -E -n -t simp_%{simp_major_version} -b singlehtml -d sphinx_cache docs html-single

%install
mkdir -p %{buildroot}%{prefix}
src_dirs="changelogs Changelog*.rst ldifs html"
for dir in $src_dirs; do
  if [ -e $dir ]; then
    cp -r $dir %{buildroot}%{prefix}
  fi
done

# Publican Material
mkdir -p %{buildroot}%{prefix}/html
cp -r "html/"        %{buildroot}%{prefix}/html/
cp -r "html-single/" %{buildroot}%{prefix}/html-single/

chmod -R u=rwX,g=rX,o=rX %{buildroot}%{prefix}

%clean
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%docdir %{prefix}
%{prefix}

%post
# Post install stuff

%postun
# Post uninstall stuff

%changelog
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

* Thu Aug 06 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-RC2
- Updated the changelog to refelct the changes in RC2.

* Wed Jul 08 2014 Nick Markowski <nmarkowski@keywcorp.com> - 4.1.0-RC1
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


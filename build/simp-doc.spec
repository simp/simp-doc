%define publican_lang en-US

Summary: SIMP Documentation
Name: simp-doc
Version: 4.1.0
Release: 1
License: Apache License, Version 2.0
Group: Documentation
Source: %{name}-%{version}-%{release}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Buildarch: noarch
Requires: links
BuildRequires: publican >= 2.1
BuildRequires: ruby

Prefix:"/usr/share/doc/simp-%{version}"

%description
Documentation for SIMP %{version}-%{release}

You can access the documentation on a text-based system using the
command 'simp doc'. Alternatively, a PDF is provided in
/usr/share/doc/simp-%{version}/pdf.

%prep
%setup

%build
cd publican
os_version=`ruby -e 'if File.read("/etc/redhat-release") =~ /(\d+(:?\.\d+)?)/ then puts $1 end'`
if [ ! -z "$os_version" ]; then
  find . -type f -exec sed -i "s/STUB_OS_VERSION/$os_version/" {} \;
fi

publican build --formats=html,html-single,pdf --langs=%{publican_lang}
cd -

%install
mkdir -p %{buildroot}/usr/share/doc/simp-%{version}
src_dirs="changelogs Changelog.txt examples ldifs html upgrade_utils"

for dir in $src_dirs; do
  if [ -e $dir ]; then
    cp -r $dir %{buildroot}/usr/share/doc/simp-%{version}
  fi
done

# Publican Material
mkdir -p %{buildroot}/usr/share/doc/simp-%{version}/html/users_guide
cp -r "build_docs/%{publican_lang}/html" %{buildroot}/usr/share/doc/simp-%{version}/html/users_guide
cp -r "build_docs/%{publican_lang}/html-single" %{buildroot}/usr/share/doc/simp-%{version}/html/users_guide
cp -r "build_docs/%{publican_lang}/pdf" %{buildroot}/usr/share/doc/simp-%{version}

chmod -R u=rwX,g=rX,o=rX %{buildroot}/usr/share/doc/simp-%{version}

%clean
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%docdir /usr/share/doc/simp-%{version}
/usr/share/doc/simp-%{version}

%post
# Post install stuff

%postun
# Post uninstall stuff

%changelog
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

* Fri Nov 25 2013 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- First release of 4.1.0-Alpha2

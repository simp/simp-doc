%define publican_lang en-US

Summary: SIMP Documentation
Name: simp-doc
Version: 5.1.0
Release: Alpha
License: Apache License, Version 2.0
Group: Documentation
Source: %{name}-%{version}-%{release}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Buildarch: noarch
Requires: links
BuildRequires: publican >= 2.1

Prefix: /usr/share/doc/simp-%{version}

%description
Documentation for SIMP %{version}-%{release}

You can access the documentation on a text-based system using the
command 'simp doc'. Alternatively, a PDF is provided in
%{prefix}/pdf

%prep
%setup

%build
cd publican
publican build --formats=html,html-single,pdf --langs=%{publican_lang}
cd -

%install
mkdir -p %{buildroot}%{prefix}
src_dirs="changelogs Changelog.txt examples ldifs html upgrade_utils"

for dir in $src_dirs; do
  if [ -e $dir ]; then
    cp -r $dir %{buildroot}%{prefix}
  fi
done

# Publican Material
mkdir -p %{buildroot}%{prefix}/html/users_guide
cp -r "build_docs/%{publican_lang}/html" %{buildroot}%{prefix}/html/users_guide
cp -r "build_docs/%{publican_lang}/html-single" %{buildroot}%{prefix}/html/users_guide
cp -r "build_docs/%{publican_lang}/pdf" %{buildroot}%{prefix}

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
* Wed Mar 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.1.0-Alpha
- Added text to cover the move to the new Puppet Server and the migration to
  Environments.
- Removed some old material.

* Sat Dec 20 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-2
- Update the changelog for 5.0.0-2

* Mon Dec 08 2014 Kendall Moore <kmoore@keywcorp.com> - 5.0.0-1
- No longer suggest grub-crypt to encrpy passwords and instead use a simple ruby script.

* Tue Nov 25 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-0
- Final release of 5.0.0

* Wed Oct 29 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-RC1
- Updated the Changelog for the 5.0.0-RC1 release.

* Thu Aug 07 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 5.0.0-Beta
- Updated for the 5.0.0-Beta release

* Fri Jan 10 2014 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- Added a script for converting LDAP users to InetOrgPerson entries
  and updated the LDIFs to account for such.

* Fri Nov 25 2013 Trevor Vaughan <tvaughan@onyxpoint.com> - 4.1.0-Alpha2
- First release of 4.1.0-Alpha2

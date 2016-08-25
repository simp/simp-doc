# DO NOT CHECK IN MODIFIED VERSIONS OF THIS FILE!

%{lua:

src_dir = rpm.expand('%{pup_module_info_dir}')
if string.match(src_dir, '^%%') or (posix.stat(src_dir, 'type') ~= 'directory') then
  src_dir = './'
end

-- These UNKNOWN entries should break the build if something bad happens

module_name = "UNKNOWN"
module_version = "UNKNOWN"
module_license = "UNKNOWN"

-- Default to 0
module_release = '0'

-- Snag the RPM-specific items out of the 'build/rpm_metadata' directory
local rel_file = io.open(src_dir .. "/build/rpm_metadata/release", "r")
if rel_file then
  for line in rel_file:lines() do
    is_comment = string.match(line, "^%s*#")
    is_blank = string.match(line, "^%s*$")

    if not (is_comment or is_blank) then
      if string.match(line, "^%s?version:") then
        version_match = line
      elseif string.match(line, "^%s?release:") then
        release_match = line
      end
    end
  end
  if version_match then
    module_version = string.gsub(version_match,"version:","")
  end
  if release_match then
    module_release = string.gsub(release_match,"release:","")
  end
end
}

%if 0%{?el6}
%define simp_major_version 4
%else
%define simp_major_version 5
%endif

Summary: SIMP Documentation
Name: simp-doc
Version: %{lua: print(module_version)}
Release: %{lua: print(module_release)}
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

if [ ! -d changelogs ]; then
  mkdir changelogs
fi

if [ ! -f pdf/SIMP_Documentation.pdf ] || [ `stat --printf="%s" pdf/SIMP_Documentation.pdf` -eq 0 ]; then
  echo "Error: PDF output has size 0"
  exit 1
fi

mv pdf/SIMP_Documentation.pdf pdf/SIMP-%{version}-%{release}.pdf

%install
# Just the Docs...

%clean
[ "%{buildroot}" != "/" ] && rm -rf %{buildroot}

%files
%defattr(0644,root,root,0755)
%doc html html-single pdf ldifs

%post
# Post install stuff

%postun
# Post uninstall stuff

%changelog
* Thu Aug 25 2016 Nick Markowski <nmarkowski@keywcorp.com>
- Added warning not to enable krb5 with autofs.

* Thu Aug 04 2016 Nick Markowski <nmarkowski@keywcorp.com>
- Fixed syntax errors in Local User creation docs.
- Added SSSD Local domain/user creation docs.
- Updated Why can't I login?? docs to include a how-to unlock
  accounts via LDAP and faillock.

* Fri Jun 10 2016 Nick Miller <nick.miller@onyxpoint.com>
- Added FAQ entries for disabling DHCP and named

* Fri May 13 2016 Trevor Vaughan <tvaughan@onyxpoint.com>
- First cut at the consolidated Documentation

* Wed May 11 2016 Nick Markowski <nmarkowski@keywcorp.com>
- Updated tftpboot default entry to use noverifyssl.

* Thu Apr 28 2016 Nick Markowski <nmarkowski@keywcorp.com>
- Updated kickstart docs to use https.

* Mon Apr 04 2016 Trevor Vaughan <tvaughan@onyxpoint.com>
- Starting on the 5.1.0-4 release...
- Changed the tftpboot docs to use https.
- Fixed a bug that was preventing PDF builds
- Updated the RPM build to fail if the resulting PDF is size 0

* Wed Nov 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com>
- Update to fix SIMP RPM dependencies

* Wed Nov 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com>
- Updated the default passwords to be easier overall

* Tue Sep 22 2015 Trevor Vaughan <tvaughan@onyxpoint.com>
- Preparing for the 5.1.0-RC1 release.

* Tue Aug 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com>
- Updated the spec file to properly build the docs.

* Fri Jul 31 2015 Judy Johnson <judy.johnson@onyxpoint.com>
- Converted docs from Publican to ReStructured Text.

* Wed Mar 11 2015 Trevor Vaughan <tvaughan@onyxpoint.com>
- Added text to cover the move to the new Puppet Server and the migration to
  Environments.
- Removed some old material.

* Sat Dec 20 2014 Trevor Vaughan <tvaughan@onyxpoint.com>
- Update the changelog for 5.0.0-2

* Mon Dec 08 2014 Kendall Moore <kmoore@keywcorp.com>
- No longer suggest grub-crypt to encrypt passwords and instead use a simple
  ruby script.

* Tue Nov 25 2014 Trevor Vaughan <tvaughan@onyxpoint.com>
- Final release of 5.0.0

* Wed Oct 29 2014 Trevor Vaughan <tvaughan@onyxpoint.com>
- Updated the Changelog for the 5.0.0-RC1 release.

* Thu Aug 07 2014 Trevor Vaughan <tvaughan@onyxpoint.com>
- Updated for the 5.0.0-Beta release

* Fri Jan 10 2014 Trevor Vaughan <tvaughan@onyxpoint.com>
- Added a script for converting LDAP users to InetOrgPerson entries
  and updated the LDIFs to account for such.

* Mon Nov 25 2013 Trevor Vaughan <tvaughan@onyxpoint.com>
- First release of 4.1.0-Alpha2

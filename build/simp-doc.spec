# DO NOT CHECK IN MODIFIED VERSIONS OF THIS FILE!

%{lua:

src_dir = rpm.expand('%{pup_module_info_dir}')

if string.match(src_dir, '^%%') or (posix.stat(src_dir, 'type') ~= 'directory') then
  io.stderr:write("AA '"..src_dir.."'\n")
  src_dir = rpm.expand('%{_sourcedir}')

  -- NOTE: this only recently became this mad as I tried to find a way to
  -- acommodate local and mock-based release detections & SOURCE1builds.
  -- So far, it has not worked
  io.stderr:write("BB '"..src_dir.."'\n")
  if (posix.stat( (src_dir .. '/build/rpm_metadata/release' ), 'type')  ~= 'regular') then
    src_dir = './'
    io.stderr:write("CC '"..src_dir.."'\n")
    if (posix.stat( (src_dir .. '/build/rpm_metadata/release' ), 'type')  ~= 'regular') then
      src_dir = rpm.expand('%{_sourcedir}')
      io.stderr:write("DD '"..src_dir.."'\n")
      if (posix.stat( (src_dir .. '/release' ), 'type')  ~= 'regular') then
        src_dir = './'
        io.stderr:write("EE '"..src_dir.."'\n")
      end
    end
  end
end

-- These UNKNOWN entries should break the build if something bad happens

package_version = "UNKNOWN"
rel_file_exists = false

--
-- Default to 2016
-- This was done due to the change in naming scheme across all of the modules.
--
package_release = '2016'

-- Snag the RPM-specific items out of the 'build/rpm_metadata' directory
rel_file = src_dir .. "/build/rpm_metadata/release"
local rel_file_handle = io.open(rel_file, "r")
if rel_file_handle then
  rel_file_exists = true
  for line in rel_file_handle:lines() do
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
    package_version = string.gsub(version_match,"version:","")
  end
  if release_match then
    package_release = string.gsub(release_match,"release:","")
  end
end
io.stderr:write("----------------------------------------\n")
io.stderr:write("          Lua sanity checks              \n")
io.stderr:write("----------------------------------------\n")
io.stderr:write("src_dir: '" .. src_dir .. "' \n")
io.stderr:write("rel_file: '" .. rel_file .. "' \n")
io.stderr:write("rel_file_exists: '" .. tostring(rel_file_exists) .. "' \n")
io.stderr:write("package_version: '" .. package_version .. "' \n")
io.stderr:write("package_release: '" .. package_release .. "' \n")
io.stderr:write("----------------------------------------\n")
-- uncomment in case you really want to be awesome
-- rpm.interactive()
}

%if 0%{?el6}
%define simp_major_version 4
%else
%define simp_major_version 5
%endif

Summary: SIMP Documentation
Name: simp-doc
Version: %{lua: print(package_version)}
Release: %{lua: print(package_release)}
License: Apache License, Version 2.0
Group: Documentation
Source0:    %{name}-%{version}-%{release}.tar.gz

%{lua:
  -- Include our sources as appropriate
  if (posix.stat( rel_file, 'type')  == 'regular') then
    print("Source2: " .. rel_file)

--    posix.symlink( rel_file, ( rpm.expand('%{_sourcedir}') .. '/release' ) )

--    -- RPM wants basename(Source2) at the top level, will not settle for less
--    -- This was a quick crack at just shoving it over there.  However, it
--    -- breaks the release detection up top
--    new_rel_file = ( rpm.expand('%{_sourcedir}') .. '/release' )
--    local new_rel_file_handle = io.open( new_rel_file, 'w' )
--    for line in io.lines( rel_file ) do
--      io.stderr:write("GRAAAAAH("..rel_file.."): "..line.."\n")
--      new_rel_file_handle:write( line .. "\n" )
--    end
--    new_rel_file_handle:close()

  end
}
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Buildarch: noarch
Requires: links
%if 0%{?el6}
BuildRequires: scl-utils, python27
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


%{lua:

io.stderr:write( "SOURCES:\n--------\n" )
for i, s in ipairs(sources) do io.stderr:write( "  - Source"..(i-1)..": "..s.."\n") end}

%description
Documentation for SIMP %{version}-%{release}

You can access the documentation on a text-based system using the
command 'simp doc'.

Alternatively, you can read the docs at https://simp.readthedocs.org

%prep
cp -p %{SOURCE1}  .
%setup

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

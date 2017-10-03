# DO NOT CHECK IN MODIFIED VERSIONS OF THIS FILE!

%{lua:

--
-- When you build you must pass this along so that we know how to get the
-- preliminary information.
-- This directory should hold the following items:
--   * 'build' directory
--   * 'CHANGELOG' <- The RPM formatted Changelog
--

src_dir = rpm.expand('%{pup_module_info_dir}')

if string.match(src_dir, '^%%') or (posix.stat(src_dir, 'type') ~= 'directory') then
  src_dir = rpm.expand('%{_sourcedir}')

  if (posix.stat((src_dir .. "/CHANGELOG"), 'type') ~= 'regular') then
    src_dir = './'
  end
end

-- These UNKNOWN entries should break the build if something bad happens

package_name = "UNKNOWN"
package_version = "UNKNOWN"
package_license = "UNKNOWN"

-- Default to 0
package_release = '0'

-- This starts as an empty string so that we can build it later
package_requires = ''

-- Snag the RPM-specific items out of the 'build/rpm_metadata' directory
rel_file = io.open(src_dir .. "/build/rpm_metadata/release", "r")

if not rel_file then
  -- Need this for the SRPM case
  rel_file = io.open(src_dir .. "/release", "r")
end

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
    package_version = string.gsub(version_match,"version:","")
  end
  if release_match then
    package_release = string.gsub(release_match,"release:","")
  end
end
}

%{lua:

-- Next, the Requirements

req_file = io.open(src_dir .. "/build/rpm_metadata/requires", "r")

if not req_file then
  -- Need this for the SRPM case
  req_file = io.open(src_dir .. "/requires", "r")
end

if req_file then
  for line in req_file:lines() do
    valid_line = (string.match(line, "^Requires: ") or string.match(line, "^Obsoletes: ") or string.match(line, "^Provides: "))

    if valid_line then
      package_requires = (package_requires .. "\\n" .. line)
    end
  end
end
}

Summary: SIMP Documentation
Name: simp-doc
Version: %{lua: print(package_version)}
Release: %{lua: print(package_release)}
License: Apache License, Version 2.0
Group: Documentation
Source0: %{name}-%{version}-%{release}.tar.gz
Source1: CHANGELOG
%{lua:
  -- Include our sources as appropriate
  if rel_file then
    print("Source2: release")
  end
  if req_file then
    print("Source3: requires")
  end
}
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot
Buildarch: noarch
%{lua: print(package_requires) }
Requires: links
%if 0%{?el6}
BuildRequires: centos-release-scl
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

pip install -U pip>=8.0 # ancient pips break on these requirements
pip install --upgrade -r requirements.txt --log dist/pip.log

sphinx-build -E -n -b html -d sphinx_cache docs html
sphinx-build -E -n -b singlehtml -d sphinx_cache docs html-single

# Rst2pdf is currently broken on references so we have to remove them.
# This should be removed when this bug is fixed.
find docs -name "*.rst" -exec sed -i 's/:ref://g' {} \;

sphinx-build -E -n -b pdf -d sphinx_cache docs pdf

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
%doc html html-single pdf

%post
# Post install stuff

%postun
# Post uninstall stuff

%changelog
%{lua:
-- Finally, the CHANGELOG

-- A default CHANGELOG in case we cannot find a real one

default_changelog = [===[
* $date Auto Changelog <auto@no.body> - $version-$release
- Latest release of $name
]===]

default_lookup_table = {
  date = os.date("%a %b %d %Y"),
  version = package_version,
  release = package_release,
  name = package_name
}

changelog = io.open(src_dir .. "/CHANGELOG","r")
if changelog then
  first_line = changelog:read()
  if string.match(first_line, "^*%s+%a%a%a%s+%a%a%a%s+%d%d?%s+%d%d%d%d%s+.+") then
    changelog:seek("set",0)
    print(changelog:read("*all"))
  else
    print((default_changelog:gsub('$(%w+)', default_lookup_table)))
  end
else
  print((default_changelog:gsub('$(%w+)', default_lookup_table)))
end
}

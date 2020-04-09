#!/usr/bin/python


import pprint
import shutil
import sys
import tempfile
import unittest

from .release_mapping import *
from .constants import *

# Globals
debug = True

def build_single_release_mappings_yaml(local_simp_core_path):
  build_path = os.path.join(local_simp_core_path, 'build')
  os.mkdir(build_path)
  content = """
---
simp_releases:
  5.1.X:
    flavors:
      CentOS:
        isos:
          - name: 'CentOS-7-x86_64-DVD-1511.iso'
            size: 4329570304
            checksum: '907e5755f824c5848b9c8efbb484f3cd945e93faa024bad6ba875226f9683b16'
        build_command: 'bundle exec rake build:auto[5.1.X,CentOS-7-x86_64-DVD-1511.iso]'
        mock: 'epel-7-x86_64'
        os_version: '7.0'
      RedHat:
        isos:
          - name: 'rhel-server-7.2-x86_64-dvd.iso'
            size: 4043309056
            checksum: '03f3a0291634335f6995534d829bd21ffaa0d000004dfeb1b2fb81052d64a4d5'
        build_command: 'bundle exec rake build:auto[5.1.X,RedHat-7-x86_64-DVD-1511.iso]'
        mock: 'epel-7-x86_64'
        os_version: '7.2'
  5.2.1-0:
    flavors:
      CentOS:
        isos:
          - name: 'CentOS-7-x86_64-DVD-1511.iso'
            size: 4329570304
            checksum: '907e5755f824c5848b9c8efbb484f3cd945e93faa024bad6ba875226f9683b16'
        build_command: 'bundle exec rake build:auto[5.2.1-0,CentOS-7-x86_64-DVD-1511.iso]'
        mock: 'epel-7-x86_64'
        os_version: '7.0'
      RedHat:
        isos:
          - name: 'rhel-server-7.2-x86_64-dvd.iso'
            size: 4043309056
            checksum: '03f3a0291634335f6995534d829bd21ffaa0d000004dfeb1b2fb81052d64a4d5'
        build_command: 'bundle exec rake build:auto[5.2.1-0,RedHat-7-x86_64-DVD-1511.iso]'
        os_version: '7.2'
"""
  mappings_file = open(os.path.join(build_path, 'release_mappings.yaml'), 'w', 0)
  mappings_file.write(content)
  mappings_file.close


# TODO use key-based access to prevent gitlab API lockout
print('\n*********************************************************************')
print('WARNING:  This test suite queries gitlab API numerous times and ')
print('          can cause temporary lockout if run repetitively.  Once')
print('          lockout is reached, no queries will be allowed for an hour.')
print('  See  https://developer.github.com/v3/#rate-limiting')
print('*********************************************************************\n')

class TestReleaseMapping(unittest.TestCase):
  def test_1(self):
    print('\n#####################################################################')
    print('Not building on Read-The-Docs,')
    print('no local release_mappings.yaml files exist,')
    print('version corresponds to master')

    simp_version_dict = {
      'version': '6.0.1',
      'release': 'RC1',
      'full_version': '6.0.1-RC1',
      'version_family': ['6.0.X', '6.X'],
      'simp_branch': None
    }

    doc = known_os_compatibility_rst(
      simp_version_dict,
      '/oops',
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      False
    )
    if debug: print(doc)

    self.assertTrue('Known OS Compatibility' in doc)
    self.assertFalse('No SIMP Mapping Data Found' in doc)


  def test_2(self):
    print('\n#####################################################################')
    print('Not building on Read-The-Docs,')
    print('a single release_mappings.yaml file exists in simp-core/build,')
    print('version matches exactly')
    local_simp_core_path = tempfile.mkdtemp()
    build_single_release_mappings_yaml(local_simp_core_path)

    simp_version_dict = {
      'version': '5.2.1',
      'release': '0',
      'full_version': '5.2.1-0',
      'version_family': ['5.2.X', '5.X'],
      'simp_branch': None
    }

    doc = known_os_compatibility_rst(
      simp_version_dict,
      local_simp_core_path,
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      False
    )
    if debug: print(doc)
    shutil.rmtree(local_simp_core_path)

    expected ="""
Known OS Compatibility
----------------------

* **SIMP 5.2.1-0**

* **CentOS 7.0**

  * **ISO #1:** CentOS-7-x86_64-DVD-1511.iso
  * **Checksum:** 907e5755f824c5848b9c8efbb484f3cd945e93faa024bad6ba875226f9683b16

* **RedHat 7.2**

  * **ISO #1:** rhel-server-7.2-x86_64-dvd.iso
  * **Checksum:** 03f3a0291634335f6995534d829bd21ffaa0d000004dfeb1b2fb81052d64a4d5

"""
    self.assertEqual(expected, doc)

  def test_3(self):
    print('\n#####################################################################')
    print('Not building on Read-The-Docs,')
    print('a single release_mappings.yaml file exists in simp-core/build,')
    print('5.1.X version family match needed')

    local_simp_core_path = tempfile.mkdtemp()
    build_single_release_mappings_yaml(local_simp_core_path)
    simp_version_dict = {
      'version': '5.3.2',
      'release': 'RC1',
      'full_version': '5.3.2-RC1',
      'version_family': ['5.3.X', '5.X'],
      'simp_branch': None
    }

    doc = known_os_compatibility_rst(
      simp_version_dict,
      local_simp_core_path,
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      False
    )
    if debug: print(doc)
    shutil.rmtree(local_simp_core_path)

    expected ="""
Known OS Compatibility
----------------------

* **SIMP 5.1.X**

* **CentOS 7.0**

  * **ISO #1:** CentOS-7-x86_64-DVD-1511.iso
  * **Checksum:** 907e5755f824c5848b9c8efbb484f3cd945e93faa024bad6ba875226f9683b16

* **RedHat 7.2**

  * **ISO #1:** rhel-server-7.2-x86_64-dvd.iso
  * **Checksum:** 03f3a0291634335f6995534d829bd21ffaa0d000004dfeb1b2fb81052d64a4d5

"""
    self.assertEqual(expected, doc)

  def test_4(self):
    print('\n#####################################################################')
    print('Not building on Read-The-Docs,')
    print('a single release_mappings.yaml file exists in simp-core/build,')
    print('version match not possible')

    local_simp_core_path = tempfile.mkdtemp()
    build_single_release_mappings_yaml(local_simp_core_path)
    simp_version_dict = {
      'version': '0.0.0',
      'release': 'NEED_FULL_SIMP_BUILD_TREE',
      'full_version': '0.0.0-NEED_FULL_SIMP_BUILD_TREE',
      'version_family': ['0.0.X', '0.X'],
      'simp_branch': None
    }
    doc = known_os_compatibility_rst(
      simp_version_dict,
      local_simp_core_path,
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      False
    )
    if debug: print(doc)
    shutil.rmtree(local_simp_core_path)

    self.assertTrue('Known OS Compatibility' in doc)
    self.assertTrue('No SIMP Mapping Data Found' in doc)

  def test_5(self):
    print('\n#####################################################################')
    print('Building on Read-The-Docs,')
    print('local release_mapping.yaml to be ignored,')
    print('multiple release_mapping.yaml files to be pulled,')
    print('exact version match')
    local_simp_core_path = tempfile.mkdtemp()
    build_single_release_mappings_yaml(local_simp_core_path)
    simp_version_dict = {
      'version': '6.0.0',
      'release': '0',
      'full_version': '6.0.0-0',
      'version_family': ['6.0.X', '6.X'],
      'simp_branch': '6.0.0-0'
    }
    doc = known_os_compatibility_rst(
      simp_version_dict,
      local_simp_core_path,
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      True
    )
    if debug: print(doc)
    shutil.rmtree(local_simp_core_path)

    # This result is correct, but shows problems with the mapping files...
    # - only 6.X was in release mappings file
    # - there was a Fedora mappings file with old CentOS/RHEL versions
    expected ="""
Known OS Compatibility
----------------------

* **SIMP 6.X**

* **CentOS 6.8**

  * **ISO #1:** CentOS-6.8-x86_64-bin-DVD1.iso
  * **Checksum:** 1dda55622614a8b43b448a72f87d6cb7f79de1eff49ee8c5881a7d9db28d4e35

  * **ISO #2:** CentOS-6.8-x86_64-bin-DVD2.iso
  * **Checksum:** 0aba869427b4ce04e100d72744daf7fea1f7be2e4be56b658095bd9e99e04e6d

* **CentOS 7.0**

  * **ISO #1:** CentOS-7-x86_64-DVD-1611.iso
  * **Checksum:** c455ee948e872ad2194bdddd39045b83634e8613249182b88f549bb2319d97eb

  * **ISO #2:** CentOS-7-x86_64-DVD-1511.iso
  * **Checksum:** 907e5755f824c5848b9c8efbb484f3cd945e93faa024bad6ba875226f9683b16

* **RedHat 6.8**

  * **ISO #1:** rhel-server-6.8-x86_64-dvd.iso
  * **Checksum:** d35fd1af20f6adef9b11b46c2534ae8b6e18de7754889e2b51808b436dff2804

* **RedHat 7.2**

  * **ISO #1:** rhel-server-7.2-x86_64-dvd.iso
  * **Checksum:** 03f3a0291634335f6995534d829bd21ffaa0d000004dfeb1b2fb81052d64a4d5

* **RedHat 7.3**

  * **ISO #1:** rhel-server-7.3-x86_64-dvd.iso
  * **Checksum:** 120acbca7b3d55465eb9f8ef53ad7365f2997d42d4f83d7cc285bf5c71e1131f

"""
    self.assertEqual(expected, doc)

  def test_6(self):
    print('\n#####################################################################')
    print('Building on Read-The-Docs,')
    print('single release_mapping.yaml file to be pulled,')
    print('exact version match')

    simp_version_dict = {
      'version': '4.3.1',
      'release': '0',
      'full_version': '4.3.1-0',
      'version_family': ['4.3.X', '4.X'],
      'simp_branch': '4.3.1-0'
    }
    doc = known_os_compatibility_rst(
      simp_version_dict,
      'unused',
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      True
    )
    if debug: print(doc)
    expected ="""
Known OS Compatibility
----------------------

* **SIMP 4.3.1-0**

* **CentOS 6.8**

  * **ISO #1:** CentOS-6.8-x86_64-bin-DVD1.iso
  * **Checksum:** 1dda55622614a8b43b448a72f87d6cb7f79de1eff49ee8c5881a7d9db28d4e35

  * **ISO #2:** CentOS-6.8-x86_64-bin-DVD2.iso
  * **Checksum:** 0aba869427b4ce04e100d72744daf7fea1f7be2e4be56b658095bd9e99e04e6d

* **RedHat 6.8**

  * **ISO #1:** rhel-server-6.8-x86_64-dvd.iso
  * **Checksum:** d35fd1af20f6adef9b11b46c2534ae8b6e18de7754889e2b51808b436dff2804

"""

  def test_7(self):
    print('\n#####################################################################')
    print('Building on Read-The-Docs, bad version')

    simp_version_dict = {
      'version': '1.0.0',
      'release': '0',
      'full_version': '1.0.0-0',
      'version_family': ['1.0.X', '1.X'],
      'simp_branch': '1.0.0-0'
    }
    doc = known_os_compatibility_rst(
      simp_version_dict,
      'unused',
      SIMP_GITHUB_API_BASE,
      DEFAULT_SIMP_BRANCH,
      True
    )
    if debug: print(doc)

    self.assertTrue('Known OS Compatibility' in doc)
    self.assertTrue('No SIMP Mapping Data Found' in doc)

if __name__ == '__main__':
  suite = unittest.TestLoader().loadTestsFromTestCase(TestReleaseMapping)
  unittest.TextTestRunner(verbosity=2).run(suite)


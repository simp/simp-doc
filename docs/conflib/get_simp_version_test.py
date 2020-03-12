#!/usr/bin/python


import sys
import re
import unittest

from .get_simp_version import *
from .constants import *

# Globals
debug = False
build_dir = os.path.join(ROOTDIR, 'build')
release_dir = os.path.join(build_dir, 'rpm_metadata')
release_filename = os.path.join(release_dir, 'release')

def print_simp_version(simp_version_dict):
  for k in simp_version_dict:
    print(k + ' => ' + str(simp_version_dict[k]), file=sys.stdout)
  if (re.search(r'^NEED_', simp_version_dict['release']) or (simp_version_dict['version'] == '0.0')):
    print('>>>Error: No valid SIMP version found\n', file=sys.stderr)

def create_release_file(version=None, release=None, separator=':'):
  # python 2.7 doesn't allow mkdir -p type recursive directory creation
  if not os.path.exists(build_dir):
    os.mkdir(build_dir)

  if not os.path.exists(release_dir):
    os.mkdir(release_dir)

  # if we don't eliminate buffering, the small file doesn't
  # get written until program exits (even though we have closed it!)
   # (Calling release_file.flush() doesn't solve the problem either.)
  release_file = open(release_filename, 'w')
  if version:
    release_file.write("version" + separator + version + "\n")

  if release:
    release_file.write("release" + separator + release + "\n")

  release_file.close


class TestGetSimpVersion(unittest.TestCase):
  def test_1(self):

    print('\n##############################################################')
    print('Not building on Read-The-Docs and there is no')
    print('build/rpm_metadata/release file')

    # clear out environment
    if os.environ.get('READTHEDOCS_VERSION'):
      os.environ.pop('READTHEDOCS_VERSION')

    if os.path.isfile(release_filename):
      os.remove( release_filename)

    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False #on_rtd
    )
    print_simp_version(simp_version_dict)

    # ASSUMING that internet connectivity allowed us to pull down the
    # simp-core master branch Changelog.rst
    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)
    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)
    self.assertIsNotNone(simp_version_dict['full_version'])
    self.assertIsNotNone(simp_version_dict['version_family'])
    self.assertEqual(simp_version_dict['simp_branch'], 'master')

  def test_2(self):
    print('\n##############################################################')
    print('Not building on Read-The-Docs and there is a')
    print('build/rpm_metadata/release file with valid entries')

    # clear out environment
    if os.environ.get('READTHEDOCS_VERSION'):
      os.environ.pop('READTHEDOCS_VERSION')

    create_release_file('7.8.9', 'Beta1')
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False #on_rtd
    )
    print_simp_version(simp_version_dict)

    self.assertEqual(simp_version_dict['version'], '7.8.9')
    self.assertEqual(simp_version_dict['release'], 'Beta1')
    self.assertEqual(simp_version_dict['full_version'], '7.8.9-Beta1')
    self.assertEqual(simp_version_dict['version_family'], ['7.8.X', '7.X'])
    self.assertIsNone(simp_version_dict['simp_branch'])

  def test_3(self):
    print('\n##############################################################')
    print('Not building on Read-The-Docs and there is a')
    print('build/rpm_metadata/release file with invalid entries')

    # clear out environment
    if os.environ.get('READTHEDOCS_VERSION'):
      os.environ.pop('READTHEDOCS_VERSION')

    create_release_file('7.8.9', 'Beta1', ' ') # bad tag/value separator
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False #on_rtd
    )
    print_simp_version(simp_version_dict)

    self.assertNotEqual(simp_version_dict['version'], '7.8.9')
    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)

    self.assertNotEqual(simp_version_dict['release'], 'Beta1')
    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)

    self.assertNotEqual(simp_version_dict['full_version'], '7.8.9-Beta1')
    self.assertIsNotNone(simp_version_dict['full_version'])

    self.assertNotEqual(simp_version_dict['version_family'], ['7.8.X', '7.X'])
    self.assertIsNotNone(simp_version_dict['version_family'])

    self.assertEqual(simp_version_dict['simp_branch'], 'master')

  def test_4(self):
    print('\n##############################################################')
    print('Not building on Read-The-Docs and there is a')
    print('build/rpm_metadata/release file without a version')

    # clear out environment
    if os.environ.get('READTHEDOCS_VERSION'):
      os.environ.pop('READTHEDOCS_VERSION')

    create_release_file(None, 'Beta1')
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False #on_rtd
    )
    print_simp_version(simp_version_dict)

    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)

    self.assertNotEqual(simp_version_dict['release'], 'Beta1')
    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)

    self.assertIsNotNone(simp_version_dict['full_version'])
    self.assertIsNotNone(simp_version_dict['version_family'])
    self.assertEqual(simp_version_dict['simp_branch'], 'master')

  def test_5(self):
    print('\n##############################################################')
    print('Not building on Read-The-Docs and there is a')
    print('build/rpm_metadata/release file without a release')

    # clear out environment
    if os.environ.get('READTHEDOCS_VERSION'):
      os.environ.pop('READTHEDOCS_VERSION')

    create_release_file('7.8.9', None)
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False #on_rtd
    )
    print_simp_version(simp_version_dict)

    self.assertNotEqual(simp_version_dict['version'], '7.8.9')
    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)

    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)
    self.assertIsNotNone(simp_version_dict['full_version'])
    self.assertIsNotNone(simp_version_dict['version_family'])
    self.assertEqual(simp_version_dict['simp_branch'], 'master')

  def test_6(self):
    print('\n##############################################################')
    print("Building on Read-The-Docs with master branch specified")
    os.environ['READTHEDOCS_VERSION'] = 'master'
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      True #on_rtd
    )
    print_simp_version(simp_version_dict)

    # ASSUMING that internet connectivity allowed us to pull down the
    # simp-core master branch Changelog.rst
    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)
    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)
    self.assertIsNotNone(simp_version_dict['full_version'])
    self.assertIsNotNone(simp_version_dict['version_family'])
    self.assertEqual(simp_version_dict['simp_branch'], 'master')

  def test_7(self):
    print('\n##############################################################')
    print("Building on Read-The-Docs with 5.1.X branch specified")

    os.environ['READTHEDOCS_VERSION'] = '5.1.X'
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      True #on_rtd
    )
    print_simp_version(simp_version_dict)

    # ASSUMING that internet connectivity allowed us to pull down the
    # simp-core master branch Changelog.rst
    self.assertEqual(simp_version_dict['version'], '5.2.1')
    self.assertEqual(simp_version_dict['release'], '0')
    self.assertEqual(simp_version_dict['full_version'], '5.2.1-0')
    self.assertEqual(simp_version_dict['version_family'], ['5.2.X', '5.X'])
    self.assertEqual(simp_version_dict['simp_branch'], '5.1.X')

  def test_8(self):
    print('\n##############################################################')
    print("Building on Read-The-Docs with '4.3.1-0' version specified")

    os.environ['READTHEDOCS_VERSION'] = '4.3.1-0'
    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      True #on_rtd
    )
    print_simp_version(simp_version_dict)

    # ASSUMING that internet connectivity allowed us to pull down the
    # simp-core master branch Changelog.rst
    self.assertEqual(simp_version_dict['version'], '4.3.1')
    self.assertEqual(simp_version_dict['release'], '0')
    self.assertEqual(simp_version_dict['full_version'], '4.3.1-0')
    self.assertEqual(simp_version_dict['version_family'], ['4.3.X', '4.X'])
    self.assertEqual(simp_version_dict['simp_branch'], '4.3.1-0')

  def test_9(self):
    print('\n##############################################################')
    print("Building on Read-The-Docs with invalid version specified")
    print("(RTD config error)")

    os.environ['READTHEDOCS_VERSION'] = '5.X'

    simp_version_dict = get_simp_version(
      ROOTDIR,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      True #on_rtd
    )
    print_simp_version(simp_version_dict)

    # ASSUMING that internet connectivity allowed us to pull down the
    # simp-core master branch Changelog.rst
    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)
    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)
    self.assertIsNotNone(simp_version_dict['full_version'])
    self.assertIsNotNone(simp_version_dict['version_family'])
    self.assertEqual(simp_version_dict['simp_branch'], 'master')

  def test_10(self):
    print('\n##############################################################')
    print("Calling with defaults")

    simp_version_dict = get_simp_version()
    print_simp_version(simp_version_dict)

    # ASSUMING that internet connectivity allowed us to pull down the
    # simp-core master branch Changelog.rst
    self.assertIsNot(simp_version_dict['version'], SIMP_INVALID_VERSION)
    self.assertIsNot(simp_version_dict['release'], SIMP_INVALID_RELEASE)
    self.assertIsNotNone(simp_version_dict['full_version'])
    self.assertIsNotNone(simp_version_dict['version_family'])
    self.assertEqual(simp_version_dict['simp_branch'], 'master')

if __name__ == '__main__':
  suite = unittest.TestLoader().loadTestsFromTestCase(TestGetSimpVersion)
  unittest.TextTestRunner(verbosity=2).run(suite)

#!/usr/bin/python

from __future__ import print_function
import shutil
import sys
import tempfile
import unittest

from changelog import *
from constants import *

debug = False

class TestChangelogToRst(unittest.TestCase):
  def test_1(self):
    print('\n#########################################################################')
    print('Not building on Read-The-Docs and the local changelog file does not exist')

    changelog = changelog_to_rst(
      None,  # simp_branch
      CHANGELOG_TGT,
      '/oops', # simp_core_path
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False  # on_rtd
    )
    if debug: print(changelog)

    #TODO more rigorous validation
    self.assertIsNot(len(changelog), 0)

  def test_2(self):
    print('\n#########################################################################')
    print('Not building on Read-The-Docs and the local changelog file')
    print('does exist')
    simp_core_path = tempfile.mkdtemp()
    changelog_file = open(os.path.join(simp_core_path, CHANGELOG_TGT), 'w', 0)
    changelog_file.write("My Changelog\n")
    changelog_file.write("============\n")
    changelog_file.write("\n")
    changelog_file.write("Change 1\n")
    changelog_file.write("Change 2\n")
    changelog_file.close

    changelog = changelog_to_rst(
      None,  # simp_branch
      CHANGELOG_TGT,
      simp_core_path,
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False  # on_rtd
    )
    if debug: print(changelog)
    shutil.rmtree(simp_core_path)

    expected = """My Changelog
============

Change 1
Change 2
"""
    self.assertEqual(expected, changelog)

  def test_3(self):
    print('\n#########################################################################')
    print('Building on Read-The-Docs and the changelog file for the')
    print('specified version does not exist')

    changelog = changelog_to_rst(
      'oops-0',  # simp_branch
      CHANGELOG_TGT,
      'unused', # simp_core_path
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False  # on_rtd
    )
    if debug: print(changelog)

    #TODO more rigorous validation
    self.assertIsNot(len(changelog), 0)

  def test_4(self):
    print('\n#########################################################################')
    print('Building on Read-The-Docs and the changelog file for the')
    print('specified version does exist')

    changelog = changelog_to_rst(
      '5.2.1-0',  # simp_branch
      CHANGELOG_TGT,
      'unused', # simp_core_path
      SIMP_GITHUB_RAW_BASE,
      DEFAULT_SIMP_BRANCH,
      False  # on_rtd
    )
    if debug: print(changelog)

    #TODO more rigorous validation
    self.assertIsNot(len(changelog), 0)

  def test_5(self):
    print('\n#########################################################################')
    print('Calling with defaults')

    changelog = changelog_to_rst('4.3.1-0')
    if debug: print(changelog)

    #TODO more rigorous validation
    self.assertIsNot(len(changelog), 0)

if __name__ == '__main__':
  suite = unittest.TestLoader().loadTestsFromTestCase(TestChangelogToRst)
  unittest.TextTestRunner(verbosity=2).run(suite)

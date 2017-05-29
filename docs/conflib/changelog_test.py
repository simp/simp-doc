#!/usr/bin/python

from __future__ import print_function
import sys

from changelog import *
from constants import *

def print_changelog():
    """ Test the changelog fetch """

    print(changelog_to_rst(
        'Changelog.rst',
        BASEDIR,
        GITHUB_BASE,
        GITHUB_VERSION_TARGETS,
        ON_RTD
    ), file=sys.stdout)

print_changelog()

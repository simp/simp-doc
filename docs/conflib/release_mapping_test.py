#!/usr/bin/python

from __future__ import print_function
import sys
import pprint

from release_mapping import *
from constants import *

def print_release_mapping():
    """ Test the release mapping """

    print(known_os_compatibility_rst(
        '6.X',
        BASEDIR,
        GITHUB_VERSION_TARGETS,
        ON_RTD
    ), file=sys.stdout)

print_release_mapping()

#!/usr/bin/python

from __future__ import print_function
import sys
import re

from get_simp_version import *
from constants import *

def print_simp_version():
    """ Small test for get_simp_version() """

    simp_version_dict = get_simp_version(
        BASEDIR,
        GITHUB_BASE,
        GITHUB_VERSION_TARGETS,
        ON_RTD
    )

    if re.search(r'^NEED_', simp_version_dict['release']):
        print('Error: No valid SIMP version found', file=sys.stderr)
    else:
        for k in simp_version_dict:
            print(k + ' => ' + simp_version_dict[k], file=sys.stdout)

print_simp_version()

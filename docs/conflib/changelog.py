from __future__ import print_function
import os
import sys
import time
import urllib2

from textwrap import dedent

sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))

from conflib.constants import *

def get_changelog(simp_branch, changelog_name, local_simp_core_path,
     simp_github_raw_base, default_simp_branch, on_rtd):

    """ Get the Changelog either from local disk or GitHub """

    disk_changelog = os.path.join(local_simp_core_path, changelog_name)

    if simp_branch:
        changelog_url = '/'.join([simp_github_raw_base, 'simp-core',
            simp_branch, changelog_name])
    else:
        changelog_url = '/'.join([simp_github_raw_base, 'simp-core',
            default_simp_branch, changelog_name])

    changelog = None

    if (not on_rtd) and os.path.isfile(disk_changelog):
        # Is the Changelog on disk?
        with open(disk_changelog, 'r') as changelog_content:
            changelog = changelog_content.read()
    else:
        # Grab it from the Internet!
        # This is really designed for use with ReadTheDocs
        for i in range(0, MAX_SIMP_URL_GET_ATTEMPTS):
            try:
                print("NOTICE: Downloading Changelog: " + changelog_url, file=sys.stderr)
                changelog = urllib2.urlopen(changelog_url).read()
                break
            except urllib2.URLError:
                print('WARNING:  Could not download ' + changelog_url, file=sys.stderr)
                time.sleep(1)
                continue
            break

    return changelog

def changelog_to_rst(simp_branch, changelog_name=CHANGELOG_TGT,
     local_simp_core_path=LOCAL_SIMP_CORE_PATH,
     simp_github_raw_base=SIMP_GITHUB_RAW_BASE,
     default_simp_branch=DEFAULT_SIMP_BRANCH, on_rtd=ON_RTD):
    """ Return a RST representation of the Changelog """

    changelog = """
    Changelog Stub
    ==============

    .. warning::
        The build scripts could not find a valid Changelog either locally or on the Internet!

    .. note::
        Please check your Internet connectivity as well as your local build system.
    """

    changelog = dedent(changelog)

    changelog_content = get_changelog(simp_branch, changelog_name,
        local_simp_core_path, simp_github_raw_base, default_simp_branch,
        on_rtd)

    if changelog_content:
        changelog = changelog_content
    else:
        sys.stderr.write("Warning: Could not find a valid Changelog, using the stub....\n")

    return changelog

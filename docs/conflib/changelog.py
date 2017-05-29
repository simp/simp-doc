from __future__ import print_function
import sys
import os
import urllib2

from textwrap import dedent

def get_changelog(changelog_name, basedir, github_base, github_version_targets, on_rtd):
    """ Get the Changelog either from local disk or GitHub """

    disk_changelog = os.getenv('SIMP_CHANGELOG_PATH',
                               os.path.join(basedir, '..', '..', '..', changelog_name)
                              )

    changelog_urls = []
    for version_target in github_version_targets:
        changelog_urls.append('/'.join([github_base, 'simp-core', version_target, changelog_name]))

    changelog = None

    if (not on_rtd) and os.path.isfile(disk_changelog):
        # Is the Changelog on disk?
        with open(disk_changelog, 'r') as changelog_content:
            changelog = changelog_content.read()
    else:
        # Grab it from the Internet!
        # This is really designed for use with ReadTheDocs

        for changelog_url in changelog_urls:
            try:
                print("NOTICE: Downloading Changelog: " + changelog_url, file=sys.stderr)
                changelog = urllib2.urlopen(changelog_url).read()
                break
            except urllib2.URLError:
                continue

    return changelog

def changelog_to_rst(changelog_name, basedir, github_base, github_version_targets, on_rtd):
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

    changelog_content = get_changelog(changelog_name, basedir, github_base, github_version_targets, on_rtd)

    if changelog_content:
        changelog = changelog_content
    else:
        sys.stderr.write("Warning: Could not find a valid Changelog, using the stub....\n")

    return changelog

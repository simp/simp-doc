from __future__ import print_function
import sys
import os
import re
import urllib2

def get_simp_version(basedir, github_base, github_version_targets, on_rtd):
    """
    Figure out how to go get the SIMP version

    This starts by checking the local directory structure and then moves on to
    pulling things from GitHub if they cannot be found locally.
    """

    release_placeholder = 'NEED_FULL_SIMP_BUID_TREE'

    retval = {
        'version': '0.0',
        'release': release_placeholder,
        'full_version': None,
        'version_family': None
    }

    # If we're running on ReadTheDocs, we should go fetch the content from the
    # actual branch that we're using
    if not on_rtd:
        # Attempt to read auto-generated release file. Needs to be run after
        # rake munge:prep
        rel_file = os.path.join(basedir, '..', 'build/rpm_metadata/release')
        if os.path.isfile(rel_file):
            with open(rel_file, 'r') as f:
                for line in f:
                    _tmp = line.split(':')
                    if 'version' in _tmp:
                        retval['version'] = _tmp[-1].strip()
                    elif 'release' in _tmp:
                        retval['release'] = _tmp[-1].strip()

    if on_rtd or (retval['release'] == release_placeholder):
        os_simp_spec_urls = []

        rtd_version = os.environ.get('READTHEDOCS_VERSION')

        if rtd_version:
            github_version_targets.insert(0, rtd_version)

        for version_target in github_version_targets:
            new_url_tgt = '/'.join([
                github_base, 'simp-core', version_target, 'src', 'build', 'simp.spec'
            ])

            if not new_url_tgt in os_simp_spec_urls:
                os_simp_spec_urls.insert(0, new_url_tgt)

            # Grab release and version from the Internet!
            for os_simp_spec_url in os_simp_spec_urls:
                try:
                    print("NOTICE: Downloading SIMP Spec File: " + os_simp_spec_url, file=sys.stderr)
                    os_simp_spec_content = urllib2.urlopen(os_simp_spec_url).read().splitlines()

                    # Read the version out of the spec file and run with it.
                    for line in os_simp_spec_content:
                        _tmp = line.split()
                        if 'Version:' in _tmp:
                            version_list = _tmp[-1].split('.')
                            version = '.'.join(version_list[0:3]).strip()
                            retval['version'] = re.sub(r'%\{.*?\}', '', version)

                        elif 'Release:' in _tmp:
                            release = _tmp[-1].strip()
                            retval['release'] = re.sub(r'%\{.*?\}', '', release)
                    break
                except urllib2.URLError:
                    continue

            if retval['version'] and retval['release']:
                break

    retval['full_version'] = '-'.join([retval['version'], retval['release']])
    retval['version_family'] = re.sub(r'\.\d$', '.X', retval['version'])

    return retval

from __future__ import print_function
import os
import re
import sys
import time
import urllib2

sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))

from conflib.constants import *

def valid_version_and_release(version, release):
    return (version != SIMP_INVALID_VERSION) and (release != SIMP_INVALID_RELEASE)


def get_simp_version(rootdir=ROOTDIR, simp_github_raw_base=SIMP_GITHUB_RAW_BASE,
        default_simp_branch=DEFAULT_SIMP_BRANCH, on_rtd=ON_RTD):
    """
    Get the SIMP version and release either from local disk or GitHub

    PASS 1
    - When run from ReadTheDocs, attempt to extract the version from the
      simp.spec file in the git repository for a specific simp-core, where
      the version is specified by the 'READTHEDOCS_VERSION' environment
      variable.
    - When not run from ReadTheDocs, attempt to extract the version from
      a local, auto-generated release file within this project's tree.

    PASS 2
    If PASS 1 fails, attempt to extract the simp.spec file in the git
    repository for the default simp tag/branch.

    If both PASS 1 and PASS 2 fail, return invalid/unset values.
    NOTE:  The invalid version value must be a numeric to allow
    ReadTheDocs to render the rest of the documentation in a
    best-effort fashion.
    """


    retval = {
        'version': SIMP_INVALID_VERSION,
        'release': SIMP_INVALID_RELEASE,
        'full_version': None,
        'version_family': None,
        'simp_branch': None
    }

    # If we're running on ReadTheDocs, we should go fetch the content from the
    # actual branch that we're using
    if on_rtd:
        rtd_version = os.environ.get('READTHEDOCS_VERSION')
        old_version_regex = re.compile('^4.|^5.|^6.0')
        if (old_version_regex.match(rtd_version) == None):
          url_tgt = '/'.join([
            simp_github_raw_base, 'simp-core', rtd_version, 'src', 'assets',
            'simp', 'build', 'simp.spec'
          ])
        else:
          url_tgt = '/'.join([
            simp_github_raw_base, 'simp-core', rtd_version, 'src', 'build',
            'simp.spec'
          ])

        result = __extract_from_url(url_tgt)
        if  valid_version_and_release(result['version'], result['release']):
            retval['version'] = result['version']
            retval['release'] = result['release']
            retval['simp_branch'] = rtd_version

    else:
        # Attempt to read auto-generated release file. This file is generated
        # by rake munge:prep for all rake doc:* and pkg:* targets.
        rel_file = os.path.join(rootdir, 'build/rpm_metadata/release')
        result = __extract_from_file(rel_file)
        retval['version'] = result['version']
        retval['release'] = result['release']
        retval['simp_branch'] = None

    if not valid_version_and_release(retval['version'], retval['release']):
        # Fall back to something valid
        url_tgt = '/'.join([
            simp_github_raw_base, 'simp-core', default_simp_branch, 'src',
           'assets', 'simp', 'build', 'simp.spec'
        ])
        result = __extract_from_url(url_tgt)
        if valid_version_and_release(result['version'],result['release']):
            retval['version'] = result['version']
            retval['release'] = result['release']
            retval['simp_branch'] = default_simp_branch

    retval['full_version'] = '-'.join([retval['version'], retval['release']])
    patch_wildcard = re.sub(r'\.\d$', '.X', retval['version'])
    minor_wildcard = re.sub(r'\.\d\.X$', '.X', patch_wildcard)
    retval['version_family'] = [patch_wildcard, minor_wildcard]

    return retval

### Private Methods
def __extract_from_file(release_file):
    result = {
        'version': SIMP_INVALID_VERSION,
        'release': SIMP_INVALID_RELEASE
    }
    if os.path.isfile(release_file):
        with open(release_file, 'r') as f:
            for line in f:
                _tmp = line.split(':')
                if 'version' in _tmp:
                    result['version'] = _tmp[-1].strip()
                elif 'release' in _tmp:
                    result['release'] = _tmp[-1].strip()
    return result

def __extract_from_url(simp_spec_url):
    result = {
        'version': SIMP_INVALID_VERSION,
        'release': SIMP_INVALID_RELEASE
    }
    for i in range(0, MAX_SIMP_URL_GET_ATTEMPTS):
        try:
            print("NOTICE: Downloading SIMP Spec File: " + simp_spec_url, file=sys.stderr)
            simp_spec_content = urllib2.urlopen(simp_spec_url).read().splitlines()

            # Read the version out of the spec file and run with it.
            for line in simp_spec_content:
                _tmp = line.split()
                if 'Version:' in _tmp:
                    version_list = _tmp[-1].split('.')
                    version = '.'.join(version_list[0:3]).strip()
                    result['version'] = re.sub(r'%\{.*?\}', '', version)

                elif 'Release:' in _tmp:
                    release = _tmp[-1].strip()
                    result['release'] = re.sub(r'%\{.*?\}', '', release)
        except urllib2.URLError:
            print('WARNING:  Could not download ' + simp_spec_url, file=sys.stderr)
            time.sleep(1)
            continue
        break

    return result

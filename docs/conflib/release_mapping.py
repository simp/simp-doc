from __future__ import print_function
import base64
import copy
import glob
import json
import os
import re
import sys
import time
import urllib2
from textwrap import dedent
import yaml

sys.path.append(os.path.abspath(os.path.dirname(os.path.dirname(__file__))))

from conflib.constants import *

def get_version_map(simp_branch, local_simp_core_path, simp_github_api_base,
    default_simp_branch, on_rtd):
    """ Fetch the version map either from local disk or GitHub """

    ver_map = {}

    ver_mapper_name = 'release_mappings.yaml'

    if not on_rtd:
        # SIMP 6 and later
        os_ver_mappers = glob.glob(
            os.path.join(local_simp_core_path, 'build', 'distributions',
            '*', '*', '*', ver_mapper_name)
        )

        # SIMP 4/5
        if not os_ver_mappers:
            os_ver_mappers = glob.glob(
                os.path.join(local_simp_core_path, 'build', ver_mapper_name)
            )

        if os_ver_mappers:
            for os_ver_mapper in os_ver_mappers:
                with open(os_ver_mapper, 'r') as f:
                    __update_ver_map(ver_map, yaml.load(f.read()))

    if on_rtd or not ver_map:
        github_api_base = simp_github_api_base + '/simp-core/git/trees/'
        if simp_branch:
            branch_to_query = simp_branch
        else:
            branch_to_query = default_simp_branch

        github_api_target = github_api_base + branch_to_query
        github_opts = '?recursive=1'

        # only try retrieving each API URL from github once, because API calls
        # are rate limited
        try:
            # Grab the distribution tree
            distro_json = json.load(urllib2.urlopen(github_api_target + github_opts))

            release_mapping_targets = [x for x in distro_json['tree'] if (
                x['path'] and re.search(r'release_mappings.yaml$', x['path'])
            )]

            for release_mapping_target in release_mapping_targets:
                url = SIMP_GITHUB_RAW_BASE + '/simp-core/' + branch_to_query + '/' + release_mapping_target['path']
                print("NOTICE: Downloading Version Mapper: " + url, file=sys.stderr)

                for i in range(0, MAX_SIMP_URL_GET_ATTEMPTS):
                    try:
                        release_yaml_string = urllib2.urlopen(url).read()
                        release_yaml = yaml.load(release_yaml_string)
                        if isinstance(release_yaml, basestring):
                          # This is ugly...
                          # A string is returned when the release mapping file
                          # is actually a link.  So, need to pull down the
                          # content of the link, instead.
                          parts = release_yaml.split('/')
                          partial_url = '/'.join(filter(lambda a: a != '..', parts))
                          for target in release_mapping_targets:
                            if partial_url in target['path']:
                              url = SIMP_GITHUB_RAW_BASE + '/simp-core/' + branch_to_query + \
                                '/' + target['path']
                              release_yaml_string = urllib2.urlopen(url).read()
                              release_yaml = yaml.load(release_yaml_string)
                              break

                        __update_ver_map(ver_map, release_yaml)

                    except urllib2.URLError:
                        print('Error downloading ' + url, file=sys.stderr)
                        time.sleep(1)
                        continue
                    break

        except urllib2.URLError:
            print('Error downloading ' + github_api_target + github_opts, file=sys.stderr)

    return ver_map

def version_map_to_rst(full_version, version_family, ver_map):
    """ Return a version of the version map that is suitable for printing. """

    none_found_msg = '* No SIMP Mapping Data Found for "' + full_version + '"'

    # Easy cop out
    if not ver_map:
        return none_found_msg

    simp_release_list = __generate_version_list(full_version, version_family)

    # Build the Release mapping table for insertion into the docs
    release_mapping_list = []

    ver_map_releases = ver_map.keys()
    simp_release = full_version
    if not simp_release in ver_map_releases:
        for ver in simp_release_list:
            if ver in ver_map_releases:
                simp_release = ver
                print("Warning: version mapper falling back to " + simp_release, file=sys.stderr)
            else:
                simp_release = None

    if simp_release:
        release_mapping_list.append('* **SIMP ' + simp_release + '**')

        for os_key in sorted(ver_map[simp_release].keys()):
            release_mapping_list.append("\n    * **" + os_key + '**')

            for i, iso in enumerate(ver_map[simp_release][os_key]['isos']):
                release_mapping_list.append("\n      * **ISO #" + str(i+1) + ":** " + iso['name'])
                release_mapping_list.append("      * **Checksum:** " + iso['checksum'])

    if not release_mapping_list:
        release_mapping_list.append(none_found_msg)

    # Trailing newline
    release_mapping_list.append('')

    return "\n".join(release_mapping_list)

def known_os_compatibility_rst(simp_version_dict,
    local_simp_core_path=LOCAL_SIMP_CORE_PATH,
    simp_github_api_base=SIMP_GITHUB_API_BASE,
    default_simp_branch=DEFAULT_SIMP_BRANCH, on_rtd=ON_RTD):

    """ Output the fully formatted OS Compatibility RST """

    ver_map = get_version_map(simp_version_dict['simp_branch'],
        local_simp_core_path, simp_github_api_base, 
        default_simp_branch, on_rtd)

    os_compat_rst = """
    Known OS Compatibility
    ----------------------

    {0}
    """.format(version_map_to_rst(simp_version_dict['full_version'],
        simp_version_dict['version_family'], ver_map))

    return dedent(os_compat_rst)

### Private Methods

def __update_ver_map(ver_map, data):
    """
    This bunch of nonsense is to translate the release_mappings.yaml into
    something that can be output to the Compatibility list in a sane manner
    """

    simp_versions = sorted(data['simp_releases'].keys(), reverse=True)

    for simp_version in simp_versions:
        for flavor in data['simp_releases'][simp_version]['flavors'].keys():
            isos = data['simp_releases'][simp_version]['flavors'][flavor]['isos']
            os_key = flavor + ' ' + data['simp_releases'][simp_version]['flavors'][flavor]['os_version']

            if not (isos and os_key):
                continue

            if not ver_map:
                ver_map[simp_version] = {os_key: {'isos': isos}}
            else:
                if ver_map.get(simp_version):
                    if not ver_map[simp_version].get(os_key):
                        ver_map[simp_version][os_key] = {'isos': []}
                else:
                    ver_map[simp_version] = {os_key: {'isos': []}}

                for iso in isos:
                    if iso not in ver_map[simp_version][os_key]['isos']:
                        ver_map[simp_version][os_key]['isos'].append(iso)

def __generate_version_list(full_version, version_family):
    """
    Put together an ordered list that will provide a quick match for the
    provided version
    """

    # From SIMP 6 on, full_version and version_family is sufficient.
    # For earlier version, custom (odd) version families are needed.
    version_list = [ full_version ]
    version_list.extend(version_family)
    if full_version.startswith('5'):
      version_list.extend(['5.1.X']) # 5.1.X for a 5.2.2 or later
    elif full_version.startswith('4'):
      version_list.extend(['4.2.X']) # 4.2.X for a 4.3.2 or later

    return version_list 

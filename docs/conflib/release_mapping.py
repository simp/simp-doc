from __future__ import print_function
import copy
import sys
import glob
import os
import urllib2
import re
import base64
import json
from textwrap import dedent
import yaml


def get_version_map(basedir, github_version_targets, on_rtd):
    """
    Fetch the version map

    Local directories are checked first and, if those fail, maps are pulled
    from GitHub directly
    """

    ver_map = {}

    ver_mapper_name = 'release_mappings.yaml'

    if not on_rtd:
        os_ver_mappers = glob.glob(os.path.join(basedir, '..', '..', '..', 'build', 'distributions', '*', '*', '*', ver_mapper_name))

        if not os_ver_mappers:
            os_ver_mappers = glob.glob(os.path.join(basedir, '..', '..', '..', 'build', ver_mapper_name))

        if os_ver_mappers:
            for os_ver_mapper in os_ver_mappers:
                with open(os_ver_mapper, 'r') as f:
                    __update_ver_map(ver_map, yaml.load(f.read()))

    if on_rtd or not ver_map:
        github_api_base = 'https://api.github.com/repos/simp/simp-core/git/trees/'

        for version_target in github_version_targets:
            github_api_target = github_api_base + version_target
            github_opts = '?recursive=1'

            # We've found it, bail
            if ver_map:
                break

            try:
                # Grab the distribution tree
                distro_json = json.load(urllib2.urlopen(github_api_target + github_opts))

                release_mapping_targets = [x for x in distro_json['tree'] if (
                    x['path'] and re.search(r'release_mappings.yaml$', x['path'])
                )]

                for release_mapping_target in release_mapping_targets:
                    print("NOTICE: Downloading Version Mapper: " + release_mapping_target['path'], file=sys.stderr)

                    try:
                        release_obj = json.load(urllib2.urlopen(release_mapping_target['url']))

                        release_yaml = base64.b64decode(release_obj['content'])

                        __update_ver_map(ver_map, yaml.load(release_yaml))

                    except urllib2.URLError:
                        print('Error downloading ' + release_mapping_target['path'], file=sys.stderr)
                        continue

            except urllib2.URLError:
                print('Error downloading ' + github_api_target + github_opts, file=sys.stderr)
                continue

    return ver_map

def version_map_to_rst(simp_release, ver_map):
    """ Return a version of the version map that is suitable for printing. """

    none_found_msg = '* No SIMP Mapping Data Found for "' + simp_release + '"'

    # Easy cop out
    if not ver_map:
        return none_found_msg

    simp_release_list = __generate_version_list(simp_release)

    # Build the Release mapping table for insertion into the docs
    release_mapping_list = []

    ver_map_releases = ver_map.keys()
    if not simp_release in ver_map_releases:
        simp_release = [ ver for ver in simp_release_list if ver in ver_map_releases ][0]
        print("Warning: version mapper falling back to " + simp_release, file=sys.stderr)

    if simp_release:
        release_mapping_list.append('* **SIMP ' + simp_release + '**')

        for os_key in sorted(ver_map[simp_release].keys()):
            release_mapping_list.append("\n    * **" + os_key + ' ' + '**')

            for i, iso in enumerate(ver_map[simp_release][os_key]['isos']):
                release_mapping_list.append("\n      * **ISO #" + str(i+1) + ":** " + iso['name'])
                release_mapping_list.append("      * **Checksum:** " + iso['checksum'])

    if not release_mapping_list:
        release_mapping_list.append(none_found_msg)

    # Trailing newline
    release_mapping_list.append('')

    return "\n".join(release_mapping_list)

def known_os_compatibility_rst(simp_release, basedir, github_version_targets, on_rtd):
    """ Output the fullly formatted OS Compatibility RST """

    ver_map = get_version_map(basedir, github_version_targets, on_rtd)

    os_compat_rst = """
    Known OS Compatibility
    ----------------------

    {0}
    """.format(version_map_to_rst(simp_release, ver_map))

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

def __generate_version_list(full_version):
    """
    Put together an ordered list that will provide a quick match for the
    provided version
    """

    version_list = []

    if '-' in full_version:
        version, release = full_version.split('-')
    else:
        release = None
        version = full_version

    version_parts = version.split('.')

    # This can have all sorts of junk in it
    if release:
        try:
            release = int(release)

            for i in range(0, release+1) + ['X']:
                version_list.append(version + '-' + str(i))
        except ValueError:
            version_list.append(version + '-' + release)

    reverse_version_parts = copy.copy(version_parts)
    reverse_version_parts.reverse()

    # Three digits, worked in least significant order
    for i, v in enumerate(reverse_version_parts):
        # Don't loop the last set
        if i+1 == len(version_parts):
            break

        if v == 'X':
            version_list.append('.'.join(version_parts[0:len(version_parts)-(i+1)] + [v]))
            break

        v_num = int(v)

        for x in range(0, v_num+1) + ['X']:
            version_list.append('.'.join(version_parts[0:len(version_parts)-(i+1)] + [str(x)]))

    return version_list



# A list of constants used throughout the code and for testing

import os

# Root directory of the local simp-doc git repo to which this file belongs
ROOTDIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))

# Directory containing *.rst files
DOCSDIR = os.path.join(ROOTDIR, 'docs')

# Whether this project is being built by ReadTheDocs
ON_RTD = os.environ.get('READTHEDOCS') == 'True'

# URLs to simp-core files used to generate dynamic content
SIMP_GITHUB_RAW_BASE = os.getenv('SIMP_GITHUB_RAW_BASE', 'https://raw.githubusercontent.com/simp')
SIMP_GITHUB_API_BASE = os.getenv('SIMP_GITHUB_API_BASE', 'https://api.github.com/repos/simp')

# Default simp github tag/branch to use to determine the SIMP version
# for which the docs will be rendered.
# 1) Used as the tag/branch to download in a non-ReadTheDocs, non-simp-core build.
# 2) Used as a fallback when
#    - The version and release information cannot be pulled from a local file
#      in a non-ReadTheDocs build.
#    - There is a ReadTheDocs configuration error and the version requested
#      does not exist.
# 3) Can be overridden by an environment variable in a non-ReadTheDocs
#    build.
DEFAULT_SIMP_BRANCH = os.getenv('SIMP_BRANCH', 'master')

# Location of local simp-core git repo
# Default assumes simp-doc project has been checked out by simp-core
# during an ISO build
LOCAL_SIMP_CORE_PATH = os.getenv('SIMP_CORE_PATH',
    os.path.abspath(os.path.join(ROOTDIR, '..', '..')))

KNOWN_OS_COMPATIBILITY_TGT = 'Known_OS_Compatibility.rst'

SIMP_INVALID_VERSION = '0.0.0'
SIMP_INVALID_RELEASE = 'NEED_FULL_SIMP_BUILD_TREE'

MAX_SIMP_URL_GET_ATTEMPTS = 3

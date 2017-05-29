# A list of constants used throughout the code and for testing

import os

BASEDIR = os.path.abspath(os.getcwd())

ON_RTD = os.environ.get('READTHEDOCS') == 'True'

GITHUB_BASE = os.getenv('SIMP_GITHUB_BASE', 'https://raw.githubusercontent.com/simp')

# This ordering matches our usual default fallback branch scheme
GITHUB_VERSION_TARGETS = [
    'master',
    '5.1.X',
    '4.2.X'
]

CHANGELOG_TGT = 'Changelog.rst'

KNOWN_OS_COMPATIBILITY_TGT = 'Known_OS_Compatibility.rst'

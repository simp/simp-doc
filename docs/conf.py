# -*- coding: utf-8 -*-
#
# SIMP documentation build configuration file, created by
# sphinx-quickstart on Tue May 26 11:09:13 2015.
#
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# auto-generated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.


import sys
import os
import datetime

sys.path.append(os.path.abspath(os.path.dirname(__file__)))
sys.path.append(os.path.abspath(os.path.dirname(__file__) + '/_extensions'))

from conflib.constants import *
from conflib.get_simp_version import *
from conflib.release_mapping import *

# Pre-Build Manipulation Code

target_dirs = ['dynamic']

# Allow this to be built up over time
epilog = []

# determine version information:
#   'full_version'  : X.Y.Z-R (e.g., 6.1.0-RC1)
#   'version'       : X.Y.Z   (e.g., 6.1.0)
#   'release'       : R       (e.g., RC1)
#   'version_family': X.X     (e.g., 6.X)
#   'simp_branch'   : simp-core tag/branch from which the information was
#                     derived or None, when derived from local files
simp_version_dict = get_simp_version()

# Just for convenience
#
# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents
#

# The full version, X.Y.Z-R, where
# - X.Y.Z is the version in which X, Y, and Z must be numbers
# - R is the release, which can be alpha-numeric, e.g. '0', 'Beta', 'RC1'
full_version = simp_version_dict['full_version']

# The simp tag/branch name. When a branch (e.g., 'master'), the name
# won't necessarily match the full_version name.
simp_branch = simp_version_dict['simp_branch']

epilog.append('.. |simp_version| replace:: %s' % full_version)

def setup(app):
    app.add_config_value('simp_version', full_version, 'env') # The third value must always be 'env'

    # write out dynamic content
    known_os_compat_content = known_os_compatibility_rst(simp_version_dict)

    for target_dir in target_dirs:
        target_dir = os.path.join(DOCSDIR, target_dir)

        if not os.path.exists(target_dir):
            os.mkdir(target_dir)

        known_os_compat_dest = os.path.join(target_dir, KNOWN_OS_COMPATIBILITY_TGT)

        with open(known_os_compat_dest, 'w') as f:
            f.write(known_os_compat_content)

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#sys.path.insert(0, os.path.abspath('.'))

# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx_rtd_theme',
    'simp_roles',
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.todo',
    # To use this, you need to make the necessary variable available in setup
    # using the 'add_config_value' function.
    #
    # Example:
    #   def setup(app):
    #     app.add_config_value('releaselevel', '', 'env') # The third value must always be 'env'
    #
    # Usage:
    #   .. ifconfig:: releaselevel in ('alpha', 'beta', 'rc')
    'sphinx.ext.ifconfig',
    'rst2pdf.pdfbuilder',
    'crate.sphinx.csv' # docs: https://github.com/crate/sphinx_csv_filter
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The encoding of source files.
#source_encoding = 'utf-8-sig'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = 'SIMP'
copyright = str(datetime.datetime.now().year) + ', The System Integrity Management Project'
author = 'The System Integrity Management Project'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = 'en_US'

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
#today = ''
# Else, today_fmt is used as the format for a strftime call.
#today_fmt = '%B %d, %Y'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
exclude_patterns = [
    # Build artifacts
    '_build',
    # Files to be included in other files
    '**/*.inc',
    # Informational files
    '**/README',
    # Templates
    '**/*.template'
]

# Set a parameter that will strip out certain items that cause the build to be
# slow. Should not be used for real documentation builds but is great for 90%
# of all testing.
#
# In testing the 'html' target, this took the build time on a fast system from
# 40s to 9s.

if os.environ.get('SIMP_FAST_DOCS', 'false') == 'true':
    exclude_patterns.append('security_mapping')

# The reST default role (used for this markup: `text`) to use for all
# documents.
#default_role = None

# If true, '()' will be appended to :func: etc. cross-reference text.
#add_function_parentheses = True

# If true, the current module name will be prepended to all description
# unit titles (such as .. function::).
#add_module_names = True

# If true, sectionauthor and moduleauthor directives will be shown in the
# output. They are ignored by default.
#show_authors = False

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# A list of ignored prefixes for module index sorting.
#modindex_common_prefix = []

# If true, keep warnings as "system message" paragraphs in the built documents.
#keep_warnings = False

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False
if os.environ.get('SIMP_DOCS_TODO', 'false') == 'true':
    todo_include_todos = True

# This causes issues with our documentation
linkcheck_anchors = False

# A regex of links that the linkcheck builder should ignore
linkcheck_ignore = [
    r'^https?:\/\/.*samba.org',
    # ignore rpms
    r'^https?:\/\/.*\.(src\.)?rpm$',
    # ignore pdfs
    r'^https?:\/\/.*\.pdf$',
    # ignore doc
    r'^https?:\/\/.*\.doc(x)?$',
    # ignore isos
    r'^https?:\/\/.*\.iso$',
    # ignore tarballs
    r'^https?:\/\/.*\.tar(\..{2,3})?$',
    # links that the resolver has trouble with
    r'^https?:\/\/groups\.google\.com\/forum\/.+',
    r'^https?:\/\/bundler\.io/rationale\.html',
    # Everything from pgp.mit.edu
    r'^https?:\/\/pgp\.mit\.edu(/.*|$)',
    # These are for the templates in the release emails
    r'.*VERSION.*Upgrade_SIMP.*',
    r'.*VERSION-.*NUM.*Changelog\.html',
    # Ignore github searches since they'll cause the API to timeout
    r'^https?:\/\/github\.com\/search\?.*',
    # Qualys randomly fails
    r'^https?:\/\/.*.qualys\.com.*'
]

# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#html_theme_path = ["_themes"]
html_theme = "sphinx_rtd_theme"

# adds a file for overwriting the default css. We use this for fixing tables
html_context = {
        'css_files': [
            'https://media.readthedocs.org/css/sphinx_rtd_theme.css',
            'https://media.readthedocs.org/css/readthedocs-doc-embed.css',
            '_static/css_overrides.css',
        ],
    }

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#html_theme_options = {}

# Add any paths that contain custom themes here, relative to this directory.
#html_theme_path = []

# The name for this set of Sphinx documents.  If None, it defaults to
# "<project> v<release> documentation".
html_title = "%s %s documentation" % (project, full_version)

# A shorter title for the navigation bar.  Default is the same as html_title.
#html_short_title = None

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
html_logo = "images/SIMP_Logo.png"

# The name of an image file (within the static path) to use as favicon of the
# docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
#html_favicon = None

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Add any extra paths that contain custom files (such as robots.txt or
# .htaccess) here, relative to this directory. These files are copied
# directly to the root of the documentation.
#html_extra_path = []

# If not '', a 'Last updated on:' timestamp is inserted at every page bottom,
# using the given strftime format.
#html_last_updated_fmt = '%b %d, %Y'

# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
#html_use_smartypants = True

# Custom sidebar templates, maps document names to template names.
#html_sidebars = {}

# Additional templates that should be rendered to pages, maps page names to
# template names.
#html_additional_pages = {}

# If false, no module index is generated.
#html_domain_indices = True

# If false, no index is generated.
#html_use_index = True

# If true, the index is split into individual pages for each letter.
#html_split_index = False

# If true, links to the reST sources are added to the pages.
#html_show_sourcelink = True

# If true, "Created using Sphinx" is shown in the HTML footer. Default is True.
#html_show_sphinx = True

# If true, "(C) Copyright ..." is shown in the HTML footer. Default is True.
#html_show_copyright = True

# If true, an OpenSearch description file will be output, and all pages will
# contain a <link> tag referring to it.  The value of this option must be the
# base URL from which the finished HTML is served.
#html_use_opensearch = ''

# This is the file name suffix for HTML files (e.g. ".xhtml").
#html_file_suffix = None

# Language to be used for generating the HTML full-text search index.
# Sphinx supports the following languages:
#   'da', 'de', 'en', 'es', 'fi', 'fr', 'hu', 'it', 'ja'
#   'nl', 'no', 'pt', 'ro', 'ru', 'sv', 'tr'
#html_search_language = 'en'

# A dictionary with options for the search language support, empty by default.
# Now only 'ja' uses this config value
#html_search_options = {'type': 'default'}

# The name of a javascript file (relative to the configuration directory) that
# implements a search results scorer. If empty, the default will be used.
#html_search_scorer = 'scorer.js'

# Output file base name for HTML help builder.
htmlhelp_basename = 'SIMPdoc'

# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
# The paper size ('letterpaper' or 'a4paper').
#'papersize': 'letterpaper',

# The font size ('10pt', '11pt' or '12pt').
#'pointsize': '10pt',

# Additional stuff for the LaTeX preamble.
#'preamble': '',

# Latex figure (float) alignment
#'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'SIMP.tex', 'SIMP Documentation',
     'THE SIMP TEAM', 'manual')
]

# The name of an image file (relative to this directory) to place at the top of
# the title page.
#latex_logo = None

# For "manual" documents, if this is true, then toplevel headings are parts,
# not chapters.
#latex_use_parts = False

# If true, show page references after internal links.
#latex_show_pagerefs = False

# If true, show URL addresses after external links.
#latex_show_urls = False

# Documents to append as an appendix to all manuals.
#latex_appendices = []

# If false, no module index is generated.
#latex_domain_indices = True


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'simp', 'SIMP Documentation',
     [author], 1)
]

# If true, show URL addresses after external links.
#man_show_urls = False


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'SIMP', 'SIMP Documentation',
     author, 'SIMP', 'One line description of project.',
     'Miscellaneous')
]

# Documents to append as an appendix to all manuals.
#texinfo_appendices = []

# If false, no module index is generated.
#texinfo_domain_indices = True

# How to display URL addresses: 'footnote', 'no', or 'inline'.
#texinfo_show_urls = 'footnote'

# If true, do not generate a @detailmenu in the "Top" node's menu.
#texinfo_no_detailmenu = False


# Example configuration for intersphinx: refer to the Python standard library.
#intersphinx_mapping = {'https://docs.python.org/': None}

# PDF
pdf_documents = [
    (master_doc, 'SIMP_Documentation', 'SIMP Documentation', 'SIMP')
]

pdf_language = "en_US"
pdf_fit_background_mode = "scale"
pdf_compressed = True
pdf_stylesheets = ['sphinx', 'kerning', 'letter']
pdf_use_toc = True
pdf_use_index = False
pdf_toc_depth = 3

# tag
tags.add('simp_%s' % full_version.split('.')[0])

rst_epilog = "\n".join(epilog)

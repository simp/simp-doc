import re
from docutils import nodes
from docutils.parsers.rst import roles
import sphinx.errors

def setup(app):
    app.add_role('github', auto_class( 'github', 'inline', 'https://github.com/%s'))
    app.add_role('jira', auto_class('jira', 'inline', 'https://simp-project.atlassian.net/browse/%s'))
    app.add_role('package', auto_class('package'))
    app.add_role('pupmod', pupmod_class())
    app.add_role('param', auto_class('param', 'literal'))

# Ensure options dict contains a classes list with a 'simp-{role_name}' item
def append_to_options_classes(options, role_name):
  roles.set_classes(options)
  if 'classes' not in options.keys():
    options['classes'] = []
  options['classes'].append( 'simp-%s' % role_name )
  return(options)

# Add a :<role_name>:`<text>` role processor function
def auto_class(role_name,role_type='inline',pattern=False):
  def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    options = append_to_options_classes(options, role_name)

    if pattern:
      if role_type == 'literal':
        options = append_to_options_classes(options, 'literal')
      # From: https://protips.readthedocs.io/link-roles.html
      url = pattern % (text,)
      node = nodes.reference(rawtext, text, refuri=url, **options)
    elif role_type == 'literal':
      node = nodes.literal(rawtext, text, **options)
    else:
      node = nodes.inline(rawtext, text, **options)

    return [node], []
  return role

# Add a :pupmod:`<text>` role that auto-links "org-name" to the Puppet Forge
def pupmod_class():
  def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    options = append_to_options_classes(options, 'pupmod')
    forge_names = re.split("[-/]", text)

    # Link to the module on Puppet Forge (if the text is in the right format)
    if len(forge_names) == 2:
      url = 'https://forge.puppet.com/%s/%s' % (forge_names[0], forge_names[1])
      node = nodes.reference(rawtext, text, refuri=url, **options)
      if re.search('/', text):
        fixed_text = re.sub('/', '-', text)
        print("WARNING: Use '-' to separate Puppet module names (.e.g., %s', not '%s')\n" % (fixed_text, text))
    else:
      node = nodes.inline(rawtext, text, **options)

    return [node], []
  return role

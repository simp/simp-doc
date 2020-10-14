import re
from docutils import nodes
from docutils.parsers.rst import roles

def setup(app):
    app.add_role('github', auto_class( 'github', 'inline', 'https://github.com/%s'))
    app.add_role('jira', auto_class('jira', 'inline', 'https://simp-project.atlassian.net/browse/%s'))
    app.add_role('package', auto_class('package'))
    app.add_role('pupmod', forge_class('pupmod'))
    app.add_role('param', auto_class('param', 'literal'))


def append_to_classes(options, role_name):
  roles.set_classes(options)
  if 'classes' not in options.keys():
    options['classes'] = []
  options['classes'].append( 'simp-%s' % role_name )
  return(options)

def auto_class(role_name,role_type='inline',pattern=False):
  def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    options = append_to_classes(options, role_name)

    if pattern:
      if role_type == 'literal':
        options = append_to_classes(options, 'literal')
      # From: https://protips.readthedocs.io/link-roles.html
      url = pattern % (text,)
      node = nodes.reference(rawtext, text, refuri=url, **options)
    elif role_type == 'literal':
      node = nodes.literal(rawtext, text, **options)
    else:
      node = nodes.inline(rawtext, text, **options)

    return [node], []
  return role

def forge_class(role_name, role_type='inline'):
  def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    options = append_to_classes(options, role_name)
    forge_names = re.split("[-/]", text)
    if len(forge_names) == 2:
      url = 'https://forge.puppet.com/%s/%s' % (forge_names[0], forge_names[1])
      node = nodes.reference(rawtext, text, refuri=url, **options)
    else:
      node = nodes.inline(rawtext, text, **options)

    return [node], []
  return role

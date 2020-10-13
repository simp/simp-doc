from docutils import nodes
from docutils.parsers.rst import roles


def setup(app):
    app.add_role('github', auto_link('https://github.com/%s'))
    app.add_role('jira', auto_link('https://simp-project.atlassian.net/browse/%s'))
    app.add_role('package', auto_class('package'))
    app.add_role('pupmod', auto_class('pupmod'))
    app.add_role('param', auto_class('param', 'literal'))

def auto_class(role_name,role_type='inline'):
  def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    roles.set_classes(options)
    if 'classes' not in options.keys():
      options['classes'] = []
    options['classes'].append( 'simp-%s' % role_name )

    if role_type == 'literal':
      node = nodes.literal(rawtext, text, **options)
    else:
      node = nodes.inline(rawtext, text, **options)

    return [[node], []]
  return role


# From: https://protips.readthedocs.io/link-roles.html
def auto_link(pattern):
  def role(name, rawtext, text, lineno, inliner, options={}, content=[]):
    url = pattern % (text,)
    node = nodes.reference(rawtext, text, refuri=url, **options)
    return [node], []
  return role


Testing on FIPS Systems
=======================

If you're running a system that requires compliance with :term:`NIST 800-53` or
:term:`NIST 800-171`, you may find that having your system :term:`FIPS`-enabled
is causing your workflow to simply fall apart.

Since we try to eat our own dog food, we try to develop on SIMP as much is as
practical and have the following advice that works at the time of writing this
document.

Many of the tools that we use are getting better, and we have been diligent
about filing bugs with projects that fail to meet the requirements set out by
FIPS or which simply crash due to being run on a FIPS enabled system. We do
understand that not all operations require FIPS security but, unfortunately,
the underlying software simply can't tell whether an algorithm is being used
for security or convenience.

Bundler
-------

`Bundler`_ is probably the first hurdle that you will encounter.

There is an `original bug`_ that we filed that has a `fix`_ released in Bundler
1.14.X. While this has worked for us (and is what we recommend), apparently
there were `some issues`_ with the patch and it was reverted. Likewise, a
`new bug`_ has been filed that is tracking current progress and we have faith that
the team will get it fully fixed in the near future.

To pin your runs to a FIPS-compatible Bundler, you will need to both install a
non-crashing version, as well as ensure that you always use that version during
your runs.

A simple method for doing this would be to do the following:

.. code-block:: bash

   gem install bundler -v 1.14.6
   alias bundle='bundle _1.14.6_'

RSpec-Puppet
------------

There is one change that you need to make to your ``spec/spec_helper.rb`` file
to ensure that ``rspec`` does not attempt to use MD5 checksums.

You simply need to add something like the following to your ``RSpec.configure``
section:

.. code-block:: ruby

   RSpec.configure do |c|
     c.before(:each) do
       Puppet[:digest_algorithm] = 'sha256'
     end
   end

.. _Bundler: https://bundler.io/
.. _fix: https://github.com/rubygems/bundler/issues/5440
.. _new bug: https://github.com/rubygems/bundler/issues/5584
.. _original bug: https://github.com/rubygems/bundler/issues/4989
.. _some issues: https://github.com/rubygems/bundler/issues/4989#issuecomment-280503064

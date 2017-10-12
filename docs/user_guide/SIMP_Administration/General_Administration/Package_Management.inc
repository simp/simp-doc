Package Management
------------------

The SIMP infrastructure has a consistent philosophy that managed packages
should be at the latest version in the available repositories whenever the
system is brought into alignment by :term:`Puppet`.

All SIMP produced modules should, by default, have their versions set to
``present`` but ``simp_options``, when set initially by ``simp config``, should
set that to ``latest`` across the environment.

The rationale behind this is that it is far easier to update a set of
repositories than it is to precisely pin versions of all packages managed on a
given system. Since repositories are generally common packages, the ability to
create a set of symlinks that represent the latest tested state of a system
should be far simpler than doing minutia management across your Puppet code.

Workflow
^^^^^^^^

The general workflow to keep your system properly up to date would be as follows.

.. NOTE::

   We **highly** recommend using :term:`Beaker` for testing these scenarios

#. Update the **Test** repository
#. Assign a test node to the repository via a ``yumrepo`` resource
#. Run Puppet and evaluate the results
#. Run a full system update and evaluate the results

   * This simulates the :ref:`ug-sa-ga-nightly-updates`, if enabled

#. If all goes well, migrate the changes to the **Production** repository
#. Let Puppet do the rest

See :ref:`ug-sa-ga-nightly-updates` for more information on setting up the repositories
and providing packages to your clients.

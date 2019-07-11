SIMP File Structure
-------------------

Upon a fresh install, the default :term:`Puppet environment` in SIMP
is located at ``/etc/puppetlabs/code/environments/production`` and contains
almost all necessary files for a Puppet infrastructure. It will look like this:

.. code-block:: bash

   /etc/puppetlabs/code/environments/production/
   ├── environment.conf
   ├── data/
   ├── manifests/
   └── modules/

- ``environment.conf`` - Sets the environment to include the second SIMP
  modulepath.
- ``manifests/`` - Contains SIMP's ``site.pp`` at a minimum.
- ``data/`` - Default location of the YAML files used by Hiera; contains your
  node data.
- ``modules/`` - Default install location of Puppet modules. Populated
  automatically for you by ``simp config`` on a fresh SIMP install.

The skeleton for SIMP's Puppet environment is installed by the
``simp-environment-skeleton`` RPM into
``/usr/share/simp/environment-skeleton/puppet``. This skeleton is copied
into the ``production`` Puppet environment by ``simp config``.  However, at
any time, you can generate a new :term:`SIMP Omni-Environment` that this
skeleton Puppet environment by using ``simp environment new``.

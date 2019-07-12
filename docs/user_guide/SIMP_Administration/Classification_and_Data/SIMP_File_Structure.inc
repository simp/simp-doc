SIMP File Structure
-------------------

The default :term:`Puppet environment` in SIMP is located at
``/etc/puppetlabs/code/environments/production`` and contains almost all
necessary files for a Puppet infrastructure. It will look like this:

.. code-block:: bash

   /etc/puppetlabs/code/environments/production/
   ├── environment.conf
   ├── data/
   ├── manifests/
   └── modules/

* ``environment.conf``

  * Sets the environment to include the :term:`SIMP Secondary Environment` modulepath.

* ``manifests/``

  * Contains SIMP's ``site.pp``.

* ``data/``

  * Default location of the YAML files used by Hiera; contains your node data.

* ``modules/``

  * Default install location of Puppet modules. Populated automatically for you
    by ``simp config``.

The skeleton for SIMP's Puppet environment is installed by the
``simp-environment-skeleton`` RPM into
``/usr/share/simp/environment-skeleton/puppet``. This skeleton is copied into
the ``production`` Puppet environment by ``simp config``.  However, at any
time, you can regenerate this skeleton :term:`SIMP Omni-Environment` by using
``simp environment new``.

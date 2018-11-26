SIMP File Structure
-------------------

The default puppet environment in SIMP, located at
``/etc/puppetlabs/code/environments/simp``, contains almost
all necessary files for a Puppet infrastructure. It will look like this on a
fresh SIMP system:

.. code-block:: bash

   /etc/puppetlabs/code/environments/simp/
   ├── environment.conf
   ├── data/
   ├── manifests/
   └── modules/

- ``environment.conf`` - Sets the environment to include the second SIMP modulepath.
- ``manifests/`` - Contains site.pp and all other node manifests.
- ``data/`` - Default location of the yaml files used by hiera  which contain your node data
- ``modules/`` - Default install location of Puppet modules. Each module RPM copies files here during installation from ``/usr/share/simp/modules``.

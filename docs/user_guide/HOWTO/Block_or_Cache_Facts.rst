HOWTO Block or Cache Facts
==========================

As described in `Configuring Facter with facter.conf`_, you can block or cache
groups of facts on a node using settings in ``facter.conf`` on that node.  SIMP
has made this easier by adding management of ``facter.conf`` to ``simp-pupmod``
(``simp-pupmod`` version >= '8.0.0'). Using this module, you can enable
management of ``facter.conf`` and specify its configuration in :term:`Hiera`.

Here is example Hiera that blocks and caches select groups of facts:

.. code-block:: yaml

   ---
   pupmod::manage_facter_conf: true
   pupmod::facter_options:
     facts:
       blocklist:
         - EC2
       ttls:
         - kernel: "8 hours"
         - memory: "8 hours"
         - operating system: "8 hours"
         - processor: "8 hours"

.. _Configuring Facter with facter.conf: https://puppet.com/docs/facter/latest/configuring_facter.html

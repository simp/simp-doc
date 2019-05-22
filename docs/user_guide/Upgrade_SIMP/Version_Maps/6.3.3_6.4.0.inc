.. _upgrade-6.3.3-to-6.4.0:

Upgrading from SIMP-6.3.3 to SIMP-6.4.0
---------------------------------------

.. WARNING::

   You **MUST** be on at least SIMP 6.3.3 to upgrade to 6.4.0.

   If you need to upgrade from 6.2.0 to 6.3.X, please read
   :ref:`upgrade-6.2.0-to-6.3.0` with care.

As indicated by gradual updates in previous :ref:`Changelogs`, SIMP has been
working on becoming more friendly to alternative modes of deployment and
maintenance. The team has specifically been focusing on fully supporting
:term:`r10k` and :term:`Code Manager` as methods of installation and
maintenance over time.

SIMP 6.4.0 is the release where we make this happen!

We have taken care to make no technologically breaking changes and, in keeping
with the version description in the :ref:`ug-general-upgrade-instructions`, have
not made changes that affect the API of modules provided by the supported
module set.

That said, there are quite a few procedural items that need to be taken into
account when upgrading to SIMP 6.4.0 so that things work generally like you are
used to.

To get a full accounting of the major changes in behavior for 6.4.0, please see
the :ref:`changelog-6.4.0` Changelog.

Package Upgrade Order
^^^^^^^^^^^^^^^^^^^^^

simp-rsync
""""""""""

Due to the nature of RPM uninstallation activities, to avoid ruining any active
files in ``/var/simp/environments/simp/rsync``, we had to create a final
version of ``simp-rsync`` that is generally dependency free. Nothing requires
this RPM since it is not technically required any longer but you **MUST**
upgrade this RPM if you want to ensure that no files are accidentally removed
from the ``rsync`` space on your SIMP server.

File Restoration
^^^^^^^^^^^^^^^^

FakeCA
""""""

Previously, the ``simp-environment`` RPM would place helpful ``FakeCA`` files
into ``/var/simp/environments/simp/FakeCA``. These files were for convenience
and their presence, or absence, does not affect a running system in any way.

Upon upgrade, the ``simp-environment`` RPM will be removed and you will find
that various ``FakeCA`` files are either missing or have ``.rpmsave`` appended
to them if they were modified configuration files.

You may run the following script as ``root`` to restore the ``FakeCA``
functionality:

.. code-block:: bash

   #!/bin/sh

   cd /var/simp/environments/simp
   \cp -r /usr/share/simp/environment-skeleton/FakeCA .

   cd FakeCA
   for x in *.rpmsave; do
     mv $x `basename $x .rpmsave`
   done

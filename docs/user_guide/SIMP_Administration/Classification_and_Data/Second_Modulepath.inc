Second Modulepath
-----------------

SIMP utilizes a second modulepath in the :term:`SIMP Omni-Environment` to ensure
that deployment tools like ``r10k`` do not squash ``keydist`` and some ``krb5``
files. For the ``production`` environment, the path is
``/var/simp/environments/production/site_files/``.
:ref:`PKI Certificates<Certificates>` are stored there.

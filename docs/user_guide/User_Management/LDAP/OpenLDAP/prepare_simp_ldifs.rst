:orphan:

.. _ug-user_management-ldap-openldap-prepare_simp_ldifs:

Prepare SIMP LDIFS for OpenLDAP
===============================

.. contents::
   :local:

Actionable copies of the :term:`LDAP` Data Interchange Format (.ldif) files can
be found on the system in the ``/usr/share/simp/ldifs`` directory.

Copy these files into ``/root/ldifs`` and fix their Distinguished Names:

.. code-block:: bash

   # mkdir /root/ldifs
   # cp /usr/share/simp/ldifs/* /root/ldifs
   # cd /root/ldifs
   # sed -i 's/dc=your,dc=domain/<your actual DN information>/g' *.ldif

.. WARNING::

   Do not leave any extraneous spaces in LDIF files!

   Use `:set list` in vim to see hidden spaces at the end of lines.

   Use the following to strip out inappropriate characters:

.. code-block:: bash

   # sed -i \
       's/\\(^[[:graph:]]\*:\\)[[:space:]]\*\\ ([[:graph:]]\*\\) \\[[:space:]]\*$/\\1\\2/' \
       file.ldif

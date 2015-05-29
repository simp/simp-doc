Using Kerberos 5 in SIMP
========================

The :term:`Kerberos (Krb5)` module helps an administrator obtain a
working :term:`Key Distribution Center (KDC)` setup and configure
clients to use the KDC.

    **Important**

    Given the highly sensitive nature of Kerberos passwords and tokens,
    this module does not store or use any passwords related to the
    Kerberos KDC.

    Remember the passwords chosen for the Kerberos KDC. Puppet does not
    have the ability to retrieve forgotten passwords.

As a result of the nature of Kerberos, an administrator must run
**/usr/sbin/kdb5\_util create -s** on the KDC to set the principal
administrator password and initialize the database.

The following sections provide instruction on how to get started with
Kerberos 5. For more detailed information, review the official Red Hat
documentation at
*https://access.redhat.com/knowledge/docs/en-US/Red\_Hat\_Enterprise\_Linux/6/html/Managing\_Smart\_Cards/Configuring\_a\_Kerberos\_5\_Server.html*.

Creating Principals
-------------------

Once all of the systems using Kerberos are properly configured, either
via the *krb::stock* classes or otherwise, the administrator must
register principals with the KDC.

Create the Admin Principal
~~~~~~~~~~~~~~~~~~~~~~~~~~

The first principal to be registered is an admin principal that manages
the environment, since it is in the admin group. This principal must be
created on the KDC system.

Before creating the admin principal, the user must first create an
:term:`Access Control List (ACL)` appropriate ?. To accomplish this,
add the following Puppet code to the site manifest for the KDC system.
If a custom implementation of Kerberos is being used, changes may
need to be made to the code.

Code for Creating an Admin Principal Kerberos

.. code-block:: Ruby

            krb5_acl{ "${::domain}_admin":
              principal       => "*/admin@${::domain}",
              operation_mask  => '*'
            }
            

The table below lists the steps to create an admin principal that is
appropriate for common organizations. These steps should be accomplished
after creating the ACL by using the code provided in the previous
example.

+--------+---------------------------------------------------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                                                                  |
+========+=================================================================================================================================+
| 1.     | After using the code from the previous example, run **puppet agent -t** to allow the changes to take effect.                    |
+--------+---------------------------------------------------------------------------------------------------------------------------------+
| 2.     | To finish creating the principal, type **/usr/bin/kadmin.local -r ***<Your.Domain>*** -q "addprinc ***<User Name>***/admin"**   |
|        |                                                                                                                                 |
|        | **NOTE**: By following this step, all features of the admin principal can be used remotely.                                     |
+--------+---------------------------------------------------------------------------------------------------------------------------------+
| 3.     | To load the principal, type **/usr/bin/kinit ***<User Name>***/admin**                                                          |
+--------+---------------------------------------------------------------------------------------------------------------------------------+

Table: Creating the Admin Principal Procedure

Create the Host Principal(s)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once the admin principal has been created, host principals for each host
can be made. The table below lists the steps to complete this action.

+--------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                                                                                                                                                               |
+========+==============================================================================================================================================================================================================================+
| 1.     | On the KDC, generate a principal for each host in the environment by typing **/usr/sbin/kadmin.local -r ***<Your.Domain>*** -q 'addprinc -randkey host/\ ***<FQDN>***'**                                                     |
|        |                                                                                                                                                                                                                              |
|        | **NOTE**: To use much of the functionality of the host, the user must first ensure that each host has a keytab. SIMP uses the */etc/puppet/keydist* directory for each host to distribute keytabs securely to the clients.   |
+--------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2.     | To create a keytab file for each of the above hosts, type **/usr/sbin/kadmin.local -r ***<Your.Domain>*** -q 'ktadd -k ***<FQDN>***.keytab host/\ ***<FQDN>***'**                                                            |
+--------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 3.     | Propagate all keytabs to the Puppet server by moving all of the resulting keytab files securely to the */etc/puppet/keydist/<FQDN>/keytabs* directory on the Puppet server, as appropriate for each file.                    |
+--------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 4.     | Update the node declarations to include *krb::keytab*.                                                                                                                                                                       |
|        |                                                                                                                                                                                                                              |
|        | **NOTE**: Ensure that all keytab directories are readable by the group Puppet, but not globally.                                                                                                                             |
+--------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Table: Creating Host Principals Procedure

Once the Puppet Agent runs on the clients, the keytabs are copied to the
*/etc/krb5\_keytabs* directory. The keytab matching the FQDN is set in
place as the default keytab, */etc/krb5.keytab*.

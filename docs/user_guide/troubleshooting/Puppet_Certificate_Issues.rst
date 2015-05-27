Puppet Server Certificate Issues
================================

If Puppet has any certificate issues, regenerate the server CAs. To do
this, remove the contents of the *ssl* folder and regenerate those
*.pem* files.

The following table lists the steps to regenerate the server CAs:

+--------+------------------------------------------------+
| Step   | Process/Action                                 |
+========+================================================+
| 1.     | Type **service httpd stop**                    |
+--------+------------------------------------------------+
| 2.     | Type **rm -rf /var/lib/puppet/ssl**            |
+--------+------------------------------------------------+
| 3.     | Type **puppet cert list --all**                |
+--------+------------------------------------------------+
| 4.     | Type **puppet cert --generate ***<fqdn>*****   |
+--------+------------------------------------------------+
| 5.     | Type **service httpd start**                   |
+--------+------------------------------------------------+
| 6.     | Type **puppet agent --test**                   |
+--------+------------------------------------------------+

Table: Regenerate the Server CAs

Type **rm -rf /var/lib/puppet/ssl** on the client to delete old
certificates.

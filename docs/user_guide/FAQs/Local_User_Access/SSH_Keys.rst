SSH Keys with Puppet
====================

This section provides guidance on managing a local service account, and
propagating keys and sudo permissions for the user via Puppet.

Resource Setup
--------------

In cases where there is a need to SSH to a target machine/VM as a local
user, there are several items that need to be addressed prior to
experiencing a successful connection. First, the group account must
exist locally, as well as the local user (belonging to the respective
account). PAM must allow the desired group access to the machine/VM, and
keys must be put in the correct directories to allow the user access.

The below code is a template to make this happen, with explanations of
the generic variables included, as well as 2048-bit :term:`RSA` key generation.

Code to Set Up a Local User SSH Connection Examples

.. code-block:: Ruby

            class <CLASS NAME> {
              include "ssh"

              group { "<GROUP NAME>":
                gid => "<GROUP ID NUMBER>",
                allowdupe => false,
                ensure => present,
              }

              user { "<USER NAME>":
                uid => "<USER ID NUMBER>",
                allowdupe => false,
                ensure => present,
                gid => "<GROUP NAME>",
                home => "/srv/<USER NAME>",
                managehome => true,
                shell => "/bin/sh",
                require => Group["<GROUP NAME>"]
              }

              file { "/srv/<USER NAME>/.ssh":
                owner => "<USER NAME>",
                group => "<GROUP NAME>",
                mode => "700",
                ensure => directory,
              }

              ssh_authorized_key { "<USER NAME>":
                type => "ssh-rsa",
                key => ssh_autokey( "<USER NAME>", "2048" ),
                target =>"/srv/<USER NAME>/.ssh/authorized_keys",
                require => [
                  File["/srv/<USER NAME>/.ssh"],
                  User["<USER NAME>"]
                ]
              }

              file { "/srv/<USER NAME>/.ssh/id_rsa":
                mode => "600",
                owner => "<USER NAME>",
                group => "<GROUP NAME>",
                source =>"puppet://$puppet_server/site/ssh_autokeys/<USER NAME>",
                require => Ssh_authorized_key["<USER NAME>"]
              }

              file { "/etc/ssh/local_keys/<USER NAME>":
                ensure => present,
                owner => "root",
                group => "root",
                mode => "644"
                source =>"puppet://$puppet_server/site/ssh_autokeys/<USER NAME>.pub",
                require => Ssh_authorized_key["<USER NAME>"]
              }

              sudo::user_specification { "<USER NAME>":
                user_list => ["<USERN AME>"],
                host_list => "<HOST>",
                runas => "<RUN AS>",
                cmnd => ["<COMMAND LIST>"],
                passwd => "false",
                require => User["<USER NAME>"]
              }

              pam::access::manage { "Allow <USER NAME>":
                users => <USER NAME>,
                origins => ['ALL']
              }
            }
          

Variables
~~~~~~~~~

The table below provides explanations of the variables included in the
template code in the previous section.

+---------------------+--------------------------------------------------------------+
| Variable            | Explanation                                                  |
+=====================+==============================================================+
| <CLASS NAME>        | Descriptive name of class                                    |
+---------------------+--------------------------------------------------------------+
| <COMMAND LIST>      | Commands that the local account is able to run               |
+---------------------+--------------------------------------------------------------+
| <GROUP ID NUMBER>   | Numerical ID of the group to which the user belongs          |
+---------------------+--------------------------------------------------------------+
| <GROUP NAME>        | Name of the local group to which the user belongs            |
+---------------------+--------------------------------------------------------------+
| <HOST>              | Host on which the sudo commands apply                        |
+---------------------+--------------------------------------------------------------+
| <USER NAME>         | Name of the local service account user                       |
+---------------------+--------------------------------------------------------------+
| <USER ID NUMBER>    | Numerical ID of the user                                     |
+---------------------+--------------------------------------------------------------+
| <RUN AS>            | User the local account is able to run the sudo commands as   |
+---------------------+--------------------------------------------------------------+

Table: Variable Explanations

Testing
-------

The table below lists the steps to test that the configuration was
applied correctly.

+--------+----------------------------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                                           |
+========+==========================================================================================================+
| 1.     | Log on to a server that has the template code configuration applied.                                     |
+--------+----------------------------------------------------------------------------------------------------------+
| 2.     | Type **su - ***<USER NAME>*****                                                                          |
+--------+----------------------------------------------------------------------------------------------------------+
| 3.     | Type **exec /usr/bin/ssh-agent /bin/bash** to ensure that ssh-agent has a shell running.                 |
+--------+----------------------------------------------------------------------------------------------------------+
| 4.     | Type **/usr/bin/ssh-add** to attach the user's certificates.                                             |
+--------+----------------------------------------------------------------------------------------------------------+
| 5.     | Type **/usr/bin/ssh-add -l** to double check that the user's certificates were added successfully.       |
|        |                                                                                                          |
|        | **NOTE**: This step is optional.                                                                         |
+--------+----------------------------------------------------------------------------------------------------------+
| 6.     | Type **ssh ***<HOST>***** to SSH to a target machine that has the template code configuration applied.   |
+--------+----------------------------------------------------------------------------------------------------------+

Table: Test the Configuration Procedure

If successful, the user should be authenticated and gain access to the
target machine without entering a password. If the user is prompted for
a password, check to see if the permissions are set up properly and that
the certificate keys are in the correct locations. In addition, check
the */etc/security/access.conf* file to ensure that it contains the user
or user's group in an allow statement. See access.conf(5) for details.

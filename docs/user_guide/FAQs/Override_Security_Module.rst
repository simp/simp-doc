Overriding the Security Module
==============================

The section explains how to override the security module.

Introduction
------------

SIMP includes a module that applies a large set of security-related
enforcements to the systems to which it is installed. This module
resides in */etc/puppet/modules/sec*.

It is recommended that any changes the user needs to make to a base
module are done via overrides instead of modifying the base module
itself. There are two reasons for this approach: to avoid breaking other
logic within the module hierarchies and to avoid erasing all changes by
updating the base module via :term:`Resource Package Manager (RPM)`.

Split the Application of the Security Settings
----------------------------------------------

First, break the security settings out of the default application to the
nodes. To do this, enter information similar to the example structure
below in the *site.pp* or imported file.

Break Out Security Settings from the Application

.. code-block:: Ruby

            class base_config {


              import "common"
              include "common"
              <etc../...>

              < Actions normally done to a node
              in the site. >
            }

            class secure_config inherits base_config {


              include "sec::advanced"
            }

            node default {


              include "secure_config"
            }
            

By entering this information, the user can ensure that all nodes that
are built have the advanced security class applied to them if they are
not otherwise defined.

Create the Override Class
-------------------------

After providing the logical separation needed to apply the security
settings separately from the rest of the site configuration, create a
class that overrides *sec::advanced*.

In the example below, creating an override class is done via a site
module. The information can also be included in the *site.pp*; however,
the site module is more flexible.

The table below lists the steps to create the module directories.

+--------+-----------------------------------------------------+
| Step   | Process/Action                                      |
+========+=====================================================+
| 1.     | Type **cd /etc/puppet/modules;**                    |
+--------+-----------------------------------------------------+
| 2.     | Type **mkdir site;**                                |
+--------+-----------------------------------------------------+
| 3.     | Type **mkdir site/files;**                          |
+--------+-----------------------------------------------------+
| 4.     | Type **mkdir site/modules;**                        |
+--------+-----------------------------------------------------+
| 5.     | Type **mkdir site/templates;**                      |
+--------+-----------------------------------------------------+
| 6.     | Type **touch site/modules/init.pp;**                |
+--------+-----------------------------------------------------+
| 7.     | Type **find site -type d -exec chmod 750 {} \\;**   |
+--------+-----------------------------------------------------+
| 8.     | Type **find site -type f -exec chmod 640 {} \\;**   |
+--------+-----------------------------------------------------+
| 9.     | Type **chown -R root.puppet site;**                 |
+--------+-----------------------------------------------------+

Table: Create the Module Directories Procedure

Create the contents of the *site/modules/init.pp* file using the example
below to override those portions of *sec::advanced* that need to be
changed.

Remove Security Module from Site Files

.. code-block:: Bash

            class site::security_override inherits sec::advanced
            {


               if $security_override_enable_nfs == "true" {
                 Service["nfs"] {
                   enable => true,
                   ensure => "running",
                   hasrestart => true,
                   hasstatus => false
                 }
               }
             }
            

By using the example, the :term:`Network File System (NFS)` service will be enabled if, and only if, the
*$security\_override\_enable\_nfs* variable is set to the value
**true**.

    **Important**

    Variables in Puppet are dependent upon the parse order of the
    configuration files and cannot be overridden once defined.

Create the Node with the Override
---------------------------------

Enter the information from the example below in *site.pp* or an included
file.

Set All Security Default Site Settings Examples

.. code-block:: Ruby

            node clientfqdn {

              # We need to do this to get all of the default site settings
              include "base_config"

              $security_override_enable_nfs = "true"
              include "site::security_override"
            }
            

These steps for overriding the security module can be applied as needed
to any of the base modules.

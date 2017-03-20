HOWTO Work with the SIMP Rsync Shares
=====================================

When we added support for multiple environments, the SIMP rsync space in
``/var/simp/environments/simp/rsync`` became quite complex.

This will guide you through the new rsync layout as well as providing guidance
on setting up new rsync shares for your various components.

This is very SIMP-specific and does not preclude you from using rsync however
you like. However, if you want multi-environment support, you'll need to
replicate something like what we've done for your custom directories.

Why SIMP Uses Rsync
-------------------

Rsync support was introduced in SIMP in the early days due to the fact that the
Puppet native file synchronization mechanisms were relatively horrible at
syncing large files (too much in memory) and large numbers of files in a
directory tree (too many resources and system load).

Rsync neatly solves both of these issues and is present on all SIMP systems by
default.

By default, SIMP wraps all rsync connections in an Stunnel connection to
provide encrypted connections. Additionally, SIMP adds randomly generated
passwords to sensitive shares to prevent unauthorized connections.

You can restrict this as far as necessary in your environment but the defaults
should suit most needs.

Standard Capabilities
---------------------

The standard SIMP rsync shares exist at ``/var/simp/environments/simp/rsync``.
This is an assumed path and changing this path will break some aspects of the
system.

Within this directory, you will find a set of files with the name ``.shares``.
This file is used by the fact ``simp_rsync_environments`` to indicate that all
directories at the given location should be added as rsync shared directories.

The data structure in the ``simp_rsync_environments`` is based on the **lower
cased** name of the containing directory. This means that there should
**NEVER** be two directories with the same name at the same level of the
directory hierarchy. In this case, the last one present alphabetically will
win.

As a concrete example, given the following directory structure:

.. code-block:: bash

   var
   └── simp
       └── environments
           └── simp
               └── rsync
                   ├── Global
                   │   ├── .shares
                   │   └── clamav
                   │       ├── main.cvd
                   │       ├── daily.cld
                   │       └── bytecode.cld
                   ├── .rsync.facl
                   ├── README
                   └── RedHat
                       ├── Global
                       │   ├── freeradius
                       │   ├── .shares
                       │   ├── tftpboot
                       │   │   └── linux-install
                       │   │       └── README
                       │   ├── dhcpd
                       │   │   ├── dhcpd.conf
                       │   │   └── LICENSE
                       │   ├── snmp
                       │   │   ├── mibs
                       │   │   └── dlmod
                       │   └── apache
                       │       └── www
                       │           ├── cgi-bin
                       │           ├── error
                       │           │   └── include
                       │           ├── icons
                       │           │   └── small
                       │           └── html
                       ├── 6
                       │   ├── bind_dns
                       │   │   └── LICENSE
                       │   └── .shares
                       └── 7
                           ├── bind_dns
                           │   └── LICENSE
                           └── .shares

The following would be returned by the ``simp_rsync_environments`` fact:

.. code-block:: json

   {
     "simp": {
       "id": "simp",
       "rsync": {
         "id": "rsync",
         "global": {
           "id": "Global",
           "shares": [
             "clamav"
           ]
         },
         "redhat": {
           "id": "RedHat",
           "6": {
             "id": "6",
             "shares": [
               "bind_dns"
             ]
           },
           "7": {
             "id": "7",
             "shares": [
               "bind_dns"
             ]
           },
           "global": {
             "id": "Global",
             "shares": [
               "freeradius",
               "tftpboot",
               "dhcpd",
               "snmp",
               "apache"
             ]
           }
         }
       }
     }
   }

Breaking this down, the following data is shown:

.. code-block:: json

   {
    "downcased_directory_name": {
      "id": "Original_Directory_Name",
      "downcased_subdirectory_name": {
        "id": "Original_Subdirectory_Name",
        "shares": [
          "Directory One",
          "directory two"
        ]
      }
    }
  }

.. NOTE::
   The presence of the ``.shares`` file in the directory tree tells the
   ``simp_rsync_environments`` fact that all directories at that level are to
   be exposed as shares in the returned data structure.

   That said, it is up to your Puppet logic to actually expose them as such!

   See the ``simp::server::rsync_shares`` class to see how we do this for the
   default rsync shares.

Supporting Additional Environments
----------------------------------

Generally, in a SIMP environment, you are going to want to start with the
directory structure that we have and simply copy the entire data structure to a
directory with your custom name.

.. WARNING::
   Be sure not to copy any sensitive information into the space!

For example, if you wanted to create the standard dev/test/prod structure, and
assuming that ``production`` is already symlinked to ``simp``:

```bash
cd /var/simp/environments
cp -a simp dev
cp -a simp test
```

After this, you will now have an enhanced ``simp_rsync_environments`` data
structure that holds all of the information for the ``dev``, ``test``,
``production``, and ``simp`` environments.

You can then manipulate the contents of the different environments to suit your
needs.

.. NOTE::
   The contents of the various rsync directories are not under version control
   by default. While you may add them to a VCS of your choosing (SVN, Git,
   etc...), there may be some VERY large files present in these directories.

   Make sure your system can handle the load before adding rsync content into a
   VCS!

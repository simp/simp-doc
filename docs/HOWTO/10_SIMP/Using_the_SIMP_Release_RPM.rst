.. _howto-use-the-simp-release-rpm:

Using the SIMP Release RPM
==========================

Download Details
----------------

Location
^^^^^^^^

The latest SIMP Community Release RPM is always available at https://download.simp-project.com/simp-release-community.rpm.
Install it by running:

.. code-block:: bash

   sudo yum install https://download.simp-project.com/simp-release-community.rpm

RPM Contents and Actions
^^^^^^^^^^^^^^^^^^^^^^^^

The RPM will do the following, which are detailed in sections below:

- Create two :program:`yum` variable files to target specific SIMP releases

- Add the necessary repository files to the
  :file:`/etc/yum.repos.d/` folder

- Add the necessary GPG keys needed to validate downloads to :file:`/etc/simp/gpgkeys`

Yum Variables
-------------

The SIMP Release RPM utilizes Yum variables to determine which upstream
repositories you access for SIMP files. These variables are created in two files
located in the :file:`/etc/yum/vars` folder.

+-------------------------+---------------------------------------------------------------+
| YUM Variable File       | Description                                                   |
+=========================+===============================================================+
| :file:`simprelease`     | Manages the target SIMP release version                       |
+-------------------------+---------------------------------------------------------------+
| :file:`simpreleasetype` | Manages the SIMP release type (production, rolling, unstable) |
+-------------------------+---------------------------------------------------------------+

SIMP Release Versions
^^^^^^^^^^^^^^^^^^^^^

The default setting for :file:`/etc/yum/vars/simprelease` is the latest Major
version of SIMP. The options for this file include:

=========================== ========================================= ==================================================
Option (format)             Description                               Example
=========================== ========================================= ==================================================
Major                       Downloads the latest Major release        ``6`` will install the latest release of SIMP 6,
                            of SIMP, updating to all Minor, Patch,    upgrading everything that follows for SIMP 6,
                            and Iteration releases that follow for    but not updating to SIMP 7.
                            that Major release, but not upgrading to
                            the next Major release.

Major.Minor                 Downloads the latest Major.Minor          ``6.4`` will install the latest version of SIMP 6.4,
                            release of SIMP, updating to all Patch    6.4, upgrading only the Patch releases, not
                            and Iteration releases in the Minor       upgrading to SIMP 6.5.
                            release.

Major.Minor.Patch           Downloads the latest Major.Minor.Patch    ``6.4.0`` will install SIMP 6.4.0, and update to
                            release of SIMP, only updating if an      any iteration updates, but not update to
                            iteration release is available.           SIMP 6.4.1.

Major.Minor.Patch-Iteration Downloads the specific version of SIMP,   ``6.4.0-0`` will install SIMP 6.4.0-0 and never
                            never updating to a newer version.        update.
=========================== ========================================= ==================================================

SIMP Release Types
^^^^^^^^^^^^^^^^^^

The :file:`/etc/yum/vars/simpreleasetype` variable file controls the type of
releases you want to download.

=================================== ===========================================
Option (exact contents of the file) Description
=================================== ===========================================
releases                            This setting will have the repos grab only
                                    fully tested SIMP releases.

rolling                             This setting will have the repos grab
                                    updates to RPMs that have not yet made it
                                    into a SIMP Release, but have been tested
                                    and released individually with confidence.
                                    You can specify a Major or Major.Minor release
                                    for rolling updates.

unstable/simp6                      This setting will have the repos grab all
                                    updates to RPMs in the unstable repo
                                    (This is extremely dangerous and not
                                    recommended for production environments).
                                    ``unstable/simp7`` will become available
                                    in the future when SIMP 7 is released.
=================================== ===========================================

These variables allow you to control the exact updates you receive for SIMP,
and provide a dynamic system that won't need to be updated or re-installed for
future versions of SIMP.

.. WARNING::

   Setting these Yum Var files to invalid contents will break the repo files and prevent successful downloads.
   The :file:`simprelease` file should only include numbers, dots, and dashes, no words or other characters.
   The :file:`simpreleasetype` file should only include words and potentially a slash for the unstable repos.

   Avoid any quotes and other characters that would potentially break the repo URLs

Repository Files
----------------

The RPM will add the SIMP Community repo, as well as other necessary SIMP repo files, such as Postresql, Puppet, and EPEL.
These files point to the same Release folder specified by the :file:`/etc/yum/var/` files,
but access the vendor specific repositories maintained there.

SIMP Enterprise Release RPM
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The latest SIMP Enterprise Release RPM is always available at https://download.simp-project.com/simp-release-enterprise.rpm.
You can install it as ``root`` by running

.. code-block:: bash

   yum install https://download.simp-project.com/simp-release-enterprise.rpm

The Enterprise Release RPM includes the same files as the Community version, with added repo files for SIMP Enterprise, SIMP Console, and SIMP Scanner.

GPG Keys
--------

The SIMP Release RPM will also add necessary GPG Keys to the :file:`/etc/simp/gpgkeys` folder.
These GPG Keys are placed in this folder to prevent the ``simp-gpgkeys`` package from conflicting with them,
since the SIMP and Puppet keys are required to download the package via :program:`yum`.

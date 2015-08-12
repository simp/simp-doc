Network-based Initial Server Build
==================================

This section provides guidance to install the initial SIMP server via an
existing kickstart infrastructure.

Prepare the Kickstart
---------------------

To kickstart the initial server, copy the *netboot.cfg* file into the
kickstart location from *ks/* at the root level of the extracted DVD.

Replace the *KS\_SERVER* and *KS\_BASE* variables in the *netboot.cfg*
file to match the system settings.

Kickstart the System
--------------------

Kickstart the system against the *netboot.cfg* file; this will build a
functional SIMP server identical to the one that the user would have
received from the DVD.

Post-Installation
-----------------

This section describes the post installation procedures to use the
server.

Setting up the new YUM repo
~~~~~~~~~~~~~~~~~~~~~~~~~~~

All of the SIMP systems must be able to reference two YUM locations
after install. The first is the *Local* repo, which is spawned from the
*Local* directory at the top of the DVD. This is expected to be
referenced as *http://yum\_server/yum/SIMP/<Architecture>* by the
clients.

The second location is the *Updates* repo, which contains a repo with
all of the base operating system RPMs. This is expected to be referenced
as
*http://yum\_server/yum/(RedHat\|CentOS)/<Version>/<Architecture>/Updates*
by the clients.

The user is responsible for adjusting these locations in the
pre-existing system; however, the table below lists the steps to adjust
these locations on the newly built SIMP server.


Table: Set Up the New YUM Repo Procedure

Follow the instructions in the :ref:`Client_Management` for additional assistance.

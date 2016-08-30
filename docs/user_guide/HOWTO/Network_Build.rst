HOWTO Kickstart the Initial Server
==================================

This section provides guidance to install the initial SIMP server via an
existing kickstart infrastructure.

Prepare the Kickstart
---------------------

To kickstart the initial server, copy the ``netboot.cfg`` file into the
kickstart location from ``ks/`` at the root level of the extracted DVD.

Replace the *KS\_SERVER* and *KS\_BASE* variables in the ``netboot.cfg``
file to match the system settings.

Kickstart the System
--------------------

Kickstart the system against the ``netboot.cfg`` file; this will build a
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
referenced as ``http://yum_server/yum/SIMP/<Architecture>`` by the
clients.

The second location is the *Updates* repo, which contains a repo with
all of the base operating system RPMs. This is expected to be referenced
as
``http://yum_server/yum/(RedHat|CentOS)/<Version>/<Architecture>/Updates``
by the clients.

The user is responsible for adjusting these locations in the
pre-existing system; however, the table below lists the steps to adjust
these locations on the newly built SIMP server.


.. note::

  These steps assume that the SIMP DVD material is copied in its unpacked form to the ``/srv/SIMP`` directory and that the version unpacked is RHEL 5.8. Adjust the paths appropriately if the CentOS or 5.7 version is being used.

1. Copy the entire SIMP DVD material to the SIMP server.
2. Type ``cd /srv;``
3. Type ``mkdir -p www/yum/RedHat/5.8/x86_64;``
4. Type ``mv /srv/SIMP/SIMP www/yum;``
5. Type ``mv /srv/SIMP/ks www;``
6. Type ``cd www/yum/RedHat``
7. Type ``ln -s 5.8 6; and then cd 5.8/x86_64;`` to be able to move to newer versions more easily.
8. Type ``mkdir Updates;``
9. Type ``cd Updates;``
10. Type ``find .. -type f -name “*.rpm” -exec ln -s {} \;``
11. Type ``createrepo -p .``
12. Type ``cd /var/www/yum/SIMP;``
13. Type ``updaterepos;``
14. Type ``chown -R root.apache /var/www;``
15. Type ``chmod -R u+rwX,g+rX,o-rwx /var/www;``
16. Enter the following commands into the command line to adjust the file.

  ::

    cat << EOF >> /etc/yum.repos.d/filesystem.repo
    [flocal-x86_64]
    name=Local within the filesystem
    baseurl=file:///var/www/yum/SIMP/x86_64
    enabled=1
    gpgcheck=0
    EOF

17. Enter the following commands into the command line to adjust the file.

  ::

    cat << EOF >> /etc/yum.repos.d/filesystem.repo
    [frhbase]
    name=$ostype $rhversion base repo
    baseurl=file:///var/www/yum/RedHat/6/x86_64/Server
    enabled=1
    gpgcheck=0
    EOF


Follow the instructions in the :ref:`Client_Management` for additional assistance.

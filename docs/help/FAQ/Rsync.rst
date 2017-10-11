.. _rsync_justification:

Why does SIMP use rsync?
========================

SIMP uses `rsync` to manage both large files and large numbers of small files.
This is to reduce the number of resources in the catalog and take advantage of
rsync's syncing engine to reduce network load and Puppet run times.

The common SIMP use cases for rsync include:

   * clamav
   * tftpboot
   * named
   * dhcpd


Large Files
-----------

Both the system kickstart images, and the clamav virus definitions are fairly
large (100MB+).  This isn't itself an issue. However, as the file changes over
time, Puppet would have to transfer the entire file every time it changes.

To access the accuracy of a file defined in the catalog, Puppet checksums the
file and compares it to the checksum of the expected content. This process
could take a long time, depending on the size of the file. If the sums don't
match, Puppet replaces and transfers the entire file. Rsync is smarter than
that, and only replaces the parts of the file that need replacing. In this
case, rsync saves bandwidth, Puppet run time, and a few CPU cycles.


Large Numbers of Files
----------------------

``named`` and ``dhcpd`` are the opposite situation. In both of these cases,
they may manage large numbers of files.  Typically, like above, Puppet would
have to checksum every file and see if it needed changing, with each file
setting up a new connection to the Puppet server transferring each file
indivudually.  A small number of file resources wouldn't be the end of the
world when managing something with Puppet, but rsync limits every one of these
files to one transaction and one resource. If you have a highly complex site,
without rsync, this could grow your catalog to the point where Puppet would
have a difficult time processing the entries in a timely manner.  Syncing
directories in this fashion also allows for configuration to be managed outside
of the Puppet space.


Where are the rsync files?
--------------------------

SIMP disributes the rsync materials in the ``simp-rsync`` rpm, which installs a
file tree in ``/var/simp/environments/simp/rsync``. These directories are
shared by the ``simp::server::rsync_shares`` class, which is included on the
SIMP server if the ``simp_options::rsync`` catalyst is enabled.


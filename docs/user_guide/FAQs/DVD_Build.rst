Building a Bootable DVD from the SIMP tarball
=============================================

SIMP is an overlay on top of RHEL, not a complete distribution. As such,
the user must build a bootable DVD if provided with the SIMP source code
or *tar* file.

To build a bootable SIMP DVD, if provided a RHEL DVD and the SIMP *tar*
file, follow the steps in the sections below.

Build the DVD
-------------

The table below lists the steps to build a SIMP DVD, assuming that the
user has copied the DVD to a location with enough space to house and
unpack the ISO (around 10G).

Starting from the directory with the ISO, complete the steps outlined
below. These steps are based on an example ISO of
*rhel-server-5.8-x86\_64-dvd.iso*.

+--------+-----------------------------------------------------------------------------------------+
| Step   | Process/Action                                                                          |
+========+=========================================================================================+
| 1.     | Type **for file in \`isoinfo -Rf -i rhel-server-5.8-x86\_64-dvd.iso \| tac\`; \\**      |
|        |                                                                                         |
|        | **do mkdir -p RHEL5.8-xi6\_64\`dirname $file\`; \\**                                    |
|        |                                                                                         |
|        | **isoinfo -R -x $file -i rhel-server-5.8-x86\_64-dvd.iso > RHEL5.8-x86\_64$file; \\**   |
|        |                                                                                         |
|        | **done**                                                                                |
+--------+-----------------------------------------------------------------------------------------+
| 1.     | Type **tar -C RHEL5.8-x86\_64 -xzf ***<SIMP tarball>*****                               |
+--------+-----------------------------------------------------------------------------------------+
| 1.     | Type **mkisofs -o SIMP-5.8-\ ***<SIMP Version>***-x86\_64.iso \\**                      |
|        |                                                                                         |
|        | **-b isolinux/isolinux.bin -c boot.cat \\**                                             |
|        |                                                                                         |
|        | **-no-emul-boot -boot-load-size 4 \\**                                                  |
|        |                                                                                         |
|        | **-boot-info-table \\**                                                                 |
|        |                                                                                         |
|        | **-R -m TRANS.TBL -uid 0 -gid 0 RHEL5.8-x86\_64**                                       |
+--------+-----------------------------------------------------------------------------------------+

Table: Build a SIMP DVD Procedure

The fully bootable SIMP DVD is ready to install on a new system. Replace
the RHEL version and architecture to fit the user's needs. See the
Changelog for compatible RHEL versions.

Use the Alternative Method
--------------------------

If the Ruby *rake* utility is installed, use the *Rakefile* provided in
the *Docs/examples* directory of the *tar* file.

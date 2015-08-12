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


Table: Build a SIMP DVD Procedure

The fully bootable SIMP DVD is ready to install on a new system. Replace
the RHEL version and architecture to fit the user's needs. See the
Changelog for compatible RHEL versions.

Use the Alternative Method
--------------------------

If the Ruby *rake* utility is installed, use the *Rakefile* provided in
the *Docs/examples* directory of the *tar* file.

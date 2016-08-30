.. _SIMP ISO:

HOWTO Build a Bootable DVD from the SIMP tarball
================================================

SIMP is an overlay on top of :term:`Enterprise Linux`, not a complete
distribution. As such, you must build a bootable DVD if provided with the SIMP
source code or *tar* file following the steps below.

Build the DVD
-------------

The table below lists the steps to build a SIMP DVD, assuming that you have
copied the DVD to a location with enough space to house and unpack the ISO
(around 10G).

Starting from the directory with the ISO, complete the steps outlined below.
These steps are based on an example ISO of ``rhel-server-6.7-x86_64-dvd.iso``.


1. Type

  .. code-block:: bash

    for file in `isoinfo -Rf -i rhel-server-6.7-x86_64-dvd.iso | \
      tac`; do mkdir -p RHEL6.7-x86_64`dirname $file`; \
      isoinfo -R -x $file -i rhel-server-6.7-x86_64-dvd.iso > RHEL6.7-x86_64$file; done

2. Type ``tar -C RHEL6.7-x86_64 -xzf ***<SIMP tarball>***``

3. Type

  .. code-block:: bash

    mkisofs -o SIMP-6.7-***<SIMP Version>-x86_64.iso \***
      -b isolinux/isolinux.bin -c boot.cat -no-emul-boot -boot-load-size 4 \
      -boot-info-table -R -m TRANS.TBL -uid 0 -gid 0 RHEL6.7-x86_64

The fully bootable SIMP DVD is ready to install on a new system. Replace the
RHEL version and architecture to fit the user's needs. See the Changelog for
compatible RHEL versions.

Use the Alternative Method
--------------------------

If the Ruby *rake* utility is installed, use the *Rakefile* provided in the
*Docs/examples* directory of the *tar* file.

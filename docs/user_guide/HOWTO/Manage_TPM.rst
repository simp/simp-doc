How to Manage a TPM Device with SIMP
====================================

This document serves as a guide to enable and use TPM devices in SIMP.
Currently, only :term:`TPM` **1.2** and EL7 are supported.

TPM features in SIMP:

   * Taking ownership
   * Enabling basic :term:`IMA` measuring

     * Setting custom IMA policy (broken)

   * Enabling a TPM-based PKCS#11 interface
   * Intel TXT and Trusted Boot

We do not support clearing ownership, EVM, or measured boot at this time.
``ima-evm-utils`` and kernel support are not available on SIMP platforms.

Requirements
------------

General Requirements:
^^^^^^^^^^^^^^^^^^^^^

   * A host with a TPM 1.2 chip on the motherboard
   * A legacy, non-UEFI bootloader
   * A BIOS password (one should be required to enable the TPM)
   * Easy physical access to the machine to enter the BIOS password


Trusted Boot Hardware Requirements:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   * A CPU with Intel Trusted Execution Technology (TXT)
   * A chipset with Intel Trusted Execution Technology (TXT)


Starting with TPM
-----------------

Follow the steps below to enable and take ownership of the :term:`TPM`.

#. Ensure the system has a TPM by checking the ``has_tpm`` fact, the ``status``
   section of the tpm structured fact, or by checking the sys path manually.
   You can also look for the character device ``/dev/tpm0``.

   .. code-block:: bash

      # facter -p has_tpm
      true
      # facter -p tpm.status
      ...
      owned: 0,
      enabled: 1,
      active: 1,
      ...
      # cat /sys/class/tpm/tpm0/device/active
      1
      # file /dev/tpm0
      /dev/tpm0: character special (10/224)


#. A BIOS password must be set to make sure no third parties can boot the host.
   Please set the admin password and the user password in the BIOS. If there is
   an option to require password at boot time, enable it. Do not enable Intel
   Platform Trust Technology (PTT) or Intel TXT at this time.

#. Before a TPM can be accessed by the operating system, it must first be
   enabled. This has to be done in the BIOS. Refer to the documentation
   provided with the hardware.

#. At this point, the SIMP TPM module can take over management of the device.
   Add ``tpm`` to the host's hieradata according to the example below or use
   the ``tpm_ownership`` type directly.

   .. code-block:: yaml

     classes:
       - tpm

     tpm::take_ownership: true
     tpm::ownership::advanced_facts: true

   .. NOTE::
     The ``tpm_ownership`` type does not support clearing the TPM. The process
     could possibly be destructive and has been left to be a manual process.

#. Run puppet

Enabling Trusted Boot (tboot)
-----------------------------

General Process
^^^^^^^^^^^^^^^

The steps in the section below provide guidance and automation to perform the
following:

#. Set BIOS password
#. Activate and own the TPM
#. Install the ``tboot`` package and reboot into the ``tboot no policy`` kernel
   entry
#. Download SINIT and put it in ``/boot``
#. Generate a policy and install it in the TPM NVRAM and ``/boot``
#. Update GRUB
#. Reboot into a measured state

For more information about tboot in general, reference external documentation:

*  https://fedoraproject.org/wiki/Tboot
*  The ``tboot`` docs found in ``/usr/share/tboot-*/*``
*  https://wiki.gentoo.org/wiki/Trusted_Boot
*  https://software.intel.com/sites/default/files/managed/2f/7f/Config_Guide_for_Trusted_Compute_Pools_in_RHEL_OpenStack_Platform.pdf


Steps
^^^^^

#. Enable Intel TXT and VT-d in the BIOS.

#. Boot into the kernel you want to trust (do not worry, this kernel will be
   preserved!)

#. Follow the instructions in 'Starting With TPM' and ensure:

   * The TPM is owned
   * You know the owner password
   * The SRK password is 'well-known' (``-z``)


#. Go to the `Intel site`_ and download the appropriate SINIT binary for your
   platform. Place this binary on a webserver, on the host itself, or in a
   profile module. This cannot be distributed by SIMP for licensing reasons.

#. Add the following settings to your hieradata for nodes that will be using
   Trusted Boot. It is recommended to use a `hostgroup` for this.

   * ``tpm::tboot::sinit_name`` - The name of the binary downloaded in the previous step
   * ``tpm::tboot::sinit_source`` - Where Puppet can find this binary
   * ``tpm::tboot::owner_password`` - The owner password

   Here is an example used for testing:

   .. code-block:: yaml

      tpm::tboot::sinit_name: 2nd_gen_i5_i7_SINIT_51.BIN
      tpm::tboot::sinit_source: 'file:///root/txt/2nd_gen_i5_i7-SINIT_51/2nd_gen_i5_i7_SINIT_51.BIN'
      tpm::tboot::owner_password: "%{alias('tpm::ownership::owner_pass')}"

#. Add the ``tpm::tboot`` class to the classes array with ``tpm``.

   * The ``tpm::tboot`` class adds two boot entries to the GRUB configuration.
     One should read ``tboot``, and there should be one above it called
     something along the lines of ``tboot, no policy``.
   * The Trusted Boot process requires booting into the tboot kernel before
     creating the policy, so we have opted to create both entries. The
     intermediate, ``no policy`` boot option can later be removed by setting
     ``tpm::tboot::intermediate_grub_entry`` to ``false`` in Hiera.


#. Reboot into the ``tboot, no policy`` kernel entry.

#. Puppet should run at next boot, and create the policy. Log in, ensure
   ``/boot/list.data`` exists. If not, run puppet again.

#. Reboot into the ``tboot`` kernel entry.

#. Verify that the system has completed a measured launch by running
   ``txt-stat`` or checking the ``tboot`` fact.

   .. code-block:: bash

      # txt-stat
      # facter -p tboot

Trusted Boot Debugging Tips and Warnings
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*  The ``parse_err`` command will show the error code, ready to lookup in the
   error table included in the zip.
*  The ``tboot`` kernel option ``min_ram=0x2000000`` (which is default) is
   **REQUIRED** on systems with more than 4GB of memory.
*  Trusted Boot measures the file required to boot into a Linux environment,
   and updating those file will cause a system to boot into an untrusted state.
   Be careful updating the ``kernel`` packages and rebuilding the ``initramfs``
   (or running ``dracut``).


Enable Basic IMA Measuring
--------------------------

This section assumes the previous section is complete, the TPM in the host is
owned, and it is being managed with Puppet.

IMA is a neat tool that hashes the contents of a system, and stores that hash in
the TPM. IMA is a kernel-level tool, and needs a few kernel parameters and
reboots to be completely set up.

#. Follow the above steps ensure the tpm is owned.

#. Modify the hieradata and add just one line:

   .. code-block:: yaml

     tpm::ima: true

#. Run puppet, then reboot.


Managing IMA policy
^^^^^^^^^^^^^^^^^^^

.. WARNING::
  This automated management of IMA policy is disabled for now. The policy 
  generated tends to cause systems to become read only.

This module can also support modifying what files IMA watching by editing the
``/sys/kernel/security/ima/policy``. Reference the module source file, located
at ``<environment path>/modules/tpm/manifests/ima/policy.pp`` for further
details on what can and cannot be measured.

.. WARNING::
   Pushing poorly configured policy can result in a read-only system. A reboot
   will fix the issue, but with a TPM you will have to enter the password again.
   Be very careful not to push bad policy.
   That being said, the module itself should generate proper policy and
   simultaneously make it difficult to generate malformed policy.


IMA Appraisal
^^^^^^^^^^^^^

IMA Appraisal is the process that actually measures the state of the file and
will stop changes to the filesystem if there is an issue detected.

#. Run puppet once with ``tpm::use_ima: true``, like it was set up earlier.

#. Disable the puppet agent on the host.

   .. code-block:: bash

      # puppet agent --disable

#. Make sure ``/`` and ``/home`` are mounted with the ``i_version option``. They
   are created by default with these options enabled.

#. Add the ``ima_appraise=fix`` kernel parameter temporarily.

   .. code-block:: bash

      # puppet resource kernel_parameter ima_appraise ensure=present value=fix

#. Reboot.

#. The files on the system must now be measured and saved. In order to do this,
   every file owned by root and included in the policy must be touched. This
   step will take some time.

   .. code-block:: bash

      # find / \( -fstype rootfs -o -fstype ext4 \) -type f -uid 0 -exec head -n 1 '{}' > /dev/null \;

#. After that process finishes, set the ``ima_appraise`` kernel parameter to
   ``enforce``.

   .. NOTE::
     In kernels above 4.0, we would opt for the ``log`` parameter instead of
     ``enforce``. For now, ``enforce`` is all we have. Be aware, this may cause
     your system not to boot.

   .. code-block:: bash

     # puppet resource kernel_parameter ima_appraise ensure=present value=enforce
     # # or add it to a puppet manifest

#. Reboot.

.. _Intel Site: https://software.intel.com/en-us/articles/intel-trusted-execution-technology

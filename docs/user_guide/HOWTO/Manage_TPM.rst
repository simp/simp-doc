How to Manage a TPM Device With SIMP
====================================

A great effort has been placed on automating the usage of :term:`TPM` **1.2**
devices in SIMP. This document will serve as a guide on how to enable a TPM and
use it in EL 6/7.

Supported TPM features in SIMP:

   * Taking ownership (but not clear ownership)
   * Enable basic :term:`IMA` measuring

     * Setting custom IMA policy (broken)

   * Enable a TPM-based PKCS#11 interface
   * (Future) Intel TXT and Trusted Boot

We do not support EVM or measured boot at this time. The tools (ima-evm-utils)
are not available on our supported platforms and the kernel provided doesn't
support it at this time.

Overview
--------

General hardware requirements:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   * A host with a TPM 1.2 chip on the motherboard


Trusted Boot hardware requirements:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   * A CPU with Intel Trusted Execution Technology (TXT)
   * A chipset with Intel Trusted Execution Technology (TXT)


Other non-puppet requirements:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

   * A legacy, non-UEFI bootloader
   * A BIOS password (one should be required to enable the TPM)
   * Easy physical access to the machine to enter the BIOS password


Enable and take ownership
-------------------------

#. You can see if you have a TPM by either checking with the ``has_tpm`` fact,
   the ``status`` section of the tpm structured fact, or by checking the sys
   path manually. You can also look for the character device at ``/dev/tpm0``.

   .. code-block:: bash

      $ facter -p has_tpm
      true
      $ facter -p tpm.status
      ...
      owned: 0,
      enabled: 1,
      active: 1,
      ...
      $ cat /sys/class/tpm/tpm0/device/active
      1
      $ file /dev/tpm0
      /dev/tpm0: character special (10/224)


#. A TPM would be much less useful if the boot process can't be protected. A
   BIOS password must be set to make sure no third parties can boot the host.
   Please set the admin password and the user password in the BIOS. If there is
   an option to require password at boot time, enable it. Do not enable Intel
   Platform Trust Techonology (PTT) or Intel TXT at this time.

#. Before a TPM can be accessed by the operating system, it must first be
   enabled. This has to be done in the BIOS. Refer to the documentation
   provided with the hardware.

#. At this point, the TPM module can take over management of the device. Add
   ``tpm`` to the host's hieradata according to the example below or use the
   ``tpm_ownership`` type directly.

   .. code-block:: yaml

     classes:
       - tpm

     tpm::take_ownership: true
     tpm::ownership::advanced_facts: true

   .. NOTE::
     The ``tpm_ownership`` type does not support clearing the TPM. The process
     could possibly be destructive and has been left to be a manual process.

#. Run puppet!


Enable basic IMA measuring
--------------------------

This section assumes the previous section is complete, the TPM in the host is
owned, and it is being managed with Puppet.

IMA is a neat tool that hashes the contents of a system, and stores that hash in
the TPM. IMA is a kernel-level tool, and needs a few kernel parameters and
reboots to be completely set up.

#. Follow the above steps ensure the tpm is owned

#. Modify the hieradata and add just one line:

   .. code-block:: yaml

     tpm::ima: true

#. Run puppet, then reboot.


Managing IMA policy
^^^^^^^^^^^^^^^^^^^

.. WARNING::
  This automated management of IMA policy is disabled for now. The policy generated tends to cause
  systems to become read only.

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
will stop changes to the filesystem if there is a issue detected.

#. Run puppet once with ``tpm::use_ima: true``, like it was set up earlier.

#. Disable the puppet agent on the host

   .. code-block:: bash

      $ puppet agent --disable

#. Make sure ``/`` and ``/home`` are mounted with the ``i_version option``. They
   are created by default with these options enabled.

#. Add the ``ima_appraise=fix`` kernel parameter temporarily

   .. code-block:: bash

      $ puppet resource kernel_parameter ima_appraise ensure=present value=fix

#. Reboot

#. The files on the system must now be measured and saved. In order to do this,
   every file owned by root and included in the policy must be touched. This
   step will take some time.

   .. code-block:: bash

      $ find / \( -fstype rootfs -o -fstype ext4 \) -type f -uid 0 -exec head -n 1 '{}' > /dev/null \;

#. After that process finishes, set the ``ima_appraise`` kernel parameter to
   ``enforce``.

   .. NOTE::
     In kernels above 4.0, we would opt for the ``log`` parameter instead of
     ``enforce``. For now, ``enforce`` is all we have. Be aware, this may cause
     your system not to boot.

  .. code-block:: bash

     $ puppet resource kernel_parameter ima_appraise ensure=present value=enforce
     $ # or add it to a puppet manifest

#. Reboot

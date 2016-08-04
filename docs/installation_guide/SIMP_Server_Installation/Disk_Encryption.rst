.. _ig-disk-encryption:

Disk Encryption
---------------

The default :term:`ISO` and kickstart files in SIMP now encrypt the first
physical volume if the ``simp_disk_crypt`` option is provided at the boot
command line.

.. warning::
  The system is set to **automatically** decrypt at boot! This means that the
  password is embedded in the :term:`initrd` file.

.. note::
  The ``/boot`` directory is **not** encrypted, since that would prevent the
  system from booting automatically.

Method
^^^^^^

When enabled, SIMP implements disk encryption, with automatic decryption, so
that users have the option to use their own keys in the future. Alternatively,
users may remove the system local keys and require that a password be entered
at each boot.

The primary goal of providing automatic decryption was to give users a clean
and seamless experience when using the initial system. It is understood that
this is not best practice since automatic decryption of the disks requires the
system to embed the password files in the system :term:`initrd`.

Disk encryption was not enabled by default for two reasons. The first is that
it can take an unacceptable amount of time to build a system if enough entropy
is not present. The second is that a lot of hardware contains the ability to
encrypt the disk at that level. If this is present, the utility of a second
layer of disk encryption is not necessarily warranted or a good idea.

Implementation
^^^^^^^^^^^^^^

The system keys are referenced in ``/etc/crypttab`` and, by default, reside at
``/etc/.cryptcreds``. At build time, these files are copied into all
:term:`initrd` files present on the system. This ensures that all kernels can
successfully boot the system.

The ``/etc/dracut.conf`` file is also updated to ensure that any new kernel
loads will be able to boot successfully.

.. warning::
  The ``/etc/.cryptcreds`` file **is** encrypted when the system is off.
  However, a copy is in the unencrypted :term:`initrd` files in ``/boot`` and
  should not be considered secure from physical access to the raw disk image.

.. note::
  Please be aware that **all** characters in the ``/etc/.cryptcreds`` file are
  part of the password. The lack of a trailing newline is **very** important.

Replacing the Current Password
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. note::
  The underlying system uses :term:`LUKS`, so any usage outside of this
  document should refer to the :term:`LUKS` implementation that matches your
  system version.

To change the password, you will need to perform the following steps.

1. Back up the original password file

  * If something goes amiss, you're seriously going to need this

2. Get the :term:`UUID` of your partition

  * This will be in the ``/etc/crypttab`` file. You'll want the entire
    ``UUID=<uuid>`` string

3. Create the new password

  * Remember that this needs to be **exactly** what you will use. If you ever
    expect to type this at the command line, don't forget to strip your
    trailing spaces.

    .. code-block:: python

      #!/usr/bin/python

      import sys
      import random
      import string

      # The length of the new password
      length = 1024

      # What the password should consist of
      charset = string.lowercase+string.uppercase+string.digits

      passfile = open('/etc/.cryptcreds.new','w')

      passfile.write("".join(random.choice(charset) for i in range(length)))

4. Update the key

  * There is a faster way to do this in :term:`EL` 7, but this method works on
    both systems

    .. code-block:: bash

      $ cryptsetup luksAddKey --key-slot 1 --key-file /etc/.cryptcreds UUID=<uuid> /etc/.cryptcreds.new
      $ cryptsetup luksKillSlot --key-file /etc/.cryptcreds 0

      $ cryptsetup luksAddKey --key-slot 0 --key-file /etc/.cryptcreds.new UUID=<uuid> /etc/.cryptcreds.new
      $ cryptsetup luksKillSlot --key-file /etc/.cryptcreds.new 1

      # Only do this step if the previous steps succeeded!
      $ mv /etc/.cryptcreds.new /etc/.cryptcreds

5. Update your :term:`initrd` files

  * You want to make sure to update **all** of your :term:`initrd` files since
    you'll want to be able to boot from any kernel.

    .. code-block:: bash

      for x in `ls -d /lib/modules/*`; do
        installed_kernel=`basename $x`
        dracut -f "/boot/initramfs-${installed_kernel}.img" $installed_kernel
      done

Removing the Password File
^^^^^^^^^^^^^^^^^^^^^^^^^^

If you wish to remove the password file from your system, you will need to
perform the following steps:

1. Back up the password file!

  * If you lose this, you won't be able to get into your system after reboot

2. Using your favorite text editor, remove the `install_items` line in
   `/etc/dracut.conf` that contains the reference to `/etc/.cryptcreds`
3. Remove the `/etc/.cryptcreds` file from the system
4. Update your :term:`initrd` files

  * You want to make sure to update **all** of your :term:`initrd` files since
    you'll want to be able to boot from any kernel.

    .. code-block:: bash

      for x in `ls -d /lib/modules/*`; do
        installed_kernel=`basename $x`
        dracut -f "/boot/initramfs-${installed_kernel}.img" $installed_kernel
      done

Configure the PXE Boot
======================

In order to :term:`Preboot Execution Environment (PXE)` boot clients, a copy of the ISOs for all versions of RHEL
being kickstarted is required.

Setting Up the Kickstart
------------------------

The system follows the standard kickstart model. Kickstart files are
placed in the */var/www/ks;* directory. Custom packages are placed in an
appropriate repository created under the */var/www/yum* directory.

Once the model is ready, the default SIMP settings provide access to the
user's trusted subnets as defined in the
*/etc/puppet/hieradata/simp\_def.yaml* directory.

The *pupclient\_x86\_64.cfg* file in the */var/www/ks;* directory is
used as an example in the following sections.

    **Note**

    Sample kickstart templates have been provided in the *ks* directory
    on the SIMP DVD at the *root* level.

Setting up TFTP
---------------

This section describes the process of setting up static files and
manifests for TFTP.

Static Files
~~~~~~~~~~~~

Type **cd /var/simp/rsync/CentOS/RHEL\_MAJOR\_VERSION/tftpboot** and
then type **ls** to check for the existence of the
*/var/simp/rsync/CentOS/RHEL\_MAJOR\_VERSION/tftpboot/linux-install/rhel<Version>-<Architecture>*
directory.

If the directory does not exist, create one in that location and add the
*vmlinuz* and *initrd.img* files from the *images/pxeboot* directory of
the SIMP DVD. An example is provided below for setting up the CentOS
RHEL\_MAJOR\_MINOR\_VERSION distribution.

.. code-block:: bash

  cd /var/simp/rsync/CentOS/RHEL\_MAJOR\_VERSION/tftpboot/linux-install

  mkdir centosRHEL\_MAJOR\_MINOR\_VERSION\_x86\_64; cd
   centosRHEL\_MAJOR\_MINOR\_VERSION\_x86\_64

  cp -p
   /var/www/yum/CentOS/RHEL\_MAJOR\_MINOR\_VERSION/x86\_64/images/pxeboot/\*
   .

  cd ..

  chmod 640 centosRHEL\_MAJOR\_MINOR\_VERSION\_x86\_64; chown
   root:nobody centosRHEL\_MAJOR\_MINOR\_VERSION\_x86\_64

  unlink centosRHEL\_MAJOR\_VERSION\_x86\_64

  ln -s centosRHEL\_MAJOR\_MINOR\_VERSION\_x86\_64
   centosRHEL\_MAJOR\_VERSION\_x86\_64

Manifest
~~~~~~~~

Assuming that the Puppet server is being used, create and add the
following example code to a site manifest,
*/etc/puppet/modules/site/manifests/tftpboot.pp*. Keep in mind that the
code varies based on the model being kickstarted.

Source Code for Setting Up TFTP on Puppet Server
TFTP Examples

.. code-block:: Ruby

            # Set KSSERVER statically or use Hiera for lookup
            class site::tftpboot {
              include 'tftpboot'

              tftpboot::linux_model { 'CentOS_RHEL_MAJOR_VERSION':
                kernel => 'centosRHEL_MAJOR_VERSION_x86_64/vmlinuz',
                initrd => 'centosRHEL_MAJOR_VERSION_x86_64/initrd.img',
                ks     => "http://KSSERVER/ks/pupclient_x86_64.cfg",
                extra  => "ksdevice=bootif\nipappend 2"
              }

              tftpboot::assign_host { 'default': model => 'CentOS_RHEL_MAJOR_VERSION' }
            }
            

Next, add the tftpboot site manifest to your puppet server node via
Hiera. If it does not already exist, create
*/etc/puppet/hieradata/hosts/your.server.fqdn.yaml*. Add the following
example code to that yaml file.

Source Adding TFTP Site Manifest to Hiera
TFTP Examples

.. code-block:: XML

            ---
            classes:
              - 'site::tftpboot'
            

After updating the above file, type **puppet agent -t --tags tftpboot**
on the Puppet server.

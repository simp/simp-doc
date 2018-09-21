.. _gsg-simp_on_aws:

SIMP on Amazon Web Services
===========================

This chapter provides notes and guidance on using the official SIMP Amazon
Machine Image (AMI) to run the SIMP server in the cloud.

The SIMP AMI is built from the SIMP ISO, so much of the information contained
in the ISO installation section :ref:`ug-initial_server_configuration` applies
here.

Provision a New EC2-Instance
----------------------------

To provision a new ec2-instance in the AWS cloud running the official SIMP AMI,
follow these steps:

- Launch a new instance in the normal way, and navigate to the AWS
  Marketplace tab when prompted to choose a Machine Image.
- Search the marketplace for the SIMP AMI, and locate the official published
  SIMP AMI. You can also find the AMI by the following ID: ``ami-efbf8ef9``
- Your ec2-instance should be **at least** ``t2.medium``, with 2 cpus and
  4GB of memory. Less than 4GB of memory will significantly slow down the
  bootstrapping process, and might cause problems in the future. See the
  AWS documentation_ for details on instance sizes, and the Puppet
  `Installation Guide`_ for details on hardware requirements.
- When selecting security group rules for your instance, ensure that you
  have the necessary ports open. At the very least, you need to ensure that
  you can SSH into the instance after it is running (port 22), and that the
  PuppetServer service (port 8140) is accessible from any Puppet agents that
  will connect to your SIMP Server.
- Upon launching the instance you will be prompted to provide a key pair
  that will be used to allow access to the system. you **must** provide a
  key that you have access to, as the key you provide will be the only key
  that you can log in to the instance with. The key will be automatically
  assigned to the ``ec2-user``.

Sign in with the EC2-User
-------------------------

Upon logging in with the ``ec2-user``, you will be able to switch to the root
user with the ``sudo su - root`` command.

Installing SIMP with a Partially Complete Answers File
------------------------------------------------------

Follow these steps to populate an answers file, and use it to complete the SIMP
installation:

- When you are ready to enable SIMP on the system, navigate to the
  ``/usr/share/simp`` directory and run the ``generate_answers.sh``
  bash script. This script leverages cloud-init to populate an answers file
  with the network settings that AWS has defined for the system.
- After reviewing the answers file that is in the same directory, run
  ``simp config -A simp_conf.yaml`` to begin the configuration process,
  with a subset of the answers already provided. You will be prompted for
  answers to keys that have not been filled.
- Complete the installation with the ``simp bootstrap`` command.

Ensuring Users Have Access
--------------------------

There are several steps that must be taken in Puppet to ensure that users
retain their ability to log into the system after bootstrap completes. These
steps have already been encoded in the case of the ec2-user, and the SIMP
Server AMI is by default classified with that code in the
``/etc/puppetlabs/code/environments/simp/modules/site/manifests/simp/ec2-init``
class. In particular, you should be able to log in as the ec2-user and become
root using the command ``sudo su - root`` If you decide to create new users,
or use something other than the ec2-user, you will need to ensure Puppet
is granting that user the requisite access.

See the :ref:`User_Management` section for more details on managing user
access in SIMP.

.. include:: ../jump_to_config.inc

.. _documentation: https://aws.amazon.com/ec2/instance-types/
.. _Installation Guide: https://docs.puppet.com/pe/latest/sys_req_hw.html

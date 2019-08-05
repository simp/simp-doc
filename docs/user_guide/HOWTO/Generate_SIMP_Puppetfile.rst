.. _howto-generate-a-simp-puppetfile:

Create Puppetfile
=================

This section describes how to create a Puppetfile to be used by r10k
or code manager to deploy SIMP and site modules.  It assumes the user is
on the puppetserver.

.. Warning::

   Any module not listed in the ``Puppetfile`` will be deleted from the
   target environment's ``modules`` directory, when you use :term:`r10k` to
   deploy the modules.


#. Generate the Puppetfile skeleton

   .. code-block:: ruby

      simp puppetfile generate -s > Puppetfile

#. Edit the Puppetfile to include non-simp site modules.

   Make sure the ``Puppetfile`` used to deploy from includes the following:

   * A line that includes the ``Puppetfile.simp`` which should look like:

     .. code-block:: ruby

        instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   * A line for each of your own modules.

     The SIMP :term:`CLI` can help identify local modules in an environment
     that need to be listed in the ``Puppetfile``, because they may not be under
     Git source control and, when removed by ``r10k``, may not be able to be
     readily restored.

     To generate a list of local modules in an environment (``production`` in
     this example) do the following:

     .. code-block:: sh

        simp puppetfile generate --skeleton --local-modules production  > /tmp/Puppetfile

     This will generate ``/tmp/Puppetfile`` which has

     * a directive to include the file ``Puppetfile.simp``
     * a local entry for each module presently in the ``production``
       environment, that does not have a local, SIMP-managed Git repostory and
       does not appear to be managed by Git.


     The local entries will look like the following:

     .. code-block:: yaml

        mod 'module name', :local => true

     Verify that all modules with a local entry in ``/tmp/Puppetfile`` are in
     your environment's ``Puppetfile`` in one of the following forms:

     .. code-block:: yaml

         # a module that is not a Git repository and resides in the ``modules`` directory
         mod 'site',
           :local => true

         # a Git repository that resides in a directory on the Puppet server
         mod 'mymodule'
          :git => 'file:///usr/share/mymodules/mymodule',
          :tag => '1.1.1'

         #  a Git repository on a remote server
         mod 'mysrvmod'
           :git => 'https://gitserver.my.domain/mygitproject/mysrvmod.git'
           :tag => '1.0.1'

     .. Note::

        Any modules on the local system (ones that use the
        ``:local => true`` directive), should be converted to a Git repository.
        This will ensure that if r10k removes them because of an error in the
        Puppetfile they can easily be restored from the Git repository.

#. Generate the Puppetfile.simp file

   * If using local SIMP git repositories:

     Make sure all the SIMP module RPMs are installed using yum
     that you want to use and then run:

     .. code-block:: bash

        simp puppetfile generate > Puppetfile.simp

   * If using SIMP internet repositories:

     - Download the ``Puppetfile`` used to create a SIMP ISO for a specific release
       from the SIMP `simp-core repository`_. This example uses the
       SIMP ``6.4.0-0`` release.

       .. code-block:: bash

          curl -o Puppetfile.simp https://github.com/simp/simp-core/blob/6.4.0-0/Puppetfile.pinned

     - Manually edit the ``Puppetfile.simp`` to remove components that are not Puppet
       modules, by deleting all lines up to and including
       ``moduledir  'src/puppet/modules'``.

     - Optionally, edit the ``Puppetfile.simp`` to remove any non-core SIMP
       modules that are packaged with the ``simp-extras`` RPM, that are not needed.
       The list of the SIMP extra modules can be obtained  by examining the RPM
       requirements of the ``simp-extras`` RPM as follows:

       .. code-block:: bash

          yum deplist simp-extras

#. Copy the Pupptfile and Puppetfile.simp to the appropriate directory



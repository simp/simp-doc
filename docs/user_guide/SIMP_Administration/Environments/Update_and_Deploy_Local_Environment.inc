.. _ug-update-and-deploy-using-local-repos:

#. Generate the new ``Puppetfile.simp``, a Puppetfile containing the latest
   versions of only SIMP-packaged Puppet modules:

   .. code-block:: sh

      $ cd /etc/puppetlabs/code/environments/test
      $ simp puppetfile generate > Puppetfile.simp

#. Verify the environment's ``Puppetfile``:

   .. Warning::

      Any module not listed in the ``Puppetfile`` will be deleted from the
      target environment's ``modules`` directory, when you use :term:`r10k` to
      deploy the modules.

   Make sure the ``Puppetfile`` you will be deploying from includes the following:

   * A line that includes the ``Puppetfile.simp`` which should look like:

     .. code-block:: ruby

        instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   * A line for each of your own modules.

     The SIMP :term:`CLI` can help you identify local modules in an environment
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

        If you have any modules on the local system that are not also in a
        Git repository (the ones that use the ``:local => true`` directive),
        you should seriously consider creating a Git repository for each to
        make sure none get removed by ``r10k`` and cannot be restored.

#. Deploy the modules from the local Git repositories into the environment

   Use ``r10k`` to deploy the modules, making sure the ``umask`` and ``group``
   are set correctly so that the ``puppetserver`` has access to the files.

   .. code-block:: sh

      # Set the umask and Run r10k as the puppet group to make sure the modules
      # to make sure the permissions and ownership are correct on the modules
      ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
      --puppetfile /etc/puppetlabs/code/environments/test/Puppetfile \
      --moduledir /etc/puppetlabs/code/environments/test/modules' )

.. TIP::

   Use ``simp --help`` for more information on the ``simp`` utility and alternate
   options provided for each command.


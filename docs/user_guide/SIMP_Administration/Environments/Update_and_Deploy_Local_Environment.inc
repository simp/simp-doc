.. _ug-update-and-deploy-using-local-repos:

#. Generate the new ``Puppetfile.simp``

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/<name of your environment>

      simp puppetfile generate > Puppetfile.simp

#. Verify the environment's ``Puppetfile``

   .. Warning::

      Any module not listed in the ``Puppetfile`` will be deleted from the
      target environment's (``production`` by default) ``modules`` directory,
      when you use :term:`r10k` to deploy the modules.

   Make sure the ``Puppetfile`` you will be deploying from includes the following:

   * A line that includes the ``Puppetfile.simp`` which should look like:

     .. code-block:: ruby

        instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   * A line for each of your own modules.

     To generate a list of non-SIMP modules in an environment do the following:

     .. code-block:: sh

        simp puppetfile generate -s -l <name of your environment>  > /tmp/Puppetfile

     This will generate ``/tmp/Puppetfile`` which has a directive to include
     the file ``Puppetfile.simp`` and  a local entry for each module that
     presently exists in the ``production`` environment's ``modules`` directory
     that is not also in the  SIMP repository directory,
     ``/usr/share/simp/git/puppet_modules``.

     These entries will look like the following:

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

       If there are any modules on the local system that are not also in a
       ``git`` repository (the ones that use the ``:local => true`` directive),
       you should seriously consider creating a ``git`` repository for it to
       make sure it does not get removed by ``r10k``.

#. Deploy the modules from the local ``git`` repositories into the Environment

   Use ``r10k`` to deploy the modules making sure the ``umask`` and ``group``
   are set correctly so that the ``puppetserver`` has access to the files.

   .. code-block:: sh

      # Set the umask and Run r10k as the puppet group to make sure the modules
      # to make sure the permissions and ownership are correct on the modules
      ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
      --puppetfile /etc/puppetlabs/code/environments/<name of your environment>/Puppetfile \
      --moduledir /etc/puppetlabs/code/environments/<name of your environment>/modules' )

.. Note::

   Use ``simp --help`` for more information on the simp command and alternate
   options.



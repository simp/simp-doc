.. _ug-update-and-deploy-using-local-repos:

#. **Generate the environment's :file:`Puppetfile.simp`**

   Run :code:`simp` to pull all the latest versions of the SIMP-packaged Puppet modules
   from the local git repositories:

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/test
      simp puppetfile generate > Puppetfile.simp

#. **Verify the environment's :file:`Puppetfile`**

   Make sure the :file:`Puppetfile` you will be deploying from includes the following:

   * A line that includes the :file:`Puppetfile.simp` which should look like:

     .. code-block:: ruby

        instance_eval(File.read(File.join(__dir__,"Puppetfile.simp")))

   * A line for each of your own modules.  See :ref:`How To Generate a SIMP Puppetfile<howto-generate-a-simp-puppetfile>`
   for more information on how to generate and clean up the Puppetfile if needed.


   .. Warning::

      Any module not listed in the :file:`Puppetfile` will be deleted from the
      target environment's :file:`modules` directory, when you use :term:`r10k` to
      deploy the modules.

#. **Deploy the modules**


   Use :term:`r10k` to deploy the modules from the local Git repositories into
   the environment.  Make sure the ``umask`` and ``group`` are set correctly so that
   the puppetserver has access to the files.

   .. code-block:: sh

      # Set the umask and Run r10k as the puppet group to make sure the modules
      # to make sure the permissions and ownership are correct on the modules
      ( umask 0027 && sg puppet -c '/usr/share/simp/bin/r10k puppetfile install \
      --puppetfile /etc/puppetlabs/code/environments/test/Puppetfile \
      --moduledir /etc/puppetlabs/code/environments/test/modules' )

    Use the --force option if you get warnings that local changes will get
    overwritten and you are sure you do not have changes that need saving.

.. TIP::

   Use :command:`simp --help` for more information on the :command:`simp` utility and alternate
   options provided for each command.


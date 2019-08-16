.. _ug-update-and-deploy-using-local-repos:

#. Generate the new ``Puppetfile.simp``, a Puppetfile containing the latest
   versions of only SIMP-packaged Puppet modules:

   .. code-block:: sh

      cd /etc/puppetlabs/code/environments/test
      simp puppetfile generate > Puppetfile.simp

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

   See :ref:`How To Generate a SIMP Puppetfile<howto-generate-a-simp-puppetfile>` for
   more information on how to generate and clean up the Puppetfile if needed.


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


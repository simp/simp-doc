Build Signed RPM and Deploy to packagecloud
-------------------------------------------

If a New RPM Needs to be Built
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. Build the RPM for the component that you wish to publish

.. code-block:: bash

   git clone simp-core
   git checkout master # or an appropriate branch
   bundle update
   bundle exec rake pkg:single[MODULE_NAME or PATH]

.. NOTE::

   If, for some reason, the above does not work, you can go into the target
   component and run ``rake pkg:rpm``

   The output will be in the ``dist`` directory of the targeted artifact

#. Pass the RPM over to an authorized signing team member who will sign it using ``rpm --resign``

Publish to PackageCloud
^^^^^^^^^^^^^^^^^^^^^^^

   * ``package_cloud push simp-project/REPO_NAME/el/OS_MAJOR_VERSION /path/to/packages``

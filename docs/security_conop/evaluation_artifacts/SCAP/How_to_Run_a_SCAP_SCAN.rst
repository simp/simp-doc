How to Run a SCAN
-----------------

#. Install the scanner and the SCAP Security Guide content from the OS
   repositories:

   .. code-block:: bash

      sudo dnf install -y openscap-scanner scap-security-guide

   The data streams are installed in
   :file:`/usr/share/xml/scap/ssg/content/`.  For newer content, download the
   latest `SSG Release OVAL ZIP file`_ instead and unzip it.

#. Run ``oscap xccdf eval --profile <profile_name> --results ~/scan-output.xml \``
   ``--report ~/scan-output.html ssg-<OS>-ds.xml``

   * You can get the list of available profiles by running
     ``oscap info ssg-<OS>-ds.xml``
   * For example, to run the ``STIG`` profile on AlmaLinux 9, you would run
     the following command:

     .. code-block:: bash

        sudo oscap xccdf eval --profile xccdf_org.ssgproject.content_profile_stig \
        --results ~/scan-output.xml --report ~/scan-output.html \
        /usr/share/xml/scap/ssg/content/ssg-almalinux9-ds.xml

.. _SSG Release OVAL ZIP file: https://github.com/ComplianceAsCode/content/releases

.. _ht-enable-client-reporting:

HOWTO Enable Client Reporting
=============================

Puppet has the ability to send run status reports back to the server at the
conclusion of each client run.

SIMP natively supports the `store` (default) and `puppetdb` report storage
endpoints and can be configured as shown below.

Enable Client Reporting
-----------------------

Set the following in :term:`Hiera` to enable client reporting:

.. code-block:: yaml

   ---
   pupmod::report: true

Once puppet applies this setting, clients will start sending reports to the
server at the conclusion of each puppet run.

Filesystem Reports
------------------

By default, the puppet sever will enable the `store` reports target which will
store the client reports on the local filesystem.

To view the raw reports, you can navigate to the directory output by
``puppet config print reportdir``.

If the system has been set up for `puppetdb` reports and you need to change it
back to `store`, set the following in :term:`Hiera`:

.. code-block:: yaml

   ---
   puppetdb::master::config::manage_report_processor: true
   puppetdb::master::config::enable_reports: false

PuppetDB Reports
----------------

Prior to proceeding, you should read :ref:`ht-enable-puppetdb` if you have not
already enabled :program:`puppetdb`.

If you want to use a :term:`GUI` application like `Puppetboard`_ to connect to
:program:`puppetdb` for reports, then you will need to ensure that the reports
are being sent to :program:`puppetdb` by the server.

To do this, set the following in :term:`Hiera`:

.. code-block:: yaml

   ---
   puppetdb::master::config::manage_report_processor: true
   puppetdb::master::config::enable_reports: true

CLI Reporting
-------------

You can use the `PuppetDB API`_ to get node reporting using :program:`curl` or
any other application that connect to a web endpoint and process :term:`JSON`
output.

SIMP provides a tool called :program:`puppetlast` that can read from both the
`PuppetDB API`_ as well as the locally stored :term:`YAML` reports for a simple
view of your environment.

The :program:`puppetlast` command is provided by the :package:`simp-utils` RPM.

.. _Puppetboard: https://github.com/voxpupuli/puppetboard
.. _PuppetDB API: https://puppet.com/docs/puppetdb/7/api/overview.html

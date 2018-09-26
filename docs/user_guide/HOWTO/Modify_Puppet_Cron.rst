.. _howto-modify-puppet-cron:

HOWTO Modify the Puppet Cron Schedule
=====================================

SIMP deploys a cron-job, via ``pupmod::agent::cron``, to run a non-daemonized
puppet agent to ensure compliance, over time. By default, the cron-job is run
twice every hour on a semi-random interval, to ensure all agents do not run
puppet simultaneously.  Additionally, the cron-job forcibly re-enables the
puppet agent every 4.5 hours.

Overriding Timing Parameters
----------------------------

In the example below, Puppet runs are scheduled during working hours, 0900-1700
M..F, twice every hour, in random intervals.

.. code-block:: ruby

  # Restrict puppet runs during working hours
  pupmod::agent::cron::weekday: ['1-5']
  pupmod::agent::cron::hour: ['9-17']
  pupmod::agent::cron::minute: 'rand'
  pupmod::agent::cron::run_timeframe: 60
  pupmod::agent::cron::runs_per_timeframe: 2

For more information about timing parameters, refer to the
``pupmod::agent::cron`` class documentation.

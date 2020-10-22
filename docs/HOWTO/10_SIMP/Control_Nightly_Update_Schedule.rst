.. _howto-disable-nightly-update-schedule:

HOWTO Modify the Nightly Update Schedule
========================================

By default, SIMP applies :ref:`ug-sa-ga-nightly-updates` from all configured
repositories.

This behavior is controlled by the `simp::yum::schedule`_ class from the
``simp-simp`` Puppet module and the parameters therein can be used to modify
the schedule.

If you simply want to disable the nightly updates, you can set
``simp::yum::schedule::enable`` to ``false`` in :term:`Hiera`.

.. _simp::yum::schedule: https://github.com/simp/pupmod-simp-simp/blob/master/manifests/yum/schedule.pp

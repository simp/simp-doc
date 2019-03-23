.. _gsg-system_requirements:

System Requirements
===================

The scalability of SIMP correlates to the scalability of Puppet.  From the
`Puppet tuning guide`_, a number of factors contribute to scalability,
including:

* Speed and quantity of available hardware
* Number of nodes, and frequency of check-in
* Number of modules in your module path
* Amount of hieradata

Official `hardware requirements` **for your SIMP server**:

* At least 3.4 GB of RAM + swap.

With less than 3.4 GB it is possible for either the puppet database or
puppet server to not start.

We recommend the following `hardware requirements` **for your SIMP
server**:

* **2** CPUs and **6 GB** of RAM, at a minimum
* **2 - 4** CPUs and **10 GB** of RAM to serve up to *1,000* nodes

The SIMP team recommends allocating the latter, in addition to a minimum of
**50 GB** HDD space. Again, these are not hard requirements, but anything less
may not leave adequate room for logs, applications, rsync data, etc.

.. NOTE::

   If you want to optimize the Puppet server, the `Puppet tuning guide`_ is a
   good place to start.  Use the `advanced memory debugging guide`_ for further
   optimization.

.. _Puppet tuning guide: https://docs.puppet.com/puppetserver/latest/tuning_guide.html
.. _hardware requirements: https://docs.puppet.com/puppet/latest/system_requirements.html
.. _advanced memory debugging guide: https://puppet.com/blog/puppet-server-advanced-memory-debugging

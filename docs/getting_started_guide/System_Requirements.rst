.. _gsg-system_requirements:

System Requirements
===================

The scalability of SIMP correlates to the scalability of :term:`OpenVox`.  From
the `OpenVox Server tuning guide`_, a number of factors contribute to scalability,
including:

* Speed and quantity of available hardware
* Number of nodes, and frequency of check-in
* Number of modules in your module path
* Amount of hieradata

Official `hardware requirements` **for your SIMP server**:

* At least 3.4 GB of RAM + swap.

With less than 3.4 GB it is possible for either OpenVoxDB or OpenVox Server
to not start.

We recommend the following `hardware requirements`_ **for your SIMP
server**:

* **2** CPUs and **6 GB** of RAM, at a minimum
* **2 - 4** CPUs and **10 GB** of RAM to serve up to *1,000* nodes

The SIMP team recommends allocating the latter, in addition to a minimum of
**50 GB** HDD space. Again, these are not hard requirements, but anything less
may not leave adequate room for logs, applications, rsync data, etc.

.. NOTE::

   If you want to optimize OpenVox Server, the `OpenVox Server tuning guide`_
   is a good place to start.

.. _OpenVox Server tuning guide: https://docs.openvoxproject.org/openvox-server/latest/tuning_guide.html
.. _hardware requirements: https://docs.openvoxproject.org/openvox/8.x/system_requirements.html

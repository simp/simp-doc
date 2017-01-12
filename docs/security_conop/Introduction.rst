Introduction
============

This manual describes the security concepts of the SIMP system. The system was
originally designed to meet a specific set of technical security controls using
industry best practices and has been modified recently to meet as many of the
security controls provided by the National Institute of Standards and
Technology's (:term:`NIST`) special publication `800-53`_ as possible.

This manual outlines three categories of security:

*  **Technical Architecture**: discusses the technical approaches to securing
   the system

*  **Operational Security**: discusses the security of SIMP in an operational
   setting

*  **Information System Management**: discusses how SIMP helps achieve security
   in terms of system management

A brief discussion of how the SIMP system helps achieve categories of controls
is provided; additional technical details regarding each control can be found
in the :ref:`SIMP_Security_Control_Mapping`.

When possible, the NIST security control identifier will be found at the end of
a concept to provide the reader with a reference to the specific control that
is being discussed. The identifier is written as [AB-X(Y)], where A is the
control family, X is the control section, and Y is the control enhancement.

.. NOTE::
  At present, this document will **not** be mapped to any additional standards
  since there are available mappings of the 800-53 to various other security
  frameworks.

  If you believe that we are missing anything in particular, please `file a bug`_!

.. _800-53: https://web.nvd.nist.gov/view/800-53/home
.. _file a bug: https://simp-project.atlassian.net

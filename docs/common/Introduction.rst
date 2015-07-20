Introduction
============

This guide will walk a user through the process of managing a :term:`SIMP` system.
This system includes, at a minimum, a SIMP server with properly
configured networking information and a working Puppet server.
Additionally, this document outlines the process of managing clients and
users associated with the SIMP system.

Level of Knowledge
------------------

SIMP is designed for use by system administrators or users with a strong
background using Linux operating systems. The core applications that
make up SIMP and require prerequisite knowledge are:

- :term:`Puppet` - 2.7.13 or later

- :term:`Domain Name System (DNS)` - BIND 9

- :term:`Dynamic Host Configuration Protocol (DHCP)` - Internet Systems Consortium (ISC) DHCP

- :term:`Lightweight Directory Access Protocol (LDAP)` - OpenLDAP

-  RedHat Kickstart (including all tools behind it) - :term:`Trivial File Transfer Protocol (TFTP)`, PXELinux, etc.

-  Apache

-  :term:`Yellowdog Updater, Modified (YUM)`

-  Rsyslog Version 3+

-  :term:`Internet Protocol Tables (IPtables)` (Basic knowledge of the rules)

-  :term:`Auditd` (Basic knowledge of the rules)

-  :term:`Advanced Intrusion Detection Environment (AIDE)` (Basic knowledge of the rules)

-  Basic X.509 Key Management

By itself, SIMP does as much initial setup and configuration of these
tools as possible. However, without at least some understanding, a user
will be unable to tailor a SIMP system to fit the desired environment. A
general understanding of how to control and manipulate these tools from
the command line interface (CLI) will be necessary, as SIMP does not
come stock with a graphical user interface (GUI). Additionally,
knowledge of scripting and :term:`Ruby` programming will also help to further
customize a SIMP install. For example, in order to use the advanced
features of Puppet, some Ruby programming is required.

SIMP Defined
------------

SIMP is a continually managed minimal Linux framework compatible with
both :term:`Red Hat Enterprise Linux (RHEL)` and :term:`Community Enterprise Operating System (CentOS)`. By maintaining and managing file-level and network
configuration consistency, SIMP addresses process degradation on an
operating system level. SIMP uses Puppet to provide multi-system
consistency over time while augmenting the software with tools like
Capistrano for controlled application of one-time mass operations.

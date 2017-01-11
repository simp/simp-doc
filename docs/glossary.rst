Glossary of Terms
=================

.. NOTE::
  Many terms here have been reproduced from various locations across the
  Internet and are governed by the licenses surrounding the source material.
  Please see the reference links for specifics on usage and reproducibility.

.. glossary::
   :sorted:

   ACL
   Access Control List
      A list of permissions attached to an object. An ACL specifies which users
      or system processes are granted access to objects, as well as what
      operations are allowed on given objects. Each entry in a typical ACL
      specifies a subject and an operation.

   AIDE
   Advanced Intrusion Detection Environment
      An intrusion detection system for checking the integrity of files under
      Linux. AIDE can be used to help track file integrity by comparing a
      snapshot of the system's files prior to and after a suspected incident.
      It is maintained by Rami Lehti and Pablo Virolainen.

   Auditd
      The userspace component to the Linux Auditing System. It is responsible
      for writing audit records to the disk. Viewing the logs is done with the
      ausearch or aureport utilities. Configuring the audit rules is done with
      the auditctl utility. During startup, the rules in /etc/audit/audit.rules
      are read by auditctl. The audit daemon itself has some configuration
      options that the admin may wish to customize.  They are found in the
      auditd.conf file.

   Beaker
     An acceptance testing harness, written in Ruby, by the Puppet team.

     Source: `Beaker Source Repository: <https://github.com/puppetlabs/beaker>`__

   BIOS
   Basic Input/Output System
      A type of firmware used to perform hardware initialization during
      the booting process (power-on startup) on IBM PC compatible
      computers.

      Source: `Wikipedia: BIOS <https://en.wikipedia.org/wiki/BIOS>`__

   CentOS
   Community Enterprise Operating System
      An Enterprise-grade Operating System that is mostly compatible with a
      prominent Linux distribution.

   CA
   Certificate Authority
      An entity that issues :term:`X.509` digital certificates.

   CLI
   Command Line Interface
      A means of interacting with a computer program where the user (or client)
      issues commands to the program in the form of successive lines of text
      (command lines).


       Source: `Wikipedia: Command Line Interface <https://en.wikipedia.org/wiki/Command-line_interface>`__

   CPU
   Central Processing Unit
      A central processing unit (CPU) is the electronic circuitry within a
      computer that carries out the instructions of a computer program by
      performing the basic arithmetic, logical, control and input/output (I/O)
      operations specified by the instructions


      Source: `Wikipedia: Central Processing Unit <https://en.wikipedia.org/wiki/Central_processing_unit>`__

   DAC
   Discretionary Access Control
      A type of access control defined by the Trusted Computer System
      Evaluation Criteria "as a means of restricting access to objects
      based on the identity of subjects and/or groups to which they belong.
      The controls are discretionary in the sense that a subject with a
      certain access permission is capable of passing that permission (perhaps
      indirectly) on to any other subject (unless restrained by mandatory
      access control)".

      Source: `Wikipedia: Discretionary access control <https://en.wikipedia.org/wiki/Discretionary_access_control>`__

   DNS
   Domain Name System
      A database system that translates a computer's fully qualified domain
      name into an IP address and the reverse.

   DHCP
   Dynamic Host Configuration Protocol
      A network protocol that enables a server to automatically assign an IP
      address to a computer.

   Docker
      Docker containers wrap a piece of software in a complete filesystem that
      contains everything needed to run: code, runtime, system tools, system
      libraries – anything that can be installed on a server. This guarantees
      that the software will always run the same, regardless of its
      environment.

      Source: `Docker: What is Docker? <https://www.docker.com/what-docker>`__

   DoS
   Denial of Service
   Denial of Service Attack
      An attempt to make a machine or network resource unavailable to its
      intended users, such as to temporarily or indefinitely interrupt or
      suspend services of a host connected to the Internet.

      Source: `Wikipedia: Denial-of-service attack <https://en.wikipedia.org/wiki/Denial-of-service_attack>`__

   EL
   Enterprise Linux
      In the context of SIMP, EL is a generic term for `Enterprise Linux` and
      covers both :term:`RHEL` and :term:`CentOS` as well as other :term:`RHEL`
      derivatives such as Oracle Linux.

   Elasticsearch
      A distributed, RESTful search and analytics engine capable of solving a
      growing number of use cases. As the heart of the Elastic Stack, it
      centrally stores your data so you can discover the expected and uncover
      the unexpected.

      Source: `Elasticsearch Homepage <https://www.elastic.co/products/elasticsearch>`__

   ELG
      An acronym for :term:`Elasticsearch`, :term:`Logstash`, and
      :term:`Grafana`

   ENC
   External Node Classifier
      An arbitrary script or application which can tell :term:`Puppet` which
      classes a node should have. It can replace or work in concert with the
      node definitions in the main site manifest (site.pp).

      The `Puppet Enterprise Console
      <https://docs.puppet.com/pe/latest/console_accessing.html>`__ and
      `The Foreman <https://theforeman.org/>`__ are two examples of External
      Node Classifiers.

      Source: `External Node Classifiers <https://docs.puppet.com/guides/external_nodes.html>`__

   EPEL
   Extra Packages for Enterprise Linux
     A Fedora Special Interest Group that creates, maintains, and manages a
     high quality set of additional packages for :term:`Enterprise Linux`,
     including, but not limited to, Red Hat Enterprise Linux (:term:`RHEL`),
     :term:`CentOS` and Scientific Linux (SL), Oracle Linux (OL).E

     EPEL packages are usually based on their Fedora counterparts and will
     never conflict with or replace packages in the base Enterprise Linux
     distributions. EPEL uses much of the same infrastructure as Fedora,
     including buildsystem, bugzilla instance, updates manager, mirror manager
     and more.

     Source: `EPEL Homepage <https://fedoraproject.org/wiki/EPEL>`__

   FIPS
   Federal Information Processing Standard
      Federal Information Processing Standards (FIPS) Publications are
      standards issued by :term:`NIST` after approval by the Secretary of
      Commerce pursuant to the Federal Information Security Management Act
      (FISMA)

      The particular standard of note in SIMP is `FIPS 140-2 <http://csrc.nist.gov/publications/fips/fips140-2/fips1402.pdf>`__

      Source: `FIPS Publications <http://csrc.nist.gov/publications/PubsFIPS.html>`__

   FQDN
   Fully Qualified Domain Name
      A domain name that specifies its exact location in the tree hierarchy of
      the :term:`DNS`. It specifies all domain levels, including the top-level
      domain and the root zone. An FQDN is distinguished by its unambiguity; it
      can only be interpreted one way.

   GPG
   GnuPG
   Gnu Privacy Guard
      A complete and free implementation of the OpenPGP standard as defined by
      RFC4880 (also known as PGP).

      Source: `GnuPG Homepage <https://www.gnupg.org/>`__

   Grafana
       A system of pluggable panels and data sources allowing easy
       extensibility and a variety of panels, including fully featured graph
       panels with rich visualization options. There is built in support for
       many of the most popular time series data sources.

       Source: `Grafana Homepage <http://grafana.org/>`__

   GUI
   Graphical User Interface
      A type of interface that allows users to interact with electronic devices
      through graphical icons and visual indicators such as secondary notation,
      as opposed to text-based interfaces, typed command labels or text
      navigation.

      Source: `Wikipedia: Graphical User Interface <https://en.wikipedia.org/wiki/Graphical_user_interface>`__

   HDD
   Hard Disk Drive
      A device for storing and retrieving digital information, primarily
      computer data.

   Hiera
      A key/value lookup tool for configuration data, built to make
      :term:`Puppet` better and let you set node-specific data without
      repeating yourself.

      Source: `Hiera Overview <https://docs.puppet.com/hiera/latest/>`__

   initrd
      The `Initial RAMDisk`. A complete environment that is loaded at boot time
      to enable booting the rest of the operating system.

   IMA
   Integrity Management Architecture
      The integrity subsystem is to detect if files have been
      accidentally or maliciously altered, both remotely and locally.

      Source: `IMA Sourceforge Page <http://linux-ima.sourceforge.net/linux-ima-content.html-20110907>`_

   IP
   IP Address
   Internet Protocol Address
      A numerical label assigned to each device (e.g., computer,
      printer) participating in a computer network that uses the
      Internet Protocol for communication.

      Source: `Wikipedia: IP Address <https://en.wikipedia.org/wiki/IP_address>`__

   IPTables
   Internet Protocol Tables
      A user space application that provides an interface to the IPv4 firewall
      rules on modern Linux systems.

   IP6Tables
   Internet Protocol 6 Tables
      A user space application that provides an interface to the IPv6 firewall
      rules on modern Linux systems.

   ISO
   ISO 9660
     A file system standard published by the International Organization for
     Standardization (ISO) or optical disc media.

      Source: `Wikipedia: ISO_9660 <https://en.wikipedia.org/wiki/ISO_9660>`__

   Kerberos
      A computer network authentication protocol that works on the basis of
      "tickets" to allow nodes communicating over a non-secure network to prove
      their identity to one another in a secure manner.

   KDC
   Key Distribution Center
      Part of a cryptosystem intended to reduce the risks inherent in
      exchanging keys. KDCs often operate in systems within which some users
      may have permission to use certain services at some times and not at
      others.

   LDAP
   Lightweight Directory Access Protocol
      A protocol for querying and modifying LDAP directory services including
      information such as names, addresses, email, phone numbers, and other
      information from an online directory.

   LDIF
   Lightweight Directory Interchange Format
     A standard plain text data interchange format for representing
     :term:`LDAP` (Lightweight Directory Access Protocol) directory content and
     update requests. LDIF conveys directory content as a set of records, one
     record for each object (or entry). It also represents update requests,
     such as Add, Modify, Delete, and Rename, as a set of records, one record
     for each update request.

      Source: `Wikipedia: LDAP Data Interchange Format <https://en.wikipedia.org/wiki/LDAP_Data_Interchange_Format>`__

   Logstash
      An open source, server-side data processing pipeline that ingests data
      from a multitude of sources simultaneously, transforms it, and then sends
      it to your favorite “stash.”

      Source: `Logstash Homepage <https://www.elastic.co/products/logstash>`__

   LUKS
   Linux Unified Key Setup
      The standard for Linux hard disk encryption.

      See: `The LUKS Homepage <https://gitlab.com/cryptsetup/cryptsetup/blob/master/README.md>`__

   Mandatory Access Control
      A type of access control by which the operating system constrains the
      ability of a subject or initiator to access or generally perform some
      sort of operation on an object or target.

      Source: `Wikipedia: Mandatory access control <https://en.wikipedia.org/wiki/Mandatory_access_control>`__

   MAC
   MAC Address
   Media Access Control
   Media Access Control Address
      A unique identifier assigned to network interfaces for
      communications on the physical network segment.

      Source: `Wikipedia: MAC address <https://en.wikipedia.org/wiki/MAC_address>`__

   NAT
   Network Address Translation
      The process of modifying IP address information in IP packet headers
      while in transit across a traffic routing device.

   NIST
   National Institute of Standards and Technology
      The National Institute of Standards and Technology (NIST) was founded in
      1901 and now part of the U.S. Department of Commerce. NIST is one of the
      nation's oldest physical science laboratories.

      Source: `NIST - About NIST <https://www.nist.gov/about-nist>`__

   NIST SP
   NIST Special Publication
      A set of publications that provide computer/cyber/information security
      and guidelines, recommendations, and reference materials.

      See: `NIST Special Publications <http://csrc.nist.gov/publications/PubsSPs.html>`__

   NIST 800-53
   NIST SP 800-53
   NIST Special Publication 800-53
      Security and Privacy Controls for Federal Information Systems and
      Organizations

      See: `SP 800-53 <https://web.nvd.nist.gov/view/800-53/home>`__

   NFS
   Network File System
      A distributed file system protocol that allows a user on a client
      computer to access files over a network in a manner similar to how local
      storage is accessed.

   FOSS
   Open Source
      Following an Open Source Initiative approved License.

      See: `The Open Source Definition <http://opensource.org/osd-annotated>`__

   OpenSCAP
      The OpenSCAP project provides tools that are free to use anywhere you
      like, for any purpose. Availability of the code results in greater
      portability – anyone can send patches to add support for their platform
      of choice.

      Source: `OpenSCAP Features <https://www.open-scap.org/features/>`__

   OS
   Operating System
      System software that manages computer hardware and software resources and
      provides common services for computer programs. All computer programs,
      excluding firmware, require an operating system to function.

      Source: `Wikipedia: Operating system <https://en.wikipedia.org/wiki/Operating_system>`__

   PSSH
   Parallel Secure Shell
      A tool that provides parallel versions of OpenSSH and other related
      tools.

   PAM
   Pluggable Authentication Modules
      A mechanism to integrate multiple low-level authentication schemes into a
      high-level application programming interface (API). It allows programs
      that rely on authentication to be written independent of the underlying
      authentication scheme.

   PERL
   Practical Extraction and Report Language
      A high-level, general-purpose, interpreted, dynamic programming language.
      PERL was originally developed by Larry Wall in 1987 as a general-purpose
      Unix scripting language to make report processing easier.

   PXE
   Preboot Execution Environment
      An environment to boot computers using a network interface independently
      of data storage devices (like hard disks) or installed operating systems.

   PEM
   Privacy Enhanced Mail
      An early standard for securing electronic mail. This is the public-key of
      a specific certificate. This is also the format used for Certificate
      Authority certificates.

   PKI
   Public Key Infrastructure
      A security architecture that has been introduced to provide an increased
      level of confidence for exchanging information over an increasingly
      insecure Internet. PKI enables users of a basically insecure public
      networks, such as the Internet, to securely authenticate to systems and
      exchange data. The exchange of data is done by using a combination of
      cryptographically bound public and private keys.

   Puppet
      An :term:`Open Source` configuration management tool written and
      maintained by `Puppet Labs <http://www.puppet.com>`__. Written as a
      Ruby DSL, Puppet provides a declarative language that allows system
      administrators to provide a consistently applied management
      infrastructure. Users describes system resource and resource state in the
      Puppet language.  Puppet discovers system specific information via facter
      and compiles Puppet manifests into a system specific catalog containing
      resources and resource dependencies, which are applied to each client
      system.

   Rake
   Ruby Make
      A Make-like program implemented in Ruby.

      Source: `Rake Homepage <https://ruby.github.io/rake/>`__

   RAM
   Random Access Memory
      A form of computer data storage. A random access device allows stored
      data to be accessed in nearly the same amount of time for any storage
      location, so data can be accessed quickly in any random order.

   Red Hat
   Red Hat®
   Red Hat®, Inc.
      A collection of many different software programs, developed by
      `Red Hat®, Inc. <http://www.redhat.com>`__ and other members of the Open
      Source community. All software programs included in Red Hat Enterprise
      Linux® are GPG signed by Red Hat®, Inc. to indicate that they were
      supplied by Red Hat®, Inc.

      See also :term:`RHEL`.

   RHEL
   Red Hat Enterprise Linux
      A commercial Linux operating system produced by :term:`Red Hat®`, Inc.
      RHEL is designed to provide an Enterprise-ready Linux distribution
      suitable to multiple target applications.

   RPM
   RPM Package Manager
      A package management system. The name RPM is associated with the .rpm
      file format, files in this format, software packaged in such files, and
      the package manager itself. RPM was developed primarily for GNU/Linux
      distributions; the file format is the baseline package format of the
      Linux Standard Base.

   RSA
      An algorithm for public-key cryptography that is based on the presumed
      difficulty of factoring large integers, the factoring problem. RSA stands
      for Ron Rivest, Adi Shamir and Leonard Adleman, who first publicly
      described it in 1977.

   Ruby
      A dynamic, reflective, general-purpose object-oriented programming
      language that combines syntax inspired by Perl with Smalltalk-like
      features. Ruby originated in Japan during the mid-1990s and was first
      developed and designed by Yukihiro "Matz" Matsumoto. It was influenced
      primarily by Perl, Smalltalk, Eiffel, and Lisp. Ruby supports multiple
      programming paradigms, including functional, object oriented, imperative
      and reflective. It also has a dynamic type system and automatic memory
      management; it is therefore similar in varying respects to Smalltalk,
      Python, Perl, Lisp, Dylan, Pike, and CLU.

   RVM
   Ruby Version Manager
      command-line tool which allows you to easily install, manage, and work
      with multiple :term:`Ruby` environments from interpreters to sets of
      gems.

      Source: `RVM Homepage <https://rvm.io/>`__

   SCAP
   Security Content Automation Protocol
      A synthesis of interoperable specifications derived from community ideas.

      Source: `SCAP Homepage <https://scap.nist.gov/>`__

   SSG
   SCAP Security Guide
      A security policy written in a form of :term:`SCAP` documents. The
      security policy created in SCAP Security Guide covers many areas
      of computer security and provides the best-practice solutions. The guide
      consists of rules with very detailed description and also includes proven
      remediation scripts, optimized for target systems. SCAP Security Guide,
      together with :term:`OpenSCAP` tools, can be used for auditing your
      system in an automated way.

      Source: `OpenSCAP Homepage <http://www.open-scap.org/security-policies/scap-security-guide/>`__

      See Also: :term:`SCAP`

   Service Account
      An account that is not for use by a human user but which still requires
      login access to a host.

   SSH
   Secure Shell
      An application for secure data communication, remote shell services, or
      command execution between networked computers. SSH utilizes a
      server/client model for point-to-point secure communication.

   SSL
   Secure Sockets Layer
      The standard security technology for using :term:`PKI` keys to provide a
      secure channel between two servers.

      See also :term:`TLS`.

   SELinux
      A Linux kernel security module that provides a mechanism for supporting
      access control security policies, including United States Department of
      Defense–style mandatory access controls (MAC).

      Source: `Wikipedia: Security-Enhanced Linux <https://en.wikipedia.org/wiki/Security-Enhanced_Linux>`__

   SIMP
   System Integrity Management Platform
      A security framework that sits on top of :term:`RHEL` or :term:`CentOS`.

   SFTP
   SSH File Transfer Protocol
      A network protocol that provides file access, file transfer, and file
      management functionalities over any reliable data stream. It was designed
      by the Internet Engineering Task Force (IETF) as an extension of the
      Secure Shell protocol (:term:`SSH`) version 2.0 to provide secure file
      transfer capability, but is also intended to be usable with other
      protocols.

   Sudosh
      An application that acts as an echo logger to enhance the auditing of
      privileged activities at the command line of the operating system.
      Utilities are available for playing back sudosh sessions in real time.

   SYN cookies
   syncookies
      A technique used to resist SYN flood attacks.

      Source: `Wikipedia: SYN cookies <https://en.wikipedia.org/wiki/SYN_cookies>`__

   SSSD
   System Security Services Daemon
      A daemon that provides access to identity and authentication remote
      resource through a common framework that can provide caching and offline
      support to the system.

      Source: `SSSD Homepage <https://fedorahosted.org/sssd/>`__

   STIG
   DISA STIG
   Defense Information Systems Agency Secure Technical Implementation Guide
      Configuration standards for DOD IA and IA-enabled devices/systems.

      Source: `DISA IASE <http://iase.disa.mil/stigs/Pages/index.aspx>`__

   TLS
   Transport Layer Security
      A cryptographic protocol that provides network communications security.
      TLS and :term:`SSL` encrypt the segments of network connections above the
      Transport Layer, using asymmetric cryptography for privacy and a keyed
      message authentication codes for message reliability.

      See also :term:`SSL`.

   TFTP
   Trivial File Transfer Protocol
      A file transfer protocol generally used for automated transfer of
      configuration or boot files between machines in a local environment.

   TPM
   Trusted Platform Module
      An international standard for a secure cryptoprocessor, which is a
      dedicated microcontroller designed to secure hardware by integrating
      cryptographic keys into devices.

      Source: `Wikipedia: Trusted Platform Module <https://en.wikipedia.org/wiki/Trusted_Platform_Module>`_

   TXT
   Trusted Execution Technology
      A hardware feature designed to harden platforms from the emerging threats
      of hypervisor attacks, BIOS, or other firmware attacks, malicious root kit
      installations, or other software-based attacks. It increases protection by
      allowing greater control of the launch stack through a Measured Launch
      Environment (MLE) and enabling isolation in the boot process.

      Source: `Intel Trusted Execution Technology: White Paper <http://www.intel.com/content/www/us/en/architecture-and-technology/trusted-execution-technology/trusted-execution-technology-security-paper..html>`_

   tboot
   Trusted Boot
     See :term:`TXT`.

   UUID
   Universally Unique Identifier
      A 128-bit unique value that is generally written as groups of hexadecimal
      digits separated by hyphens.

      See also: UUIDGEN(1)

   TTY
      A Unix command that prints to standard output the name of the terminal
      connected to standard input. The name of the program comes from
      teletypewriter, abbreviated "TTY".

   Vagrant
      A tool for building complete development environments. With an
      easy-to-use workflow and focus on automation, Vagrant lowers development
      environment setup time, increases development/production parity, and
      makes the "works on my machine" excuse a relic of the past.

      Source: `Vagrant: About Vagrant <https://www.vagrantup.com/about.html>`__

   VirtualBox
      A general-purpose full virtualizer for x86 hardware, targeted at server,
      desktop and embedded use.

      Source: `VirtualBox: About VirtualBox <https://www.virtualbox.org/wiki/VirtualBox>`__

   VM
   Virtual Machine
      An isolated guest operating system installation running within a host
      operating system.

   VNC
   Virtual Network Computing
      A graphical desktop sharing system that uses the remote framebuffer (RFB)
      protocol to control another computer remotely. It transmits the keyboard
      and mouse events from one computer to another, relaying the graphical
      screen updates back in the other direction, over a network.

   WAN
   Wide Area Network
      A computer networking technology used to transmit ata over long
      distances, and between different Local Area Networks (LANs),
      Metropolitan Area Networks (MANs), and other localized computer
      networking architectures.

   X.509
      An ITU-T standard for a public key infrastructure (PKI) and Privilege
      Management Infrastructure (PMI). X.509 specifies, amongst other things,
      standard formats for public key certificates, certificate revocation
      lists, attribute certificates, and a certification path validation
      algorithm.

      Source: `Wikipedia: X.509 <https://en.wikipedia.org/wiki/X.509>`__

   YUM
   Yellowdog Updater, Modified
      A software installation tool for Linux. It is a complete software
      management system that works with RPM files. YUM is designed to be
      used over a network or the Internet.

      See also :term:`RPM`.

Least Functionality
--------------------

Whenever possible, SIMP prevents kernel modules that could cause harm or are
unnecessary from loading.  The operating system's modprobe blacklist feature is
used to stop the following kernel modules from loading:

- bluetooth
- cramfs
- dccp
- dccp_ipv4
- dccp_ipv6
- freevxfs
- hfs
- hfsplus
- ieee1394
- jffs2
- net-pf-31
- rds
- sctp
- squashfs
- tipc
- udf
- usb-storage

Certain applications or application features are also explicitly disabled.  The``hosts.equiv`` (part of the r-series of commands) is disabled.  Prelinking, which changes binaries to increase startup time, is also disabled.

References: :ref:`CM-7`

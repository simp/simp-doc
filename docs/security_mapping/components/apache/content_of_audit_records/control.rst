Content of Audit Records
------------------------

The SIMP Apache configuration uses the following string to populate the Apache
logs: ``%h %l %u %t "%r" %>s %b "%{Referer}i" "%{User-Agent}i"``

That will capture the remote hostname, the request log ID, the remote username,
the time of the request, the first line of the request, the request status, the
size of the response, the referrer, and the user agent used for the request.

There is an additional log file written for SSL logs.  The following string is
used for that log: ``%t %h %{SSL_CLIENT_S_DN_CN}x %{SSL_PROTOCOL}x %{SSL_CIPHER}x \"%r\" %b %s``

That will capture the time stamp, hostname, the distinguished name of the client
certification, SSL protocol used, first line of the request, size of the
response, and the request status.

References: :ref:`AU-3`

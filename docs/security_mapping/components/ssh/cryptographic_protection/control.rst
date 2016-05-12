Cryptographic Protection
-------------------------

In the default FIPS mode, the SSH daemon limits the key exchange algorithms to:

- ecdh-sha2-nistp521
- ecdh-sha2-nistp384
- ecdh-sha2-nistp256
- diffie-hellman-group-exchange-sha256

In the default FIPS mode, the SSH daemon limits the message authentication code (MAC) algorithms to:

- hmac-sha2-256
- hmac-sha1'

In the default FIPS mode, the SSH client limits the key exchange algorithms to:

- aes256-gcm@openssh.com
- aes128-gcm@openssh.com

In the default FIPS mode, the SSH client limits the MAC algorithms to:

- hmac-sha2-256
- hmac-sha1'

References: :ref:`SC-13`

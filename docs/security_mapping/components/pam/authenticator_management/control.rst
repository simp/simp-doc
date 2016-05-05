Authenticator Management
------------------------

Authenticator strength is enforced using pam_crack_lib.so. The SIMP settings
ensure that passwords:

- Have at least four characters that are different from the previous password
- Do not repeat a character more than two times in a row
- Do not have the username (forward or reversed) in the password
- Have at lease one character from each class (upper, lower, number, special character)
- Have at least 14 characters
- Are not the same as any of the previous 24 passwords

Passwords are hashed using the SHA512 algorithm.  It's hashed using 1000 rounds.

References: :ref:`IA-5 (1)(a)`, :ref:`IA-5 (1)(e)`

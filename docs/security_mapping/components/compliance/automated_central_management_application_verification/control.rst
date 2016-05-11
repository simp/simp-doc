Automated Central Management / Application / Verification
----------------------------------------------------------

SIMP has a custom function that is embedded within the module code to validate
each variable.  Those variables are then verified against SIMP default
configuration settings using hiera.  Each time puppet runs on a client, the
hiera variables are validated against SIMP defaults.

References: :ref:`CM-7 (1)`

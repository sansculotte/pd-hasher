Cryptoraphic hashing for Pd

Uses (libgcrypt)[https://www.gnupg.org/documentation/manuals/gcrypt/]

Available hash algorithms in libcrypt [https://www.gnupg.org/documentation/manuals/gcrypt/Available-hash-algorithms.html]

Implemented Pd algorithms: SHA512, SHA256, SHA1, MD5, TIGER

To build on MacOS:

$ brew install libcrypt
$ make -f Makefile.pdlibbuilder
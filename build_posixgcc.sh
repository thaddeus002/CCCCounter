#! /bin/sh
#  build_posixgcc.sh

# This file builds and tests CCCC on a POSIX platform using the
# GNU C++ compiler.


make -C src -f posixgcc.mak clean
make -C src -f posixgcc.mak

# also make de tests
make -C test -f posix.mak


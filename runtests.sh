#!/bin/bash

CFILES=`find . -name "*.c"`

set -e

for F in $CFILES
do
    echo
    echo "*** Running test in $F ***"
    echo
    gcc -I`pwd` -DUNIT_TEST -Wall -Wextra -Werror -g $F -o testprogram
    valgrind --leak-check=full --show-reachable=yes --error-exitcode=1 ./testprogram
done

echo "All done!"

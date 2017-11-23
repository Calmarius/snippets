#!/bin/bash

CFILES=`find . -name "*.c"`

set -e

for F in $CFILES
do
    echo "Running test in $F"
    gcc -DUNIT_TEST -Wall -Wextra -Werror -g $F
    valgrind --leak-check=full --show-reachable=yes --error-exitcode=1 ./a.out
done

echo "All done!"

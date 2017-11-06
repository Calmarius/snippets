#!/bin/bash

CFILES=`find . -name "*.c"`

set -e

for F in $CFILES
do
    echo "Running test in $F"
    gcc -DUNIT_TEST -Wall -Wextra -Werror $F
    ./a.out
done

echo "All done!"

#!/bin/bash

CFILES=`find . -name "*.c"`

set -e

for F in $CFILES
do
    echo
    echo "*** Running test in $F ***"
    echo
    ./buildfile.sh $F
    valgrind --leak-check=full --show-reachable=yes --error-exitcode=1 ./testprogram
done

echo "All done!"

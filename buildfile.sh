#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Need file as an argument."
    exit 1
fi

OPTIONS="-Wall -Wextra -Werror -Wfatal-errors"

gcc -I`pwd` $2 -g $OPTIONS -DUNIT_TEST $1 -o testprogram


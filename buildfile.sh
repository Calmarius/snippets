#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Need file as an argument."
    exit 1
fi

gcc -I`pwd` -g -Wall -Wextra -Werror -Wfatal-errors -DUNIT_TEST $1 -o testprogram


#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Need file as an argument."
    exit 1
fi

gcc -Wall -g -Wextra -DUNIT_TEST $1


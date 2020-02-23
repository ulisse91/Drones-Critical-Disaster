#!/bin/bash

make clean
echo
make test
echo
./mainTest
echo
make clean
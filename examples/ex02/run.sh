#!/bin/bash
set -ex

for d in N2197; do
#for d in N729; do
    cd $d
    bindir=../../../bin
    ${bindir}/bounce inp | tee out
    #${bindir}/bounce inp
    cd ..
done

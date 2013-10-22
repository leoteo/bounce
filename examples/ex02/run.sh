#!/bin/bash
set -ex

#for d in N*; do
for d in N729; do
    cd $d
    bindir=../../../bin
    ${bindir}/bounce inp | tee out
    #${bindir}/bounce inp
    cd ..
done

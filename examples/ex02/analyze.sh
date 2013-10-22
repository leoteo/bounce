#!/bin/bash
set -ex

for d in N*; do
    cd $d
    grep "s" out | awk '{print $3}'
    cd ..
done

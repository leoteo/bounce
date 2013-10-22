#!/bin/bash
echo "#t[fs]  eKin[kJ/mol]  ePot[kJ/mol]  eTot[kJ/mol]  pTot[amu*A/fs]"
awk '{print $6, $10, $14, $18, $22}' $1

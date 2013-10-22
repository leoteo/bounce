#!/usr/bin/env python
""" Example 03

Testing the scaling behavior of bounce.
"""
from ase.lattice.cubic import SimpleCubic
import ase.io as io
import os

def get_input(d):
    return """
dt=2.0
t=1000 
nwrite=100
fil_xyz = p.xyz

temp0=300
f=lj
lj_sigma=3.4
lj_epsilon=0.997
lj_rcut=6.5

cell_x={DIM}
cell_y={DIM}
cell_z={DIM}
""" . format(DIM=d)


for d in [4,6,8,9]:

    nat=d**3
    dirname="N" + str(nat)
    fname=str(nat) + ".xyz"

    if not os.path.exists(dirname):
        os.makedirs(dirname)

    #os.chdir(dirname)

    # Set up xyz file containing a cube of d Argon atoms along each side
    xyzname=dirname+'/p.xyz'
    myatoms = SimpleCubic('Ar', latticeconstant=5.0, size=(d,d,d))
    io.write(xyzname, myatoms)

    # Set up input file with proper cell dimension
    inpname=dirname+'/inp'
    f = open(inpname, 'w')
    f.write( get_input(d * 5.0) )
    f.close()

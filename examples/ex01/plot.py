#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt('cons.dat')
t, eKin, ePot, eTot, pTot = zip(*data)

delta=1e-4

#### Plotting the energies
plt.xlabel('t [fs]')
plt.ylabel('Energy [kJ/mol]')
#plt.ylim([ eTot[0]*(1-delta), eTot[0]*(1+delta)])

plt.plot(t,eKin)
plt.plot(t,ePot)
plt.plot(t,eTot)

plt.legend(['Kinetic energy', 'Potential energy', 'Total energy'])

plt.savefig('energies.png', dpi=200)

##### Plotting the energies
plt.clf()
plt.xlabel('t [fs]')
plt.ylabel('Total Energy [kJ/mol]')
#plt.ylim([ eTot[0]*(1-delta), eTot[0]*(1+delta)])

plt.plot(t,eTot)

plt.savefig('etot.png', dpi=200)

#### Plotting total momentum
plt.clf()
plt.xlabel('t [fs]')
plt.ylabel('Total linear momentum [amu $\AA$/fs]')
pTot = np.array(pTot)
plt.ylim([ pTot.min(), pTot.max()])
plt.plot(t,pTot)

plt.savefig('ptot.png', dpi=200)
#plt.show()

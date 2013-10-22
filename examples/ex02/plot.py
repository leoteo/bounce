#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt('timings')
N, t = zip(*data)

#### Plotting the timings
plt.xlabel('Number of particles')
plt.ylabel('execution time [s]')
#plt.ylim([ eTot[0]*(1-delta), eTot[0]*(1+delta)])

plt.plot(N,t,'ro')


#### Parabolic fit
fitpar  = np.polyfit(N,t,2)
print (fitpar)
poly    = np.poly1d(fitpar) 

dense = np.arange(0,1000, 1)
plt.plot(dense, poly(dense))


plt.legend(['data', '2nd order fit'])

plt.savefig('scaling.png', dpi=200)

#plt.show()

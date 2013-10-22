#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

for fname in ['timings_debug', 'timings_O2']: 
    data = np.genfromtxt(fname)
    N, t = zip(*data)

    #### Plotting the timings
    plt.xlabel('Number of particles')
    plt.ylabel('execution time [s]')
    #plt.ylim([ eTot[0]*(1-delta), eTot[0]*(1+delta)])

    plt.plot(N,t,'o')


    #### Parabolic fit
    fitpar  = np.polyfit(N,t,2)
    print (fitpar)
    poly    = np.poly1d(fitpar) 

    dense = np.arange(0,N[-1], 1)
    plt.plot(dense, poly(dense))



plt.legend(['compiled with -O0 -g', 
            '2nd order fit',
           'compiled with -O2',  
            '2nd order fit'
            ])

outname='scaling.png'
plt.savefig(outname, dpi=200)

#plt.show()

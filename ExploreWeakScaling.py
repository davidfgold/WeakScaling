# Process the results of weak scaling
# Author:  D. Gold

import numpy as np
import matplotlib.pyplot as plt


def calc_efficiency(time, core):
    """
    Calculates the average Efficiency across a range of tests from 2-600 core, each with 5 seeds
    :param time:  (string) the function evaluation time (01 for .1 seconds, 001 for .01 seconds, etc.)
    :param core:  (list) the list of cores run in the experiment
    
    :return:
        efficiency: (float) the average parallel efficiency for the runs
    """
    nfe = np.zeros([6, 5])
    for i, c in enumerate(core):
        nfe[i, :] = np.loadtxt('Data/MS_' + str(c) + '_' + time + '_5.err', max_rows=5)

    mean_NFE = np.mean(nfe, axis=1)

    efficiency = np.ones(6)
    for i in range(5):
        efficiency[i+1] = mean_NFE[i+1]/(mean_NFE[0]*core[i+1]-1)

    return efficiency


# Explore weak scaling results
proc = [2, 20, 40, 200, 400, 600]
efficiency_05 = calc_efficiency('05', proc)
efficiency_01 = calc_efficiency('01', proc)
efficiency_005 = calc_efficiency('005', proc)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(proc, np.ones(len(proc)), linestyle=':', color='darkgrey')
ax.plot(proc[1:], efficiency_05[1:], color='indianred')
ax.plot(proc[1:], efficiency_01[1:], color='cornflowerblue')
ax.plot(proc[1:], efficiency_005[1:], color='goldenrod')
ax.set_ylim([0, 1.1])
ax.set_xlim([0,600])
ax.set_xlabel('Num. Proc.')
ax.set_ylabel('Efficiency')
plt.legend(['ideal', 'T=0.50s', 'T=0.10s', 'T=0.05s'], loc='lower left')
plt.title('Weak scaling of the Borg MOEA on DTLZ2-5 Obj')

plt.savefig('WeakScaling.png', bbox_inches='tight', dpi=300)

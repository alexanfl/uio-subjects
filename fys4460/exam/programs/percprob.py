import matplotlib.pyplot as plt
import numpy as np

L = 2

def Pi(p):
    q = 1 - p
    return 4*p**2*q**2 + 4*p**3*q + p**4

def P(p):
    q = 1 - p
    return 2*p**2*q**2 + 3*p**3*q + p**4

n = 100
p = np.linspace(0,1,n)




plt.rc('font', weight='medium', size='12', style='normal',family='Times New Roman')

plt.figure(1)
plt.subplot(211)
plt.plot(p, Pi(p), 'black') 
plt.hold('on')
plt.plot(p, p, '--', color='dimgray')

plt.title(r"Percolation probability $\Pi$  and spanning cluster density $P$", fontweight='bold')
plt.legend([r"Percolation probability $\Pi(p, L)$", "Probability $p$"], 'best')
plt.ylabel(r"$\Pi(p,L=2)$", fontsize='14')
plt.xticks(fontsize='14')
plt.grid()

plt.subplot(212)
plt.plot(p, P(p), 'black') 
plt.hold('on')
plt.plot(p, p, '--', color='dimgray')

plt.legend([r"Spanning cluster density $P(p, L)$", "Probability $p$"], 'best')
plt.xlabel(r"Probability $p$", fontsize='14')
plt.ylabel(r"$P(p,L=2)$", fontsize='14')
plt.xticks(fontsize='14')
plt.yticks(fontsize='14')
plt.grid()

plt.savefig("../figures/percolation-figures/perc-small-L.eps")
plt.show()

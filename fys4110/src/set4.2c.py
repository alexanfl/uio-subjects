import numpy as np
import matplotlib.pyplot as plt

l = 0.5
E_0 = 0.
D_low = -1.
D_high = 1.
N = 1000

def energy(sign, D, L, E_0):
    return E_0 + sign*np.sqrt(D*D + L*L)

x = np.linspace(D_low, D_high, N)

E_pos = energy(1, x, l, E_0)
E_neg = energy(-1, x, l, E_0)

def overlap(E, E_0, D, L):
    return (E-E_0+D)*(E-E_0+D)/(L*L)

o_pos = overlap(E_pos, E_0, x, l)
o_neg = overlap(E_neg, E_0, x, l)

plt.plot(x, o_pos, x, o_neg)
plt.legend(("Overlap+", "Overlap-"))
plt.show()

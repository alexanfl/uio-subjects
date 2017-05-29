import numpy as np
import matplotlib.pyplot as plt

l = 0.5
E_0 = 0.
D_low = -1.
D_high = 1.
N = 1000

def energy(sign, D, L, E_0):
    return E_0 + sign*np.sqrt(D*D + L*L)

x = np.linspace(D_low, D_high, N)/l

E_pos = energy(1, x, l, E_0)/l
E_neg = energy(-1, x, l, E_0)/l

plt.plot(x, E_pos, x, E_neg, x, E_0*np.ones(N))
plt.legend(("E+", "E-", "E0"))
plt.show()

import numpy as np
import matplotlib.pyplot as plt

N = 1e3
x = np.linspace(0,5,N)

def S(x):
    a = 2./x
    e = np.exp(-a)
    return a*e/(1-e) - np.log(1-e)

plt.plot(x, S(x))
plt.legend(("Entropy", "Maximum entropy"))
plt.show()

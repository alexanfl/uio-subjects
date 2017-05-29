import numpy as np
import matplotlib.pyplot as plt

l = 0.01
l2 = l*l
N = 1000



t = np.linspace(0,30/l,N)
pi = np.pi
tau = 2*pi*l*t

def psi(w, D):
    w1 = D
    term = (w0-w)*(w0-w) + w1*w1
    sin2theta = w1*w1/term
    O = np.sqrt(term)
    return sin2theta*np.sin(O*tau/(4.*pi*l))

w0 = 2*l
w_offres = w0/10.
w_res = w0

D0 = 2*l

plt.plot(tau,psi(w_offres, D0), tau, psi(w_res, D0), tau, psi(w_offres, 0))
plt.legend(['Off resonance', 'Resonance', 'Off resonance -- no field'])
plt.show()

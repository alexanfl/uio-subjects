import numpy as np
import matplotlib.pyplot as plt

filename='../../res/lattice.xyz'

argonfile = open(filename, 'r')
next(argonfile); next(argonfile)

c = []

for line in argonfile:
    line = line.split()
    line = line[1:]
    c.append(line)

N = len(c)

vx = []
vy = []
vz = []
vabs = []

for i in range(N):
    vx.append(float(c[i][3]))
    vy.append(float(c[i][4]))
    vz.append(float(c[i][5]))
    vabs.append(np.sqrt(vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]))


vx = np.sort(vx)
vy = np.sort(vy)
vz = np.sort(vz)
vabs = np.sort(vabs)

print 'vx mean: ', np.mean(vx), ' and vx STD: ', np.std(vx)
print 'vy mean: ', np.mean(vy), ' and vy STD: ', np.std(vy)
print 'vz mean: ', np.mean(vz), ' and vz STD: ', np.std(vz)

print 'vabs mean: ', np.mean(vabs), ' and vabs STD: ', np.std(vabs)

def f(v):
    k = 1.38066e-23         # Boltzmann constant.
    amu = 1.660538921e-27   # atomic mass unit.
    mol = 6.0221e23         # mole / Avogadro's number.
    m = 39.948*amu          # mass in amu.
    M = m*mol               # mass in kg/mol.
    T = 100.                # Temperature
    R = k*mol               # Gas constant.
    print 'mass in kg/mol: ', M
    print 'vmean: ', np.sqrt(8.*k*T/(np.pi*m))
    print 'vprob: ', np.sqrt(2.*k*T/m)
    print 'vrms: ', np.sqrt(3.*k*T/m)

    return np.sqrt((m/(2*np.pi*k*T))**3)\
            *4*np.pi*v**2*np.exp(-m*v**2/(2*k*T))

font = {'family' : 'serif',
        'size'   : 10}

plt.rc('font', **font)

# plt.plot(vx,f(vx), vy,f(vy), vz,f(vz), vabs,f(vabs))
plt.plot(vabs, f(vabs))
plt.legend([r'$f(v_x)$',r'$f(v_y)$',r'$f(v_z)$',r'$f(v_{abs})$'])
plt.show()

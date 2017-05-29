import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as an

# Number of mass points
N = 200

# All mass points have the same mass (except endpoints):
m = 0.02*np.ones(N) # kg
m[0] = m[-1] = 1.

# Spring stiffness / constant
k = 10*np.ones(N) # kg/s^2

vB = np.sqrt(k[10]/m[10])

# Length of time step
dt = 0.1*np.sqrt(m[10]/k[10]) # s
numberOfTimeSteps = 1200 #int(t_max/dt) + 1
t_max = 1200*dt
print("t_max: ", t_max)
print("Time steps: ", numberOfTimeSteps)

# Matrix of the fluctuations of the string (for time and x values)
# End points are zero due to reflecting edges.
y = np.zeros([N,numberOfTimeSteps])

# Set the initial conditions, t = 0 and 
for i in range(N):
    y[i,0] = np.sin(7*np.pi*i/(N-1))

    # Since we don't have an expression for y(-dt)
    y[i,numberOfTimeSteps-1] = np.sin(7*np.pi*i/(N-1))

totalEnergy = np.zeros(numberOfTimeSteps)

for j in range(numberOfTimeSteps-1):
    potentialEnergy = 0
    kineticEnergy = 0
    for i in range(1,N-1):

        F_left = -k[i-1]*(y[i,j] - y[i-1,j])
        F_right = -k[i]*(y[i,j] - y[i+1,j])
        F_i = F_left + F_right

        y[i,j+1] = F_i*dt**2/m[i] + 2*y[i,j] - y[i,j-1]

        potentialEnergy += 0.5*k[i]*(y[i,j] - y[i-1,j])**2
        kineticEnergy += 0.5*m[i]*((y[i,j+1]-y[i,j])/dt)**2

    totalEnergy[j+1] = potentialEnergy + kineticEnergy

plt.plot(np.linspace(1,t_max,numberOfTimeSteps-1), totalEnergy[1:])
plt.xlabel("Tid [s]")
plt.ylabel("Energi [J]")
plt.title("Totalenergi, endring gjennom svingetiden")
plt.savefig("problem6.png")
plt.show()

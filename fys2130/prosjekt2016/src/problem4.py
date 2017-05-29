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

wave_length = N/3.5
frequency = vB/wave_length


print("prop speed: ", vB)
print("Frequency: ", frequency)

# Length of time step
dt = 0.1*np.sqrt(m[10]/k[10]) # s
numberOfTimeSteps = 1200
t_max = numberOfTimeSteps*dt # s
time = dt*range(numberOfTimeSteps)

# Matrix of the fluctuations of the string (for time and x values)
# End points are zero due to reflecting edges.
y = np.zeros([N,numberOfTimeSteps])

# Set the initial conditions, t = 0
for i in range(N):
    y[i,0] = np.sin(7*np.pi*i/(N-1))
    y[i,numberOfTimeSteps-1] = np.sin(7*np.pi*i/(N-1))

for j in range(numberOfTimeSteps-1):
    for i in range(1,N-1):

        F_left = -k[i-1]*(y[i,j] - y[i-1,j])
        F_right = -k[i]*(y[i,j] - y[i+1,j])
        F_i = F_left + F_right

        # y[i,j-1] = 0 since we haven't calculated y[i,-1] yet:
        y[i,j+1] = F_i*dt**2/m[i] + 2*y[i,j] - y[i,j-1]




# plt.legend(["timestep = 0", "timestep = 60",  "timestep = 120", "timestep = 140", "timestep = 170", "timestep = 250"])


fig = plt.figure()
ax = plt.subplot(111)

ax.plot(y[:,0], label=r"t = $0\cdot\Delta t$")
ax.plot(y[:,60], label=r"t = $60\cdot\Delta t$")
ax.plot(y[:,120], label=r"t = $120\cdot\Delta t$")
ax.plot(y[:,140], label=r"t = $140\cdot\Delta t$")
ax.plot(y[:,170], label=r"t = $250\cdot\Delta t$")
ax.plot(y[:,250], label=r"t = $250\cdot\Delta t$")
plt.title("Strengens svinginger ved forskjellige tidssteg")
ax.set_xlabel(r"x [$\Delta x$]")
ax.set_ylabel("Amplitude")

# Shrink current axis by 20%
box = ax.get_position()
ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])

# Put a legend to the right of the current axis
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
plt.savefig("problem4.png")

plt.show()

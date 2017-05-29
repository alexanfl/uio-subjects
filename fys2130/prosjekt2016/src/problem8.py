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
period = 1./frequency

t_max = 10*period

print("Propagation speed: ", vB)
print("Frequency: ", frequency)

# Length of time step
dt = 0.1*np.sqrt(m[10]/k[10]) # s
numberOfTimeSteps = int(t_max/dt) + 1
print("t_max: ", t_max)
print("Time steps: ", numberOfTimeSteps)

# Matrix of the fluctuations of the string (for time and x values)
# End points are zero due to reflecting edges.
y = np.zeros([N,numberOfTimeSteps])

# Set the initial conditions, t = 0 and 
for i in range(N):
    if 1 <= i <= 30:
        y[i,0] = i/30.
    elif 31 <= i <= 60:
        y[i,0] = (60. - i)/30.
    else:
        y[i,0] = 0


        
for i in range(N):
    if 0 <= i <= 29:
        y[i,-1] = y[i,0] + 1/30.*dt*vB
    elif 30 <= i <= 59:
        y[i,-1] = y[i,0] - 1/30.*dt*vB
    else:
        y[i,-1] = y[i,0]




# fig = plt.figure()
# images = []

for j in range(numberOfTimeSteps-1):
    for i in range(1,N-1):

        F_left = -k[i-1]*(y[i,j] - y[i-1,j])
        F_right = -k[i]*(y[i,j] - y[i+1,j])
        F_i = F_left + F_right

        y[i,j+1] = F_i*dt**2/m[i] + 2*y[i,j] - y[i,j-1]
    
    # images.append(plt.plot(y[:,j+1], "-b"))

plt.title("Trekantølge som beveger seg sidelengs for forskjellige tidssteg")
plt.plot(y[:,0])
plt.plot(y[:,800])
plt.plot(y[:,1800])
plt.plot(y[:,2500])
plt.legend(["t = 0", "t = 800", "t = 1800", "t = 2500"])
plt.xlabel("")
plt.savefig("problem8.png")
plt.show()

# theanimation = an.ArtistAnimation(fig, images, interval=10)
# plt.show()

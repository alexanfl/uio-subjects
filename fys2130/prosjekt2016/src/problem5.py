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
    y[i,0] = np.sin(7*np.pi*i/(N-1))

    # Since we don't have an expression for y(-dt)
    y[i,numberOfTimeSteps-1] = np.sin(7*np.pi*i/(N-1))

t_0 = 0
t_1 = 0
counted_freq = 0
tol = 0.00005
cnt = 0


for j in range(numberOfTimeSteps-1):
    for i in range(1,N-1):

        F_left = -k[i-1]*(y[i,j] - y[i-1,j])
        F_right = -k[i]*(y[i,j] - y[i+1,j])
        F_i = F_left + F_right

        y[i,j+1] = F_i*dt**2/m[i] + 2*y[i,j] - y[i,j-1]

    # Count how many minimum points we encounter in y_99(t):
    if y[99, j+1] - y[99, t_0] < tol and j+1-t_0 > 1:
        t_1 = j+1

        counted_freq = 1./(dt*(t_1 - t_0))
        t_0 = t_1

        cnt += 1

print("Counted frequency: ", cnt/(dt*t_1))

# The take the Fast Fourier Transform of y_99(t):
y99_abs = abs(np.fft.fft(y[99,:]))**2

freqs = np.linspace(0, 1./dt, numberOfTimeSteps)
print("FFT frequency: ", freqs[10])

plt.plot(range(numberOfTimeSteps)/t_max, y99_abs[:]/numberOfTimeSteps**2)
plt.axis([0.3,0.5,0,0.25])
plt.title(r"Frekvensspekter for $y_{99}(t)$")
plt.xlabel("Frekvens [Hz]")
plt.ylabel(r"|FFT[$y_{99}(t)$]|$^2$ [Arbitrær enhet]")
plt.savefig("problem5-frekvens.png")
plt.show()

plt.plot(y[99, :])
plt.title("Svingning til det 99-ende massepunktet for %d tidssteg" % numberOfTimeSteps)
plt.xlabel("Tid [s]")
plt.ylabel("Amplitude [m]")
plt.legend([r"$y_{99}(t)$"])
plt.savefig("problem5-svingning.png")
plt.show()

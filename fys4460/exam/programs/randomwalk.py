import matplotlib.pyplot as plt
import numpy as np

n = 10000 # number of random walk steps
N = 100 # number of walkers/particles


## x and y positions of all N walkers for all n steps --
## (cont.) randomized starting positions to simulate particle positions
x = np.random.rand(N,n)*np.sqrt(n)
y = np.random.rand(N,n)*np.sqrt(n)

## 'Time' t: the number of steps walked
t = range(n)

## Mean displacement of all walkers for all n steps
disp = np.zeros((n,1))

for i in range(0, N):
    for j in range(1, n):
        ## Random number in [0,1) 
        a = np.random.rand(1)

        ## Decide if walker goes up/down/left/right

        if a < 0.25: 
            x[i][j] = x[i][j-1] + 1 
            y[i][j] = y[i][j-1]
        elif a >= 0.25 and a < 0.5:
            x[i][j] = x[i][j-1] - 1
            y[i][j] = y[i][j-1]
        elif a >= 0.5 and a < 0.75:
            y[i][j] = y[i][j-1] + 1
            x[i][j] = x[i][j-1]
        else: 
            y[i][j] = y[i][j-1] - 1 
            x[i][j] = x[i][j-1]
        
        ## Calculate average displacement at step j of n
        disp[j] += ((x[i][j] - x[i][0])**2 + (y[i][j] - y[i][0])**2 )/N
   

                
## Plot all walkers' path
# for i in range(N):
#     plt.plot(x[i], y[i])
#     plt.hold('on')
# plt.show()

## Plot mean displacement as a function of time/steps versus the number of steps
plt.rc('font', weight='medium', size='12', style='normal',family='Times New Roman')
plt.plot(t, np.sqrt(disp), '-', color='black')
plt.hold('on')
plt.plot(t, np.sqrt(t), '--', color='dimgray')
plt.title("Mean displacement of %d walkers for %d steps" % (N, n), fontweight='bold')
plt.legend([r"Displacement $\sqrt{\langle r^2 \rangle}$", r"$\sqrt{\mathrm{steps}}$"], 'best')
plt.xlabel("Number of steps", fontsize='14')
plt.ylabel("Distance from starting point", fontsize='14')
plt.xticks(fontsize='14')
plt.yticks(fontsize='14')
# plt.gca().spines['top'].set_visible(False)
# plt.gca().spines['right'].set_visible(False)
plt.grid()
plt.savefig("../figures/md-figures/random-walk-mean-distance.eps")
plt.show()

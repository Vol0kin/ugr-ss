import matplotlib.pyplot as plt
import numpy as np

comps = np.array([5, 7, 9, 11, 12])
rep = np.array([3.56635, 0.393049, 0.02624, 0.000315176, 0.000970965])
life = np.array([2.4367, 0.207335, 0.0154164, 0, 0])

plt.plot(comps, rep, 'r', label='Tiempo reparación modificado')
plt.plot(comps, life, 'b', label='Tiempo medio vida modificado')
plt.legend()
plt.show()

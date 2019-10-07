import numpy as np
import matplotlib.pyplot as plt
import sys

time = []
small = []
big = []

with open(sys.argv[1], 'r') as f:
	line = f.readline()

	for line in f:
		t, s, b = map(float, line.split())
		time.append(t)
		small.append(s)
		big.append(b)

time = np.array(time, dtype=np.int)
small = np.array(small)
big = np.array(big)

plt.plot(time, small, 'r')
plt.xlabel('Tiempo')
plt.ylabel('Número de peces pequeños')
plt.show()

plt.plot(time, big, 'b')
plt.xlabel('Tiempo')
plt.ylabel('Número de peces grandes')
plt.show()

plt.plot(big, small, 'g')
plt.xlabel('Número de peces grandes')
plt.ylabel('Número de peces pequeños')
plt.show()
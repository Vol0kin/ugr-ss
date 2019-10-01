from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax = plt.axes(projection='3d')

y_c = np.array([99, 99, 99, 99, 99, 98, 98, 98, 97, 94, 36])
x_prob = np.array([0.01, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.99])
z_dist = np.array([0.008400, 0.100000, 0.213100, 0.344800, 0.495100, 0.744800, 1.074900, 1.659600, 2.901000, 6.477300, 67.957802 ])

ax.set_xlabel('Probabilidad de ocupacion')
ax.set_ylabel('Posicion inicial')
ax.set_zlabel('Distancia media al destino')

ax.set_zlim(0, 100)

ax.plot3D(x_prob, y_c, z_dist, 'r')
ax.scatter3D(x_prob, y_c, z_dist, 'b')

plt.show()

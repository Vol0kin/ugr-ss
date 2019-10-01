from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt

fig = plt.figure()
ax = plt.axes(projection='3d')

y_c = np.array([94, 94, 94, 93, 92, 90, 86, 83, 82, 72, 80])
x_vis = np.array([2, 4, 6, 8, 10, 15, 20, 25, 30, 50, 100])
z_dist = np.array([6.480800, 6.339700, 6.069100, 5.706100, 5.505300, 5.244000, 4.954400, 4.890900, 4.778700, 4.687100, 4.624000])

ax.set_xlabel('Alcance de visión')
ax.set_ylabel('Posicion inicial')
ax.set_zlabel('Distancia media al destino')

ax.set_zlim(0, 7)

ax.plot3D(x_vis, y_c, z_dist, 'r')
ax.scatter3D(x_vis, y_c, z_dist, 'b')

print(y_c)
print(x_vis)
plt.show()

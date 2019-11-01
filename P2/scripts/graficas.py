import numpy as np
import matplotlib.pyplot as plt

x_axis = np.array([100, 500, 1000, 5000,10000, 50000, 100000, 500000, 1000000, 10000000])
table_1 = np.array([0.000057, 0.000332, 0.000568, 0.002561, 0.003894, 0.009291, 0.041933, 0.067777, 0.135245, 1.322750])
table_2 = np.array([0.000040, 0.000191, 0.000391, 0.001926, 0.003787, 0.019221, 0.033009, 0.051434, 0.125639, 0.977140])
table_3 = np.array([0.000050, 0.000294, 0.000644, 0.001603, 0.004232, 0.009200, 0.016211, 0.069050, 0.128061, 1.296004])

table_3_mej_1 = np.array([0.000048, 0.000198, 0.000398, 0.001960, 0.003879, 0.019846, 0.014623, 0.052680, 0.127474, 0.997172])

table_1_mej_2 = np.array([0.000048, 0.000054, 0.000324, 0.001488, 0.001714, 0.014507, 0.018518, 0.041926, 0.104198, 0.761227])
table_2_mej_2 = np.array([0.000051, 0.000147, 0.000281, 0.001341, 0.002922, 0.005925, 0.026527, 0.066614, 0.099769, 0.672048])
table_3_mej_2 = np.array([0.000036, 0.000145, 0.000407, 0.001365, 0.002764, 0.013514, 0.007122, 0.038306, 0.071970, 0.668524])

table_1_mej_3 = np.array([0.000009, 0.000029, 0.000056, 0.000249, 0.000494, 0.002546, 0.005344, 0.024600, 0.041979, 0.133087])


plt.plot(x_axis, table_1, 'r', label='Generador base')
plt.plot(x_axis, table_1_mej_2, 'g', label='Búsqueda binaria')
plt.plot(x_axis, table_1_mej_3, 'b', label='Acceso directo')
plt.scatter(x_axis, table_1, c='r')
plt.scatter(x_axis, table_1_mej_2, c='g')
plt.scatter(x_axis, table_1_mej_3, c='b')
plt.xlabel('Numero de valores aleatorios')
plt.ylabel('Tiempo de ejecución')
plt.legend()
plt.show()


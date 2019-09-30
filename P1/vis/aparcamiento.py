import sys
import numpy as np
import matplotlib.pyplot as plt
import re

in_file = sys.argv[1]

c_list = []
dist_list = []

with open(in_file, 'r') as f:
    for line in f:
        values = re.findall(r"([0-9]+(\.[0-9]+)*)", line)

        if len(values) > 0:
            num = values[0][0]

            if "INFORME PARCIAL" in line:
                c_list.append(int(num))
            elif "Distancia al punto de destino" in line:
                dist_list.append(float(num))

c_vals = np.array(c_list)
dist_vals = np.array(dist_list)

plt.plot(c_vals, dist_vals, 'r')
plt.xlabel('Posicion inicial')
plt.ylabel('Distancia media al punto de destino')
plt.show()

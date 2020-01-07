import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sys

# Leer los datos
datos = pd.read_csv(sys.argv[1], names=["x", "y", "t"])
tipo_grafica = sys.argv[2]

if tipo_grafica not in ("tx", "ty", "yx", "txy"):
    print("Error. Se esperaba que el segundo parametro fuese 'tx', 'ty', 'yx' o 'txy'")
    sys.exit(-1)

# Obtener los datos de forma separada
x = datos["x"].values
y = datos["y"].values
t = datos["t"].values

if tipo_grafica == "txy":
    plt.plot(t, x, label="Presas")
    plt.plot(t, y, label="Depredadores")
else:
    if tipo_grafica == "tx":
        eje_x = t
        eje_y = x
    elif tipo_grafica == "ty":
        eje_x = t
        eje_y = y
    else:
        eje_x = y
        eje_y = x

    plt.plot(eje_x, eje_y)

plt.show()